
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn/google/test/example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  ����� ������ IWindow.
*/

// ������������ ������ IWindow
#include <Covellite/Expanse/IWindow.hpp>
#include <Covellite/Events/Events.hpp>

ALICORN_DISABLE_GTEST_WARNINGS

// ����� �������� ����� ������ IWindow
class IWindow_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::IWindow;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
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

// ************************************************************************** //
TEST_F(IWindow_test, /*DISABLED_*/Test_Interfaces)
{
  EXPECT_TRUE((::std::is_convertible<Tested_t &,
    ::covellite::events::IEvents &>::value));
  EXPECT_TRUE((::std::is_convertible<Tested_t &,
    ::covellite::expanse::IGameScene &>::value));
  EXPECT_TRUE((::std::is_convertible<Tested_t &,
    ::covellite::expanse::I3DScene &>::value));
}

ALICORN_RESTORE_WARNINGS
