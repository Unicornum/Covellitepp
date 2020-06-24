
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn/google/test/example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  ����� ������ IGameScene.
*/

// ������������ ������ IGameScene
#include <Covellite\Expanse\IGameScene.hpp>

ALICORN_DISABLE_GTEST_WARNINGS

// ����� �������� ����� ������ IGameScene
class IGameScene_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::IGameScene;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� IGameScene 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(IGameScene_test, Test_Function);

// ************************************************************************** //
TEST_F(IGameScene_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

ALICORN_RESTORE_WARNINGS
