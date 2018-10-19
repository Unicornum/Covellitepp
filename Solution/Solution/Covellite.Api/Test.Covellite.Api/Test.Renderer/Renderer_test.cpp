
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Renderer.
*/

#include "../Mock/Eq.hpp"
#include "../Mock/RendererImpl.hpp"

// Расположение класса Renderer
#include "../../Covellite.Api/Renderer/Renderer.cpp"
#include "../../Covellite.Api/Renderer/Renderer.windows.cpp"

// Общий тестовый класс класса Renderer
class Renderer_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::Renderer;
  using RenderInterface_t = ::mock::Rocket::Core::RenderInterface;
  using ITested_t = ::covellite::api::renderer::IRenderer;
  using String_t = ::alicorn::extension::std::String;
  using Data_t = ::covellite::api::renderer::IRenderer::Data;
  using ITexture_t = ::covellite::api::renderer::IGraphicApi::ITexture;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<String_t>::Set(uT("DefaultString"));
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<String_t>::Clear();
  }

protected:
  class Texture :
    public ITexture_t
  {
  public:
    void Render(void) override {}
  };
};

namespace covellite
{

namespace api
{

namespace renderer
{

inline bool operator== (
  const IGraphicApi::ITexture::Data & _Left,
  const IGraphicApi::ITexture::Data & _Right)
{
  if (_Left.pData != _Right.pData) return false;
  if (_Left.Width != _Right.Width) return false;
  if (_Left.Height != _Right.Height) return false;
  return true;
}

inline bool operator== (
  const IGraphicApi::IGeometry::Data & _Left,
  const IGraphicApi::IGeometry::Data & _Right)
{
  if (_Left.pVertices != _Right.pVertices) return false;
  if (_Left.VerticesCount != _Right.VerticesCount) return false;
  if (_Left.pIndices != _Right.pIndices) return false;
  if (_Left.IndicesCount != _Right.IndicesCount) return false;
  if (_Left.pTexture != _Right.pTexture) return false;
  return true;
}

} // namespace renderer

} // namespace api

} // namespace covellite

// Образец макроса для подстановки в класс Renderer 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Renderer_test, Test_Function);

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_Constructor_CreateGraphicApiImpl)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  Data_t Data;
  Data.Handle = (HWND)1808231240;
  Data.Top = 1808271329;
  Data.BkColor = { 1808241312.0f, 1808241313.0f, 1808241314.0f, 1808241315.0f };
  Data.IsFullScreen = false;

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(Data))
    .Times(1);

  Tested_t Example{ uT("RendererImpl"), Data };
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_Constructor_GraphicApiNameToLog)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  const ::mock::Id_t RendererId = 1808242009;
  const auto UsingApi = uT("Version1808242008");

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  EXPECT_CALL(RendererImplProxy, GetUsingApi(RendererId))
    .Times(1)
    .WillOnce(Return(UsingApi));

  EXPECT_CALL(LoggerProxy, ToLog(uT("Info: Using graphics API: ") + UsingApi))
    .Times(1);

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_RendererGeometry)
{
  class Tested :
    public Tested_t
  {
    using CGHandle_t = ::mock::Rocket::Core::CompiledGeometryHandle;
    using Vertex_t = ::mock::Rocket::Core::Vertex;
    using THandle_t = ::mock::Rocket::Core::TextureHandle;
    using Vector2f_t = ::mock::Rocket::Core::Vector2f;

  public:
    MOCK_METHOD5(CompileGeometry, CGHandle_t(Vertex_t *, int, int *, int, THandle_t));
    MOCK_METHOD2(RenderCompiledGeometry, void(CGHandle_t, const Vector2f_t &));
    MOCK_METHOD1(ReleaseCompiledGeometry, void(CGHandle_t));

  public:
    Tested(void) :
      Tested_t(uT("RendererImpl"), Data_t{})
    {

    }
  };

  ::std::vector<::mock::Rocket::Core::Vertex> Vertex{ 10 };
  const int VertexSize = 1808251051;
  ::std::vector<int> Index{ 20 };
  const int IndexSize = 1808251052;
  ::mock::Rocket::Core::TextureHandle hTexture = 1808251053;
  ::mock::Rocket::Core::CompiledGeometryHandle hGeometry = (void *)1808251054;
  ::mock::Rocket::Core::Vector2f Position = { 1808251056.0f, 1808251057.0f };

  Tested Example;
  RenderInterface_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example,
    CompileGeometry(Vertex.data(), VertexSize, Index.data(), IndexSize, hTexture))
    .Times(1)
    .WillOnce(Return(hGeometry));

  EXPECT_CALL(Example, RenderCompiledGeometry(hGeometry, Position))
    .Times(1);

  EXPECT_CALL(Example, ReleaseCompiledGeometry(hGeometry))
    .Times(1);

  IExample.RenderGeometry(
    Vertex.data(), VertexSize, Index.data(), IndexSize, hTexture, Position);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_CompiledGeometry)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1808251110;
  auto * pVertex = (::mock::Rocket::Core::Vertex *)1808251124;
  ::mock::Rocket::Core::TextureHandle hTexture = 1808251125;
  ::mock::Rocket::Core::CompiledGeometryHandle hGeometry = (void *)1808251116;

  ::covellite::api::renderer::IGraphicApi::IGeometry::Data Data;
  Data.pVertices = (::covellite::api::renderer::IGraphicApi::Vertex *)pVertex;
  Data.VerticesCount = 1808251119;
  Data.pIndices = (int *)1808251121;
  Data.IndicesCount = 1808251122;
  Data.pTexture = (::covellite::api::renderer::IGraphicApi::ITexture *)hTexture;

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, CreateGeometry(RendererId, Data))
    .Times(1)
    .WillOnce(Return((::covellite::api::renderer::IGraphicApi::IGeometry *)hGeometry));

  const auto pGeometry = IExample.CompileGeometry(pVertex, Data.VerticesCount, 
    Data.pIndices, Data.IndicesCount, hTexture);
  EXPECT_EQ((void *)pGeometry, hGeometry);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_RendererCompiledGeometry)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  class Geometry :
    ::covellite::api::renderer::IGraphicApi::IGeometry
  {
  public:
    MOCK_METHOD2(Update, void (float, float));
    MOCK_METHOD0(Render, void(void));
  };

  const ::mock::Id_t RendererId = 1808251949;
  Geometry Geometry;
  ::mock::Rocket::Core::Vector2f Position = { 1808251126.0f, 1808251127.0f };

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Renderer(RendererId))
    .Times(1);

  EXPECT_CALL(Geometry, Update(Position.x, Position.y))
    .Times(1);

  EXPECT_CALL(Geometry, Render())
    .Times(1);

  IExample.RenderCompiledGeometry(&Geometry, Position);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_ReleaseCompiledGeometry)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1808251140;
  auto hGeometry = (::mock::Rocket::Core::CompiledGeometryHandle)1808251141;

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, DestroyGeometry(RendererId, 
    (::covellite::api::renderer::IGraphicApi::IGeometry *)hGeometry))
    .Times(1);

  IExample.ReleaseCompiledGeometry(hGeometry);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_EnableScissorRegion_True)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1808242032;

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, DisableScissorRegion(RendererId))
    .Times(0);

  IExample.EnableScissorRegion(true);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_EnableScissorRegion_False)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1808242032;

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, DisableScissorRegion(RendererId))
    .Times(1);

  IExample.EnableScissorRegion(false);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_SetScissorRegion)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1808242015;
  const int X = 1808242017;
  const int Y = 1808242018;
  const int W = 1808242019;
  const int H = 1808242020;

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, EnableScissorRegion(RendererId, X, Y, W, H))
    .Times(1);

  IExample.SetScissorRegion(X, Y, W, H);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_LoadTexture)
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
    Tested(const TextureHandle_t & _hHandle, const size_t _TextureDataSize) :
      Tested_t(uT("RendererImpl"), Data_t{}),
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
    RenderInterface_t & IExample = Example;

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
TEST_F(Renderer_test, /*DISABLED_*/Test_GenerateTexture_True)
{
  using TextureHandle_t = ::mock::Rocket::Core::TextureHandle;
  using TextureData_t = ::std::vector<::mock::Rocket::Core::byte>;
  using TextureDimensions_t = ::mock::Rocket::Core::Vector2i;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1808242042;
  const TextureData_t TextureData = { 0x00, 0x01, 0x02 };
  const TextureDimensions_t Size = { 1808242048, 1808242049 };

  ITexture_t::Data Data;
  Data.pData = TextureData.data();
  Data.Width = Size.x;
  Data.Height = Size.y;

  Texture Texture;
  ITexture_t * pTexture = &Texture;

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, CreateTexture(RendererId, Data))
    .Times(1)
    .WillOnce(Return(pTexture));

  TextureHandle_t Result = NULL;
  const auto IsSuccess = 
    IExample.GenerateTexture(Result, TextureData.data(), Size);
  EXPECT_TRUE(IsSuccess);
  EXPECT_EQ((ITexture_t *)Result, pTexture);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_GenerateTexture_False)
{
  using TextureHandle_t = ::mock::Rocket::Core::TextureHandle;
  using TextureData_t = ::std::vector<::mock::Rocket::Core::byte>;
  using TextureDimensions_t = ::mock::Rocket::Core::Vector2i;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1808242107;
  const TextureData_t TextureData = { 0x00, 0x01, 0x02 };
  const TextureDimensions_t Size = { 1808242108, 1808242109 };

  ITexture_t::Data Data;
  Data.pData = TextureData.data();
  Data.Width = Size.x;
  Data.Height = Size.y;

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, CreateTexture(RendererId, Data))
    .Times(1)
    .WillOnce(Throw(::std::exception{}));

  TextureHandle_t Result = NULL;
  const auto IsSuccess =
    IExample.GenerateTexture(Result, TextureData.data(), Size);
  EXPECT_FALSE(IsSuccess);
  EXPECT_EQ(Result, NULL);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_ReleaseTexture)
{
  using TextureHandle_t = ::mock::Rocket::Core::TextureHandle;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1808242107;

  Texture Texture;
  ITexture_t * pTexture = &Texture;
  TextureHandle_t hTexture = reinterpret_cast<TextureHandle_t>(pTexture);

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, DestroyTexture(RendererId, pTexture))
    .Times(1);

  IExample.ReleaseTexture(hTexture);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_ClearWindow)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1808232005;

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  ITested_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, ClearWindow(RendererId))
    .Times(1);

  IExample.ClearWindow();
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_Present)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1808232011;

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  ITested_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, Present(RendererId))
    .Times(1);

  IExample.Present();
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_SetWindowSize)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1808232014;
  const int32_t X = 1808232015;
  const int32_t Y = 1808232016;

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  ITested_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, ResizeWindow(RendererId, X, Y))
    .Times(1);

  IExample.ResizeWindow(X, Y);
}
