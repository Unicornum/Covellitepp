
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn/google/test/example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  ����� ������ I3DScene.
*/

// ������������ ������ I3DScene
#include <Covellite\Expanse\I3DScene.hpp>

ALICORN_DISABLE_GTEST_WARNINGS

// ����� �������� ����� ������ I3DScene
class I3DScene_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::I3DScene;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� I3DScene 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(I3DScene_test, Test_Function);

// ************************************************************************** //
TEST_F(I3DScene_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

ALICORN_RESTORE_WARNINGS
