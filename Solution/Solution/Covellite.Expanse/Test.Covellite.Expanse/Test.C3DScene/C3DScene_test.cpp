
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn/google/test/example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  ����� ������ C3DScene.
*/

// ������������ ������ C3DScene
#include "..\..\Covellite.Expanse\3DScene.impl.hpp"

// ����� �������� ����� ������ C3DScene
class C3DScene_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::C3DScene;
  using IProduce3DObject_t = ::covellite::expanse::IProduce3DObject;
  using I3DScene_t = ::covellite::expanse::I3DScene;
  using C3DObject_t = ::covellite::expanse::C3DObject_t;

  // ���������� ����� �������� ������� �����
  void SetUp(void) noexcept override
  {
  }

  // ���������� ����� ������� ������� �����
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

// ������� ������� ��� ����������� � ����� C3DScene 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(C3DScene_test, Test_Function);

// ************************************************************************** //
TEST_F(C3DScene_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

namespace covellite::expanse
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

} // namespace covellite::expanse

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

ALICORN_RESTORE_WARNINGS
