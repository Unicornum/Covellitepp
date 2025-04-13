
#include "stdafx.h"
#include <alicorn/std/chrono.mock.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/std/string/encoding.hpp>
#include <alicorn/platform/app-info.hpp>
#include <alicorn/logger.mock.hpp>
#include <Platform/Android.mock.hpp>
#include "../Mock/OpenGL.hpp"

#pragma comment(lib, "opengl32.lib")

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса OpenGLES3.
*/

#define GraphicApi GraphicApi_OpenGLES3
#define OpenGLCommon OpenGLCommon_OpenGLES3
#define OpenGLCommonShader OpenGLCommonShader_OpenGLES3

#include <Covellite.Egl/Egl.mock.hpp>
#include <Covellite/Api/Defines.hpp>
#include "../../Covellite.Api/Renderer/Component.hpp"
#include "../../Covellite.Api/Renderer/Shaders/Shaders.hpp"

// Расположение класса OpenGLES3
#include "../../Covellite.Api/Renderer/OpenGLES3.cpp"
#include "../../Covellite.Api/Renderer/GraphicApi.cpp"
#include "../../Covellite.Api/Renderer/GraphicApi.Constants.hpp"
#include "../../Covellite.Api/Renderer/GraphicApi.CapturingServiceComponent.cpp"
#include "../../Covellite.Api/Renderer/OpenGLCommon.cpp"
#include "../../Covellite.Api/Renderer/OpenGLCommon.Texture.cpp"
#include "../../Covellite.Api/Renderer/OpenGLCommonShader.cpp"

// Общий тестовый класс класса OpenGLES3
class OpenGLES3_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::OpenGLES3;
  using ITested_t = ::covellite::api::renderer::IGraphicApi;
  using String_t = ::alicorn::extension::std::String;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  using Component_t = ::covellite::api::Component;
  using Render_t = ::std::function<void(void)>;
  using Time_t = ::std::chrono::microseconds;
  using Object_t = ::std::vector<Component_t::ComponentPtr_t>;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    static const ::mock::GLfloat Viewport[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    ::testing::DefaultValue<const ::mock::GLfloat *>::Set(Viewport);

    ::testing::DefaultValue<int>::Set(1710282125);
    ::testing::DefaultValue<String_t>::Set(uT("0"));
    ::testing::DefaultValue<Time_t>::Set(Time_t{});

    Tested::GetValue() = 0;
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<const ::mock::GLfloat *>::Clear();
    ::testing::DefaultValue<int>::Clear();
    ::testing::DefaultValue<String_t>::Clear();
    ::testing::DefaultValue<Time_t>::Clear();
  }

protected:
  static ::Camera & GetCameraMartix(Tested_t & _Example)
  {
    return _Example.m_pConstants->Get<::Camera>();
  }

protected:
  class Data :
    public ::covellite::api::renderer::SettingsData
  {
  public:
    explicit Data(int _Top = 0)
    {
      Handle = (ANativeWindow *)0;
      ClientRect.Top = _Top;
      IsFullScreen = false;
    }
  };

  using Data_t = Data;

  template<class T>
  void IntroduceBufferSize(::std::vector<T> & _Data, const bool _IsLastData = true)
  {
    (*reinterpret_cast<size_t *>(_Data.data())) = _Data.size() * sizeof(T);

    if (_Data.size() > 8)
    {
      (*reinterpret_cast<bool *>(_Data.data() + sizeof(size_t))) = _IsLastData;
    }
  }

  template<class T>
  ::std::vector<uint8_t> GetExpected(const ::std::vector<T> & _Data)
  {
    ::std::vector<uint8_t> Result(sizeof(T) * _Data.size());
    memcpy(Result.data(), _Data.data(), Result.size());
    return Result;
  }
};

// Образец макроса для подстановки в класс OpenGLES3 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(OpenGLES3_test, Test_Function);

// ************************************************************************** //
TEST_F(OpenGLES3_test, /*DISABLED_*/Test_Constructor_Destructor)
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

  ::android_app App;

  const AppInfo_t Info{ &App };

  ANativeWindow NativeWindow;
  const ::mock::Id_t DisplayId = 1611232246;
  const ::mock::Id_t ConfigId = 1611232252;
  const ::mock::Id_t SurfaceId = 1611232253;
  const ::mock::Id_t ContextId = 1611232254;

  Data_t Data;
  Data.Handle = &NativeWindow;

  using namespace ::testing;

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

  EXPECT_CALL(ContextProxy, Constructor(DisplayId, ConfigId, 3))
    .Times(1)
    .WillOnce(Return(ContextId));

  EXPECT_CALL(ContextProxy, MakeCurrent(ContextId, SurfaceId))
    .Times(1);

  const Tested_t Example{ Data };

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
TEST_F(OpenGLES3_test, /*DISABLED_*/Test_GetUsingApi)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  ::android_app App;

  const AppInfo_t Info{ &App };

  using namespace ::alicorn::extension::std;

  const char * const Unknown = "Unknown1908291913";
  const char * Version = "Version1710311111";
  const auto Expected =
    string_cast<String, Encoding::Ascii128>(::std::string{ Version });

  using namespace ::testing;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GetString(GL_VERSION))
    .Times(1)
    .WillOnce(Return(reinterpret_cast<const unsigned char *>(Version)));

  EXPECT_CALL(GLProxy, GetString(_))
    .WillRepeatedly(Return(reinterpret_cast<const unsigned char *>(Unknown)));

  const auto Result = IExample.GetUsingApi();
  EXPECT_EQ(Expected, Result);
}

// ************************************************************************** //
TEST_F(OpenGLES3_test, /*DISABLED_*/Test_PresentFrame_FullScreen)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  using SurfaceProxy_t = ::mock::covellite::egl::Surface::Proxy;
  SurfaceProxy_t SurfaceProxy;
  SurfaceProxy_t::GetInstance() = &SurfaceProxy;

  const ::mock::Id_t SurfaceId = 1808271225;

  using namespace ::testing;

  EXPECT_CALL(SurfaceProxy, Constructor(_, _, _))
    .Times(1)
    .WillOnce(Return(SurfaceId));

  Tested_t Example{ Data_t{ 0 } };
  ITested_t & IExample = Example;

  EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
    .Times(0);

  EXPECT_CALL(SurfaceProxy, SwapBuffers(SurfaceId))
    .Times(1);

  EXPECT_CALL(GLProxy, GetError())
    .Times(3)
    .WillOnce(Return(2))
    .WillOnce(Return(1))
    .WillOnce(Return(GL_NO_ERROR));

  IExample.PresentFrame();
}

// ************************************************************************** //
TEST_F(OpenGLES3_test, /*DISABLED_*/Test_PresentFrame_ClearHeader)
{
  ::mock::GLProxy GLProxy;
  ::mock::covellite::egl::Surface::Proxy SurfaceProxy;

  constexpr ::mock::Id_t SurfaceId = 1808271225;
  Data_t Data;
  Data.ClientRect.Width = 2009090931;
  Data.ClientRect.Height = 2009090932;
  Data.ClientRect.Top = 12345;

  using namespace ::testing;

  EXPECT_CALL(SurfaceProxy, Constructor(_, _, _))
    .Times(1)
    .WillOnce(Return(SurfaceId));

  Tested_t Example{ Data };
  ITested_t & IExample = Example;

  {
    EXPECT_CALL(GLProxy, Enable(GL_SCISSOR_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, Scissor(0, Data.ClientRect.Height - Data.ClientRect.Top, 
      Data.ClientRect.Width, Data.ClientRect.Top))
      .Times(1);

    EXPECT_CALL(GLProxy, ClearColor(0.0f, 0.0f, 0.0f, 1.0f))
      .Times(1);

    EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_SCISSOR_TEST))
      .Times(1);

    EXPECT_CALL(SurfaceProxy, SwapBuffers(SurfaceId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(2)
      .WillOnce(Return(1))
      .WillOnce(Return(GL_NO_ERROR));

    IExample.PresentFrame();
  }

  {
    constexpr auto Top = 54321;
    constexpr auto Width = 2009090935;
    constexpr auto Height = 2009090936;

    IExample.ResizeWindow(::covellite::Rect{ 0, Top, Width, Height });

    EXPECT_CALL(GLProxy, Enable(GL_SCISSOR_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, Scissor(0, Height - Top, Width, Top))
      .Times(1);

    EXPECT_CALL(GLProxy, ClearColor(0.0f, 0.0f, 0.0f, 1.0f))
      .Times(1);

    EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_SCISSOR_TEST))
      .Times(1);

    EXPECT_CALL(SurfaceProxy, SwapBuffers(SurfaceId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(2)
      .WillOnce(Return(1))
      .WillOnce(Return(GL_NO_ERROR));

    IExample.PresentFrame();
  }
}

#define OpenGLCommon_test OpenGLES3_test
#include "../OpenGLCommon_test.hpp"

static const ::std::string ShaderHeader = 
  "#version 300 es\r\n"
  "#define COVELLITE_SHADER_MOBILE\r\n";

#define OpenGLShader_test OpenGLES3_test
#include "../OpenGLShader_test.hpp"
#include "../OpenGLShader.Present_test.hpp"

#define Updater_test OpenGLES3_test
#include "../Updater_test.hpp"

static const auto ImplClassName = uT("OpenGLES3");

#define Common_test OpenGLES3_test
#include "../Common_test.hpp"
