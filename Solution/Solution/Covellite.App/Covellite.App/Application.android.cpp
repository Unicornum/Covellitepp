﻿
#include "stdafx.h"
#include <Covellite/App/Application.hpp>
#include <Covellite/Events.hpp>
#include <Covellite/App/Events.hpp>

using AppInfo_t = ::alicorn::system::platform::AppInfo;

using namespace covellite::app;

/**
* \brief
*  Инициализирующий конструктор класса.
*/
Application::Application(const Run_t & _Run) :
  Run(_Run)
{
  auto & App = AppInfo_t::Get<android_app>();

  App.userData = this;
  App.onAppCmd = [](android_app * _pApp, int32_t _Message) 
  {
    if (_pApp == nullptr || _pApp->userData == nullptr) return;
    auto & Events = *(reinterpret_cast<Application *>(_pApp->userData));

    Events.m_Events[_Message]();
  };

  App.onInputEvent = [](android_app * _pApp, AInputEvent * _pEvent) -> int32_t
  {
    if (_pApp == nullptr || _pApp->userData == nullptr) return 0;
    auto & Events = *(reinterpret_cast<Application *>(_pApp->userData));

    const auto EventType = AInputEvent_getType(_pEvent);
    const auto ActionType = AMotionEvent_getAction(_pEvent);

    const auto IsSuccess = Events.m_Events[(EventType << 8) | ActionType](_pEvent);
    return (IsSuccess) ? 1 : 0;
  };

  m_Events[(int32_t)APP_CMD_INIT_WINDOW]
    .Connect([&]() { m_Events[events::Application.Start](); });
  m_Events[(int32_t)APP_CMD_TERM_WINDOW]
    .Connect([&]() { m_Events[events::Application.Exit](); });
  m_Events[events::Application.Exit]
    .Connect([=]() { ANativeActivity_finish(App.activity); });
}

/**
* \brief
*  Конструктор класса создания объекта приложения для непрерывной отрисовки 
*  окна программы.
*/
Application::Application(Continuous) :
  Application([&](void)
    {
      auto & DestroyRequested = AppInfo_t::Get<android_app>().destroyRequested;

      m_IsFocused = true;

      while (DestroyRequested == 0)
      {
        while (PostCommand(!m_IsFocused)) {}
        m_Events[events::Application.Update]();
      }
    })
{
  // Обслуживание отключения непрерывной отрисовки экрана при потере фокуса
  // программой.
  m_Events[(int32_t)APP_CMD_LOST_FOCUS]
    .Connect([&]() { m_IsFocused = false; });
  m_Events[(int32_t)APP_CMD_GAINED_FOCUS]
    .Connect([&]() { m_IsFocused = true; });
}

/**
* \brief
*  Конструктор класса создания объекта приложения для отрисовки окна программы
*  после события.
*/
Application::Application(EventBased) :
  Application([&](void)
    {
      auto & DestroyRequested = AppInfo_t::Get<android_app>().destroyRequested;

      while (DestroyRequested == 0)
      {
        PostCommand(true);
        m_Events[events::Application.Update]();
      }
    })
{
}

/*virtual*/ Application::~Application(void)
{
  auto & App = AppInfo_t::Get<android_app>();

  App.onAppCmd = nullptr;
  App.onInputEvent = nullptr;
  App.userData = nullptr;
}

/**
* \brief
*  Платформозависимая реализация функции точки входа в программу.
*  
* \param [in] _fnCreateApp
*  Функция создания объекта приложения.
* \param [in] _pParams
*  Дополнительные параметры запуска программы.
*/
/*static*/ void Application::Main(CreateApp_t _fnCreateApp, void * _pParams) noexcept
{
  class Logcat
  {
  public:
    inline static void Error(const char * _Format, ...)
    {
      va_list Args;
      va_start(Args, _Format);
      (void)__android_log_print(ANDROID_LOG_ERROR, "Covellite++", _Format, Args);
      va_end(Args);
    }

    inline static void Info(const char * _Format, ...)
    {
      va_list Args;
      va_start(Args, _Format);
      (void)__android_log_print(ANDROID_LOG_INFO, "Covellite++", _Format, Args);
      va_end(Args);
    }
  };

  // Использование логгера Android напрямую, т.к. логгер Alicorn здесь еще
  // не инициализирован.
  Logcat::Info("Application::Main(): start program.");

  try
  {
    auto * pApp = reinterpret_cast<android_app *>(_pParams);

    // 01 Июль 2018 13:13 (unicornum.verum@gmail.com)
    TODO("Убрать инициализацию объектов ,которые уже не нужны.");

    const AppInfo_t Info
    {
      pApp, 
      pApp->activity,
      pApp->activity->vm,
      &pApp->activity, 
      &pApp->window, 
      &pApp->config,
    };

    _fnCreateApp()->Run();
  }
  catch (const ::std::exception & _Ex)
  {
    Logcat::Error("Application::Main(): error [%s].", _Ex.what());
  }
  catch (...)
  {
    Logcat::Error("Application::Main(): error [unknown].");
  }

  Logcat::Info("Application::Main(): finish program.");
}

/**
* \details
*  - Для Android функция не имеет смысла.
*/
::std::string Application::GetCommandLine(void) const
{
  return "";
}

/**
* \brief
*  Функция вызова сигнала произошедшего события..
* \details
*  - При появлении события будет вызвана функция EventHandler::CommandHandler(), 
*  вторым параметром которой будет передан идентификатор произошедшего события
*  или или EventHandler::InputHandler().
*  
* \param [in] _IsWaitMessage
*  Признак того, нужно или нет ждать появления события.
*  
* \return \b true
*  - Появилось событие и был вызыван обработчик этого события.
* \return \b false
*  - События не было.
*/
bool Application::PostCommand(bool _IsWaitMessage)
{
  android_poll_source * pSource = nullptr;

  const auto Result = ALooper_pollAll((_IsWaitMessage) ? -1 : 0, 
    nullptr, nullptr, (void **)&pSource);
  if (Result < 0 || pSource == nullptr) return false;

  pSource->process(&AppInfo_t::Get<android_app>(), pSource);

  return true;
}
