
#include "stdafx.h"
#include <Covellite\Core\EventHandler.hpp>
#include <windows.h>
#include <windowsx.h>
#include <alicorn\std\exception.hpp>
#include <alicorn\platform\winapi-check.hpp>
#include <Covellite\Core\ClassName.windows.hpp>
#include <Covellite\Core\ClickEventListener.hpp>

using namespace covellite::core;

static EventHandler * pEvents = nullptr;
static WNDCLASSEX WindowClass = { 0 };

/**
* \brief
*  ������� �������������� ������� ������������ ������� � ������� ����������.
*
* \param [in] _Message
*  ���������� � ������������ ������� ������������ �������.
*
* \return \b true
*  ������� ���� ����������.
* \return \b false
*  ������� �� ���� ����������, ��������� ��������� �� ���������.
*/
bool CommandHandler(UINT _Message, WPARAM _wParam, LPARAM _lParam) noexcept
{
  if (pEvents == nullptr) return false;

  static const ::std::map<UINT, Event::System> System =
  {
    { WM_CLOSE, Event::Stop },
  };

  auto itSystemCommand = System.find(_Message);
  if (itSystemCommand != System.end())
  {
    return pEvents->DoCommand(itSystemCommand->second, params::Empty{});
  }

  static const ::std::map<UINT, Event::Window> Window =
  {
    // �� ������ ���� �����, Application �������� Event::Create ��� �������, 
    // � Event::Destroy - ��� ���������� ����� ��������� ���������.
    //{ WM_CREATE, Event::Create },
    //{ WM_DESTROY, Event::Destroy },

    { WM_KILLFOCUS, Event::LostFocus },
    { WM_SETFOCUS, Event::GainedFocus },
    { WM_SIZE, Event::Resize },
  };

  auto itWindowCommand = Window.find(_Message);
  if (itWindowCommand != Window.end())
  {
    return pEvents->DoCommand(itWindowCommand->second, params::Empty{});
  }

  static const ::std::map<UINT, Event::Input> Input =
  {
    { WM_LBUTTONDOWN, Event::Touch },
    { WM_LBUTTONUP, Event::Release },
  };

  auto itInputCommand = Input.find(_Message);
  if (itInputCommand != Input.end())
  {
    return pEvents->DoCommand(itInputCommand->second, params::Empty{});
  }

  if (_Message == WM_MOUSEMOVE)
  {
    return pEvents->DoCommand(Event::Motion, params::Motion{
      GET_X_LPARAM(_lParam), GET_Y_LPARAM(_lParam) });
  }

  auto GetKeyCode =
    [&]() -> int32_t { return static_cast<int32_t>(_wParam); };

  if (_Message == WM_CHAR)
  {
    auto KeyCode = GetKeyCode();

    // ����������� ������� (����� �������� ������) ������ ���������� ����� 
    // ������ KeyUp.
    if (KeyCode < VK_SPACE && KeyCode != VK_RETURN) return false;

    // ��� �������������� ������������ ���� ������ ENTER � Unicode ��� ������� 
    // �������� ������.
    if (KeyCode == VK_RETURN) KeyCode = 0x0A;

    return pEvents->DoCommand(Event::KeyPressed, params::KeyPressed{ KeyCode });
  }

  if (_Message == WM_KEYUP)
  {
    return pEvents->DoCommand(Event::KeyUp, params::KeyCode{ GetKeyCode() });
  }

  if (_Message == WM_KEYDOWN)
  {
    return pEvents->DoCommand(Event::KeyDown, params::KeyCode{ GetKeyCode() });
  }

  if (_Message == WM_SYSKEYUP)
  {
    if (GetKeyCode() == VK_LEFT)
    {
      return pEvents->DoCommand(Event::Back, params::Empty{});
    }
  }

  return false;
}

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
    const auto Result = ::CommandHandler(_Message, _wParam, _lParam);
    return (Result) ? 0 : 
      USING_MOCK ::DefWindowProc(_hWnd, _Message, _wParam, _lParam);
  };

  WINAPI_CHECK RegisterClassEx(&WindowClass);

  (*this)[Event::Destroy]
    .connect(::std::bind(&EventHandler::OnDestroy, this));
}
