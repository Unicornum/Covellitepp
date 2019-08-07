
#include "stdafx.h"
#include "ExampleApp.hpp"
#include <alicorn/version.hpp>
#include <alicorn/application/current-module.hpp>
#include <alicorn/logger.hpp>
#include "ExampleWindow.hpp"
#include "BasementWindow.hpp"

/// [Constructor main application class]
ExampleApp::ExampleApp(void) noexcept :
  Application(Continuous{})
{
  using namespace ::covellite;
    
  m_Events[events::Application.Start]
    .Connect([&]() { DoInitWindow(); } );
  m_Events[events::Error.Exception]
    .Connect([](const events::Error_t::Description & _Description)
      { LOGGER(Error) << _Description.c_str(); });
  // ...
  /// [Constructor main application class]

  using ::alicorn::system::application::CurrentModule;

  // В Android будет ошибка, игнорируем ее.
  ::boost::system::error_code ErrorCode;
  ::boost::filesystem::current_path(CurrentModule::GetAppRootPath(), ErrorCode);

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
    LOGGER(MaxLogFiles) << 20;
    LOGGER(MaxLogSize) << 10 * 1024 * 1024;
    LOGGER(report::Html);
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
    
  MakeWindow<ExampleWindow>(                         // Клиентсткий код программы, управляющий
                                                     // переходами между разными слоями GUI.
    MakeWindow<::covellite::gui::Window>(            // Класс фреймворка, обеспечивающий отрисовку GUI.
      MakeWindow<BasementWindow>(                    // Клиентский код, отрисовывающий 3D сцену.
        MakeWindow<::covellite::api::Window>(        // Класс фреймворка окна графического Api
                                                     // (реализация (OpenGL или DirectX) задается в настройках).
          MakeWindow<::covellite::os::Window>(*this) // Класс фреймворка окна операционной системы.
            ))));
}
/// [Create main window]

/// [Create entry point]
APPLICATION_ENTRY_POINT(ExampleApp);
/// [Create entry point]
