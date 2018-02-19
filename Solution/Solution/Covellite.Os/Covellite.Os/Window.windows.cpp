
#include "stdafx.h"
#include <Covellite\Os\Window.hpp>
#include <alicorn\std\string.hpp>
#include <alicorn\std\string-cast.hpp>
#include <alicorn\platform\winapi-call.hpp>
#include <alicorn\version.hpp>
#include <Covellite\Core\ClassName.windows.hpp>
#include <Covellite\Core\Settings.hpp>

#undef CreateWindow

namespace covellite
{

namespace os
{

static Handle_t CreateWindow(void)
{
  using Info_t = alicorn::system::version::Info;
  using namespace ::alicorn::extension::std;

  const auto ApplicationName = 
    string_cast<::std::wstring>(Info_t{}.GetValue(uT("ApplicationName")));

  const auto ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
  const auto ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

  const auto CovelliteppSection =
    ::covellite::core::Settings::GetInstance().GetFrameworkSection();
  const auto WindowSection = CovelliteppSection[uT("Window")];

  auto WindowFlags = WS_POPUP;
  auto WindowFlagsEx = 0;

  auto X = 0;
  auto Y = 0;
  auto WindowWidth = ScreenWidth;
  auto WindowHeight = ScreenHeight;

  const auto IsFullScreen = WindowSection.Get<String>(uT("IsFullScreen"));
  if (IsFullScreen == uT("false"))
  {
    const auto & SizeSection = WindowSection[uT("Size")];

    const auto ClientWidth = SizeSection.Get<int>(uT("Width"));
    const auto ClientHeight = SizeSection.Get<int>(uT("Height"));

    WindowFlags = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;
    WindowFlagsEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

    RECT WindowRect = { 0, 0, ClientWidth, ClientHeight };
    WINAPI_CALL USING_MOCK(AdjustWindowRectEx(&WindowRect,
      WindowFlags, FALSE, WindowFlagsEx));

    const auto CalculatedWindowWidth = WindowRect.right - WindowRect.left;
    WindowWidth = (CalculatedWindowWidth > ScreenWidth) ?
      ScreenWidth : CalculatedWindowWidth;

    const auto CalculatedWindowHeight = WindowRect.bottom - WindowRect.top;
    WindowHeight = (CalculatedWindowHeight > ScreenHeight) ?
      ScreenHeight : CalculatedWindowHeight;

    X = (ScreenWidth - WindowWidth) / 2;
    Y = (ScreenHeight - WindowHeight) / 2;
  }

  const auto hWnd = USING_MOCK(CreateWindowEx(WindowFlagsEx, 
    ::covellite::core::ClassName, ApplicationName.c_str(), WindowFlags,
    X, Y, WindowWidth, WindowHeight, nullptr, nullptr, 
    GetModuleHandle(nullptr), nullptr));
  WINAPI_CALL (hWnd != NULL);  

  // Игнорирование предупреждений при анализе кода (анализатор не понимает,
  // что макрос WINAPI_CALL вызывает исключение при нулевом значении 
  // m_Handle и что в этом случае последующий код не может быть выполнен).
# pragma warning(push)
# pragma warning(disable: 6387)
  USING_MOCK(ShowWindow(hWnd, SW_SHOW));
# pragma warning(pop)

  return hWnd;
}

Window::Window(void) :
  m_Handle(::covellite::os::CreateWindow())
{
}

Window::~Window(void) noexcept
{
  USING_MOCK(DestroyWindow(m_Handle));
}

} // namespace os

} // namespace covellite
