
#include "stdafx.h"
#include "ExampleApp.hpp"
#include <alicorn\logger.hpp>
#include <alicorn\version.hpp>
#include "ExampleWindow.hpp"
#include "ExtraWindow.hpp"

/// [Constructor main application class]
ExampleApp::ExampleApp(void) :
  Application(EventBased{})
{
  using namespace ::covellite;
    
  m_Events[events::Application.Start]
    .Connect([&]() { DoInitWindow(); } );
  m_Events[events::Error.Exception]
    .Connect([](const events::Error_t::Description & _Description)
      { LOGGER(Error) << _Description.c_str(); });
  // ...
  /// [Constructor main application class]

  DoInitLogger();

  LOGGER(Info) << "Start program.";
}

ExampleApp::~ExampleApp(void) noexcept
{
  LOGGER(Info) << "Finish program.";
}

void ExampleApp::DoInitLogger(void)
{
  if (::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION)
  {
    LOGGER(Filter) << LOGGER_MIN_LEVEL(Info);
    LOGGER(report::Html);
    LOGGER(MaxLogFiles) << 20;
    LOGGER(MaxLogSize) << 10 * 1024 * 1024;
  }
  else
  {
    LOGGER(Filter) << LOGGER_MIN_LEVEL(Trace);
    LOGGER(report::DebugOutput_t);
  }
}

/// [Create main window]
void ExampleApp::DoInitWindow(void)
{
  LOGGER(Info) << "Create main window.";
    
  // Создание обязательного набора объектов окон приложения.
  MakeWindow<ExampleWindow>(
    MakeWindow<::covellite::rocket::Window>(
      // Дополнительный код (для примера), производящий заливку окна цветом 
      // перед отрисовкой GUI.
      MakeWindow<ExtraWindow>(
        MakeWindow<::covellite::api::Window>(
          MakeWindow<::covellite::os::Window>(*this)))));
}
/// [Create main window]

/// [Create entry point]
APPLICATION_ENTRY_POINT(ExampleApp);
/// [Create entry point]
