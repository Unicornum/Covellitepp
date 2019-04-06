
#include "stdafx.h"
#include <Platform\Android.mock.hpp>

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ KeyCode.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define KeyCode_test KeyCode_android_test
#define KeyCode KeyCode_android

// ������������ ������ Window
#include "..\..\Covellite.Core\Params.android.cpp"

// ����� �������� ����� ������ KeyCode
class KeyCode_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::params::KeyCode;

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
TEST_F(KeyCode_test, /*DISABLED_*/Test_ToRocketKeyCode_UnknownCode)
{
  const Tested_t Example{ 1711011211 };

  const auto Result = Example.ToRocketKeyCode();
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_UNKNOWN, Result);
}

// ************************************************************************** //
TEST_F(KeyCode_test, /*DISABLED_*/Test_ToRocketKeyCode)
{
  const Tested_t Example{ AKEYCODE_DEL };

  const auto Result = Example.ToRocketKeyCode();
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_BACK, Result);
}
