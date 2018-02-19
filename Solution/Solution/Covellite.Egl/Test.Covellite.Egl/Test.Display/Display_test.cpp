
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Display.
*/

// ������������ ������ Display
#include "..\..\Covellite.Egl\Display.hpp"

// ����� �������� ����� ������ Display
class Display_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::egl::Display;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� Display 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Display_test, Test_Function);

// ************************************************************************** //
TEST_F(Display_test, /*DISABLED_*/Test_InitializeSuccess)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  int intDisplay = 1611181409;
  ::covellite::egl::EGLDisplay Display = &intDisplay;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(Display));

  EXPECT_CALL(EglProxy, Initialize(Display, nullptr, nullptr))
    .Times(1)
    .WillOnce(Return(1611181415));

  {
    Tested_t Example;

    EXPECT_CALL(EglProxy, Terminate(Display))
      .Times(1)
      .WillOnce(Return(1611181437));
  }
}

// ************************************************************************** //
TEST_F(Display_test, /*DISABLED_*/Test_InitializeFail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  int intDisplay = 1611181416;
  ::covellite::egl::EGLDisplay Display = &intDisplay;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(Display));

  EXPECT_CALL(EglProxy, Initialize(Display, nullptr, nullptr))
    .Times(1)
    .WillOnce(Return(0));

  {
    EXPECT_THROW(Tested_t Example, ::std::exception);

    EXPECT_CALL(EglProxy, MakeCurrent(Display,
      ::covellite::egl::EGL_NO_SURFACE, ::covellite::egl::EGL_NO_SURFACE, ::covellite::egl::EGL_NO_CONTEXT))
      .Times(0);

    EXPECT_CALL(EglProxy, Terminate(Display))
      .Times(0);
  }
}
