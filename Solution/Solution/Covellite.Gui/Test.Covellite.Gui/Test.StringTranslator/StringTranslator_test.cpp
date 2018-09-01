
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ StringTranslator.
*/

// ������������ ������ StringTranslator
#include <Covellite\Gui\StringTranslator.hpp>

// ����� �������� ����� ������ StringTranslator
class StringTranslator_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::StringTranslator;
  using ITested_t = ::covellite::gui::StringTranslator;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� StringTranslator 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(StringTranslator_test, Test_Function);

// ************************************************************************** //
TEST_F(StringTranslator_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(StringTranslator_test, /*DISABLED_*/Test_Using)
{
  Tested_t Example;
  ITested_t & IExample = Example;

  const ::std::string Unknown = u8"Unknown";
  const ::std::string Old = u8"Old";
  const ::std::string New = u8"New";

  EXPECT_EQ(Unknown, IExample.Translate(Unknown));
  EXPECT_EQ(Old, IExample.Translate(Old));

  Example.Set(
  {
    { Old, New },
  });

  EXPECT_EQ(Unknown, IExample.Translate(Unknown));
  EXPECT_EQ(New, IExample.Translate(Old));
}
