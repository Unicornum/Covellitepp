
#include "stdafx.h"
#include <alicorn\std\string.hpp>

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ IWindow.
*/

// ������������ ������ IWindow
#include <Covellite\Api\IWindow.hpp>

// ����� �������� ����� ������ IWindow
class IWindow_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::IWindow;

  // ���������� ����� �������� ������� �����
  // cppcheck-suppress unusedFunction
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  // cppcheck-suppress unusedFunction
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� IWindow 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(IWindow_test, Test_Function);

// ************************************************************************** //
TEST_F(IWindow_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}
