
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Layers.
*/

// ������������ ������ Layers
#include <Covellite\Gui\Layers.hpp>
#include <Covellite\Gui\ILayer.hpp>

// ����� �������� ����� ������ Layers
class Layers_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::Layers;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }

protected:
  class Layer :
    public ::covellite::gui::ILayer
  {
  public:
    MOCK_METHOD0(Show, void(void));
    MOCK_METHOD0(Hide, void(void));
  };

  class Layer1 : public Layer {};
  class Layer2 : public Layer {};
  class Layer3 : public Layer {};

  using LayerPtr_t = ::std::shared_ptr<Layer>;
};

// ������� ������� ��� ����������� � ����� Layers 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Layers_test, Test_Function);

// ************************************************************************** //
TEST_F(Layers_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_FALSE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Layers_test, /*DISABLED_*/Test_Push)
{
  LayerPtr_t pLayer1 = ::std::make_shared<Layer1>();
  LayerPtr_t pLayer2 = ::std::make_shared<Layer2>();
  LayerPtr_t pLayer3 = ::std::make_shared<Layer3>();

  Tested_t Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(*pLayer1, Show())
    .Times(1);

  Example.Push(pLayer1);

  EXPECT_CALL(*pLayer1, Hide())
    .Times(1);

  EXPECT_CALL(*pLayer2, Show())
    .Times(1);

  Example.Push(pLayer2);

  EXPECT_CALL(*pLayer2, Hide())
    .Times(1);

  EXPECT_CALL(*pLayer3, Show())
    .Times(1);

  Example.Push(pLayer3);
}

// ************************************************************************** //
TEST_F(Layers_test, /*DISABLED_*/Test_Push_ExistsLayerClass)
{
  LayerPtr_t pLayer1 = ::std::make_shared<Layer1>();
  LayerPtr_t pLayer2 = ::std::make_shared<Layer2>();
  LayerPtr_t pLayer3 = ::std::make_shared<Layer1>();
  LayerPtr_t pLayer4 = ::std::make_shared<Layer2>();

  Tested_t Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(*pLayer1, Show())
    .Times(1);

  Example.Push(pLayer1);

  EXPECT_CALL(*pLayer1, Hide())
    .Times(1);

  EXPECT_CALL(*pLayer2, Show())
    .Times(1);

  Example.Push(pLayer2);

  EXPECT_CALL(*pLayer2, Hide())
    .Times(0);

  EXPECT_CALL(*pLayer3, Show())
    .Times(0);

  EXPECT_STDEXCEPTION(Example.Push(pLayer3), ".*Exists layer: Layer1");

  EXPECT_CALL(*pLayer2, Hide())
    .Times(0);

  EXPECT_CALL(*pLayer4, Show())
    .Times(0);

  EXPECT_STDEXCEPTION(Example.Push(pLayer4), ".*Exists layer: Layer2");
}

// ************************************************************************** //
TEST_F(Layers_test, /*DISABLED_*/Test_Pop_EmptyStack)
{
  Tested_t Example;
  EXPECT_STDEXCEPTION(Example.Pop(), ".*Empty stack.*");
}

// ************************************************************************** //
TEST_F(Layers_test, /*DISABLED_*/Test_Pop)
{
  LayerPtr_t pLayer1 = ::std::make_shared<Layer1>();
  LayerPtr_t pLayer2 = ::std::make_shared<Layer2>();
  LayerPtr_t pLayer3 = ::std::make_shared<Layer3>();

  Tested_t Example;

  using namespace ::testing;

  InSequence Dummy;

  Example.Push(pLayer1);
  Example.Push(pLayer2);
  Example.Push(pLayer3);

  EXPECT_CALL(*pLayer3, Hide())
    .Times(1);

  EXPECT_CALL(*pLayer2, Show())
    .Times(1);

  const auto Result1 = Example.Pop();
  EXPECT_TRUE(Result1);

  EXPECT_CALL(*pLayer2, Hide())
    .Times(1);

  EXPECT_CALL(*pLayer1, Show())
    .Times(1);

  const auto Result2 = Example.Pop();
  EXPECT_TRUE(Result2);

  EXPECT_CALL(*pLayer1, Hide())
    .Times(1);

  const auto Result3 = Example.Pop();
  EXPECT_FALSE(Result3);

  EXPECT_STDEXCEPTION(Example.Pop(), ".*Empty stack.*");
}
