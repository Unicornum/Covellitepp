
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ IStringTranslator.
*/

// ������������ ������ IStringTranslator
#include <Covellite\Gui\IStringTranslator.hpp>

// ����� �������� ����� ������ IStringTranslator
class IStringTranslator_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::IStringTranslator;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� IStringTranslator 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(IStringTranslator_test, Test_Function);

// ************************************************************************** //
TEST_F(IStringTranslator_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}
