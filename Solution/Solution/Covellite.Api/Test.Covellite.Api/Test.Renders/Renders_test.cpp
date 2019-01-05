
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

// Общий тестовый класс класса Renders
class Renders_test :
  public ::testing::Test
{
protected:
  using Component_t = ::covellite::api::Component;
  using Tested_t = ::covellite::api::Component::Renders;

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
  Tested_t Example{ {} };

  const auto Result = Example.Obtain(
    {
      Component_t::Make({ { uT("type"), uT("Position") } }),
      Component_t::Make({ { uT("type"), uT("Texture") } }),
      Component_t::Make({ { uT("type"), uT("Mesh") } }),
    });
  EXPECT_TRUE(Result.empty());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateNullptr)
{
  TestCaller Caller;

  auto Creator =
    [&](const Component_t::ComponentPtr_t &) -> Tested_t::Render_t
  {
    Caller.Creator(uT("Texture"));

    return nullptr;
  };

  Tested_t Example{
  {
    { uT("Texture"), Creator },
  } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(2);

  const auto Result = Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }),
    });
  EXPECT_TRUE(Result.empty());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateException)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  const auto LogMessage = uT("Error: ") +
    uT("Create render fail: Exception1812151850 [id: TextureId, type: Texture].");

  TestCaller Caller;

  auto Creator = [&](const Component_t::ComponentPtr_t &) -> Tested_t::Render_t
  {
    Caller.Creator(uT("Texture"));

    throw ::std::exception{ "Exception1812151850" };
  };

  Tested_t Example{
  {
    { uT("Texture"), Creator },
  } };

  using namespace ::testing;

  InSequence Dummy;

  for (size_t i = 0; i < 2; i++)
  {
    EXPECT_CALL(Caller, Creator(uT("Texture")))
      .Times(1);

    EXPECT_CALL(LoggerProxy, ToLog(LogMessage))
      .Times(1);
  }

  const auto Result = Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }),
    });
  EXPECT_TRUE(Result.empty());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateNewRenders)
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

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  const auto Result = Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

  EXPECT_CALL(Caller, Render(uT("TextureId")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  for (auto & Render : Result) Render();
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_ExistsRenders)
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

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  const auto Result = Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

  EXPECT_CALL(Caller, Render(uT("TextureId")))
    .Times(2);

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(3);

  for (auto & Render : Result) Render();
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_EmptyRenders)
{
  TestCaller Caller;

  auto TextureCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(_pComponent->Type);

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  auto MeshCreator =
    [&](const Component_t::ComponentPtr_t &) -> Tested_t::Render_t
  {
    return nullptr;
  };

  Tested_t Example{
    {
      { uT("Texture"), TextureCreator },
      { uT("Mesh"), MeshCreator },
    } };

  using namespace ::testing;

  // Первый вызов - создание.
  {
    InSequence Dummy;

    EXPECT_CALL(Caller, Creator(uT("Texture")))
      .Times(1);

    EXPECT_CALL(Caller, Creator(_))
      .Times(0);

    const auto Result = Example.Obtain(
      {
        Component_t::Make({
          { uT("type"), uT("Texture") },
          { uT("id"), uT("TextureId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Texture") },
          { uT("id"), uT("TextureId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
      });
    EXPECT_EQ(2, Result.size());

    EXPECT_CALL(Caller, Render(uT("TextureId")))
      .Times(2);

    EXPECT_CALL(Caller, Render(_))
      .Times(0);

    for (auto & Render : Result) Render();
  }

  // Второй вызов - получение существующих.
  {
    InSequence Dummy;

    EXPECT_CALL(Caller, Creator(_))
      .Times(0);

    const auto Result = Example.Obtain(
      {
        Component_t::Make({
          { uT("type"), uT("Texture") },
          { uT("id"), uT("TextureId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Texture") },
          { uT("id"), uT("TextureId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
      });
    EXPECT_EQ(2, Result.size());

    EXPECT_CALL(Caller, Render(uT("TextureId")))
      .Times(2);

    EXPECT_CALL(Caller, Render(_))
      .Times(0);

    for (auto & Render : Result) Render();
  }
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

  Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

  Example.Remove(
    {
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  const auto Result = Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });
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

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("TextureId1")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("TextureId2")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("MeshId")))
    .Times(1);

  auto Result = Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("Shader") },
        { uT("id"), uT("ShaderId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId1") },
        }),
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId2") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

  EXPECT_CALL(Caller, Creator(uT("TextureId3")))
    .Times(1);

  using namespace ::alicorn::extension::std;

  Result += Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("Shader") },
        { uT("id"), uT("ShaderId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId2") },
        }),
      Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId3") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

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
