
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

#define Window Window_android
#define RendererImpl RendererImpl_android
#define IGraphicApi IGraphicApi_android

#include <Covellite/App.mock.hpp>
#include <Covellite/Os.mock.hpp>

// Расположение класса Window
#include "../../Covellite.Api/Window.cpp"
#include "../../Covellite.Api/Window.android.cpp"

#include "../Mock/RendererImpl.hpp"

namespace mock
{

namespace android
{

DEFINE_RENDER_IMPL(Dummy);
DEFINE_RENDER_IMPL(OpenGLES3);
DEFINE_RENDER_IMPL(OpenGLES2);
DEFINE_RENDER_IMPL(OpenGLES);

} // namespace android

} // namespace mock

// Общий тестовый класс класса Renderer
class Window_android_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::Window;
  using WindowOs_t = ::mock::covellite::os::Window;
  using String_t = ::alicorn::extension::std::String;
  using Rect_t = ::covellite::Rect;

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
// FRIEND_TEST(Window_android_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_android_test, /*DISABLED_*/Test_Dummy)
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
TEST_F(Window_android_test, /*DISABLED_*/Test_Constructor_Auto_OpenGLES3)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1908091012;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillOnce(Return(uT("Auto")));

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("OpenGLES3")))
    .Times(1);

  WindowOs_t WindowOs{ m_App };
  Tested_t Example{ WindowOs };
}

// ************************************************************************** //
TEST_F(Window_android_test, /*DISABLED_*/Test_Constructor_Auto_OpenGLES2)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1908091012;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillOnce(Return(uT("Auto")));

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("OpenGLES2")))
    .Times(1);

  WindowOs_t WindowOs{ m_App };
  Tested_t Example{ WindowOs };
}

// ************************************************************************** //
TEST_F(Window_android_test, /*DISABLED_*/Test_Constructor_Auto_OpenGLES)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1809031258;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillOnce(Return(uT("Auto")));

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("OpenGLES")))
    .Times(1);

  WindowOs_t WindowOs{ m_App };
  Tested_t Example{ WindowOs };
}

// ************************************************************************** //
TEST_F(Window_android_test, /*DISABLED_*/Test_Constructor_Auto_Nothing)
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
    { uT("OpenGLES3"), "Error1908131253" },
    { uT("OpenGLES2"), "Error1908091010" },
    { uT("OpenGLES"), "Error1809031435" }
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillOnce(Return(uT("Auto")));

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
