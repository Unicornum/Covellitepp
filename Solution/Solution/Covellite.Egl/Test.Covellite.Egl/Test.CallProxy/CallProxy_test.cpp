
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ CallProxy.
*/

// ������������ ������ CallProxy
#include "..\..\Covellite.Egl\CallProxy.hpp"

// ����� �������� ����� ������ CallProxy
class CallProxy_test :
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

protected:
  ::covellite::egl::EGLBoolean Test(::covellite::egl::EGLBoolean _Result)
  {
    return _Result;
  }
};

// ������� ������� ��� ����������� � ����� CallProxy 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(CallProxy_test, Test_Function);

// ************************************************************************** //
TEST_F(CallProxy_test, /*DISABLED_*/Test_Success)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetError())
    .Times(0);

  EXPECT_NO_THROW(EGL_CALL Test(1611181235));
}

// ************************************************************************** //
TEST_F(CallProxy_test, /*DISABLED_*/Test_Fail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetError())
    .Times(1)
    .WillOnce(Return(12345));

  EXPECT_STDEXCEPTION(EGL_CALL Test(0), 
    ".*callproxy_test.cpp \\(79\\): EGL error: 12345"); // 79 - ��� ��� ������
}
