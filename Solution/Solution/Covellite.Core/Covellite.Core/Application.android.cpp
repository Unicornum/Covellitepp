
#include "stdafx.h"
#include <Covellite\Core\Application.hpp>
#include <Covellite\Core\EventHandler.hpp>

using AppInfo_t = ::alicorn::system::platform::AppInfo;

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
    .connect([](const Params &)
      { ANativeActivity_finish(AppInfo_t::Get<android_app>().activity); });
  (*m_pEvents)[Event::Close]
    .connect([&](const Params &) { m_Windows = Windows_t{}; });
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
        m_pEvents->PostCommandDrawWindow();
      }
    })
{
  // Обслуживание отключения непрерывной отрисовки экрана при потере фокуса
  // программой.
  (*m_pEvents)[Event::LostFocus]
    .connect([&](const ::covellite::core::Params &) { m_IsFocused = false; });
  (*m_pEvents)[Event::GainedFocus]
    .connect([&](const ::covellite::core::Params &) { m_IsFocused = true; });
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
        m_pEvents->PostCommandDrawWindow();
      }
    })
{
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
bool Application::PostCommand(bool _IsWaitMessage) const
{
  android_poll_source * pSource = nullptr;

  const auto Result = ALooper_pollAll((_IsWaitMessage) ? -1 : 0, 
    nullptr, nullptr, (void **)&pSource);
  if (Result < 0 || pSource == nullptr) return false;

  pSource->process(&AppInfo_t::Get<android_app>(), pSource);

  return true;
}
