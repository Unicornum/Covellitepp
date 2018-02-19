
#include "stdafx.h"
#include "ExampleApp.hpp"
#include <alicorn\logger.hpp>
#include <alicorn\version.hpp>
#include "ExampleWindow.hpp"
#include "ExtraWindow.hpp"

namespace alicorn { namespace modules { namespace logger { namespace report {

#if BOOST_OS_WINDOWS
using DebugOutput_t = windows::VisualStudioOutput;
#elif BOOST_OS_ANDROID
using DebugOutput_t = android::LogCat;
#endif

} } } }

/// [Constructor main application class]
ExampleApp::ExampleApp(void) :
  Application(EventBased{})
{
  using ::covellite::core::Event;
    
  (*m_pEvents)[Event::Create]
    .connect(::std::bind(&ExampleApp::DoInitWindow, this));
  (*m_pEvents)[Event::Error]
    .connect([](const ::covellite::core::params::Error & _Description)
      { LOGGER(Error) << _Description.Description.c_str(); });
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
# if BOOST_OS_WINDOWS
  using ApiImpl_t = ::covellite::api::OpenGL;
# elif BOOST_OS_ANDROID
  using ApiImpl_t = ::covellite::api::OpenGLES;
# endif
    
  LOGGER(Info) << "Create main window.";
    
  // Дополнительный код (для примера), производящий заливку окна цветом перед 
  // отрисовкой GUI.
  MakeWindow<ExtraWindow>();
    
  // Создание обязательного набора объектов окон приложения.
  MakeWindow<ExampleWindow>(
    MakeWindow<::covellite::rocket::Window>(
      MakeWindow<::covellite::api::Window<ApiImpl_t>>(
        MakeWindow<::covellite::os::Window>())));
}
/// [Create main window]

/// [Create entry point]
APPLICATION_ENTRY_POINT(ExampleApp);
/// [Create entry point]
