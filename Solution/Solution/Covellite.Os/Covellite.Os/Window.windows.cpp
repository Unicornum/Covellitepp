
#include "stdafx.h"
#include <Covellite/Os/Window.hpp>
#include <windowsx.h>
#include <alicorn/std/string.hpp>
#include <alicorn/std/string-cast.hpp>
#include <alicorn/platform/winapi-check.hpp>
#include <alicorn/version.hpp>
#include <Covellite/Core/ClassName.windows.hpp>
#include <Covellite/Events.hpp>
#include <Covellite/App/IApplication.hpp>
#include <Covellite/App/ClassName.windows.hpp>
#include <Covellite/App/Settings.hpp>
#include <Covellite/App/Events.hpp>
#include <Covellite/Os/Events.hpp>

#undef CreateWindow

namespace covellite
{

namespace os
{

static Handle_t CreateWindow(const wchar_t * _ClassName,
  const alicorn::modules::settings::Section & _WindowSettings,
  long & _MinWindowWidth, long & _MinWindowHeight)
{
  using Info_t = alicorn::system::version::Info;
  using namespace ::alicorn::extension::std;

  const auto ApplicationName = 
    string_cast<::std::wstring>(Info_t{}.GetValue(uT("ApplicationName")));

  const auto ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
  const auto ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

  auto WindowFlags = WS_POPUP;
  auto WindowFlagsEx = 0;

  auto X = 0;
  auto Y = 0;
  auto WindowWidth = ScreenWidth;
  auto WindowHeight = ScreenHeight;

  const auto IsFullScreen = _WindowSettings.Get<String>(uT("IsFullScreen"));
  if (IsFullScreen == uT("false"))
  {
    WindowFlags = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;
    WindowFlagsEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

    const auto & SizeSection = _WindowSettings[uT("Size")];

    const auto IsResized = _WindowSettings.Get<String>(uT("IsResized"));
    if (IsResized == uT("false"))
    {
      WindowFlags ^= WS_THICKFRAME | WS_MAXIMIZEBOX;
    }
    else
    {
      const auto MinClientWidth = SizeSection.Get<int>(uT("MinClientWidth"));
      const auto MinClientHeight = SizeSection.Get<int>(uT("MinClientHeight"));

      RECT WindowRect = { 0, 0, MinClientWidth, MinClientHeight };
      WINAPI_CHECK USING_MOCK ::AdjustWindowRectEx(&WindowRect,
        WindowFlags, FALSE, WindowFlagsEx);

      _MinWindowWidth = WindowRect.right - WindowRect.left;
      _MinWindowHeight = WindowRect.bottom - WindowRect.top;
    }

    const auto ClientWidth = SizeSection.Get<int>(uT("Width"));
    const auto ClientHeight = SizeSection.Get<int>(uT("Height"));

    RECT WindowRect = { 0, 0, ClientWidth, ClientHeight };
    WINAPI_CHECK USING_MOCK ::AdjustWindowRectEx(&WindowRect,
      WindowFlags, FALSE, WindowFlagsEx);

    const auto CalculatedWindowWidth = WindowRect.right - WindowRect.left;
    WindowWidth = (CalculatedWindowWidth > ScreenWidth) ?
      ScreenWidth : CalculatedWindowWidth;

    const auto CalculatedWindowHeight = WindowRect.bottom - WindowRect.top;
    WindowHeight = (CalculatedWindowHeight > ScreenHeight) ?
      ScreenHeight : CalculatedWindowHeight;

    X = (ScreenWidth - WindowWidth) / 2;
    Y = (ScreenHeight - WindowHeight) / 2;
  }

  const auto hWnd = USING_MOCK ::CreateWindowEx(WindowFlagsEx, 
    _ClassName, ApplicationName.c_str(), WindowFlags,
    X, Y, WindowWidth, WindowHeight, nullptr, nullptr, 
    GetModuleHandle(nullptr), nullptr);
  WINAPI_CHECK (hWnd != NULL);  

  // Игнорирование предупреждений при анализе кода (анализатор не понимает,
  // что макрос WINAPI_CHECK вызывает исключение при нулевом значении 
  // m_Handle и что в этом случае последующий код не может быть выполнен).
# pragma warning(push)
# pragma warning(disable: 6387)
  USING_MOCK ::ShowWindow(hWnd, SW_SHOW);
# pragma warning(pop)

  return hWnd;
}

/**
* \deprecated
*  Устаревший конструктор, используемый совместно с классами из проекта
*  Covellite.Core, вместо них использовать классы из Covellite.App.
*/
Window::Window(void) :
  m_Handle(::covellite::os::CreateWindow(::covellite::core::ClassName,
    ::covellite::app::Settings_t::GetInstance()[uT("Window")],
    m_MinWindowWidth, m_MinWindowHeight)),
  m_LastTypeSizeMessage(SIZE_RESTORED)
{
}

Window::Window(const ::covellite::app::IApplication & _Application) :
  m_Events(_Application),
  m_Handle(::covellite::os::CreateWindow(::covellite::app::ClassName,
    ::covellite::app::Settings_t::GetInstance()[uT("Window")],
    m_MinWindowWidth, m_MinWindowHeight)),
  m_LastTypeSizeMessage(SIZE_RESTORED)
{
  USING_MOCK ::SetWindowLongPtrW(m_Handle, GWLP_USERDATA,
    reinterpret_cast<LONG_PTR>(&m_Events));

  ActivateApplicationEvents();
  ActivateResizeEvents();
  ActivateMouseEvents();
  ActivateKeyEvents();
}

Window::~Window(void) noexcept
{
  USING_MOCK ::DestroyWindow(m_Handle);
}

Window::Rect Window::GetClientRect(void) const /*override*/
{
  RECT ClientRect;
  WINAPI_CHECK USING_MOCK ::GetClientRect(m_Handle, &ClientRect);
  return { 0, 0,
    ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top };
}

void Window::ActivateApplicationEvents(void)
{
  using RawParams_t = ::std::pair<WPARAM, LPARAM>;

  m_Events[(UINT)WM_CLOSE].Connect([=](void)
  {
    m_Events[events::Application.Exit]();
  });

  m_Events[(UINT)WM_ACTIVATEAPP].Connect([=](const RawParams_t & _Params)
  {
    if (_Params.first == TRUE)
    {
      m_Events[events::Window.Activate]();
    }
    else if (_Params.first == FALSE)
    {
      m_Events[events::Window.Deactivate]();
    }
  });
}

void Window::ActivateResizeEvents(void)
{
  using RawParams_t = ::std::pair<WPARAM, LPARAM>;

  m_Events[(UINT)WM_EXITSIZEMOVE].Connect([=](void)
  {
    // Это событие вызывается также и в конце перетаскивания окна за заголовок 
    // (без изменения размера), но задержки перерисовки окна это не вызывает.
    m_Events[events::Window.Resize]();
  });

  m_Events[(UINT)WM_SIZE].Connect([=](const RawParams_t & _Params)
  {
    // SIZE_MAXIMIZED - развернуто на весь экран.
    // SIZE_MINIMIZED - свернуто в панель задач (при этом размеры окна 
    //                  устанавливаются в 0 и это вызывает проблемы рисования).
    // SIZE_RESTORED  - вернулось в обычный размер из панели задач или 
    //                  всего экрана и при изменении размеров окна мышью.
    const auto Type = _Params.first;

    if (Type == SIZE_MAXIMIZED ||
      (Type == SIZE_RESTORED && m_LastTypeSizeMessage != SIZE_RESTORED))
    {
      m_Events[events::Window.Resize]();
    }

    if (Type == SIZE_MAXIMIZED ||
      Type == SIZE_MINIMIZED ||
      Type == SIZE_RESTORED)
    {
      m_LastTypeSizeMessage = Type;
    }
  });

  m_Events[(UINT)WM_GETMINMAXINFO].Connect([=](const RawParams_t & _Params)
  {
    auto * const pInfo = reinterpret_cast<MINMAXINFO *>(_Params.second);
    pInfo->ptMinTrackSize.x = m_MinWindowWidth;
    pInfo->ptMinTrackSize.y = m_MinWindowHeight;
  });
}

void Window::ActivateMouseEvents(void)
{
  using RawParams_t = ::std::pair<WPARAM, LPARAM>;
  using Position_t = events::Cursor_t::Position;

  m_Events[(UINT)WM_MOUSEMOVE].Connect([&](const RawParams_t & _Params)
  {
    const auto X = GET_X_LPARAM(_Params.second);
    const auto Y = GET_Y_LPARAM(_Params.second);
    m_Events[events::Cursor.Motion](Position_t{ X, Y });
  });

  m_Events[(UINT)WM_LBUTTONDOWN].Connect([=](void)
  {
    m_Events[events::Cursor.Touch]();
  });

  m_Events[(UINT)WM_LBUTTONUP].Connect([=](void)
  {
    m_Events[events::Cursor.Release]();
  });
}

void Window::ActivateKeyEvents(void)
{
  using RawParams_t = ::std::pair<WPARAM, LPARAM>;

  m_Events[(UINT)WM_CHAR].Connect([=](const RawParams_t & _Params)
  {
    auto KeyCode = static_cast<int32_t>(_Params.first);

    // Управляющие символы (кроме перевода строки) должны передавать через 
    // сигнал KeyUp.
    if (KeyCode < VK_SPACE && KeyCode != VK_RETURN) return;

    // Это преобразование виртуального кода кнопки ENTER в Unicode код символа 
    // переноса строки.
    if (KeyCode == VK_RETURN) KeyCode = 0x0A;

    m_Events[events::Key.Pressed](KeyCode);
  });

  const auto pKeyMap = ::std::make_shared<::std::map<int32_t, bool>>();

  m_Events[(UINT)WM_KEYDOWN].Connect([=](const RawParams_t & _Params)
  {
    const auto KeyCode = static_cast<int32_t>(_Params.first);

    if ((*pKeyMap)[KeyCode]) return;

    (*pKeyMap)[KeyCode] = true;
    m_Events[events::Key.Down](KeyCode);
  });

  m_Events[(UINT)WM_KEYUP].Connect([=](const RawParams_t & _Params)
  {
    const auto KeyCode = static_cast<int32_t>(_Params.first);

    (*pKeyMap)[KeyCode] = false;
    m_Events[events::Key.Up](KeyCode);
  });

  m_Events[(UINT)WM_SYSKEYUP].Connect([=](const RawParams_t & _Params)
  {
    if (_Params.first == VK_LEFT)
    {
      m_Events[events::Key.Back]();
    }
    else if (_Params.first == VK_SPACE)
    {
      m_Events[events::Key.Menu]();
    }
  });
}

} // namespace os

} // namespace covellite
