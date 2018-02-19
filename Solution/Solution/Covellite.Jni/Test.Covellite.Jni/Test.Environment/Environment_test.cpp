
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Environment.
*/

// ������������ ������ Environment
#include "..\..\Covellite.Jni\Environment.hpp"

// ����� �������� ����� ������ Environment
class Environment_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::jni::Environment;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� Environment 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Environment_test, Test_Function);

// ************************************************************************** //
TEST_F(Environment_test, /*DISABLED_*/Test_Inheritance)
{
  using EnvironmentProxy_t = ::mock::alicorn::system::platform::Environment::Proxy;
  EnvironmentProxy_t EnvironmentProxy;
  EnvironmentProxy_t::GetInstance() = &EnvironmentProxy;

  using namespace ::testing;

  EXPECT_CALL(EnvironmentProxy, Constructor())
    .Times(1);

  Tested_t Example;
}
