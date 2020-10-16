
#include "stdafx.h"
#include <alicorn/std/vector.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Renders.
*/

// Расположение класса Renders
#include <Covellite/Api/Renders.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Vertex.hpp>

// Общий тестовый класс класса Renders
class Renders_test :
  public ::testing::Test
{
protected:
  using Component_t = ::covellite::api::Component;
  using Tested_t = ::covellite::api::Component::Renders;
  using String_t = ::alicorn::extension::std::String;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }

protected:
  class TestCaller
  {
  public:
    MOCK_METHOD1(Creator, void(Component_t::Id_t));
    MOCK_METHOD1(Render, void(Component_t::Id_t));
  };

  bool IsExistsAnyTypeParameter(const Component_t & _Component, const String_t & _Name)
  {
    try
    {
      _Component[_Name];
      return true;
    }
    catch (const ::std::exception &) {}

    return false;
  };
};

// Образец макроса для подстановки в класс Renders 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Renders_test, Test_Function);

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_FALSE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_UnknownType)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  Tested_t Example{ {} };

  const auto LogMessage = uT("Warning: ") +
    uT("Creating a component of an unknown type: Position [id: Id2006111517].");

  using namespace ::testing;

  EXPECT_CALL(LoggerProxy, ToLog(LogMessage))
    .Times(1);

  const auto Result = Example.Obtain(Component_t::Make(
    { 
      { uT("type"), uT("Position") },
      { uT("id"), uT("Id2006111517") },
   }));
  EXPECT_NO_THROW(Result());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateEmptyRender)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  const auto Creator = [&](const Component_t::ComponentPtr_t &) -> Tested_t::Render_t
  {
    return nullptr;
  };

  Tested_t Example{
  {
    { uT("Texture"), Creator },
  } };

  const auto LogMessage = uT("Warning: ") +
    uT("Received an empty render: [id: Id2006111532, type: Texture].");

  using namespace ::testing;

  EXPECT_CALL(LoggerProxy, ToLog(LogMessage))
    .Times(1);

  const auto Result = Example.Obtain(Component_t::Make(
    {
      { uT("type"), uT("Texture") },
      { uT("id"), uT("Id2006111532") },
    }));
  EXPECT_NO_THROW(Result());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateException)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  const auto Creator = [&](const Component_t::ComponentPtr_t &) -> Tested_t::Render_t
  {
    throw ::std::exception{ "Exception2006111545" };
  };

  Tested_t Example{
  {
    { uT("Shader"), Creator },
  } };

  const auto LogMessage = uT("Error: ") +
    uT("Create render fail: Exception2006111545 [id: Id2006111546, type: Shader].");

  using namespace ::testing;

  EXPECT_CALL(LoggerProxy, ToLog(LogMessage))
    .Times(1);

  const auto Result = Example.Obtain(Component_t::Make(
    {
      { uT("type"), uT("Shader") },
      { uT("id"), uT("Id2006111546") },
   }));
  EXPECT_NO_THROW(Result());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateNewRender)
{
  TestCaller Caller;

  auto TextureCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(uT("Texture"));

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  auto MeshCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(uT("Mesh"));

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{
    {
      { uT("Texture"), TextureCreator },
      { uT("Mesh"), MeshCreator },
    } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(1);

  auto Result = Example.Obtain(Component_t::Make(
    {
      { uT("type"), uT("Texture") },
      { uT("id"), uT("TextureId") },
    }));

  EXPECT_CALL(Caller, Render(uT("TextureId")))
    .Times(1);

  Result();

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  Result = Example.Obtain(Component_t::Make(
    {
      { uT("type"), uT("Mesh") },
      { uT("id"), uT("MeshId") },
    }));

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  Result();
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_ExistsRender)
{
  TestCaller Caller;

  auto Creator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(_pComponent->Type);

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{
    {
      { uT("Texture"), Creator },
      { uT("Mesh"), Creator },
    } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(1);

  auto Result = Example.Obtain(Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }));

  EXPECT_CALL(Caller, Render(uT("TextureId")))
    .Times(1);

  Result();

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(0);

  Result = Example.Obtain(Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }));

  EXPECT_CALL(Caller, Render(uT("TextureId")))
    .Times(1);

  Result();

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  Result = Example.Obtain(Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }));

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  Result();

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(0);

  Result = Example.Obtain(Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }));

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  Result();

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(0);

  Result = Example.Obtain(Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }));

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  Result();
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Remove)
{
  TestCaller Caller;

  auto Creator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(_pComponent->Type);

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{
    {
      { uT("Texture"), Creator },
      { uT("Mesh"), Creator },
    } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(1);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
    }));

  EXPECT_CALL(Caller, Creator(_))
    .Times(0);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  Example.Remove(Component_t::Make(
    {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
    }));

  Example.Remove(Component_t::Make(
    {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
    }));

  EXPECT_CALL(Caller, Creator(_))
    .Times(0);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  Example.Remove(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  Example.Remove(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  Example.Remove( // Лишний раз - умышленно
      Component_t::Make(
        {
          { uT("type"), uT("Texture") },
          { uT("id"), uT("TextureId") },
        }));

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(1);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
    }));
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_ComplexUsing)
{
  TestCaller Caller;

  auto Creator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(_pComponent->Id);

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{
    {
      { uT("Texture"), Creator },
      { uT("Mesh"), Creator },
    } };

  Tested_t::Renders_t Result;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("TextureId1")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("TextureId2")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("MeshId")))
    .Times(1);

  using namespace ::alicorn::extension::std;

  Result += Tested_t::Renders_t
  {
    Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Shader") },
        { uT("id"), uT("ShaderId") },
    })),
    Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId1") },
    })),
    Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId2") },
    })),
    Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
    }))
  };

  EXPECT_CALL(Caller, Creator(uT("TextureId3")))
    .Times(1);

  Result += Tested_t::Renders_t
  {
    Example.Obtain(Component_t::Make(
      {
        { uT("type"), uT("Shader") },
        { uT("id"), uT("ShaderId") },
      })),
    Example.Obtain(Component_t::Make(
      {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId2") },
      })),
    Example.Obtain(Component_t::Make(
      {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId3") },
      })),
    Example.Obtain(Component_t::Make(
      {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
      })),
  };

  EXPECT_CALL(Caller, Render(uT("TextureId1")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("TextureId2")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("TextureId2")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("TextureId3")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  for (auto & Render : Result) Render();
}
