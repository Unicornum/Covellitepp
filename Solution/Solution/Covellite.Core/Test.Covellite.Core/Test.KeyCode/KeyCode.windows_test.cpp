
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ KeyCode.
*/

#define KeyCode_test KeyCode_windows_test
#define KeyCode KeyCode_windows

// ������������ ������ Window
#include "..\..\Covellite.Core\Params.windows.cpp"

// ����� �������� ����� ������ KeyCode
class KeyCode_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::params::KeyCode;
  using RocketKeyCode_t = ::mock::Rocket::Core::Input::KeyIdentifier;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� KeyCode 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(KeyCode_test, Test_Function);

// ************************************************************************** //
TEST_F(KeyCode_test, /*DISABLED_*/Test_ToRocketKeyCode_Unknown)
{
  const Tested_t Example{ 1711011232 };

  const auto Result = Example.ToRocketKeyCode();
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_UNKNOWN, Result);
}

// ************************************************************************** //
TEST_F(KeyCode_test, /*DISABLED_*/Test_ToRocketKeyCode)
{
  static const ::std::map<int32_t, RocketKeyCode_t> Keys =
  {
    { VK_BACK, ::mock::Rocket::Core::Input::KI_BACK }, // Backspace
    { VK_END, ::mock::Rocket::Core::Input::KI_END }, // End
    { VK_HOME, ::mock::Rocket::Core::Input::KI_HOME }, // Home
    { VK_LEFT, ::mock::Rocket::Core::Input::KI_LEFT }, // Left
    { VK_UP, ::mock::Rocket::Core::Input::KI_UP }, // Up
    { VK_RIGHT, ::mock::Rocket::Core::Input::KI_RIGHT }, // Right
    { VK_DOWN, ::mock::Rocket::Core::Input::KI_DOWN }, // Down
    { VK_DELETE, ::mock::Rocket::Core::Input::KI_DELETE }, // Del
  };

  for (const auto Key : Keys)
  {
    const Tested_t Example{ Key.first };

    const auto Result = Example.ToRocketKeyCode();
    EXPECT_EQ(Key.second, Result);
  }
}
