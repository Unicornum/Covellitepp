
#include "stdafx.h"
#include <Covellite\Core\EventHandler.hpp>
#include <windows.h>
#include <windowsx.h>
#include <alicorn\platform\winapi-call.hpp>
#include <alicorn\std\exception.hpp>
#include <Covellite\Core\Message.hpp>
#include <Covellite\Core\ClassName.windows.hpp>
#include <Covellite\Core\ClickEventListener.hpp>

using namespace covellite::core;

static EventHandler * pEvents = nullptr;
static WNDCLASSEX WindowClass = { 0 };

EventHandler::EventHandler(void) :
  m_pClickEventListener(::std::make_unique<ClickEventListener>(OnRocket, OnFramework)),
  m_fnCleaner([](void)
  {
    USING_MOCK ::UnregisterClass(ClassName, GetModuleHandle(nullptr));
    pEvents = nullptr;
  })
{
  pEvents = this;

  const auto hInstance = GetModuleHandle(nullptr);

  WindowClass.cbSize = sizeof(WindowClass);
  WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  WindowClass.hInstance = hInstance;
  WindowClass.lpszClassName = ClassName;
  WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  WindowClass.hIcon = USING_MOCK ::LoadIcon(hInstance, L"ApplicationIcon");
  WindowClass.hIconSm = USING_MOCK ::LoadIcon(hInstance, L"ApplicationIcon");

  WindowClass.lpfnWndProc = 
    [](HWND _hWnd, UINT _Message, WPARAM _wParam, LPARAM _lParam) -> LRESULT 
  {
    if (pEvents == nullptr) return 0;

    const auto Result = pEvents->CommandHandler({ _Message, _wParam, _lParam });
    return (Result) ? 0 : 
      USING_MOCK ::DefWindowProc(_hWnd, _Message, _wParam, _lParam);
  };

  WINAPI_CALL RegisterClassEx(&WindowClass);

  (*this)[Event::Destroy]
    .connect(::std::bind(&EventHandler::OnDestroy, this));
}

/**
* \brief
*  Функция преобразования событий операционной системы в события фреймворка.
*  
* \param [in] _Message
*  Информация о произошедшем событии операционной системы.
*
* \return \b true
*  Событие было обработано.
* \return \b false
*  Событие не было обработано, требуется обработка по умолчанию.
*/
bool EventHandler::CommandHandler(const Message & _Message) noexcept
{
  static const ::std::map<UINT, Event::System> System =
  {
    { WM_CLOSE, Event::Stop },
  };

  auto itSystemCommand = System.find(_Message.Command);
  if (itSystemCommand != System.end())
  {
    return DoCommand(itSystemCommand->second, params::Empty{});
  }

  static const ::std::map<UINT, Event::Window> Window =
  {
    // Не должно быть здесь, Application вызывает Event::Create при запуске, 
    // а Event::Destroy - при завершении цикла обработки сообщений.
    //{ WM_CREATE, Event::Create },
    //{ WM_DESTROY, Event::Destroy },

    { WM_KILLFOCUS, Event::LostFocus },
    { WM_SETFOCUS, Event::GainedFocus },
    { WM_SIZE, Event::Resize },
  };

  auto itWindowCommand = Window.find(_Message.Command);
  if (itWindowCommand != Window.end())
  {
    return DoCommand(itWindowCommand->second, params::Empty{});
  }

  static const ::std::map<UINT, Event::Input> Input =
  {
    { WM_LBUTTONDOWN, Event::Touch },
    { WM_LBUTTONUP, Event::Release },
  };

  auto itInputCommand = Input.find(_Message.Command);
  if (itInputCommand != Input.end())
  {
    return DoCommand(itInputCommand->second, params::Empty{});
  }

  if (_Message.Command == WM_MOUSEMOVE)
  {
    return DoCommand(Event::Motion, params::Motion{ 
      GET_X_LPARAM(_Message.lParam), GET_Y_LPARAM(_Message.lParam) });
  }

  auto GetKeyCode = 
    [&]() ->int32_t { return static_cast<int32_t>(_Message.wParam); };

  if (_Message.Command == WM_CHAR)
  {
    auto KeyCode = GetKeyCode();

    // Это преобразование виртуального кода кнопки ENTER в Unicode код символа 
    // переноса строки.
    if (KeyCode == VK_RETURN) KeyCode = 0x0A;

    // Управляющие символы (кроме перевода строки) должны передавать через 
    // сигнал KeyUp.
    if (KeyCode < 32 && KeyCode != 10) return false;

    return DoCommand(Event::KeyPressed, params::KeyPressed{ KeyCode });
  }

  if (_Message.Command == WM_KEYUP)
  {
    return DoCommand(Event::KeyUp, params::KeyCode{ GetKeyCode() });
  }

  if (_Message.Command == WM_KEYDOWN)
  {
    return DoCommand(Event::KeyDown, params::KeyCode{ GetKeyCode() });
  }

  if (_Message.Command == WM_SYSKEYUP)
  {
    if (GetKeyCode() == VK_LEFT)
    {
      return DoCommand(Event::Back, params::Empty{});
    }
  }

  return false;
}
