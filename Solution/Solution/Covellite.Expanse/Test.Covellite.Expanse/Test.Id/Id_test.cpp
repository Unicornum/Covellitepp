
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn/google/test/example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  ����� ������ Id.
*/

// ������������ ������ Id
#include "../../Covellite.Expanse/Id.impl.hpp"

// ����� �������� ����� ������ Id
class Id_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::Id;

  // ���������� ����� �������� ������� �����
  void SetUp(void) noexcept override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) noexcept override
  {
  }
};

ALICORN_DISABLE_GTEST_WARNINGS

// ������� ������� ��� ����������� � ����� Id 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Id_test, Test_Function);

// ************************************************************************** //
TEST_F(Id_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_FALSE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Id_test, /*DISABLED_*/Test_Using)
{
  Tested_t Example;

  EXPECT_EQ(0, Example.GetFreeId());
  EXPECT_EQ(1, Example.GetFreeId());
  EXPECT_EQ(2, Example.GetFreeId());
  EXPECT_EQ(3, Example.GetFreeId());
  EXPECT_EQ(4, Example.GetFreeId());

  Example.AddRemovedObjectId(0);
  Example.AddRemovedObjectId(3);
  Example.AddRemovedObjectId(1);

  EXPECT_EQ(0, Example.GetFreeId());
  EXPECT_EQ(3, Example.GetFreeId());
  EXPECT_EQ(1, Example.GetFreeId());
  EXPECT_EQ(5, Example.GetFreeId());
  EXPECT_EQ(6, Example.GetFreeId());
}

ALICORN_RESTORE_WARNINGS
