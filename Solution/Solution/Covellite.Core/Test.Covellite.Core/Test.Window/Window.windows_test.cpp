
#include "stdafx.h"
#include <Platform\Windows.mock.hpp>

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Window.
*/

#define Window_test Window_windows_test
#define Window Window_windows

// ������������ ������ Window
#include "..\..\Covellite.Core\Window.windows.cpp"

// ����� �������� ����� ������ Window
class Window_test :
  public ::testing::Test
{
protected:
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
  const Tested_t Example;
  EXPECT_EQ(0, Example.GetStatusBarHeight());
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Exit)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const Tested_t Example;

  using namespace ::testing;

  EXPECT_CALL(Proxy, PostQuitMessage(0))
    .Times(1);

  Example.Exit();
}
