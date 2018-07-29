﻿
#include "stdafx.h"

#include <windows.h>
#undef GetCommandLine

#include <Covellite\App\Application.hpp>
#include <alicorn\platform\winapi-check.hpp>
#include <Covellite\Events.hpp>
#include <Covellite\App\ClassName.windows.hpp>
#include <Covellite\App\Events.hpp>

using namespace covellite::app;

static WNDCLASSEX WindowClass = { 0 };

/**
* \brief
*  Инициализирующий конструктор класса.
*/
Application::Application(const Run_t & _Run) :
  Run(_Run)
{
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
    auto * pEvents = reinterpret_cast<::covellite::events::Events *>(
      USING_MOCK ::GetWindowLongPtr(_hWnd, GWLP_USERDATA));
    if (pEvents != nullptr)
    {
      const ::std::pair<WPARAM, LPARAM> Params{ _wParam, _lParam };
      const auto IsSuccess = (*pEvents)[_Message](Params);
      if (IsSuccess) return 0;
    }

    return USING_MOCK ::DefWindowProc(_hWnd, _Message, _wParam, _lParam);
  };

  WINAPI_CHECK RegisterClassEx(&WindowClass);

  m_Events[events::Application.Exit].Connect([]()
  { 
    PostQuitMessage(0); 
  });
}

/**
* \brief
*  Конструктор класса создания объекта приложения для непрерывной отрисовки 
*  окна программы.
*/
Application::Application(Continuous) :
  Application([&](void) 
  { 
    m_Events[events::Application.Start]();

    MSG Message = { 0 };
    bool IsLostFocus = false;

    while (true)
    {
      const auto Result = (IsLostFocus) ?
        USING_MOCK ::GetMessage(&Message, NULL, 0, 0) :
        USING_MOCK ::PeekMessage(&Message, 0, 0, 0, PM_REMOVE);
      if (Result != FALSE)
      {
        if (Message.message == WM_SETFOCUS) IsLostFocus = false;
        if (Message.message == WM_KILLFOCUS) IsLostFocus = true;
        if (Message.message == WM_QUIT) break;
        USING_MOCK ::TranslateMessage(&Message);
        USING_MOCK ::DispatchMessage(&Message);
      }

      m_Events[events::Application.Update]();

      Sleep(0);
    }
  })
{
}

/**
* \brief
*  Конструктор класса создания объекта приложения для отрисовки окна программы
*  после события.
*/
Application::Application(EventBased) :
  Application([&](void) 
  { 
    m_Events[events::Application.Start]();

    MSG Message = { 0 };

    while (true)
    {
      const auto Result = USING_MOCK ::GetMessage(&Message, NULL, 0, 0);
      if (Result == -1) WINAPI_CHECK FALSE;
      if (Message.message == WM_QUIT) break;

      USING_MOCK ::TranslateMessage(&Message);
      USING_MOCK ::DispatchMessage(&Message);

      m_Events[events::Application.Update]();
    }

    return true;
  })
{
}

/*virtual*/ Application::~Application(void)
{
  USING_MOCK ::UnregisterClass(ClassName, GetModuleHandle(nullptr));
}

/**
* \brief
*  Функция точки входа в программу.
* \details
*  - Функция вызывается внутри макроса APPLICATION_ENTRY_POINT.
*  
* \param [in] _fnCreateApp
*  Функция создания объекта приложения.
* \param [in] _pParams
*  Дополнительные параметры запуска программы.
*/
/*static*/ void Application::Main(CreateApp_t _fnCreateApp, void * /*_pParams*/) noexcept
{
  try
  {
    _fnCreateApp()->Run();
  }
  catch (const ::std::exception & _Ex)
  {
    const auto ErrorDescription = 
      ::std::string{ "Exception: " } + _Ex.what() + ".";
    MessageBoxA(NULL, ErrorDescription.c_str(), "Covellite++", MB_OK);
  }
  catch (...)
  {
    const auto ErrorDescription =
      ::std::string{ "Exception: " } + "unknown" + ".";
    MessageBoxA(NULL, ErrorDescription.c_str(), "Covellite++", MB_OK);
  }
}

/**
* \brief
*  Функция получения параметров командной строки, которые были переданы 
*  программе ее при запуске.
*/
::std::string Application::GetCommandLine(void) const
{
  auto * pCommandLine = GetCommandLineA();
  return (pCommandLine == nullptr) ? "" : pCommandLine;
}

bool Application::PostCommand(bool /*_IsWaitMessage*/)
{
  return true;
}
