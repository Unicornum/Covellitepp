
#include "stdafx.h"
#include "ExampleApp.hpp"
#include <alicorn\logger.hpp>
#include <alicorn\version.hpp>
#include "ExampleWindow.hpp"
#include "ExtraWindow.hpp"

/// [Constructor main application class]
ExampleApp::ExampleApp(void) noexcept :
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
    
  MakeWindow<ExampleWindow>(                          // Клиенсткий код программы.
    MakeWindow<::covellite::gui::Window>(          // Класс, обеспечивающий отрисовку GUI
      MakeWindow<ExtraWindow>(                        // Дополнительный код (для примера), производящий 
                                                      // заливку окна цветом перед отрисовкой GUI.
        MakeWindow<::covellite::api::Window>(         // Класс окна графического Api
                                                      // (OpenGL или DirectX, задается в настройках).
          MakeWindow<::covellite::os::Window>(*this)  // Класс окна операционной системы.
            ))));
}
/// [Create main window]

/// [Create entry point]
APPLICATION_ENTRY_POINT(ExampleApp);
/// [Create entry point]
