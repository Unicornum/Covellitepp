
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn/google/test/example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  ����� ������ IProduce3DObject.
*/

// ������������ ������ IProduce3DObject
#include "..\..\Covellite.Expanse\IProduce3DObject.impl.hpp"

// ����� �������� ����� ������ IProduce3DObject
class IProduce3DObject_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::IProduce3DObject;

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

// ������� ������� ��� ����������� � ����� IProduce3DObject 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(IProduce3DObject_test, Test_Function);

// ************************************************************************** //
TEST_F(IProduce3DObject_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

ALICORN_RESTORE_WARNINGS
