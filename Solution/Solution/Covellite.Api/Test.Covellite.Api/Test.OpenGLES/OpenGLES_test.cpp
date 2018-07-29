
#include "stdafx.h"
#include <alicorn/patterns/factory.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса OpenGLES.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define OpenGLCommon OpenGLCommon_android

#include <Covellite.Egl\Egl.mock.hpp>
#include <Covellite\Os.mock.hpp>
#include <Covellite\Os\Events.hpp>
#include <Covellite\App\Events.hpp>
#include <Covellite\Api\Events.hpp>

// Расположение класса OpenGLES
#include "..\..\Covellite.Api\OpenGLES.cpp"
#include "..\..\Covellite.Api\OpenGLCommon.cpp"

// Общий тестовый класс класса OpenGLES
class OpenGLES_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::OpenGLES;
  using IWindowCore_t = ::covellite::core::IWindow;
  using ITestedApi_t = ::covellite::api::IWindow;
  using IWindowOs_t = ::covellite::os::IWindow;
  using WindowOs_t = ::mock::covellite::os::Window;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  using String_t = ::alicorn::extension::std::String;
  using Events_t = ::covellite::events::Events;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<String_t>::Set(uT("0"));
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<String_t>::Clear();
  }
};

// Образец макроса для подстановки в класс OpenGLES 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(OpenGLES_test, Test_Function);

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_RegisterIntoFactory)
{
  using namespace ::alicorn::modules::patterns;
  
  const WindowOs_t WindowOs;
  const IWindowOs_t & IWindowOs = WindowOs;
  auto pExample = factory::make_unique<ITestedApi_t>(uT("OpenGLES"), IWindowOs);
  EXPECT_NO_THROW(dynamic_cast<Tested_t &>(*pExample));
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_Constructor_Destructor)
{
  using DisplayProxy_t = ::mock::covellite::egl::Display::Proxy;
  DisplayProxy_t DisplayProxy;
  DisplayProxy_t::GetInstance() = &DisplayProxy;

  using ConfigProxy_t = ::mock::covellite::egl::Config::Proxy;
  ConfigProxy_t ConfigProxy;
  ConfigProxy_t::GetInstance() = &ConfigProxy;

  using SurfaceProxy_t = ::mock::covellite::egl::Surface::Proxy;
  SurfaceProxy_t SurfaceProxy;
  SurfaceProxy_t::GetInstance() = &SurfaceProxy;

  using ContextProxy_t = ::mock::covellite::egl::Context::Proxy;
  ContextProxy_t ContextProxy;
  ContextProxy_t::GetInstance() = &ContextProxy;

  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  ::android_app App;

  const AppInfo_t Info{ &App };

  ANativeWindow NativeWindow;
  const ::mock::Id_t DisplayId = 1611232246;
  const ::mock::Id_t ConfigId = 1611232252;
  const ::mock::Id_t SurfaceId = 1611232253;
  const ::mock::Id_t ContextId = 1611232254;
  const ::mock::Id_t WindowOsId = 1709282313;

  using namespace ::testing;

  EXPECT_CALL(WindowOsProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowOsId));

  const WindowOs_t WindowOs;

  EXPECT_CALL(WindowOsProxy, GetHandle(WindowOsId))
    .Times(2)
    .WillRepeatedly(Return(&NativeWindow));

  InSequence Dummy;

  EXPECT_CALL(DisplayProxy, Constructor())
    .Times(1)
    .WillOnce(Return(DisplayId));

  EXPECT_CALL(ConfigProxy, Constructor(DisplayId, &NativeWindow))
    .Times(1)
    .WillOnce(Return(ConfigId));

  EXPECT_CALL(SurfaceProxy, Constructor(DisplayId, ConfigId, &NativeWindow))
    .Times(1)
    .WillOnce(Return(SurfaceId));

  EXPECT_CALL(ContextProxy, Constructor(DisplayId, ConfigId))
    .Times(1)
    .WillOnce(Return(ContextId));

  EXPECT_CALL(ContextProxy, MakeCurrent(ContextId, SurfaceId))
    .Times(1);

  const Tested_t Example{ WindowOs };

  EXPECT_CALL(ContextProxy, Destructor(ContextId))
    .Times(1);

  EXPECT_CALL(SurfaceProxy, Destructor(SurfaceId))
    .Times(1);

  EXPECT_CALL(ConfigProxy, Destructor(ConfigId))
    .Times(1);

  EXPECT_CALL(DisplayProxy, Destructor(DisplayId))
    .Times(1);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_GetEvents)
{
  class Proxy
  {
  public:
    MOCK_METHOD0(DoEvent, void(void));
  };

  Proxy oProxy;
  static const int Id = 1807221314;

  const WindowOs_t WindowOs;
  Events_t SourceEvents = WindowOs;
  SourceEvents[Id].Connect([&]() { oProxy.DoEvent(); });
  SourceEvents[::covellite::events::Error.Exception].Connect([]() {});

  const Tested_t Example{ WindowOs };

  using namespace ::testing;

  EXPECT_CALL(oProxy, DoEvent())
    .Times(1);

  Events_t ExampleEvents = Example;
  ExampleEvents[Id]();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_WindowResize)
{
  using SurfaceProxy_t = ::mock::covellite::egl::Surface::Proxy;
  SurfaceProxy_t SurfaceProxy;
  SurfaceProxy_t::GetInstance() = &SurfaceProxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  class Tested :
    public Tested_t
  {
  public:
    MOCK_CONST_METHOD0(GetClientRect, Rect(void));

  public:
    explicit Tested(const WindowOs_t & _Window) :
      Tested_t(_Window)
    {

    }
  };

  const ::mock::Id_t SurfaceId = 1709272210;
  const int Left = 10291057;
  const int Top = 10291058;
  const int Width = 1612211039;
  const int Height = 1612211040;
  const Tested_t::Rect Rect = { Left, Top, Width, Height };

  android_app App;

  const AppInfo_t Info{ &App };

  using namespace ::testing;

  EXPECT_CALL(SurfaceProxy, Constructor(_, _, _))
    .Times(1)
    .WillOnce(Return(SurfaceId));

  const WindowOs_t WindowOs;
  const Tested Example{ WindowOs };

  InSequence Dummy;

  EXPECT_CALL(Example, GetClientRect())
    .Times(1)
    .WillOnce(Return(Rect));

  EXPECT_CALL(GLProxy, Viewport(0, 0, Width, Height))
    .Times(1);

  EXPECT_CALL(GLProxy, MatrixMode(GL_PROJECTION))
    .Times(1);

  EXPECT_CALL(GLProxy, LoadIdentity())
    .Times(1);

  EXPECT_CALL(GLProxy, MatrixMode(GL_MODELVIEW))
    .Times(1);

  EXPECT_CALL(GLProxy, LoadIdentity())
    .Times(1);

  EXPECT_CALL(GLProxy, Orthof(0, (float)Width, (float)Height, 0, -1, 1))
    .Times(1);

  Events_t Events = Example;
  Events[::covellite::events::Window.Resize]();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_ApplicationUpdate)
{
  using SectionImplProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SectionImplProxy_t SectionImplProxy;
  SectionImplProxy_t::GetInstance() = &SectionImplProxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  using SurfaceProxy_t = ::mock::covellite::egl::Surface::Proxy;
  SurfaceProxy_t SurfaceProxy;
  SurfaceProxy_t::GetInstance() = &SurfaceProxy;

  class Tested :
    public Tested_t
  {
  public:
    MOCK_CONST_METHOD0(DoDrawing, void(void));
    MOCK_CONST_METHOD0(DoException, void(void));

  public:
    explicit Tested(const WindowOs_t & _Window) :
      Tested_t(_Window)
    {
    }
  };

  const ::mock::Id_t WindowSectionId = 1711061210;
  const ::mock::Id_t BackgroundColorSectionId = 1711061211;
  const ::mock::Id_t SurfaceId = 1611232302;

  android_app App;

  const AppInfo_t Info{ &App };

  using namespace ::testing;

  {
    InSequence Dummy;

    EXPECT_CALL(SectionImplProxy, GetChildSectionImpl(_, uT("Window")))
      .Times(1);

    EXPECT_CALL(SectionImplProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowSectionId));

    EXPECT_CALL(SectionImplProxy,
      GetChildSectionImpl(WindowSectionId, uT("BackgroundColor")))
      .Times(1);

    EXPECT_CALL(SectionImplProxy, Constructor())
      .Times(1)
      .WillOnce(Return(BackgroundColorSectionId));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("R")))
      .Times(1)
      .WillOnce(Return(uT("25.5")));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("G")))
      .Times(1)
      .WillOnce(Return(uT("51")));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("B")))
      .Times(1)
      .WillOnce(Return(uT("76.5")));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("A")))
      .Times(1)
      .WillOnce(Return(uT("102")));
  }

  EXPECT_CALL(SurfaceProxy, Constructor(_, _, _))
    .Times(1)
    .WillOnce(Return(SurfaceId));

  const WindowOs_t WindowOs;
  const Tested Example{ WindowOs };
  Events_t Events = Example;

  using namespace ::covellite::events;

  Events[Drawing.Do].Connect([&]() { Example.DoDrawing(); });
  Events[Error.Exception].Connect([&]() { Example.DoException(); });

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Disable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, ClearColor(0.1f, 0.2f, 0.3f, 0.4f))
    .Times(1);

  EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
    .Times(1);

  EXPECT_CALL(Example, DoDrawing())
    .Times(1);

  EXPECT_CALL(SurfaceProxy, SwapBuffers(SurfaceId))
    .Times(1);

  Events[::covellite::events::Application.Update]();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_GetUsingApi)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  ::android_app App;

  const AppInfo_t Info{ &App };

  using namespace ::alicorn::extension::std;

  const char * Version = "Version1710311111";
  const auto Expected = uT("OpenGL ES %VERSION%").Replace(uT("%VERSION%"),
    string_cast<String, Locale::Default>(::std::string{ Version }));

  using namespace ::testing;

  const WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };
  const ITestedApi_t & IExample = Example;

  EXPECT_CALL(GLProxy, GetString(GL_VERSION))
    .Times(1)
    .WillOnce(Return(reinterpret_cast<const unsigned char *>(Version)));

  const auto Result = IExample.GetUsingApi();
  EXPECT_EQ(Expected, Result);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_GetWidth)
{
  using SurfaceProxy_t = ::mock::covellite::egl::Surface::Proxy;
  SurfaceProxy_t SurfaceProxy;
  SurfaceProxy_t::GetInstance() = &SurfaceProxy;

  ::android_app App;

  const AppInfo_t Info{ &App };

  const int32_t Width = 1611232258;
  const ::mock::Id_t SurfaceId = 1611232259;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SurfaceProxy, Constructor(_, _, _))
    .Times(1)
    .WillOnce(Return(SurfaceId));

  const WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };

  EXPECT_CALL(SurfaceProxy, GetWidth(SurfaceId))
    .Times(1)
    .WillOnce(Return(Width));

  const ITestedApi_t & IExample = Example;
  const auto Result = IExample.GetWidth();
  EXPECT_EQ(Width, Result);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_GetHeight)
{
  using SurfaceProxy_t = ::mock::covellite::egl::Surface::Proxy;
  SurfaceProxy_t SurfaceProxy;
  SurfaceProxy_t::GetInstance() = &SurfaceProxy;

  ::android_app App;

  const AppInfo_t Info{ &App };

  const int32_t Height = 1611232300;
  const ::mock::Id_t SurfaceId = 1611232301;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SurfaceProxy, Constructor(_, _, _))
    .Times(1)
    .WillOnce(Return(SurfaceId));

  const WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };

  EXPECT_CALL(SurfaceProxy, GetHeight(SurfaceId))
    .Times(1)
    .WillOnce(Return(Height));

  const ITestedApi_t & IExample = Example;
  const auto Result = IExample.GetHeight();
  EXPECT_EQ(Height, Result);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_GetClientRect)
{
  using SurfaceProxy_t = ::mock::covellite::egl::Surface::Proxy;
  SurfaceProxy_t SurfaceProxy;
  SurfaceProxy_t::GetInstance() = &SurfaceProxy;

  using EnvironmentProxy_t = ::alicorn::system::platform::proxy::Environment;
  EnvironmentProxy_t EnvironmentProxy;
  EnvironmentProxy_t::GetInstance() = &EnvironmentProxy;

  ::android_app App;

  const AppInfo_t Info{ &App };

  const int32_t Top = 1807221323;
  const int32_t Width = 1611232258;
  const int32_t Height = 1611232300;
  const ::mock::Id_t SurfaceId = 1611232259;
  const ::mock::Id_t EnvironmentId = 1709291212;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SurfaceProxy, Constructor(_, _, _))
    .Times(1)
    .WillOnce(Return(SurfaceId));

  const WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };

  EXPECT_CALL(EnvironmentProxy, Constructor())
    .Times(1)
    .WillOnce(Return(EnvironmentId));

  EXPECT_CALL(EnvironmentProxy, GetStatusBarHeight(EnvironmentId))
    .Times(1)
    .WillOnce(Return(Top));

  EXPECT_CALL(SurfaceProxy, GetWidth(SurfaceId))
    .Times(1)
    .WillOnce(Return(Width));

  EXPECT_CALL(SurfaceProxy, GetHeight(SurfaceId))
    .Times(1)
    .WillOnce(Return(Height));

  const ITestedApi_t & IExample = Example;
  const auto Result = IExample.GetClientRect();
  EXPECT_EQ(0, Result.Left);
  EXPECT_EQ(Top, Result.Top);
  EXPECT_EQ(Width, Result.Width);
  EXPECT_EQ(Height, Result.Height);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_MakeRenderInterface)
{
  using RenderOpenGLProxy_t = ::mock::covellite::api::RenderOpenGL::Proxy;
  RenderOpenGLProxy_t RenderOpenGLProxy;
  RenderOpenGLProxy_t::GetInstance() = &RenderOpenGLProxy;

  class Tested :
    public Tested_t
  {
  public:
    MOCK_CONST_METHOD0(GetClientRect, Rect(void));

  public:
    explicit Tested(const WindowOs_t & _Window) :
      Tested_t(_Window)
    {

    }
  };

  const ::mock::Id_t RenderOpenGLId = 1709291231;
  const auto Top = 1807221330;

  ::android_app App;

  const AppInfo_t Info{ &App };

  const WindowOs_t WindowOs;
  const Tested Example{ WindowOs };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, GetClientRect())
    .Times(1)
    .WillOnce(Return(Tested::Rect{ 0, Top, 0, 0 }));

  EXPECT_CALL(RenderOpenGLProxy, Constructor(Top))
    .Times(1)
    .WillOnce(Return(RenderOpenGLId));

  const ITestedApi_t & IExample = Example;
  const auto pResult = IExample.MakeRenderInterface();

  const auto & RenderOpenGL =
    dynamic_cast<const ::mock::covellite::api::RenderOpenGL &>(*pResult);
  EXPECT_EQ(RenderOpenGLId, RenderOpenGL.m_Id);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_StartDrawing_Deprecated)
{
  using SectionImplProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SectionImplProxy_t SectionImplProxy;
  SectionImplProxy_t::GetInstance() = &SectionImplProxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::mock::Id_t WindowSectionId = 1711061210;
  const ::mock::Id_t BackgroundColorSectionId = 1711061211;

  android_app App;

  const AppInfo_t Info{ &App };

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

  using namespace ::testing;

  {
    InSequence Dummy;

    EXPECT_CALL(SectionImplProxy, GetChildSectionImpl(_, uT("Window")))
      .Times(1);

    EXPECT_CALL(SectionImplProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowSectionId));

    EXPECT_CALL(SectionImplProxy,
      GetChildSectionImpl(WindowSectionId, uT("BackgroundColor")))
      .Times(1);

    EXPECT_CALL(SectionImplProxy, Constructor())
      .Times(1)
      .WillOnce(Return(BackgroundColorSectionId));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("R")))
      .Times(1)
      .WillOnce(Return(uT("25.5")));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("G")))
      .Times(1)
      .WillOnce(Return(uT("51")));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("B")))
      .Times(1)
      .WillOnce(Return(uT("76.5")));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("A")))
      .Times(1)
      .WillOnce(Return(uT("102")));
  }

  const WindowOs_t WindowOs;
  Tested_t Example{ WindowOs };

  IWindowCore_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Disable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, ClearColor(0.1f, 0.2f, 0.3f, 0.4f))
    .Times(1);

  EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::StartDrawing, params::Empty{});
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_FinishDrawing_Deprecated)
{
  using SurfaceProxy_t = ::mock::covellite::egl::Surface::Proxy;
  SurfaceProxy_t SurfaceProxy;
  SurfaceProxy_t::GetInstance() = &SurfaceProxy;

  ::android_app App;

  const AppInfo_t Info{ &App };

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();
  const ::mock::Id_t SurfaceId = 1611232302;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SurfaceProxy, Constructor(_, _, _))
    .Times(1)
    .WillOnce(Return(SurfaceId));

  const WindowOs_t WindowOs;
  Tested_t Example{ WindowOs };

  IWindowCore_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  EXPECT_CALL(SurfaceProxy, SwapBuffers(SurfaceId))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::FinishDrawing, params::Empty{});
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_Resize_Deprecated)
{
  using SurfaceProxy_t = ::mock::covellite::egl::Surface::Proxy;
  SurfaceProxy_t SurfaceProxy;
  SurfaceProxy_t::GetInstance() = &SurfaceProxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::mock::Id_t SurfaceId = 1709272210;
  const int Width = 1612211039;
  const int Height = 1612211040;

  android_app App;

  const AppInfo_t Info{ &App };

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

  using namespace ::testing;

  EXPECT_CALL(SurfaceProxy, Constructor(_, _, _))
    .Times(1)
    .WillOnce(Return(SurfaceId));

  const WindowOs_t WindowOs;
  Tested_t Example{ WindowOs };

  IWindowCore_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  EXPECT_CALL(SurfaceProxy, GetWidth(SurfaceId))
    .WillRepeatedly(Return(Width));

  EXPECT_CALL(SurfaceProxy, GetHeight(SurfaceId))
    .WillRepeatedly(Return(Height));

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Viewport(0, 0, Width, Height))
    .Times(1);

  EXPECT_CALL(GLProxy, MatrixMode(GL_PROJECTION))
    .Times(1);

  EXPECT_CALL(GLProxy, LoadIdentity())
    .Times(1);

  EXPECT_CALL(GLProxy, MatrixMode(GL_MODELVIEW))
    .Times(1);

  EXPECT_CALL(GLProxy, LoadIdentity())
    .Times(1);

  EXPECT_CALL(GLProxy, Orthof(0, (float)Width, (float)Height, 0, -1, 1))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::Resize, params::Empty{});
}
