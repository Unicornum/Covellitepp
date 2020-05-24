
#include "stdafx.h"
#include <unordered_map>
#include <alicorn/std/chrono.mock.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/logger.mock.hpp>
#include <Platform/Windows.mock.hpp>
#include <Platform/Android.mock.hpp>
#include <Covellite/App/Settings.mock.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Window.
*/

#define Window Window_windows
#define RendererImpl RendererImpl_windows
#define IGraphicApi IGraphicApi_windows

#include <Covellite/App.mock.hpp>
#include <Covellite/Os.mock.hpp>

// Расположение класса Window
#include "../../Covellite.Api/Window.cpp"
#include "../../Covellite.Api/Window.windows.cpp"

#include "../Mock/RendererImpl.hpp"

namespace mock
{

namespace windows
{

DEFINE_RENDER_IMPL(Dummy);
DEFINE_RENDER_IMPL(DirectX11);
DEFINE_RENDER_IMPL(OpenGL);

} // namespace windows

} // namespace mock

// Общий тестовый класс класса Renderer
class Window_windows_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::Window;
  using WindowOs_t = ::mock::covellite::os::Window;
  using Data_t = ::covellite::api::renderer::SettingsData;
  using String_t = ::alicorn::extension::std::String;
  using Rect_t = ::covellite::os::IWindow::Rect;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<Rect_t>::Set(Rect_t{ 0, 0, 0, 0 });
    ::testing::DefaultValue<::covellite::Any_t>::Set((HWND)0);
    ::testing::DefaultValue<String_t>::Set(uT("DefaultString"));
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<Rect_t>::Clear();
    ::testing::DefaultValue<::covellite::Any_t>::Clear();
    ::testing::DefaultValue<String_t>::Clear();
  }

protected:
  using App_t = ::mock::covellite::app::Application;
  App_t m_App{ App_t::EventBased{} };
};

// Образец макроса для подстановки в класс Renderer 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Window_windows_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_windows_test, /*DISABLED_*/Test_Dummy)
{
  // Исключительно для того, чтобы создался объект настроек.

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("Dummy")));

  WindowOs_t WindowOs{ m_App };
  Tested_t Example{ WindowOs };
}

// ************************************************************************** //
TEST_F(Window_windows_test, /*DISABLED_*/Test_Constructor_Auto_DirectX11)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1809031242;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("Auto")));

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("DirectX11")))
    .Times(1);

  WindowOs_t WindowOs{ m_App };
  Tested_t Example{ WindowOs };
}

// ************************************************************************** //
TEST_F(Window_windows_test, /*DISABLED_*/Test_Constructor_Auto_OpenGL)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1809031245;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("Auto")));

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("OpenGL")))
    .Times(1);

  WindowOs_t WindowOs{ m_App };
  Tested_t Example{ WindowOs };
}

// ************************************************************************** //
TEST_F(Window_windows_test, /*DISABLED_*/Test_Constructor_Auto_Nothing)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::std::unordered_map<String_t, const char *> Datas =
  {
    { uT("DirectX11"), "Error1809031412" },
    { uT("OpenGL"), "Error1809031414" }
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("Auto")));

  for (const auto & Data : Datas)
  {
    const auto ExpectedToLog = uT("Warning: Create graphics API ") +
      Data.first + uT(" error: ") + String_t::GetString(Data.second);

    EXPECT_CALL(RendererImplProxy, Constructor(_))
      .Times(1)
      .WillOnce(Throw(::std::exception{ Data.second }));

    EXPECT_CALL(LoggerProxy, ToLog(Eq(ExpectedToLog)))
      .Times(1);
  }

  WindowOs_t WindowOs{ m_App };
  EXPECT_THROW(Tested_t{ WindowOs }, ::std::exception);
}
