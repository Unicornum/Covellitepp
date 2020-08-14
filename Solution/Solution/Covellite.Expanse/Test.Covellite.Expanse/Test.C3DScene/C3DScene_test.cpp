
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса C3DScene.
*/

// Расположение класса C3DScene
#include "..\..\Covellite.Expanse\3DScene.impl.hpp"

// Общий тестовый класс класса C3DScene
class C3DScene_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::C3DScene;
  using IProduce3DObject_t = ::covellite::expanse::IProduce3DObject;
  using I3DScene_t = ::covellite::expanse::I3DScene;
  using C3DObject_t = ::covellite::expanse::C3DObject_t;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) noexcept override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) noexcept override
  {
  }

protected:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Call, void(int));
  };
};

ALICORN_DISABLE_GTEST_WARNINGS

// Образец макроса для подстановки в класс C3DScene 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(C3DScene_test, Test_Function);

// ************************************************************************** //
TEST_F(C3DScene_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

namespace covellite
{

namespace expanse
{

// ************************************************************************** //
TEST_F(C3DScene_test, /*DISABLED_*/Test_Produce3DObjects)
{
  Proxy oProxy;

  Tested_t Example;
  IProduce3DObject_t & IExample = Example;

  using namespace ::testing;

  EXPECT_THROW(Example.Get(0), ::std::exception);

  IExample.Add(0, C3DObject_t{ [&]() { oProxy.Call(0); } });

  {
    EXPECT_CALL(oProxy, Call(0))
      .Times(1);

    for (const auto & Render : Example.Get(0)) Render();
    EXPECT_THROW(Example.Get(1), ::std::exception);
  }

  IExample.Add(2, C3DObject_t{ [&]() { oProxy.Call(2); } });

  {
    InSequence Dummy;

    EXPECT_CALL(oProxy, Call(0))
      .Times(1);

    for (const auto & Render : Example.Get(0)) Render();
    for (const auto & Render : Example.Get(1)) EXPECT_THROW(Render(), ::std::exception);

    EXPECT_CALL(oProxy, Call(2))
      .Times(1);

    for (const auto & Render : Example.Get(2)) Render();
    EXPECT_THROW(Example.Get(3), ::std::exception);
  }

  IExample.Remove(0);

  {
    InSequence Dummy;

    EXPECT_CALL(oProxy, Call(0))
      .Times(0);

    for (const auto & Render : Example.Get(0)) EXPECT_THROW(Render(), ::std::exception);
    for (const auto & Render : Example.Get(1)) EXPECT_THROW(Render(), ::std::exception);

    EXPECT_CALL(oProxy, Call(2))
      .Times(1);

    for (const auto & Render : Example.Get(2)) Render();
    EXPECT_THROW(Example.Get(3), ::std::exception);
  }

  IExample.Add(2, C3DObject_t{ [&]() { oProxy.Call(3); } });

  {
    InSequence Dummy;

    EXPECT_CALL(oProxy, Call(0))
      .Times(0);

    for (const auto & Render : Example.Get(0)) EXPECT_THROW(Render(), ::std::exception);
    for (const auto & Render : Example.Get(1)) EXPECT_THROW(Render(), ::std::exception);

    EXPECT_CALL(oProxy, Call(2))
      .Times(0);

    EXPECT_CALL(oProxy, Call(3))
      .Times(1);

    for (const auto & Render : Example.Get(2)) Render();
    EXPECT_THROW(Example.Get(3), ::std::exception);
  }

  IExample.Remove(2006191830);
}

} // namespace expanse

} // namespace covellite

// ************************************************************************** //
TEST_F(C3DScene_test, /*DISABLED_*/Test_Render_OnePass)
{
  Proxy oProxy;

  Tested_t Example;
  I3DScene_t & IExample = Example;

  for (int i = 0; i < 10; i++)
  {
    Example.Add(i, C3DObject_t
      { 
        [&, i]() { oProxy.Call(i); },
        [&, i]() { oProxy.Call(i); }
      });
  }

  for (int i = 9; i >= 0; i--)
  {
    IExample.Add(0, i);
  }

  using namespace ::testing;

  InSequence Dummy;

  for (int i = 9; i >= 0; i--)
  {
    EXPECT_CALL(oProxy, Call(i))
      .Times(2);
  }

  Example.Render();

  EXPECT_CALL(oProxy, Call(_))
    .Times(0);

  Example.Render();

  IExample.Add(0, 2006191354);

  EXPECT_THROW(Example.Render(), ::std::exception);
}

// ************************************************************************** //
TEST_F(C3DScene_test, /*DISABLED_*/Test_Render_Simple)
{
  Proxy oProxy;

  Tested_t Example;
  I3DScene_t & IExample = Example;

  for (int i = 0; i < 10; i++)
  {
    Example.Add(i, C3DObject_t
      {
        [&, i]() { oProxy.Call(i); },
        [&, i]() { oProxy.Call(i); }
      });
  }

  for (int i = 9; i >= 0; i--)
  {
    IExample.Add(i);
  }

  using namespace ::testing;

  InSequence Dummy;

  for (int i = 9; i >= 0; i--)
  {
    EXPECT_CALL(oProxy, Call(i))
      .Times(2);
  }

  Example.Render();

  EXPECT_CALL(oProxy, Call(_))
    .Times(0);

  Example.Render();

  IExample.Add(2008042034);

  EXPECT_THROW(Example.Render(), ::std::exception);
}

// ************************************************************************** //
TEST_F(C3DScene_test, /*DISABLED_*/Test_Render_MultiPass)
{
  Proxy oProxy;

  Tested_t Example;
  I3DScene_t & IExample = Example;

  for (int i = 0; i < 10; i++)
  {
    Example.Add(i, C3DObject_t
      {
        [&, i]() { oProxy.Call(i); },
        [&, i]() { oProxy.Call(i); },
        [&, i]() { oProxy.Call(i); }
      });
  }

  IExample.Add(1, 0);
  IExample.Add(1, 1);
  IExample.Add(0, 2);
  IExample.Add(8, 3);
  IExample.Add(8, 4);
  IExample.Add(0, 5);
  IExample.Add(0, 6);
  IExample.Add(0, 7);
  IExample.Add(1, 8);
  IExample.Add(1, 9);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(oProxy, Call(2))
    .Times(3);

  EXPECT_CALL(oProxy, Call(5))
    .Times(3);

  EXPECT_CALL(oProxy, Call(6))
    .Times(3);

  EXPECT_CALL(oProxy, Call(7))
    .Times(3);

  EXPECT_CALL(oProxy, Call(0))
    .Times(3);

  EXPECT_CALL(oProxy, Call(1))
    .Times(3);

  EXPECT_CALL(oProxy, Call(8))
    .Times(3);

  EXPECT_CALL(oProxy, Call(9))
    .Times(3);

  EXPECT_CALL(oProxy, Call(3))
    .Times(3);

  EXPECT_CALL(oProxy, Call(4))
    .Times(3);

  Example.Render();

  EXPECT_CALL(oProxy, Call(_))
    .Times(0);

  Example.Render();

  IExample.Add(10, 2006191726);

  EXPECT_THROW(Example.Render(), ::std::exception);
}

// ************************************************************************** //
TEST_F(C3DScene_test, /*DISABLED_*/Test_Render_Sort)
{
  Proxy oProxy;

  Tested_t Example;
  I3DScene_t & IExample = Example;

  for (int i = 0; i < 10; i++)
  {
    Example.Add(i, C3DObject_t
      {
        [&, i]() { oProxy.Call(i); },
        [&, i]() { oProxy.Call(i); },
        [&, i]() { oProxy.Call(i); }
      });
  }

  IExample.Add(1, 0);
  IExample.Add(1, 1);
  IExample.Add(0, 2, 4);
  IExample.Add(8, 3);
  IExample.Add(8, 4);
  IExample.Add(0, 5, 3);
  IExample.Add(0, 6, 2);
  IExample.Add(0, 7, 1);
  IExample.Add(1, 8);
  IExample.Add(1, 9);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(oProxy, Call(7))
    .Times(3);

  EXPECT_CALL(oProxy, Call(6))
    .Times(3);

  EXPECT_CALL(oProxy, Call(5))
    .Times(3);

  EXPECT_CALL(oProxy, Call(2))
    .Times(3);

  EXPECT_CALL(oProxy, Call(0))
    .Times(3);

  EXPECT_CALL(oProxy, Call(1))
    .Times(3);

  EXPECT_CALL(oProxy, Call(8))
    .Times(3);

  EXPECT_CALL(oProxy, Call(9))
    .Times(3);

  EXPECT_CALL(oProxy, Call(3))
    .Times(3);

  EXPECT_CALL(oProxy, Call(4))
    .Times(3);

  Example.Render();

  EXPECT_CALL(oProxy, Call(_))
    .Times(0);

  Example.Render();

  IExample.Add(10, 2006191726);

  EXPECT_THROW(Example.Render(), ::std::exception);
}

ALICORN_RESTORE_WARNINGS
