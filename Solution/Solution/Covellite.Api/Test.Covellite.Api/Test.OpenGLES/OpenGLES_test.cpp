
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
  using Data_t = ::covellite::api::renderer::IGraphicApi::Data;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;

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
TEST_F(OpenGLES_test, /*DISABLED_*/Test_ClearWindow)
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

  EXPECT_CALL(GLProxy, Disable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, ClearColor(Data.BkColor.R, Data.BkColor.G,
    Data.BkColor.B, Data.BkColor.A))
    .Times(1);

  EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
    .Times(1);

  IExample.ClearWindow();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_Present)
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

  IExample.Present();
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
TEST_F(OpenGLES_test, /*DISABLED_*/Test_CreateTexture)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  uint8_t Source[] = { 0 };

  Tested_t::ITexture::Data Data = { 0 };
  Data.pData = Source;
  Data.Width = 1808261902;
  Data.Height = 1808261903;
  const ::mock::GLuint TextureId = 1612182301;

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GenTextures(1))
    .Times(1)
    .WillOnce(Return(TextureId));

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
    .Times(1);

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

  EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
    Data.Width, Data.Height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, Source))
    .Times(1);

  EXPECT_CALL(GLProxy, GetError())
    .Times(1)
    .WillOnce(Return(GL_NO_ERROR));

  auto * pTexture = IExample.Create(Data);
  ASSERT_TRUE(pTexture != nullptr);

  EXPECT_CALL(GLProxy, Enable(GL_TEXTURE_2D))
    .Times(1);

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
    .Times(1);

  pTexture->Render();

  EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
    .Times(1);

  delete pTexture;
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_CreateTexture_Fail)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  uint8_t Source[] = { 0 };

  Tested_t::ITexture::Data Data = { 0 };
  Data.pData = Source;
  Data.Width = 1808261902;
  Data.Height = 1808261903;
  const ::mock::GLuint TextureId = 1612182301;

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GenTextures(1))
    .Times(1)
    .WillOnce(Return(TextureId));

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
    .Times(1);

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

  EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
    Data.Width, Data.Height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, Source))
    .Times(1);

  EXPECT_CALL(GLProxy, GetError())
    .Times(1)
    .WillOnce(Return(1808261916));

  EXPECT_THROW(IExample.Create(Data), ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_DestroyTexture)
{
  class Texture :
    public Tested_t::ITexture
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD1(Destructor, void(::mock::Id_t));
    };

  public:
    void Render(void) override {}

  private:
    const ::mock::Id_t m_Id;

  public:
    explicit Texture(::mock::Id_t _Id) : m_Id(_Id) {}
    ~Texture(void)
    {
      Proxy::GetInstance()->Destructor(m_Id);
    }
  };

  Texture::Proxy Proxy;
  Texture::Proxy::GetInstance() = &Proxy;

  const ::mock::Id_t TextureId = 1808261831;
  Tested_t::ITexture * pTexture = new Texture{ TextureId };

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  using namespace ::testing;

  EXPECT_CALL(Proxy, Destructor(TextureId))
    .Times(1);

  IExample.Destroy(pTexture);
}

namespace covellite
{

namespace api
{

namespace renderer
{

inline bool operator== (
  const ::covellite::api::renderer::IGraphicApi::Vertex & _Left,
  const ::covellite::api::renderer::IGraphicApi::Vertex & _Right)
{
  if (_Left.x != _Right.x) return false;
  if (_Left.y != _Right.y) return false;
  if (_Left.Color != _Right.Color) return false;
  if (_Left.u != _Right.u) return false;
  if (_Left.v != _Right.v) return false;
  return true;
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_CreateGeometry_WithoutTexture)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  ::std::vector<Tested_t::Vertex> m_Vertices =
  {
    { 1808261932.0f, 1808261933.0f, 1808261934, 1808261935.0f, 1808261936.0f},
    { 1808261932.0f, 1808261933.0f, 1808261934, 1808261935.0f, 1808261936.0f},
  };

  ::std::vector<int> m_Indices =
  {
    1808261927,
    1808261928,
    1808261929,
    1808261930,
    1808261931
  };

  Tested_t::IGeometry::Data Data = { 0 };
  Data.pVertices = m_Vertices.data();
  Data.VerticesCount = (int)m_Vertices.size();
  Data.pIndices = m_Indices.data();
  Data.IndicesCount = (int)m_Indices.size();
  Data.pTexture = nullptr;

  const float X = 1808261937.0f;
  const float Y = 1808261938.0f;

  Data_t SourceData;
  SourceData.Top = 1808271335;

  Tested_t Example{ SourceData };
  ITested_t & IExample = Example;

  auto * pGeometry = IExample.Create(Data);
  auto * pOpenGLGeometry = dynamic_cast<OpenGLGeometry *>(pGeometry);
  ASSERT_TRUE(pOpenGLGeometry != nullptr);

  EXPECT_TRUE(m_Vertices == pOpenGLGeometry->m_Vertices);
  EXPECT_TRUE(m_Indices == pOpenGLGeometry->m_Indices);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, PushMatrix())
    .Times(1);

  EXPECT_CALL(GLProxy, Translatef(X, Y + SourceData.Top, 0))
    .Times(1);

  pGeometry->Update(X, Y);

  EXPECT_CALL(GLProxy, VertexPointer(2, GL_FLOAT, sizeof(Tested_t::Vertex),
    &pOpenGLGeometry->m_Vertices[0].x))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_VERTEX_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, ColorPointer(4, GL_UNSIGNED_BYTE,
    sizeof(Tested_t::Vertex), &pOpenGLGeometry->m_Vertices[0].Color))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_COLOR_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_TEXTURE_2D))
    .Times(0);

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, _))
    .Times(0);

  EXPECT_CALL(GLProxy, TexCoordPointer(_, _, _, _))
    .Times(0);

  EXPECT_CALL(GLProxy, EnableClientState(GL_TEXTURE_COORD_ARRAY))
    .Times(0);

  EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)m_Indices.size(),
    GL_UNSIGNED_INT, pOpenGLGeometry->m_Indices.data()))
    .Times(1);

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

  pGeometry->Render();

  delete pGeometry;
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_CreateGeometry)
{
  class Texture :
    public Tested_t::ITexture
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD1(Render, void(::mock::Id_t));
    };

  public:
    void Render(void) override
    {
      Proxy::GetInstance()->Render(m_Id);
    }

  private:
    const ::mock::Id_t m_Id;

  public:
    explicit Texture(::mock::Id_t _Id) : m_Id(_Id) {}
  };

  Texture::Proxy Proxy;
  Texture::Proxy::GetInstance() = &Proxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  ::std::vector<Tested_t::Vertex> m_Vertices =
  {
    { 1808261932.0f, 1808261933.0f, 1808261934, 1808261935.0f, 1808261936.0f},
    { 1808261932.0f, 1808261933.0f, 1808261934, 1808261935.0f, 1808261936.0f},
  };

  ::std::vector<int> m_Indices =
  {
    1808261927,
    1808261928,
    1808261929,
    1808261930,
    1808261931
  };

  const ::mock::Id_t TextureId = 1808261946;
  Texture Texture{ TextureId };

  Tested_t::IGeometry::Data Data = { 0 };
  Data.pVertices = m_Vertices.data();
  Data.VerticesCount = (int)m_Vertices.size();
  Data.pIndices = m_Indices.data();
  Data.IndicesCount = (int)m_Indices.size();
  Data.pTexture = &Texture;

  const float X = 1808261937.0f;
  const float Y = 1808261938.0f;

  Data_t SourceData;
  SourceData.Top = 1808271336;

  Tested_t Example{ SourceData };
  ITested_t & IExample = Example;

  auto * pGeometry = IExample.Create(Data);
  auto * pOpenGLGeometry = dynamic_cast<OpenGLGeometry *>(pGeometry);
  ASSERT_TRUE(pOpenGLGeometry != nullptr);

  EXPECT_TRUE(m_Vertices == pOpenGLGeometry->m_Vertices);
  EXPECT_TRUE(m_Indices == pOpenGLGeometry->m_Indices);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, PushMatrix())
    .Times(1);

  EXPECT_CALL(GLProxy, Translatef(X, Y + SourceData.Top, 0))
    .Times(1);

  pGeometry->Update(X, Y);

  EXPECT_CALL(GLProxy, VertexPointer(2, GL_FLOAT, sizeof(Tested_t::Vertex),
    &pOpenGLGeometry->m_Vertices[0].x))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_VERTEX_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, ColorPointer(4, GL_UNSIGNED_BYTE,
    sizeof(Tested_t::Vertex), &pOpenGLGeometry->m_Vertices[0].Color))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_COLOR_ARRAY))
    .Times(1);

  EXPECT_CALL(Proxy, Render(TextureId))
    .Times(1);

  EXPECT_CALL(GLProxy, TexCoordPointer(2, GL_FLOAT,
    sizeof(Tested_t::Vertex), &pOpenGLGeometry->m_Vertices[0].u))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_TEXTURE_COORD_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)m_Indices.size(),
    GL_UNSIGNED_INT, pOpenGLGeometry->m_Indices.data()))
    .Times(1);

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

  pGeometry->Render();

  delete pGeometry;
}

} // namespace renderer

} // namespace api

} // namespace covellite

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_DestroyGeometry)
{
  class Geometry :
    public Tested_t::IGeometry
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD1(Destructor, void(::mock::Id_t));
    };

  public:
    void Update(float, float) override {}
    void Render(void) override {}

  private:
    const ::mock::Id_t m_Id;

  public:
    explicit Geometry(::mock::Id_t _Id) : m_Id(_Id) {}
    ~Geometry(void)
    {
      Proxy::GetInstance()->Destructor(m_Id);
    }
  };

  Geometry::Proxy Proxy;
  Geometry::Proxy::GetInstance() = &Proxy;

  const ::mock::Id_t GeometryId = 1808261843;
  Tested_t::IGeometry * pGeometry = new Geometry{ GeometryId };

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  using namespace ::testing;

  EXPECT_CALL(Proxy, Destructor(GeometryId))
    .Times(1);

  IExample.Destroy(pGeometry);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_EnableScissorRegion)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Data_t Data;
  Data.Top = 1808271333;

  const int X = 1509;
  const int Y = 1510;
  const int Width = 1511;
  const int Height = 1512;
  const int WindowHeight = 1513;
  const int Yo = WindowHeight - (Y + Height + Data.Top);

  Tested_t Example{ Data };
  ITested_t & IExample = Example;

  using namespace ::testing;

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

  IExample.EnableScissorRegion(X, Y, Width, Height);
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_DisableScissorRegion)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  using namespace ::testing;

  EXPECT_CALL(GLProxy, Disable(GL_SCISSOR_TEST))
    .Times(1);

  IExample.DisableScissorRegion();
}

// ************************************************************************** //
TEST_F(OpenGLES_test, /*DISABLED_*/Test_Render)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DITHER))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_LIGHTING))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
    .Times(1);

  EXPECT_CALL(GLProxy, Hint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST))
    .Times(1);

  EXPECT_CALL(GLProxy, ShadeModel(GL_SMOOTH))
    .Times(1);

  EXPECT_CALL(GLProxy, FrontFace(GL_CCW))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_CULL_FACE))
    .Times(1);

  EXPECT_CALL(GLProxy, CullFace(GL_BACK))
    .Times(1);

  IExample.Render();
}
