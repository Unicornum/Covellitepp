
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса RenderOpenGL.
*/

// Расположение класса RenderOpenGL
#include <Covellite\Api\RenderOpenGL.hpp>

// Общий тестовый класс класса RenderOpenGL
class RenderOpenGL_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::RenderOpenGL;
  using ITested_t = ::mock::Rocket::Core::RenderInterface;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс RenderOpenGL 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(RenderOpenGL_test, Test_Function);

// ************************************************************************** //
TEST_F(RenderOpenGL_test, /*DISABLED_*/Test_Constructor)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DITHER))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_LIGHTING))
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

  Tested_t Example(0);
}

// ************************************************************************** //
TEST_F(RenderOpenGL_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(RenderOpenGL_test, /*DISABLED_*/Test_RenderGeometry_WithoutTexture)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  ::mock::Rocket::Core::Vertex Vertices[1];
  int Indices = 0;
  const int IndicesCount = 1612182223;
  const ::mock::Rocket::Core::Vector2f Translation(1612182226.0f, 1612182227.0f);
  const int StatusBarHeight = 1612291432;

  Tested_t Example(StatusBarHeight);
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Enable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
    .Times(1);

  EXPECT_CALL(GLProxy, PushMatrix())
    .Times(1);

  EXPECT_CALL(GLProxy, Translatef(
    Translation.x, Translation.y + StatusBarHeight, 0))
    .Times(1);

  EXPECT_CALL(GLProxy, VertexPointer(2, GL_FLOAT, 
    sizeof(::mock::Rocket::Core::Vertex), &Vertices[0].position))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_VERTEX_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, ColorPointer(4, GL_UNSIGNED_BYTE,
    sizeof(::mock::Rocket::Core::Vertex), &Vertices[0].colour))
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

  EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, 
    &Indices))
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

  IExample.RenderGeometry(
    Vertices, 0, &Indices, IndicesCount, 0, Translation);
}

// ************************************************************************** //
TEST_F(RenderOpenGL_test, /*DISABLED_*/Test_RenderGeometry_Texture)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  ::mock::Rocket::Core::Vertex Vertices[1];
  int Indices = 0;
  const int IndicesCount = 1612182240;
  const ::mock::Rocket::Core::TextureHandle hTexture = 
    static_cast<::mock::Rocket::Core::TextureHandle>(1612182241);
  const ::mock::Rocket::Core::Vector2f Translation(1612182242.0f, 1612182243.0f);
  const int StatusBarHeight = 1612291433;

  Tested_t Example(StatusBarHeight);
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Enable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
    .Times(1);

  EXPECT_CALL(GLProxy, PushMatrix())
    .Times(1);

  EXPECT_CALL(GLProxy, Translatef(
    Translation.x, Translation.y + StatusBarHeight, 0))
    .Times(1);

  EXPECT_CALL(GLProxy, VertexPointer(2, GL_FLOAT,
    sizeof(::mock::Rocket::Core::Vertex), &Vertices[0].position))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_VERTEX_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, ColorPointer(4, GL_UNSIGNED_BYTE,
    sizeof(::mock::Rocket::Core::Vertex), &Vertices[0].colour))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_COLOR_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_TEXTURE_2D))
    .Times(1);

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 
    static_cast<::mock::GLuint>(hTexture)))
    .Times(1);

  EXPECT_CALL(GLProxy, TexCoordPointer(2, GL_FLOAT, 
    sizeof(::mock::Rocket::Core::Vertex), &Vertices[0].tex_coord))
    .Times(1);

  EXPECT_CALL(GLProxy, EnableClientState(GL_TEXTURE_COORD_ARRAY))
    .Times(1);

  EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT,
    &Indices))
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

  IExample.RenderGeometry(
    Vertices, 0, &Indices, IndicesCount, hTexture, Translation);
}

// ************************************************************************** //
TEST_F(RenderOpenGL_test, /*DISABLED_*/Test_EnableScissorRegion)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Tested_t Example(0);
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Enable(GL_SCISSOR_TEST))
    .Times(1);

  IExample.EnableScissorRegion(true);

  EXPECT_CALL(GLProxy, Disable(GL_SCISSOR_TEST))
    .Times(1);

  IExample.EnableScissorRegion(false);
}

// ************************************************************************** //
TEST_F(RenderOpenGL_test, /*DISABLED_*/Test_SetScissorRegion)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const int StatusBarHeight = 1508;
  const int X = 1509;
  const int Y = 1510;
  const int Width = 1511;
  const int Height = 1512;
  const int WindowHeight = 1513;
  const int Yo = WindowHeight - (Y + Height + StatusBarHeight);

  Tested_t Example(StatusBarHeight);
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

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

  IExample.SetScissorRegion(X, Y, Width, Height);
}

// ************************************************************************** //
TEST_F(RenderOpenGL_test, /*DISABLED_*/Test_LoadTexture)
{
  using TextureHandle_t = ::mock::Rocket::Core::TextureHandle;
  using TextureData_t = ::std::vector<::mock::Rocket::Core::byte>;
  using TextureDimensions_t = ::mock::Rocket::Core::Vector2i;

  class Tested :
    public Tested_t
  {
  public:
    MOCK_METHOD2(DoGenerateTexture, bool(TextureData_t, TextureDimensions_t));

  public:
    bool GenerateTexture(TextureHandle_t & _hTexture,
      const ::mock::Rocket::Core::byte * _pData, 
      const TextureDimensions_t & _TextureDimensions) override
    {
      _hTexture = m_hHandle;
      TextureData_t TextureData(m_TextureDataSize);
      memcpy_s(TextureData.data(), TextureData.size(), _pData, TextureData.size());

      return DoGenerateTexture(TextureData, _TextureDimensions);
    }

  private:
    const TextureHandle_t m_hHandle;
    const size_t m_TextureDataSize;

  public:
    Tested(TextureHandle_t _hHandle, const size_t _TextureDataSize) :
      Tested_t(1804101138),
      m_hHandle(_hHandle),
      m_TextureDataSize(_TextureDataSize)
    {

    }
  };

  using namespace ::boost::filesystem;

  const ::mock::GLuint TextureId = 1804101114;
  const path PathToSourceImage = THIS_DIRECTORY / "Image.png";
  const TextureDimensions_t ExpectTextureDimensions{ 4, 3 };
  const TextureData_t TextureData =
  {
    0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0xFF, 0xFF,  0x00, 0x00, 0xFF, 0xFF,  0x00, 0x00, 0xFF, 0xFF,  0x00, 0x00, 0xFF, 0xFF,
    0xFF, 0x00, 0x00, 0xFF,  0xFF, 0x00, 0x00, 0xFF,  0xFF, 0x00, 0x00, 0xFF,  0xFF, 0x00, 0x00, 0xFF,
  };

  for (const auto IsGenerate : { true, false })
  {
    Tested Example(static_cast<TextureHandle_t>(TextureId), TextureData.size());
    ITested_t & IExample = Example;

    using namespace ::testing;

    EXPECT_CALL(Example, DoGenerateTexture(TextureData, ExpectTextureDimensions))
      .Times(1)
      .WillOnce(Return(IsGenerate));

    ::mock::Rocket::Core::TextureHandle hTexture = 0;
    ::mock::Rocket::Core::Vector2i TextureDimensions{ 0, 0 };

    EXPECT_FALSE(hTexture == static_cast<TextureHandle_t>(TextureId));
    EXPECT_FALSE(ExpectTextureDimensions == TextureDimensions);

    const auto Result = IExample.LoadTexture(hTexture, TextureDimensions, 
      PathToSourceImage.string().c_str());
    EXPECT_EQ(IsGenerate, Result);
    EXPECT_TRUE(hTexture == static_cast<TextureHandle_t>(TextureId));
    EXPECT_TRUE(ExpectTextureDimensions == TextureDimensions);
  }
}

// ************************************************************************** //
TEST_F(RenderOpenGL_test, /*DISABLED_*/Test_GenerateTexture)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::mock::Rocket::Core::byte Source[] = {0};
  const ::mock::Rocket::Core::Vector2i SourceDimensions(1612182250, 1612182251);
  const ::mock::GLuint TextureId = 1612182301;

  Tested_t Example(0);
  ITested_t & IExample = Example;

  for (const auto Error : { 1612182252, GL_NO_ERROR })
  {
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
      SourceDimensions.x, SourceDimensions.y, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, Source))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(Error));

    ::mock::Rocket::Core::TextureHandle hTexture = 0;

    EXPECT_FALSE(hTexture == 
      static_cast<::mock::Rocket::Core::TextureHandle>(TextureId));

    const auto Result = IExample.GenerateTexture(
      hTexture, Source, SourceDimensions);
    EXPECT_TRUE(Result == (Error == GL_NO_ERROR));
    EXPECT_TRUE(hTexture ==
      static_cast<::mock::Rocket::Core::TextureHandle>(TextureId));
  }
}

// ************************************************************************** //
TEST_F(RenderOpenGL_test, /*DISABLED_*/Test_ReleaseTexture)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  auto hTexture =
    static_cast<::mock::Rocket::Core::TextureHandle>(1612182313);

  Tested_t Example(0);
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, DeleteTextures(1,
    static_cast<::mock::GLuint>(hTexture)))
    .Times(1);

  IExample.ReleaseTexture(hTexture);
}
