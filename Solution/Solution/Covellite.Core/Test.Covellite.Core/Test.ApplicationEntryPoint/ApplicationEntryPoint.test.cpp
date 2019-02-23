
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
TEST_F(ApplicationEntryPoint_test, /*DISABLED_*/Test_WinMain)
{
  using ApplicationProxy_t = ::mock::covellite::core::Application::Proxy;
  ApplicationProxy_t ApplicationProxy;
  ApplicationProxy_t::GetInstance() = &ApplicationProxy;

  char Params[] = "Params1710191038";

  using namespace ::testing;

  EXPECT_CALL(ApplicationProxy, Main(Eq(typeid(Application).name()), Params))
    .Times(1);

  const auto Result = WinMain(0, 0, Params, 0);
  EXPECT_EQ(0, Result);
}

} // namespace covellite
