
#include "stdafx.h"
#include <alicorn\platform\app-info.hpp>
#include <Platform\Android.mock.hpp>

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Window.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define Window_test Window_android_test
#define Window Window_android

// ������������ ������ Window
#include "..\..\Covellite.Core\Window.android.cpp"

// ����� �������� ����� ������ Window
class Window_test :
  public ::testing::Test
{
protected:
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  using Tested_t = ::covellite::core::Window;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� Window 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Window_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetStatusBarHeight)
{
  using EnvironmentProxy_t = ::alicorn::system::platform::proxy::Environment;
  EnvironmentProxy_t EnvironmentProxy;
  EnvironmentProxy_t::GetInstance() = &EnvironmentProxy;

  const ::mock::Id_t EnvironmentId = 1711011250;
  const auto StatusBarHeight = 1711011251;

  const Tested_t Example;

  using namespace ::testing;

  EXPECT_CALL(EnvironmentProxy, Constructor())
    .Times(1)
    .WillOnce(Return(EnvironmentId));

  EXPECT_CALL(EnvironmentProxy, GetStatusBarHeight(EnvironmentId))
    .Times(1)
    .WillOnce(Return(StatusBarHeight));

  const auto Result = Example.GetStatusBarHeight();
  EXPECT_EQ(StatusBarHeight, Result);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Exit)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ANativeActivity Activity;
  auto * pActivity = &Activity;

  const AppInfo_t Info{ &pActivity };

  const Tested_t Example;

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, Finish(&Activity))
    .Times(1);

  Example.Exit();
}
