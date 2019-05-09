
#include "stdafx.h"
#include <unordered_map>

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Window.
*/

#define Window Window_android
#define RendererImpl RendererImpl_android
#define IGraphicApi IGraphicApi_android

#include <Covellite/Os.mock.hpp>

// ������������ ������ Window
#include "../../Covellite.Api/Window.cpp"
#include "../../Covellite.Api/Window.android.cpp"

#include "../Mock/RendererImpl.hpp"

namespace mock
{

namespace android
{

DEFINE_RENDER_IMPL(Dummy);
DEFINE_RENDER_IMPL(Vulkan);
DEFINE_RENDER_IMPL(OpenGLES);

} // namespace android

} // namespace mock

// ����� �������� ����� ������ Renderer
class Window_android_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::Window;
  using WindowOs_t = ::mock::covellite::os::Window;
  using String_t = ::alicorn::extension::std::String;
  using Rect_t = ::covellite::os::IWindow::Rect;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
    ::testing::DefaultValue<Rect_t>::Set(Rect_t{ 0, 0, 0, 0 });
    ::testing::DefaultValue<::covellite::Any_t>::Set((HWND)0);
    ::testing::DefaultValue<String_t>::Set(uT("DefaultString"));
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
    ::testing::DefaultValue<Rect_t>::Clear();
    ::testing::DefaultValue<::covellite::Any_t>::Clear();
    ::testing::DefaultValue<String_t>::Clear();
  }
};

// ������� ������� ��� ����������� � ����� Renderer 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Window_android_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_android_test, /*DISABLED_*/Test_Dummy)
{
  // ������������� ��� ����, ����� �������� ������ ��������.

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("Dummy")));

  WindowOs_t WindowOs;
  Tested_t Example{ WindowOs };
}

// ************************************************************************** //
TEST_F(Window_android_test, /*DISABLED_*/Test_Constructor_Auto_Vulkan)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1809031257;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillOnce(Return(uT("Auto")));

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("Vulkan")))
    .Times(1);

  WindowOs_t WindowOs;
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
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("OpenGLES")))
    .Times(1);

  WindowOs_t WindowOs;
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
    { uT("Vulkan"), "Error1809031434" },
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

  WindowOs_t WindowOs;
  EXPECT_THROW(Tested_t{ WindowOs }, ::std::exception);
}