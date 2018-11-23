
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Renderer.
*/

// Расположение класса Renderer
#include "../../Covellite.Api/Renderer/Renderer.cpp"
#include "../../Covellite.Api/Renderer/Renderer.windows.cpp"

#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Renders.hpp>

#include "../Mock/Equal.hpp"
#include "../Mock/RendererImpl.hpp"

namespace std
{

bool operator== (
  const ::covellite::Any_t & _Left,
  const ::covellite::Any_t & _Right)
{
  using String_t = ::alicorn::extension::std::String;

  if (_Left.type() != _Right.type()) return false;

  if (_Left.type() == typeid(String_t))
  {
    return ::covellite::any_cast<String_t>(_Left) == 
      ::covellite::any_cast<String_t>(_Right);
  }

  if (_Left.type() == typeid(int))
  {
    return ::covellite::any_cast<int>(_Left) ==
      ::covellite::any_cast<int>(_Right);
  }

  if (_Left.type() == typeid(size_t))
  {
    return ::covellite::any_cast<size_t>(_Left) ==
      ::covellite::any_cast<size_t>(_Right);
  }

  if (_Left.type() == typeid(float))
  {
    return ::covellite::any_cast<float>(_Left) ==
      ::covellite::any_cast<float>(_Right);
  }

  if (_Left.type() == typeid(const uint8_t *))
  {
    return ::covellite::any_cast<const uint8_t *>(_Left) ==
      ::covellite::any_cast<const uint8_t *>(_Right);
  }

  if (_Left.type() == typeid(const int *))
  {
    return ::covellite::any_cast<const int *>(_Left) ==
      ::covellite::any_cast<const int *>(_Right);
  }

  using Vertex_t = ::covellite::api::renderer::IGraphicApi::Vertex;

  if (_Left.type() == typeid(const Vertex_t *))
  {
    return ::covellite::any_cast<const Vertex_t *>(_Left) ==
      ::covellite::any_cast<const Vertex_t *>(_Right);
  }

  throw STD_EXCEPTION << "Unknown type values.";
}

} // namespace std

// Общий тестовый класс класса Renderer
class Renderer_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::Renderer;
  using RenderInterface_t = ::covellite::api::RenderInterface;
  using Data_t = Tested_t::Data;
  using String_t = ::alicorn::extension::std::String;
  using Component_t = ::covellite::api::Component;
  using Renders_t = ::covellite::api::Component::Renders;
  using Values_t = ::std::vector<::covellite::Any_t>;
  using Vertex_t = ::covellite::api::renderer::IGraphicApi::Vertex;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<String_t>::Set(uT("DefaultString"));
    ::testing::DefaultValue<const Renders_t::Creators_t &>::Set(m_EmptyCreators);
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<String_t>::Clear();
    ::testing::DefaultValue<const Renders_t::Creators_t &>::Clear();
  }

protected:
  class ComponentData
  {
  public:
    template<class T>
    ComponentData & AddValue(const String_t & _Name, const T & _DefaultValue)
    {
      m_Values.push_back(m_pComponent->GetValue<T>(_Name, _DefaultValue));
      return *this;
    }

    inline Values_t GetValues(void) const
    {
      return m_Values;
    }

  private:
    Component_t::ComponentPtr_t m_pComponent;
    Values_t m_Values;

  public:
    explicit ComponentData(const Component_t::ComponentPtr_t & _pComponent) :
      m_pComponent(_pComponent)
    {

    }
  };

  class RenderProxy
  {
  public:
    MOCK_METHOD1(Render, void(Values_t));
  };

private:
  Renders_t::Creators_t m_EmptyCreators;
};

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
TEST_F(Renderer_test, /*DISABLED_*/Test_RenderGeometry)
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

  ::std::vector<::mock::Rocket::Core::Vertex> Vertices{ 10 };
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
    CompileGeometry(Vertices.data(), VertexSize, Index.data(), IndexSize, hTexture))
    .Times(1)
    .WillOnce(Return(hGeometry));

  EXPECT_CALL(Example, RenderCompiledGeometry(hGeometry, Position))
    .Times(1);

  EXPECT_CALL(Example, ReleaseCompiledGeometry(hGeometry))
    .Times(1);

  IExample.RenderGeometry(
    Vertices.data(), VertexSize, Index.data(), IndexSize, hTexture, Position);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_CompileGeometry_UniqueHahdlers)
{
  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  ::std::set<::mock::Rocket::Core::CompiledGeometryHandle> Handles;

  const size_t RepeatCount = 10;

  for (size_t i = 0; i < RepeatCount; i++)
  {
    Handles.insert(IExample.CompileGeometry(nullptr, 0, nullptr, 0, 0));
  }

  EXPECT_EQ(RepeatCount, Handles.size());
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_RenderCompiledGeometry_Textured)
{
  RenderProxy RenderProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  auto ShaderCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    const auto Kind = _pComponent->GetValue(uT("kind"), uT(""));

    if (Kind == uT("Vertex"))
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("id"), uT(""))
          .GetValues());
      };
    }
    if (Kind == uT("Pixel"))
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("kind"), uT(""))
          .AddValue(uT("version"), uT(""))
          .AddValue(uT("entry"), uT(""))
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("data"), (const uint8_t *)nullptr)
          .AddValue(uT("count"), (size_t)0)
          .GetValues());
      };
    }

    throw ::std::exception{ "Unknown shader kind." };
  };

  auto PositionCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("x"), 0.0f)
        .AddValue(uT("y"), 0.0f)
        .GetValues());
    };
  };

  auto BufferCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    const auto Kind = _pComponent->GetValue(uT("kind"), uT(""));

    if (Kind == uT("Vertex"))
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("kind"), uT(""))
          .AddValue(uT("data"), (const Vertex_t *)nullptr)
          .AddValue(uT("count"), (size_t)0)
          .GetValues());
      };
    }
    else if (Kind == uT("Index"))
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("kind"), uT(""))
          .AddValue(uT("data"), (const int *)nullptr)
          .AddValue(uT("count"), (size_t)0)
          .GetValues());
      };
    }

    throw ::std::exception{ "Unknown buffer kind." };
  };

  auto SimpleCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("id"), uT(""))
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Shader"), ShaderCreator },
    { uT("Texture"), SimpleCreator },
    { uT("Position"), PositionCreator },
    { uT("Buffer"), BufferCreator },
    { uT("DrawCall"), SimpleCreator },
  };

  const Values_t DefaultValues =
  {
    uT("Shader"),
    uT("Covellite.Api.Shader.Vertex")
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, GetCreators(_))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  auto * const pVertex = (::mock::Rocket::Core::Vertex *)1811161257;
  const int VertexCount = 1811161258;
  auto * const pIndex = (int *)1811161259;
  const int IndexCount = 1811161300;
  const size_t TextureId = 1811161754;

  const auto hGeometry =
    IExample.CompileGeometry(pVertex, VertexCount, pIndex, IndexCount, TextureId);

  const auto strObjectId = uT("{ID}").Replace(uT("{ID}"), (size_t)hGeometry);

  const Values_t PixelShaderValues =
  {
    uT("Shader"),
    uT("Pixel"),
    uT("ps_4_0"),
    uT("psTextured"),
    uT("Covellite.Api.Shader.Pixel.Textured"),
    ::Pixel.data(),
    ::Pixel.size(),
  };

  const Values_t TextureValues =
  {
    uT("Texture"),
    uT("Covellite.Api.Texture.{ID}").Replace(uT("{ID}"), TextureId)
  };

  const Values_t VertexBufferValues =
  {
    uT("Buffer"),
    uT("Covellite.Api.Buffer.Vertex.") + strObjectId,
    uT("Vertex"),
    (const Vertex_t *)pVertex,
    (size_t)VertexCount,
  };

  const Values_t IndexBufferValues =
  {
    uT("Buffer"),
    uT("Covellite.Api.Buffer.Index.") + strObjectId,
    uT("Index"),
    (const int *)pIndex,
    (size_t)IndexCount,
  };

  const Values_t DrawCallValues =
  {
    uT("DrawCall"),
    uT("Covellite.Api.DrawCall")
  };

  // Первый вызов
  {
    const float X = 1811161750.0f;
    const float Y = 1811161751.0f;

    const Values_t PositionValues =
    {
      uT("Position"),
      uT("Covellite.Api.Position.") + strObjectId,
      X,
      Y,
    };

    IExample.RenderCompiledGeometry(hGeometry, { X, Y });

    EXPECT_CALL(RenderProxy, Render(DefaultValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(PixelShaderValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(TextureValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(PositionValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(VertexBufferValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(IndexBufferValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(DrawCallValues))
      .Times(1);

    Example.PresentFrame();
  }

  // Второй вызов
  {
    const float X = 1811161752.0f;
    const float Y = 1811161753.0f;

    const Values_t PositionValues =
    {
      uT("Position"),
      uT("Covellite.Api.Position.") + strObjectId,
      X,
      Y,
    };

    IExample.RenderCompiledGeometry(hGeometry, { X, Y });

    EXPECT_CALL(RenderProxy, Render(DefaultValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(PixelShaderValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(TextureValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(PositionValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(VertexBufferValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(IndexBufferValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(DrawCallValues))
      .Times(1);

    Example.PresentFrame();
  }

  IExample.ReleaseCompiledGeometry(hGeometry);

  // Вызов для проверки удаления рендеров
  {
    IExample.RenderCompiledGeometry(hGeometry, { 0, 0 });

    EXPECT_CALL(RenderProxy, Render(DefaultValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(_))
      .Times(0);

    Example.PresentFrame();
  }
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_RenderCompiledGeometry_NonTextured)
{
  RenderProxy RenderProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  auto ShaderCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    const auto Kind = _pComponent->GetValue(uT("kind"), uT(""));

    if (Kind == uT("Vertex"))
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("id"), uT(""))
          .GetValues());
      };
    }
    if (Kind == uT("Pixel"))
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("kind"), uT(""))
          .AddValue(uT("version"), uT(""))
          .AddValue(uT("entry"), uT(""))
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("data"), (const uint8_t *)nullptr)
          .AddValue(uT("count"), (size_t)0)
          .GetValues());
      };
    }

    throw ::std::exception{ "Unknown shader kind." };
  };

  auto PositionCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("x"), 0.0f)
        .AddValue(uT("y"), 0.0f)
        .GetValues());
    };
  };

  auto BufferCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    const auto Kind = _pComponent->GetValue(uT("kind"), uT(""));

    if (Kind == uT("Vertex"))
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("kind"), uT(""))
          .AddValue(uT("data"), (const Vertex_t *)nullptr)
          .AddValue(uT("count"), (size_t)0)
          .GetValues());
      };
    }
    else if (Kind == uT("Index"))
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("kind"), uT(""))
          .AddValue(uT("data"), (const int *)nullptr)
          .AddValue(uT("count"), (size_t)0)
          .GetValues());
      };
    }

    throw ::std::exception{ "Unknown buffer kind." };
  };

  auto SimpleCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("id"), uT(""))
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Shader"), ShaderCreator },
    { uT("Position"), PositionCreator },
    { uT("Buffer"), BufferCreator },
    { uT("DrawCall"), SimpleCreator },
  };

  const Values_t DefaultValues = 
  { 
    uT("Shader"), 
    uT("Covellite.Api.Shader.Vertex") 
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, GetCreators(_))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  auto * const pVertex = (::mock::Rocket::Core::Vertex *)1811161257;
  const int VertexCount = 1811161258;
  auto * const pIndex = (int *)1811161259;
  const int IndexCount = 1811161300;

  const auto hGeometry = 
    IExample.CompileGeometry(pVertex, VertexCount, pIndex, IndexCount, 0);

  const auto strObjectId = uT("{ID}").Replace(uT("{ID}"), (size_t)hGeometry);

  const Values_t PixelShaderValues =
  {
    uT("Shader"),
    uT("Pixel"),
    uT("ps_4_0"),
    uT("psColored"),
    uT("Covellite.Api.Shader.Pixel.Colored"),
    ::Pixel.data(),
    ::Pixel.size(),
  };

  const Values_t VertexBufferValues =
  {
    uT("Buffer"),
    uT("Covellite.Api.Buffer.Vertex.") + strObjectId,
    uT("Vertex"),
    (const Vertex_t *)pVertex,
    (size_t)VertexCount,
  };

  const Values_t IndexBufferValues =
  {
    uT("Buffer"),
    uT("Covellite.Api.Buffer.Index.") + strObjectId,
    uT("Index"),
    (const int *)pIndex,
    (size_t)IndexCount,
  };

  const Values_t DrawCallValues = 
  { 
    uT("DrawCall"), 
    uT("Covellite.Api.DrawCall") 
  };

  // Первый вызов
  {
    const float X = 1811161301.0f;
    const float Y = 1811161302.0f;

    const Values_t PositionValues =
    {
      uT("Position"),
      uT("Covellite.Api.Position.") + strObjectId,
      X,
      Y,
    };

    IExample.RenderCompiledGeometry(hGeometry, { X, Y });

    EXPECT_CALL(RenderProxy, Render(DefaultValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(PixelShaderValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(PositionValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(VertexBufferValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(IndexBufferValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(DrawCallValues))
      .Times(1);

    Example.PresentFrame();
  }

  // Второй вызов
  {
    const float X = 1811161303.0f;
    const float Y = 1811161304.0f;

    const Values_t PositionValues =
    {
      uT("Position"),
      uT("Covellite.Api.Position.") + strObjectId,
      X,
      Y,
    };

    IExample.RenderCompiledGeometry(hGeometry, { X, Y });

    EXPECT_CALL(RenderProxy, Render(DefaultValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(PixelShaderValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(PositionValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(VertexBufferValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(IndexBufferValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(DrawCallValues))
      .Times(1);

    Example.PresentFrame();
  }

  IExample.ReleaseCompiledGeometry(hGeometry);

  // Вызов для проверки удаления рендеров
  {
    IExample.RenderCompiledGeometry(hGeometry, { 0, 0 });

    EXPECT_CALL(RenderProxy, Render(DefaultValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(_))
      .Times(0);

    Example.PresentFrame();
  }
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_ReleaseCompiledGeometry_RemoveUniqueComponents)
{
  RenderProxy RenderProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("data"), uT(""))
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Position"), Creator },
    { uT("Buffer"), Creator },
  };

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, GetCreators(_))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  const size_t ObjectId = 1811161821;
  const auto strObjectId = uT("{ID}").Replace(uT("{ID}"), ObjectId);

  auto pRenders = IExample.GetRenders();

  {
    auto Renders = pRenders->Obtain(
      {
        Component_t::Make(
        {
          { uT("type"), uT("Position") },
          { uT("id"), uT("Covellite.Api.Position.") + strObjectId },
          { uT("data"), uT("Position.1811161820") },
         }),
        Component_t::Make(
        {
          { uT("type"), uT("Buffer") },
          { uT("id"), uT("Covellite.Api.Buffer.Vertex.") + strObjectId },
          { uT("data"), uT("Vertex.1811161832") },
         }),
        Component_t::Make(
        {
          { uT("type"), uT("Buffer") },
          { uT("id"), uT("Covellite.Api.Buffer.Index.") + strObjectId },
          { uT("data"), uT("Index.1811161840") },
         }),
      });

    const Values_t ExpectedPositionData =
    {
      uT("Position"),
      uT("Covellite.Api.Position.") + strObjectId,
      uT("Position.1811161820")
    };

    const Values_t ExpectedVertexBufferData =
    {
      uT("Buffer"),
      uT("Covellite.Api.Buffer.Vertex.") + strObjectId,
      uT("Vertex.1811161832")
    };

    const Values_t ExpectedIndexBufferData =
    {
      uT("Buffer"),
      uT("Covellite.Api.Buffer.Index.") + strObjectId,
      uT("Index.1811161840")
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedPositionData))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(ExpectedVertexBufferData))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(ExpectedIndexBufferData))
      .Times(1);

    EXPECT_EQ(3, Renders.size());
    for (auto & Render : Renders) Render();
  }

  IExample.ReleaseCompiledGeometry(
    (::mock::Rocket::Core::CompiledGeometryHandle)ObjectId);

  {
    auto Renders = pRenders->Obtain(
      {
        Component_t::Make(
        {
          { uT("type"), uT("Position") },
          { uT("id"), uT("Covellite.Api.Position.") + strObjectId },
        }),
        Component_t::Make(
        {
          { uT("type"), uT("Buffer") },
          { uT("id"), uT("Covellite.Api.Buffer.Vertex.") + strObjectId },
         }),
        Component_t::Make(
        {
          { uT("type"), uT("Buffer") },
          { uT("id"), uT("Covellite.Api.Buffer.Index.") + strObjectId },
         }),
      });

    const Values_t ExpectedPositionData =
    {
      uT("Position"),
      uT("Covellite.Api.Position.") + strObjectId,
      uT("")
    };

    const Values_t ExpectedVertexBufferData =
    {
      uT("Buffer"),
      uT("Covellite.Api.Buffer.Vertex.") + strObjectId,
      uT("")
    };

    const Values_t ExpectedIndexBufferData =
    {
      uT("Buffer"),
      uT("Covellite.Api.Buffer.Index.") + strObjectId,
      uT("")
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedPositionData))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(ExpectedVertexBufferData))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(ExpectedIndexBufferData))
      .Times(1);

    EXPECT_EQ(3, Renders.size());
    for (auto & Render : Renders) Render();
  }
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_EnableScissorRegion_True)
{
  RenderProxy RenderProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("State"), Creator },
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, GetCreators(_))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  IExample.EnableScissorRegion(true);

  EXPECT_CALL(RenderProxy, Render(
    Values_t{ uT("State"), uT("Covellite.Api.State.Blend"), uT("Blend") }))
    .Times(1);

  EXPECT_CALL(RenderProxy, Render(
    Values_t{ uT("State"), uT("Covellite.Api.State.Sampler"), uT("Sampler") }))
    .Times(1);

  EXPECT_CALL(RenderProxy, Render(_))
    .Times(0);

  EXPECT_CALL(RendererImplProxy, PresentFrame(_))
    .Times(1);

  Example.PresentFrame();
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_EnableScissorRegion_False)
{
  RenderProxy RenderProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .AddValue(uT("is_enabled"), uT(""))
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("State"), Creator },
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, GetCreators(_))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  IExample.EnableScissorRegion(false);

  EXPECT_CALL(RenderProxy, Render(
    Values_t{ uT("State"), uT("Covellite.Api.State.Blend"), uT("Blend"), uT("") }))
    .Times(1);

  EXPECT_CALL(RenderProxy, Render(
    Values_t{ uT("State"), uT("Covellite.Api.State.Sampler"), uT("Sampler"), uT("") }))
    .Times(1);

  const Values_t ExpectScissorInfo = 
  { 
    uT("State"), 
    uT("Covellite.Api.State.Scissor.Disabled"), 
    uT("Scissor"), 
    uT("false"),
  };

  EXPECT_CALL(RenderProxy, Render(ExpectScissorInfo))
    .Times(1);

  EXPECT_CALL(RendererImplProxy, PresentFrame(_))
    .Times(1);

  Example.PresentFrame();
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_SetScissorRegion)
{
  RenderProxy RenderProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    const auto Kind = _pComponent->GetValue(uT("kind"), uT(""));

    if (Kind == uT("Scissor"))
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("kind"), uT(""))
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("is_enabled"), uT(""))
          .AddValue(uT("left"), 0)
          .AddValue(uT("right"), 0)
          .AddValue(uT("top"), 0)
          .AddValue(uT("bottom"), 0)
          .GetValues());
      };
    }
    
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("id"), uT(""))
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("State"), Creator },
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, GetCreators(_))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  // Первый вызов
  {
    const int X = 1811081503;
    const int Y = 1811081504;
    const int Width = 81505;
    const int Height = 81506;

    IExample.SetScissorRegion(X, Y, Width, Height);

    EXPECT_CALL(RenderProxy, Render(
      Values_t{ uT("State"), uT("Covellite.Api.State.Blend") }))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(
      Values_t{ uT("State"), uT("Covellite.Api.State.Sampler") }))
      .Times(1);

    const Values_t ExpectedScissorData =
    {
      uT("State"),
      uT("Scissor"),
      uT("Covellite.Api.State.Scissor.Enabled"),
      uT("true"),
      X,
      X + Width,
      Y,
      Y + Height,
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedScissorData))
      .Times(1);

    EXPECT_CALL(RendererImplProxy, PresentFrame(_))
      .Times(1);

    Example.PresentFrame();
  }

  // Второй вызов (с другими значениями координат)
  {
    const int X = 1811081519;
    const int Y = 1811081520;
    const int Width = 81505;
    const int Height = 81506;

    IExample.SetScissorRegion(X, Y, Width, Height);

    EXPECT_CALL(RenderProxy, Render(
      Values_t{ uT("State"), uT("Covellite.Api.State.Blend") }))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(
      Values_t{ uT("State"), uT("Covellite.Api.State.Sampler") }))
      .Times(1);

    const Values_t ExpectedScissorData =
    {
      uT("State"),
      uT("Scissor"),
      uT("Covellite.Api.State.Scissor.Enabled"),
      uT("true"),
      X,
      X + Width,
      Y,
      Y + Height,
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedScissorData))
      .Times(1);

    EXPECT_CALL(RendererImplProxy, PresentFrame(_))
      .Times(1);

    Example.PresentFrame();
  }
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
TEST_F(Renderer_test, /*DISABLED_*/Test_GenerateTexture)
{
  RenderProxy RenderProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("id"), uT(""))
        .AddValue<const uint8_t *>(uT("data"), nullptr)
        .AddValue(uT("width"), 0)
        .AddValue(uT("height"), 0)
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Texture"), Creator },
  };

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, GetCreators(_))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  // Первый вызов функции
  {
    const uint8_t * const pData = (uint8_t *)1811071432;
    const int Width = 1811071433;
    const int Height = 1811071434;

    ::mock::Rocket::Core::TextureHandle hTexture = 0;
    auto Result = IExample.GenerateTexture(hTexture, pData, { Width, Height });
    EXPECT_TRUE(Result);
    EXPECT_EQ(1, hTexture);

    auto Renders = IExample.GetRenders()->Obtain(
      {
        Component_t::Make(
          {
            { uT("type"), uT("Texture") },
            { uT("id"), uT("Covellite.Api.Texture.1") },
          }),
      });
    ASSERT_EQ(1, Renders.size());

    const Values_t ExpectedTextureData =
    {
      uT("Texture"),
      uT("Covellite.Api.Texture.1"),
      pData,
      Width,
      Height
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedTextureData))
      .Times(1);

    Renders[0]();
  }

  // Второй вызов функции
  {
    const uint8_t * const pData = (uint8_t *)1811071454;
    const int Width = 1811071455;
    const int Height = 1811071456;

    ::mock::Rocket::Core::TextureHandle hTexture = 0;
    auto Result = IExample.GenerateTexture(hTexture, pData, { Width, Height });
    EXPECT_TRUE(Result);
    EXPECT_EQ(2, hTexture);

    auto Renders = IExample.GetRenders()->Obtain(
      {
        Component_t::Make(
          {
            { uT("type"), uT("Texture") },
            { uT("id"), uT("Covellite.Api.Texture.2") },
          }),
      });
    ASSERT_EQ(1, Renders.size());

    const Values_t ExpectedTextureData =
    {
      uT("Texture"),
      uT("Covellite.Api.Texture.2"),
      pData,
      Width,
      Height
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedTextureData))
      .Times(1);

    Renders[0]();
  }
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_GenerateTexture_Fail)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t &) -> Renders_t::Render_t
  {
    throw ::std::exception{};
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Texture"), Creator },
  };

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, GetCreators(_))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  ::mock::Rocket::Core::TextureHandle hTexture = 0;
  auto Result = IExample.GenerateTexture(hTexture, nullptr, { 0, 0 });
  EXPECT_FALSE(Result);
  EXPECT_EQ(0, hTexture);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_ReleaseTexture)
{
  RenderProxy RenderProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("id"), uT(""))
        .AddValue<const uint8_t *>(uT("data"), nullptr)
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Texture"), Creator },
  };

  using namespace ::testing;

  EXPECT_CALL(RendererImplProxy, GetCreators(_))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };
  RenderInterface_t & IExample = Example;

  auto pRenders = IExample.GetRenders();

  {
    auto Renders = pRenders->Obtain(
      {
          Component_t::Make(
          {
            { uT("type"), uT("Texture") },
            { uT("id"), uT("Covellite.Api.Texture.1811071543") },
            { uT("data"), (const uint8_t *)1811071544 },
           })
      });

    ASSERT_EQ(1, Renders.size());

    const Values_t ExpectedTextureData =
    {
      uT("Texture"),
      uT("Covellite.Api.Texture.1811071543"),
      (const uint8_t *)1811071544,
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedTextureData))
      .Times(1);

    Renders[0]();
  }

  IExample.ReleaseTexture(1811071543);

  {
    auto Renders = pRenders->Obtain(
      {
          Component_t::Make(
          {
            { uT("type"), uT("Texture") },
            { uT("id"), uT("Covellite.Api.Texture.1811071543") },
           })
      });

    ASSERT_EQ(1, Renders.size());

    const Values_t ExpectedTextureData =
    {
      uT("Texture"),
      uT("Covellite.Api.Texture.1811071543"),
      (const uint8_t *)nullptr,
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedTextureData))
      .Times(1);

    Renders[0]();
  }
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_GetRenders)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererImplId = 1811051609;

  auto Creator = [](const Component_t::ComponentPtr_t &) -> Renders_t::Render_t
  {
    return []() {};
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Type1811051618"), Creator },
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererImplId));

  EXPECT_CALL(RendererImplProxy, GetCreators(RendererImplId))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  const Tested_t Example{ uT("RendererImpl"), Data_t{} };
  const RenderInterface_t & IExample = Example;

  const auto pResult = IExample.GetRenders();

  const auto Renders = pResult->Obtain(
    {
      Component_t::Make(
        {
          { uT("type"), uT("Type1811051618") },
        }),
    });

  EXPECT_EQ(1, Renders.size());
  EXPECT_EQ(pResult, IExample.GetRenders());
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_StartDrawingFrame)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1811061313;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };

  EXPECT_CALL(RendererImplProxy, ClearFrame(RendererId))
    .Times(1);

  Example.StartDrawingFrame();
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_PresentFrame)
{
  RenderProxy RenderProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RendererId = 1811061313;

  const Values_t ExpectedShaderData =
  {
    uT("Shader"),
    uT("Vertex"),
    uT("vs_4_0"),
    uT("VS"),
    uT("Covellite.Api.Shader.Vertex"),
    ::Vertex.data(),
    ::Vertex.size(),
  };

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .GetValues());
    };
  };

  auto ShaderCreator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .AddValue(uT("version"), uT(""))
        .AddValue(uT("entry"), uT(""))
        .AddValue(uT("id"), uT(""))
        .AddValue<const uint8_t *>(uT("data"), nullptr)
        .AddValue(uT("count"), (size_t)0)
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Camera"), Creator },
    { uT("State"), Creator },
    { uT("Shader"), ShaderCreator },
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  EXPECT_CALL(RendererImplProxy, GetCreators(RendererId))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };

  //************** Рендеры по умолчанию, общие для всех объектов *************//

  EXPECT_CALL(RenderProxy, Render(
    Values_t{ uT("Camera"), uT("Covellite.Api.Camera"), uT("") }))
    .Times(1);

  EXPECT_CALL(RenderProxy, Render(
    Values_t{ uT("State"), uT("Covellite.Api.State.Blend"), uT("Blend") }))
    .Times(1);

  EXPECT_CALL(RenderProxy, Render(
    Values_t{ uT("State"), uT("Covellite.Api.State.Sampler"), uT("Sampler") }))
    .Times(1);

  EXPECT_CALL(RenderProxy, Render(Eq(ExpectedShaderData)))
    .Times(1);

  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//

  EXPECT_CALL(RendererImplProxy, PresentFrame(RendererId))
    .Times(1);

  Example.PresentFrame();
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_ResizeWindow)
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

  EXPECT_CALL(RendererImplProxy, ResizeWindow(RendererId, X, Y))
    .Times(1);

  Example.ResizeWindow(X, Y);
}
