
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса OpenGLES.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define OpenGLCommon OpenGLCommon_android

#include <Covellite.Egl/Egl.mock.hpp>

// Расположение класса OpenGLES
#include "../../Covellite.Api/Renderer/OpenGLES.cpp"
#include "../../Covellite.Api/Renderer/OpenGLCommon.cpp"

// Общий тестовый класс класса OpenGLES
class OpenGLES_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::OpenGLES;
  using ITested_t = ::covellite::api::renderer::IGraphicApi;
  using String_t = ::alicorn::extension::std::String;
  using Data_t = ::covellite::api::renderer::Renderer::Data;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  using Component_t = ::covellite::api::Component;

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

  const Data_t Data = { 0 };
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

  Data_t Data = { 0 };
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

  EXPECT_CALL(ContextProxy, Constructor(DisplayId, ConfigId))
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
    string_cast<String, Locale::Default>(::std::string{ Version });

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
TEST_F(OpenGLES_test, /*DISABLED_*/Test_ClearFrame)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Data_t Data = { 0 };
  Data.BkColor.R = 1808261814.0f;
  Data.BkColor.G = 1808261815.0f;
  Data.BkColor.B = 1808261816.0f;
  Data.BkColor.A = 1808261817.0f;

  Tested_t Example{ Data };
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, ClearColor(Data.BkColor.R, Data.BkColor.G,
    Data.BkColor.B, Data.BkColor.A))
    .Times(1);

  EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
    .Times(1);

  IExample.ClearFrame();
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

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_ResizeWindow)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const int Width = 1710291059;
  const int Height = 1710291100;

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  using namespace ::testing;

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

  IExample.ResizeWindow(Width, Height);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_Camera)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make({}));

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Disable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DITHER))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_LIGHTING))
    .Times(1);

  EXPECT_CALL(GLProxy, ShadeModel(GL_SMOOTH))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_CULL_FACE))
    .Times(1);

  EXPECT_CALL(GLProxy, FrontFace(GL_CCW))
    .Times(1);

  EXPECT_CALL(GLProxy, CullFace(GL_BACK))
    .Times(1);

  EXPECT_CALL(GLProxy, Hint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_BlendState)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make(
    {
      { uT("kind"), uT("Blend") }
    }));

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Enable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_SamplerState)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make(
    {
      { uT("kind"), uT("Sampler") }
    }));

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    GL_LINEAR))
    .Times(1);

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
    GL_LINEAR))
    .Times(1);

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
    GL_CLAMP_TO_EDGE))
    .Times(1);

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
    GL_CLAMP_TO_EDGE))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_Scissor_Enable)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Data_t Data;
  Data.Top = 1808271333;

  const Tested_t Example{ Data };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Scissor") },
      { uT("is_enabled"), true },
    });

  auto Render = itCreator->second(pComponent);

  using namespace ::testing;

  // Первый вызов
  {
    const int X = 1509;
    const int Y = 1510;
    const int Width = 1511;
    const int Height = 1512;
    const int WindowHeight = 1513;
    const int Yo = WindowHeight - (Y + Height + Data.Top);

    pComponent->SetValue(uT("left"), X);
    pComponent->SetValue(uT("top"), Y);
    pComponent->SetValue(uT("right"), X + Width);
    pComponent->SetValue(uT("bottom"), Y + Height);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, Enable(GL_SCISSOR_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntValue(_))
      .Times(3)
      .WillRepeatedly(Return(0));

    EXPECT_CALL(GLProxy, GetIntValue(Eq("height")))
      .Times(1)
      .WillOnce(Return(WindowHeight));

    EXPECT_CALL(GLProxy, Scissor(X, Yo, Width, Height))
      .Times(1);

    Render();
  }

  // Второй вызов
  {
    const int X = 1514;
    const int Y = 1514;
    const int Width = 1516;
    const int Height = 1517;
    const int WindowHeight = 1518;
    const int Yo = WindowHeight - (Y + Height + Data.Top);

    pComponent->SetValue(uT("left"), X);
    pComponent->SetValue(uT("top"), Y);
    pComponent->SetValue(uT("right"), X + Width);
    pComponent->SetValue(uT("bottom"), Y + Height);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, Enable(GL_SCISSOR_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntValue(_))
      .Times(3)
      .WillRepeatedly(Return(0));

    EXPECT_CALL(GLProxy, GetIntValue(Eq("height")))
      .Times(1)
      .WillOnce(Return(WindowHeight));

    EXPECT_CALL(GLProxy, Scissor(X, Yo, Width, Height))
      .Times(1);

    Render();
  }
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_Scissor_Disable)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Scissor") },
      { uT("is_enabled"), false },
    });

  auto Render = itCreator->second(pComponent);

  using namespace ::testing;

  EXPECT_CALL(GLProxy, Disable(GL_SCISSOR_TEST))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_Position)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Data_t Data;
  Data.Top = 1811231228;

  const Tested_t Example{ Data };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Position"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({});

  auto Render = itCreator->second(pPosition);

  using namespace ::testing;

  // Первый вызов
  {
    const float X = 1811221956.0f;
    const float Y = 1811221957.0f;
    const float Z = 1811221958.0f;

    pPosition->SetValue(uT("x"), X);
    pPosition->SetValue(uT("y"), Y);
    pPosition->SetValue(uT("z"), Z);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, PushMatrix())
      .Times(1);

    EXPECT_CALL(GLProxy, Translatef(X, Y + Data.Top, Z))
      .Times(1);

    Render();
  }

  // Второй вызов
  {
    const float X = 1811221959.0f;
    const float Y = 1811221960.0f;
    const float Z = 1811221961.0f;

    pPosition->SetValue(uT("x"), X);
    pPosition->SetValue(uT("y"), Y);
    pPosition->SetValue(uT("z"), Z);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, PushMatrix())
      .Times(1);

    EXPECT_CALL(GLProxy, Translatef(X, Y + Data.Top, Z))
      .Times(1);

    Render();
  }
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_VertexBuffer)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::std::vector<Tested_t::Vertex> m_Vertices =
  {
    { 1808261932.0f, 1808261933.0f, 1808261934, 1808261935.0f, 1808261936.0f},
    { 1808261932.0f, 1808261933.0f, 1808261934, 1808261935.0f, 1808261936.0f},
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Vertex") },
      { uT("data"), m_Vertices.data() },
      { uT("count"), m_Vertices.size() },
    });

  auto Render = itCreator->second(pComponent);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, VertexPointer(2, GL_FLOAT,
    sizeof(Tested_t::Vertex), &m_Vertices[0].x))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_VERTEX_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, ColorPointer(4, GL_UNSIGNED_BYTE,
    sizeof(Tested_t::Vertex), &m_Vertices[0].Color))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_COLOR_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, TexCoordPointer(2, GL_FLOAT,
    sizeof(Tested_t::Vertex), &m_Vertices[0].u))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_TEXTURE_COORD_ARRAY))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_IndexBuffer)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::std::vector<int> m_Indices =
  {
    1808261927,
    1808261928,
    1808261929,
    1808261930,
    1808261931
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Index") },
      { uT("data"), m_Indices.data() },
      { uT("count"), m_Indices.size() },
    });

  auto Render = itCreator->second(pComponent);

  using namespace ::testing;

  EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)m_Indices.size(),
    GL_UNSIGNED_INT, m_Indices.data()))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_DrawCall)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("DrawCall"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make({}));

  using namespace ::testing;

  EXPECT_CALL(GLProxy, DisableClientState(GL_VERTEX_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, DisableClientState(GL_COLOR_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, DisableClientState(GL_TEXTURE_COORD_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_TEXTURE_2D))
    .Times(1);

  EXPECT_CALL(GLProxy, PopMatrix())
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_Texture_glTexImage2D_Fail)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::mock::GLuint TextureId = 1612182301;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GenTextures(1))
    .Times(1)
    .WillOnce(Return(TextureId));

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
    .Times(1);

  EXPECT_CALL(GLProxy, TexImage2D(_, _, _, _, _, _, _, _, _))
    .Times(1);

  EXPECT_CALL(GLProxy, GetError())
    .Times(1)
    .WillOnce(Return(1808261916));

  EXPECT_THROW(itCreator->second(Component_t::Make({})), ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_Texture)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const uint8_t * pSource = (uint8_t *)1811222026;

  const int Width = 1808261902;
  const int Height = 1808261903;
  const ::mock::GLuint TextureId = 1612182301;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("data"), pSource },
      { uT("width"), Width },
      { uT("height"), Height },
    });

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GenTextures(1))
    .Times(1)
    .WillOnce(Return(TextureId));

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
    .Times(1);

  EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
    Width, Height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, pSource))
    .Times(1);

  EXPECT_CALL(GLProxy, GetError())
    .Times(1)
    .WillOnce(Return(GL_NO_ERROR));

  auto Render = itCreator->second(pComponent);

  EXPECT_CALL(GLProxy, Enable(GL_TEXTURE_2D))
    .Times(1);

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
    .Times(1);

  Render();

  EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
    .Times(1);
}
