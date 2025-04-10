
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Renderer.
*/

// ������������ ������ Renderer
// ���� ���������� ������� �������� ��-�� ����, ��� ��������� �� �������,
// ���������� �������, � ��������� ������ �� ����� ���������.
#include "../../Covellite.Gui/Renderer.cpp"

namespace std
{

bool operator== (
  const ::covellite::api::Vertex & _Left,
  const ::covellite::api::Vertex & _Right)
{
  return (memcmp(&_Left, &_Right, sizeof(::covellite::api::Vertex)) == 0);
}

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

  using Vertexes_t = ::std::vector<::covellite::api::Vertex>;

  if (_Left.type() == typeid(Vertexes_t))
  {
    return ::covellite::any_cast<Vertexes_t>(_Left) ==
      ::covellite::any_cast<Vertexes_t>(_Right);
  }

  if (_Left.type() == typeid(::std::vector<int>))
  {
    return ::covellite::any_cast<::std::vector<int>>(_Left) ==
      ::covellite::any_cast<::std::vector<int>>(_Right);
  }

  if (_Left.type() == typeid(::std::vector<uint8_t>))
  {
    return ::covellite::any_cast<::std::vector<uint8_t>>(_Left) ==
      ::covellite::any_cast<::std::vector<uint8_t>>(_Right);
  }

  throw STD_EXCEPTION << "Unknown type values: " << _Left.type().name();
}

} // namespace std

// ����� �������� ����� ������ Renderer
class Renderer_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::Renderer;
  using RenderInterface_t = ::mock::CovelliteGui::RenderInterface;
  using String_t = ::alicorn::extension::std::String;
  using Component_t = ::covellite::api::Component;
  using Renders_t = Component_t::Renders;
  using Values_t = ::std::vector<::covellite::Any_t>;
  using Vertex_t = ::covellite::api::Vertex;
  using ComponentPtr_t = ::std::shared_ptr<Component_t>;
  using Object_t = ::std::vector<ComponentPtr_t>;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
    ::testing::DefaultValue<String_t>::Set(uT("DefaultString"));
    ::testing::DefaultValue<const Renders_t::Creators_t &>::Set(m_EmptyCreators);
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
    ::testing::DefaultValue<String_t>::Clear();
    ::testing::DefaultValue<const Renders_t::Creators_t &>::Clear();
  }

protected:
  class ComponentData
  {
    template<class T>
    class Support
    {
    public:
      static T GetValue(
        const Component_t::ComponentPtr_t & _pComponent,
        const String_t & _Name, 
        const T & _DefaultValue)
      {
        return (*_pComponent)[_Name].Default(_DefaultValue);
      }
    };

    template<>
    class Support<::std::vector<Vertex_t>>
    {
    public:
      static ::std::vector<Vertex_t> GetValue(
        const Component_t::ComponentPtr_t & /*_pComponent*/,
        const String_t & /*_Name*/,
        const ::std::vector<Vertex_t> & _DefaultValue)
      {
        return _DefaultValue;
      }
    };

  public:
    template<class T>
    ComponentData & AddValue(const String_t & _Name, const T & _DefaultValue)
    {
      const auto Value = Support<T>::GetValue(m_pComponent, _Name, _DefaultValue);
      m_Values.push_back(Value);
      return *this;
    }

    inline Values_t GetValues(void) const { return m_Values; }

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

protected:
  ::std::vector<Vertex_t> Convert(
    const ::std::vector<::mock::CovelliteGui::Vertex> & _Source)
  {
    ::std::vector<::covellite::api::Vertex> Result{ _Source.size() };

    for (::std::size_t i = 0; i < Result.size(); i++)
    {
      auto & Vertex = Result[i];

      Vertex.px = _Source[i].position.x;
      Vertex.py = _Source[i].position.y;
      Vertex.pz = 0.0f;
      Vertex.pw = 1.0f;

      Vertex.tu = _Source[i].tex_coord.x;
      Vertex.tv = _Source[i].tex_coord.y;

      Vertex.ex = _Source[i].colour.red / 255.0f;
      Vertex.ey = _Source[i].colour.green / 255.0f;
      Vertex.ez = _Source[i].colour.blue / 255.0f;
      Vertex.ew = _Source[i].colour.alpha / 255.0f;
    }

    return Result;
  }
};

// ������� ������� ��� ����������� � ����� Renderer 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Renderer_test, Test_Function);

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_RenderGeometry)
{
  class Tested :
    public Tested_t
  {
    using Renders_t = ::covellite::api::Component::Renders;
    using CGHandle_t = ::mock::CovelliteGui::CompiledGeometryHandle;
    using Vertex_t = ::mock::CovelliteGui::Vertex;
    using THandle_t = ::mock::CovelliteGui::TextureHandle;
    using Vector2f_t = ::mock::CovelliteGui::Vector2f;

  public:
    MOCK_METHOD5(CompileGeometry, CGHandle_t(Vertex_t *, int, int *, int, THandle_t));
    MOCK_METHOD2(RenderCompiledGeometry, void(CGHandle_t, const Vector2f_t &));
    MOCK_METHOD1(ReleaseCompiledGeometry, void(CGHandle_t));

  public:
    Tested(void) :
      Tested_t(::std::make_shared<Renders_t>(Renders_t::Creators_t{}))
    {

    }
  };

  ::std::vector<::mock::CovelliteGui::Vertex> Vertices{ 10 };
  const int VertexSize = 1808251051;
  ::std::vector<int> Index{ 20 };
  const int IndexSize = 1808251052;
  ::mock::CovelliteGui::TextureHandle hTexture = 1808251053;
  ::mock::CovelliteGui::CompiledGeometryHandle hGeometry = (void *)1808251054;
  ::mock::CovelliteGui::Vector2f Position = { 1808251056.0f, 1808251057.0f };

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
  Tested_t Example{ ::std::make_shared<Renders_t>(Renders_t::Creators_t{}) };
  RenderInterface_t & IExample = Example;

  ::std::set<::mock::CovelliteGui::CompiledGeometryHandle> Handles;

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

  auto ShaderCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    if (_pComponent->Id.Find(uT("Pixel")) == String_t::Npos) return nullptr;

    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("entry"), uT(""))
        .GetValues());
    };
  };

  auto BufferCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    if ((*_pComponent)[uT("content")].IsType<::std::vector<Vertex_t>>())
    {
      const ::std::vector<Vertex_t> Data = (*_pComponent)[uT("content")];

      return [&, _pComponent, Data]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("content"), Data)
          .AddValue(uT("dimension"), 0)
          .GetValues());
      };
    }
    else if ((*_pComponent)[uT("content")].IsType<::std::vector<int>>())
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("content"), ::std::vector<int>{})
          .GetValues());
      };
    }

    throw ::std::exception{ "Unknown buffer type." };
  };

  auto TransformCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    const Object_t Data = (*_pComponent)[uT("service")].Default(Object_t{});
    if (Data.size() != 1) return nullptr;

    const auto pPosition = Data.at(0);
    const float X = (*pPosition)[uT("x")];
    const float Y = (*pPosition)[uT("y")];

    using namespace ::alicorn::extension::std;

    return [&, _pComponent, pPosition]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .GetValues() +
        ComponentData{ pPosition }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .AddValue(uT("x"), 0.0f)
        .AddValue(uT("y"), 0.0f)
        .GetValues());
    };
  };

  auto SimpleCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      const auto Values = ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .GetValues();

      RenderProxy.Render(Values);
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Texture"), SimpleCreator },
    { uT("Shader"), ShaderCreator },
    { uT("Buffer"), BufferCreator },
    { uT("Transform"), TransformCreator },
    { uT("Present"), BufferCreator },
  };

  Tested_t Example{ ::std::make_shared<Renders_t>(Creators) };
  RenderInterface_t & IExample = Example;

  ::std::vector<::mock::CovelliteGui::Vertex> Vertexes =
  {
    {
      { 190.8202318f, 1908.202319f },
      { 0x19, 0x08, 0x20, 0x23 },
      { 19082.02320f, 190820.2321f },
    },
    {
      { 190.8202317f, 1908.202316f },
      { 0x18, 0x09, 0x21, 0x24 },
      { 19.08202315f, 1.908202314f },
    }
  };

  const auto Converted = Convert(Vertexes);

  ::std::vector<int> Indexes =
  {
    2006131139,
    2006131140,
    2006131141,
    2006131142,
    2006131143,
    2006131144
  };

  const size_t TextureId = 1811161754;

  const auto hGeometry = IExample.CompileGeometry(
    Vertexes.data(), static_cast<int>(Vertexes.size()), 
    Indexes.data(), static_cast<int>(Indexes.size()), TextureId);

  const auto strObjectId = uT("{ID}").Replace(uT("{ID}"), (size_t)hGeometry);

  const Values_t PixelShaderValues =
  {
    uT("Covellite.Gui.Shader.Pixel.Textured"),
    uT("Shader"),
    uT("psTextured"),
  };

  const Values_t TextureValues =
  {
    uT("Covellite.Gui.Texture.{ID}").Replace(uT("{ID}"), TextureId),
    uT("Texture"),
    uT("Unknown")
  };

  const Values_t VertexBufferValues =
  {
    uT("Covellite.Gui.Buffer.Vertex.") + strObjectId,
    uT("Buffer"),
    Converted,
    2
  };

  const Values_t TransformValues =
  {
    uT("Covellite.Gui.Transform.") + strObjectId,
    uT("Transform"),
    uT("Unknown")
  };

  const Values_t PresentValues =
  {
    uT("Covellite.Gui.Present.") + strObjectId,
    uT("Present"),
    Indexes,
  };

  using namespace ::testing;

  InSequence Dummy;

  const auto TestCall = [&](const float _X, const float _Y)
  {
    const Values_t PositionValues = { uT("Data"), uT("Position"), _X, _Y };

    IExample.RenderCompiledGeometry(hGeometry, { _X, _Y });

    EXPECT_CALL(RenderProxy, Render(PixelShaderValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(TextureValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(VertexBufferValues))
      .Times(1);

    using namespace ::alicorn::extension::std;

    EXPECT_CALL(RenderProxy, Render(TransformValues + PositionValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(PresentValues))
      .Times(1);

    Example.RenderScene();
  };

  TestCall(1811161750.0f, 1811161751.0f);
  TestCall(1908202233.0f, 1908202234.0f);

  IExample.ReleaseCompiledGeometry(hGeometry);

  // ����� ��� �������� �������� ��������
  {
    IExample.RenderCompiledGeometry(hGeometry, { 0, 0 });

    EXPECT_CALL(RenderProxy, Render(_))
      .Times(0);

    Example.RenderScene();
  }
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_RenderCompiledGeometry_NonTextured)
{
  RenderProxy RenderProxy;

  auto ShaderCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    if (_pComponent->Id.Find(uT("Pixel")) == String_t::Npos) return nullptr;

    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("entry"), uT(""))
        .GetValues());
    };
  };

  auto BufferCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    if ((*_pComponent)[uT("content")].IsType<::std::vector<Vertex_t>>())
    {
      const ::std::vector<Vertex_t> Data = (*_pComponent)[uT("content")];

      return [&, _pComponent, Data]()
      {
        static const ::std::vector<Vertex_t> Dummy;

        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("content"), Data)
          .AddValue(uT("dimension"), 0)
          .GetValues());
      };
    }
    else if ((*_pComponent)[uT("content")].IsType<::std::vector<int>>())
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("content"), ::std::vector<int>{})
          .GetValues());
      };
    }

    throw ::std::exception{ "Unknown buffer type." };
  };

  auto SimpleCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    if (_pComponent->Type == uT("Transform"))
    {
      const Object_t Data = (*_pComponent)[uT("service")].Default(Object_t{});
      if (Data.size() != 1) return nullptr;

      const auto pPosition = Data.at(0);
      const float X = (*pPosition)[uT("x")];
      const float Y = (*pPosition)[uT("y")];

      using namespace ::alicorn::extension::std;

      return [&, _pComponent, pPosition]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("kind"), uT(""))
          .GetValues() +
          ComponentData{ pPosition }
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("kind"), uT(""))
          .AddValue(uT("x"), 0.0f)
          .AddValue(uT("y"), 0.0f)
          .GetValues());
      };
    }
    else
    {
      return [&, _pComponent]()
      {
        RenderProxy.Render(ComponentData{ _pComponent }
          .AddValue(uT("id"), uT(""))
          .AddValue(uT("type"), uT(""))
          .AddValue(uT("kind"), uT(""))
          .GetValues());
      };
    }
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Shader"), ShaderCreator },
    { uT("Buffer"), BufferCreator },
    { uT("Transform"), SimpleCreator },
    { uT("Present"), BufferCreator },
  };

  Tested_t Example{ ::std::make_shared<Renders_t>(Creators) };
  RenderInterface_t & IExample = Example;

  ::std::vector<::mock::CovelliteGui::Vertex> Vertexes =
  {
    {
      { 190.8202318f, 1908.202319f },
      { 0x19, 0x08, 0x20, 0x23 },
      { 19082.02320f, 190820.2321f },
    },
    {
      { 190.8202317f, 1908.202316f },
      { 0x18, 0x09, 0x21, 0x24 },
      { 19.08202315f, 1.908202314f },
    },
    {
      { 190.8202318f, 1908.202319f },
      { 0x19, 0x08, 0x20, 0x23 },
      { 19082.02320f, 190820.2321f },
    },
    {
      { 190.8202317f, 1908.202316f },
      { 0x18, 0x09, 0x21, 0x24 },
      { 19.08202315f, 1.908202314f },
    },
    {
      { 190.8202318f, 1908.202319f },
      { 0x19, 0x08, 0x20, 0x23 },
      { 19082.02320f, 190820.2321f },
    },
    {
      { 190.8202317f, 1908.202316f },
      { 0x18, 0x09, 0x21, 0x24 },
      { 19.08202315f, 1.908202314f },
    },
  };

  const auto Converted = Convert(Vertexes);
  ::std::vector<int> Indexes =
  {
    2006131147,
    2006131148,
    2006131149
  };

  const auto hGeometry = IExample.CompileGeometry(
    Vertexes.data(), static_cast<int>(Vertexes.size()), 
    Indexes.data(), static_cast<int>(Indexes.size()), 0);

  const auto strObjectId = uT("{ID}").Replace(uT("{ID}"), (size_t)hGeometry);

  const Values_t PixelShaderValues =
  {
    uT("Covellite.Gui.Shader.Pixel.Colored"),
    uT("Shader"),
    uT("psColored"),
  };

  const Values_t VertexBufferValues =
  {
    uT("Covellite.Gui.Buffer.Vertex.") + strObjectId,
    uT("Buffer"),
    Converted,
    2
  };

  const Values_t TransformValues =
  {
    uT("Covellite.Gui.Transform.") + strObjectId,
    uT("Transform"),
    uT("Unknown")
  };

  const Values_t PresentValues =
  {
    uT("Covellite.Gui.Present.") + strObjectId,
    uT("Present"),
    Indexes,
  };

  using namespace ::testing;

  InSequence Dummy;

  const auto TestCall = [&](const float _X, const float _Y)
  {
    const Values_t PositionValues = { uT("Data"), uT("Position"), _X, _Y };

    IExample.RenderCompiledGeometry(hGeometry, { _X, _Y });

    EXPECT_CALL(RenderProxy, Render(PixelShaderValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(VertexBufferValues))
      .Times(1);

    using namespace ::alicorn::extension::std;

    EXPECT_CALL(RenderProxy, Render(TransformValues + PositionValues))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(PresentValues))
      .Times(1);

    Example.RenderScene();
  };

  TestCall(1811161301.0f, 1811161302.0f);
  TestCall(1811161303.0f, 1811161304.0f);

  IExample.ReleaseCompiledGeometry(hGeometry);

  // ����� ��� �������� �������� ��������
  {
    IExample.RenderCompiledGeometry(hGeometry, { 0, 0 });

    EXPECT_CALL(RenderProxy, Render(_))
      .Times(0);

    Example.RenderScene();
  }
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_ReleaseCompiledGeometry_RemoveUniqueComponents)
{
  RenderProxy RenderProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("test_data"), uT(""))
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Buffer"), Creator },
    { uT("Present"), Creator },
  };

  auto pRenders = ::std::make_shared<Renders_t>(Creators);

  Tested_t Example{ pRenders };
  RenderInterface_t & IExample = Example;

  const size_t ObjectId = 1811161821;
  const auto strObjectId = uT("{ID}").Replace(uT("{ID}"), ObjectId);

  using namespace ::testing;

  {
    const Renders_t::Renders_t Renders
    {
      pRenders->Obtain(Component_t::Make(
      {
        { uT("id"), uT("Covellite.Gui.Buffer.Vertex.") + strObjectId },
        { uT("type"), uT("Buffer") },
        { uT("test_data"), uT("Vertex.1811161832") },
      })),
      pRenders->Obtain(Component_t::Make(
      {
        { uT("id"), uT("Covellite.Gui.Present.") + strObjectId },
        { uT("type"), uT("Present") },
        { uT("test_data"), uT("Present.1811261258") },
      }))
    };

    const Values_t ExpectedVertexBufferData =
    {
      uT("Covellite.Gui.Buffer.Vertex.") + strObjectId,
      uT("Buffer"),
      uT("Vertex.1811161832")
    };

    const Values_t ExpectedPresentData =
    {
      uT("Covellite.Gui.Present.") + strObjectId,
      uT("Present"),
      uT("Present.1811261258")
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedVertexBufferData))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(ExpectedPresentData))
      .Times(1);

    EXPECT_EQ(2, Renders.size());
    for (auto & Render : Renders) Render();
  }

  IExample.ReleaseCompiledGeometry(
    (::mock::CovelliteGui::CompiledGeometryHandle)ObjectId);

  {
    const Renders_t::Renders_t Renders =
    {
      pRenders->Obtain(Component_t::Make(
      {
        { uT("id"), uT("Covellite.Gui.Buffer.Vertex.") + strObjectId },
        { uT("type"), uT("Buffer") },
      })),
      pRenders->Obtain(Component_t::Make(
      {
        { uT("id"), uT("Covellite.Gui.Present.") + strObjectId },
        { uT("type"), uT("Present") },
      }))
    };

    const Values_t ExpectedVertexBufferData =
    {
      uT("Covellite.Gui.Buffer.Vertex.") + strObjectId,
      uT("Buffer"),
      uT("")
    };

    const Values_t ExpectedPresentData =
    {
      uT("Covellite.Gui.Present.") + strObjectId,
      uT("Present"),
      uT("")
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedVertexBufferData))
      .Times(1);

    EXPECT_CALL(RenderProxy, Render(ExpectedPresentData))
      .Times(1);

    EXPECT_EQ(2, Renders.size());
    for (auto & Render : Renders) Render();
  }
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_EnableScissorRegion_True)
{
  RenderProxy RenderProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    const auto Kind = _pComponent->Kind;
    if (Kind != uT("Scissor")) return nullptr;

    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("State"), Creator },
  };

  Tested_t Example{ ::std::make_shared<Renders_t>(Creators) };
  RenderInterface_t & IExample = Example;

  IExample.EnableScissorRegion(true);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RenderProxy, Render(_))
    .Times(0);

  Example.RenderScene();
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_EnableScissorRegion_False)
{
  RenderProxy RenderProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    const auto Kind = _pComponent->Kind;
    if (Kind != uT("Scissor")) return nullptr;

    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .AddValue(uT("enabled"), uT(""))
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("State"), Creator },
  };

  Tested_t Example{ ::std::make_shared<Renders_t>(Creators) };
  RenderInterface_t & IExample = Example;

  IExample.EnableScissorRegion(false);

  using namespace ::testing;

  InSequence Dummy;

  const Values_t ExpectScissorData = 
  { 
    uT("Covellite.Gui.State.Scissor.Disabled"),
    uT("State"), 
    uT("Scissor"), 
    uT("false"),
  };

  EXPECT_CALL(RenderProxy, Render(ExpectScissorData))
    .Times(1);

  Example.RenderScene();
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_SetScissorRegion)
{
  RenderProxy RenderProxy;

  auto ScissorCreator = 
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    if (_pComponent->Kind != uT("Scissor")) return nullptr;

    const Object_t Data = (*_pComponent)[uT("service")].Default(Object_t{});
    if (Data.size() != 1) return nullptr;

    using namespace ::alicorn::extension::std;

    return [&, _pComponent, Data]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .AddValue(uT("enabled"), uT(""))
        .GetValues() +
        ComponentData{ Data[0] }
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .AddValue(uT("left"), 0)
        .AddValue(uT("right"), 0)
        .AddValue(uT("top"), 0)
        .AddValue(uT("bottom"), 0)
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("State"), ScissorCreator },
  };

  Tested_t Example{ ::std::make_shared<Renders_t>(Creators) };
  RenderInterface_t & IExample = Example;

  const Values_t ExpectedScissorData =
  {
    uT("Covellite.Gui.State.Scissor.Enabled"),
    uT("State"),
    uT("Scissor"),
    uT("true"),
  };

  using namespace ::testing;

  InSequence Dummy;

  // ������ �����
  {
    const int X = 1811081503;
    const int Y = 1811081504;
    const int Width = 81505;
    const int Height = 81506;

    IExample.SetScissorRegion(X, Y, Width, Height);

    const Values_t ExpectedRectData =
    {
      uT("Data"),
      uT("Rect"),
      X,
      X + Width,
      Y,
      Y + Height,
    };

    using namespace ::alicorn::extension::std;

    EXPECT_CALL(RenderProxy, Render(ExpectedScissorData + ExpectedRectData))
      .Times(1);

    Example.RenderScene();
  }

  // ������ ����� (� ������� ���������� ���������)
  {
    const int X = 1811081519;
    const int Y = 1811081520;
    const int Width = 81505;
    const int Height = 81506;

    IExample.SetScissorRegion(X, Y, Width, Height);

    const Values_t ExpectedRectData =
    {
      uT("Data"),
      uT("Rect"),
      X,
      X + Width,
      Y,
      Y + Height,
    };

    using namespace ::alicorn::extension::std;

    EXPECT_CALL(RenderProxy, Render(ExpectedScissorData + ExpectedRectData))
      .Times(1);

    Example.RenderScene();
  }
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_LoadTexture_NotExistsFile)
{
  using TextureHandle_t = ::mock::CovelliteGui::TextureHandle;
  using TextureDimensions_t = ::mock::CovelliteGui::Vector2i;

  class Tested :
    public Tested_t
  {
    using Renders_t = ::covellite::api::Component::Renders;

  public:
    MOCK_METHOD0(DoGenerateTexture, bool(void));

  public:
    bool GenerateTexture(TextureHandle_t &,
      const ::mock::CovelliteGui::byte *,
      const TextureDimensions_t &) override
    {
      return DoGenerateTexture();
    }

  public:
    Tested(void) :
      Tested_t(::std::make_shared<Renders_t>(Renders_t::Creators_t{}))
    {

    }
  };

  const auto PathToSourceImage = THIS_DIRECTORY / "NotExists.png";
  const TextureDimensions_t ExpectedDimensions{ 0, 0 };

  Tested Example;
  RenderInterface_t & IExample = Example;

  using namespace ::testing;

  EXPECT_CALL(Example, DoGenerateTexture())
    .Times(0);

  TextureHandle_t hTexture = 0;
  TextureDimensions_t TextureDimensions{ 0, 0 };

  const auto Result = IExample.LoadTexture(hTexture, TextureDimensions,
    PathToSourceImage.string().c_str());
  EXPECT_FALSE(Result);
  EXPECT_EQ(0, hTexture);
  EXPECT_EQ(ExpectedDimensions, TextureDimensions);
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_LoadTexture)
{
  using TextureHandle_t = ::mock::CovelliteGui::TextureHandle;
  using TextureData_t = ::std::vector<::mock::CovelliteGui::byte>;
  using TextureDimensions_t = ::mock::CovelliteGui::Vector2i;

  class Tested :
    public Tested_t
  {
    using Renders_t = ::covellite::api::Component::Renders;

  public:
    MOCK_METHOD2(DoGenerateTexture, bool(TextureData_t, TextureDimensions_t));

  public:
    bool GenerateTexture(TextureHandle_t & _hTexture,
      const ::mock::CovelliteGui::byte * _pData,
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
      Tested_t(::std::make_shared<Renders_t>(Renders_t::Creators_t{})),
      m_hHandle(_hHandle),
      m_TextureDataSize(_TextureDataSize)
    {

    }
  };

  using namespace ::boost::filesystem;

  const int TextureId = 1804101114;
  const path PathToSourceImage = "Image.png";
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

    ::mock::CovelliteGui::TextureHandle hTexture = 0;
    ::mock::CovelliteGui::Vector2i TextureDimensions{ 0, 0 };

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

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("content"), ::std::vector<uint8_t>{})
        .AddValue(uT("width"), 0)
        .AddValue(uT("height"), 0)
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Texture"), Creator },
  };

  const auto pRenders = ::std::make_shared<Renders_t>(Creators);

  Tested_t Example{ pRenders };
  RenderInterface_t & IExample = Example;

  using namespace ::testing;

  // ������ ����� �������
  {
    const ::std::vector<uint8_t> Data =
    {
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    };

    const int Width = 3;
    const int Height = 6;

    ::mock::CovelliteGui::TextureHandle hTexture = 0;
    auto Result = IExample.GenerateTexture(hTexture, Data.data(), { Width, Height });
    EXPECT_TRUE(Result);
    EXPECT_EQ(1, hTexture);

    const Renders_t::Renders_t Renders = 
    {
      pRenders->Obtain(Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.Texture.1") },
          { uT("type"), uT("Texture") },
        }))
    };
    ASSERT_EQ(1, Renders.size());

    const Values_t ExpectedTextureData =
    {
      uT("Covellite.Gui.Texture.1"),
      uT("Texture"),
      Data,
      Width,
      Height
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedTextureData))
      .Times(1);

    Renders[0]();
  }

  // ������ ����� �������
  {
    const ::std::vector<uint8_t> Data =
    {
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,
      0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,
    };

    const int Width = 2;
    const int Height = 10;

    ::mock::CovelliteGui::TextureHandle hTexture = 0;
    auto Result = IExample.GenerateTexture(hTexture, Data.data(), { Width, Height });
    EXPECT_TRUE(Result);
    EXPECT_EQ(2, hTexture);

    const Renders_t::Renders_t Renders = 
    {
      pRenders->Obtain(Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.Texture.2") },
          { uT("type"), uT("Texture") },
        }))
    };
    ASSERT_EQ(1, Renders.size());

    const Values_t ExpectedTextureData =
    {
      uT("Covellite.Gui.Texture.2"),
      uT("Texture"),
      Data,
      Width,
      Height
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedTextureData))
      .Times(1);

    Renders[0]();
  }
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_ReleaseTexture)
{
  RenderProxy RenderProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("content"), ::std::vector<uint8_t>{})
        .GetValues());
    };
  };

  const ::std::vector<uint8_t> Data =
  {
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
  };

  const int Width = 3;
  const int Height = 6;

  const Renders_t::Creators_t Creators =
  {
    { uT("Texture"), Creator },
  };

  auto pRenders = ::std::make_shared<Renders_t>(Creators);

  Tested_t Example{ pRenders };
  RenderInterface_t & IExample = Example;

  using namespace ::testing;

  {
    const Renders_t::Renders_t Renders = 
    {
      pRenders->Obtain(Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.Texture.1811071543") },
          { uT("type"), uT("Texture") },
          { uT("content"), Data },
          { uT("width"), Width },
          { uT("height"), Height },
        }))
    };

    ASSERT_EQ(1, Renders.size());

    const Values_t ExpectedTextureData =
    {
      uT("Covellite.Gui.Texture.1811071543"),
      uT("Texture"),
      Data,
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedTextureData))
      .Times(1);

    Renders[0]();
  }

  IExample.ReleaseTexture(1811071543);

  {
    const Renders_t::Renders_t Renders = 
    {
      pRenders->Obtain(Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.Texture.1811071543") },
          { uT("type"), uT("Texture") },
        }))
    };

    ASSERT_EQ(1, Renders.size());

    const Values_t ExpectedTextureData =
    {
      uT("Covellite.Gui.Texture.1811071543"),
      uT("Texture"),
      ::std::vector<uint8_t>{},
    };

    EXPECT_CALL(RenderProxy, Render(ExpectedTextureData))
      .Times(1);

    Renders[0]();
  }
}

// ************************************************************************** //
TEST_F(Renderer_test, /*DISABLED_*/Test_RenderScene)
{
  RenderProxy RenderProxy;

  auto Creator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("kind"), uT(""))
        .GetValues());
    };
  };

  auto ShaderCreator = [&](const Component_t::ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    return [&, _pComponent]()
    {
      RenderProxy.Render(ComponentData{ _pComponent }
        .AddValue(uT("id"), uT(""))
        .AddValue(uT("type"), uT(""))
        .AddValue(uT("entry"), uT(""))
        .AddValue(uT("content"), ::std::vector<uint8_t>{})
        .GetValues());
    };
  };

  const Renders_t::Creators_t Creators =
  {
    { uT("Light"), Creator },
    { uT("Camera"), Creator },
    { uT("State"), Creator },
    { uT("Shader"), ShaderCreator },
  };

  Tested_t Example{ ::std::make_shared<Renders_t>(Creators) };

  const ::mock::Id_t RendererId = 1811061313;

  const Values_t ExpectedCameraData =
  {
    uT("Covellite.Gui.Camera.Gui"),
    uT("Camera"),
    uT("Orthographic"),
  };

  const Values_t ExpectedBlendData =
  {
    uT("Covellite.Gui.State.Blend"),
    uT("State"),
    uT("Blend"),
  };

  const Values_t SamplerValues =
  {
    uT("Covellite.Gui.State.Sampler"),
    uT("State"),
    uT("Sampler")
  };

  const Values_t ExpectedShaderData =
  {
    uT("Covellite.Gui.Shader.Vertex"),
    uT("Shader"),
    uT("vsFlat"),
    ::std::vector<uint8_t>{},
  };

  using namespace ::testing;

  InSequence Dummy;

  //************** ������� �� ���������, ����� ��� ���� �������� *************//

  EXPECT_CALL(RenderProxy, Render(ExpectedCameraData))
    .Times(1);

  EXPECT_CALL(RenderProxy, Render(ExpectedBlendData))
    .Times(1);

  EXPECT_CALL(RenderProxy, Render(SamplerValues))
    .Times(1);

  EXPECT_CALL(RenderProxy, Render(Eq(ExpectedShaderData)))
    .Times(1);

  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//

  Example.RenderScene();
}
