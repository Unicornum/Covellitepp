
#include "stdafx.h"
#include <alicorn/std/chrono.mock.hpp>
#include <alicorn/std/string/encoding.hpp>
#include <alicorn/platform/app-info.hpp>
#include <Platform/Android.mock.hpp>
#include "../Mock/OpenGL.hpp"

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса OpenGLES.
*/

#define GraphicApi GraphicApi_OpenGLES
#define OpenGLCommon OpenGLCommon_OpenGLES

#include <Covellite.Egl/Egl.mock.hpp>
#include <Covellite/Api/Defines.hpp>
#include "../../Covellite.Api/Renderer/Component.hpp"
#include "../../Covellite.Api/Renderer/Shaders/Shaders.hpp"

// Расположение класса OpenGLES
#include "../../Covellite.Api/Renderer/OpenGLES.cpp"
#include "../../Covellite.Api/Renderer/GraphicApi.cpp"
#include "../../Covellite.Api/Renderer/GraphicApi.Constants.hpp"
#include "../../Covellite.Api/Renderer/GraphicApi.CapturingServiceComponent.cpp"
#include "../../Covellite.Api/Renderer/OpenGLCommon.cpp"
#include "../../Covellite.Api/Renderer/OpenGLCommon.Texture.cpp"
#include "../../Covellite.Api/Renderer/OpenGLCommonStatic.cpp"

// Общий тестовый класс класса OpenGLES
class OpenGLES_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::OpenGLES;
  using ITested_t = ::covellite::api::renderer::IGraphicApi;
  using String_t = ::alicorn::extension::std::String;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  using Component_t = ::covellite::api::Component;
  using Render_t = ::std::function<void(void)>;
  using Time_t = ::std::chrono::microseconds;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    static const ::mock::GLfloat Viewport[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    ::testing::DefaultValue<const ::mock::GLfloat *>::Set(Viewport);

    ::testing::DefaultValue<String_t>::Set(uT("0"));
    ::testing::DefaultValue<Time_t>::Set(Time_t{});
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<const ::mock::GLfloat *>::Clear();
    ::testing::DefaultValue<String_t>::Clear();
    ::testing::DefaultValue<Time_t>::Clear();
  }

protected:
  const String_t m_UsingApi = uT("OpenGLES");

protected:
  class Data :
    public ::covellite::api::renderer::SettingsData
  {
  public:
    explicit Data(int _Top = 0)
    {
      Handle = (ANativeWindow *)nullptr;
      Top = _Top;
      IsFullScreen = false;
    }
  };

  using Data_t = Data;
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

  const Data_t oData;
  const ::covellite::api::renderer::SettingsData & Data = oData;

  auto pExample = factory::make_unique<ITested_t>(uT("OpenGLES"), Data);
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

  EXPECT_CALL(ContextProxy, Constructor(DisplayId, ConfigId, 1))
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
TEST_F(OpenGLES_test, /*DISABLED_*/Test_GetUsingApi)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  ::android_app App;

  const AppInfo_t Info{ &App };

  using namespace ::alicorn::extension::std;

  const char * Version = "Version1710311111";
  const auto Expected =
    string_cast<String, Encoding::Ascii128>(::std::string{ Version });

  using namespace ::testing;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  EXPECT_CALL(GLProxy, GetString(GL_VERSION))
    .Times(1)
    .WillOnce(Return(reinterpret_cast<const unsigned char *>(Version)));

  const auto Result = IExample.GetUsingApi();
  EXPECT_EQ(Expected, Result);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_PresentFrame)
{
  using SurfaceProxy_t = ::mock::covellite::egl::Surface::Proxy;
  SurfaceProxy_t SurfaceProxy;
  SurfaceProxy_t::GetInstance() = &SurfaceProxy;

  const ::mock::Id_t SurfaceId = 1808271225;

  using namespace ::testing;

  EXPECT_CALL(SurfaceProxy, Constructor(_, _, _))
    .Times(1)
    .WillOnce(Return(SurfaceId));

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  EXPECT_CALL(SurfaceProxy, SwapBuffers(SurfaceId))
    .Times(1);

  IExample.PresentFrame();
}

#define OpenGLCommon_test OpenGLES_test
#include "../OpenGLCommon_test.hpp"

#define OpenGLStatic_test OpenGLES_test
#include "../OpenGLStatic_test.hpp"

#define Updater_test OpenGLES_test
#include "../Updater_test.hpp"
