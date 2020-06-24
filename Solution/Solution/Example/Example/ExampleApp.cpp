
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
    //LOGGER(Filter) << LOGGER_MIN_LEVEL(Info);
    LOGGER(report::DebugOutput_t);
  }
}

/// [Create main window]
void ExampleApp::DoInitWindow(void)
{
  LOGGER(Info) << "Create main window.";
    
  // Создание обязательного набора объектов окон приложения.
    
  const auto & wOs =                           // Класс фреймворка окна операционной системы.
    MakeWindow<::covellite::os::Window>(*this);     
  const auto & wApi =                          // Класс фреймворка окна графического Api
    MakeWindow<::covellite::api::Window>(wOs); // (реализация DirectXOpenGL задается в настройках).
  auto & wExpanse =                            // Класс фреймворка окна, обеспечивающего отрисовку 3D сцены.
    MakeWindow<::covellite::expanse::Window>(wApi); 
  MakeWindow<BasementWindow>(wApi, wExpanse);  // Клиентский код, формирующий 3D сцену.
  auto & wGui =                                // Класс фреймворка окна, обеспечивающего отрисовку GUI.
    MakeWindow<::covellite::gui::Window>(wApi);     
  MakeWindow<ExampleWindow>(wGui);             // Клиентсткий код программы, управляющий
                                               // переходами между разными слоями GUI.
}
/// [Create main window]

/// [Create entry point]
APPLICATION_ENTRY_POINT(ExampleApp);
/// [Create entry point]
