
#include "stdafx.h"

#include <windows.h>
#undef GetCommandLine

#include <Covellite\Core\Application.hpp>
#include <alicorn\platform\winapi-call.hpp>
#include <Covellite\Core\EventHandler.hpp>

using namespace covellite::core;

/**
* \brief
*  Инициализирующий конструктор класса.
*/
Application::Application(const Run_t & _Run) :
  Run(_Run),
  m_pEvents(::std::make_shared<covellite::core::EventHandler>())
{
  (*m_pEvents)[Event::Stop]
    .connect([](const Params &) { PostQuitMessage(0); });
  (*m_pEvents)[Event::Close]
    .connect([&](const Params &) { m_Windows = Windows_t{}; });
}

/**
* \brief
*  Конструктор класса создания объекта приложения для непрерывной отрисовки 
*  окна программы.
*/
Application::Application(Continuous) :
  Application([&](void) { PostCommand(false); })
{
}

/**
* \brief
*  Конструктор класса создания объекта приложения для отрисовки окна программы
*  после события.
*/
Application::Application(EventBased) :
  Application([&](void) { PostCommand(true); })
{
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

bool Application::PostCommand(bool _IsWaitMessage) const
{
  m_pEvents->DoCommand(Event::Create, params::Empty{});

  while (true)
  {
    MSG Message;

    const auto Result = (_IsWaitMessage) ?
      USING_MOCK ::GetMessage(&Message, NULL, 0, 0) :
      USING_MOCK ::PeekMessage(&Message, 0, 0, 0, PM_REMOVE);
    if (Result == -1) WINAPI_CALL FALSE;
    if (_IsWaitMessage || Result != FALSE)
    {
      if (Message.message == WM_QUIT) break;
      USING_MOCK ::TranslateMessage(&Message);

      // 23 Октябрь 2017 13:39 (unicornum.verum@gmail.com)
      TODO("Напрямую вызывать m_pEvents->CommandHandler()?");
      USING_MOCK ::DispatchMessage(&Message);
    }

    m_pEvents->PostCommandDrawWindow();
    Sleep(0);
  }

  m_pEvents->DoCommand(Event::Close, params::Empty{});
  m_pEvents->DoCommand(Event::Destroy, params::Empty{});

  return true;
}
