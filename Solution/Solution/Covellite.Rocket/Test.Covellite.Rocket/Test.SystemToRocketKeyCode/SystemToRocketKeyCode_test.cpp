
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ SystemToRocketKeyCode.
*/

// ����� �������� ����� ������ SystemToRocketKeyCode
class SystemToRocketKeyCode_test :
  public ::testing::Test
{
protected:
  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� SystemToRocketKeyCode 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(SystemToRocketKeyCode_test, Test_Function);

#ifdef SystemToRocketKeyCode
#undef SystemToRocketKeyCode
#endif
#define SystemToRocketKeyCode SystemToRocketKeyCode_windows
#undef COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP
#include <Covellite\Rocket\Rocket.forward.hpp>

// ************************************************************************** //
TEST_F(SystemToRocketKeyCode_test, /*DISABLED_*/Test_Windows_UnknownCode)
{
  using namespace ::covellite::rocket;

  const auto Result = SystemToRocketKeyCode(1807241220);
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_UNKNOWN, Result);
}

// ************************************************************************** //
TEST_F(SystemToRocketKeyCode_test, /*DISABLED_*/Test_Windows)
{
  using namespace ::covellite::rocket;

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
    const auto Result = SystemToRocketKeyCode(Key.first);
    EXPECT_EQ(Key.second, Result);
  }
}

#include "..\..\Covellite.Rocket\SystemToRocketKeyCode.windows.cpp"

#ifdef SystemToRocketKeyCode
#undef SystemToRocketKeyCode
#endif
#define SystemToRocketKeyCode SystemToRocketKeyCode_android
#undef COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP
#include <Covellite\Rocket\Rocket.forward.hpp>

// ************************************************************************** //
TEST_F(SystemToRocketKeyCode_test, /*DISABLED_*/Test_Android_UnknownCode)
{
  using namespace ::covellite::rocket;

  // cppcheck-suppress ConfigurationNotChecked
  const auto Result = SystemToRocketKeyCode(1807241224);
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_UNKNOWN, Result);
}

// ************************************************************************** //
TEST_F(SystemToRocketKeyCode_test, /*DISABLED_*/Test_Android)
{
  using namespace ::covellite::rocket;

  // cppcheck-suppress ConfigurationNotChecked
  const auto Result = SystemToRocketKeyCode(AKEYCODE_DEL);
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_BACK, Result);
}

#include "..\..\Covellite.Rocket\SystemToRocketKeyCode.android.cpp"
