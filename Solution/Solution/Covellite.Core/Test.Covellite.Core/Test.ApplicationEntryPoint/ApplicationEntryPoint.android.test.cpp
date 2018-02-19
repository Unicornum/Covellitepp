
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ ApplicationEntryPoint.
*/

#include <Covellite\Core.mock.hpp>

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define ApplicationEntryPoint_test ApplicationEntryPoint_android_test
#undef APPLICATION_ENTRY_POINT

// ������������ ������ ApplicationEntryPoint
#include <Covellite\Core\ApplicationEntryPoint.hpp>

namespace covellite
{

// ����� �������� ����� ������ ApplicationEntryPoint
class ApplicationEntryPoint_test :
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
  class Application { };
  APPLICATION_ENTRY_POINT(Application);
};

// ������� ������� ��� ����������� � ����� ApplicationEntryPoint 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(ApplicationEntryPoint_test, Test_Function);

// ************************************************************************** //
// cppcheck-suppress syntaxError
TEST_F(ApplicationEntryPoint_test, /*DISABLED_*/Test_AndroidMain)
{
  using ApplicationProxy_t = ::mock::covellite::core::Application::Proxy;
  ApplicationProxy_t ApplicationProxy;
  ApplicationProxy_t::GetInstance() = &ApplicationProxy;

  ::android_app AndroidApp;

  using namespace ::testing;

  EXPECT_CALL(ApplicationProxy, Main(Eq(typeid(Application).name()), &AndroidApp))
    .Times(1);

  android_main(&AndroidApp);
}

// ************************************************************************** //
TEST_F(ApplicationEntryPoint_test, /*DISABLED_*/Test_CovelliteFakeFunction)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, OnCreate(nullptr, nullptr, 0))
    .Times(1);

  CovelliteFakeFunction();
}

} // namespace covellite
