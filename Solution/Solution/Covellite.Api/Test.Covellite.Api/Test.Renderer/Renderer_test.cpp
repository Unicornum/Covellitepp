
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

#include <Covellite.Gui/Covellite.Gui/Renderer.cpp>

#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Renders.hpp>
#include <Covellite/Api/Vertex.hpp>

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

  using Vertex_t = ::covellite::api::Vertex::Polygon;

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
  using Vertex_t = ::covellite::api::Vertex::Polygon;

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
  const Renders_t::Creators_t Creators;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  EXPECT_CALL(RendererImplProxy, GetCreators(RendererId))
    .Times(1)
    .WillOnce(ReturnRef(Creators));

  Tested_t Example{ uT("RendererImpl"), Data_t{} };

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
