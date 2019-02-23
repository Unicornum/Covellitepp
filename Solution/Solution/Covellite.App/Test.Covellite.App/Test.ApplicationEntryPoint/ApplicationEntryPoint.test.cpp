
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ ApplicationEntryPoint.
*/

#include <Covellite\App.mock.hpp>

#undef APPLICATION_ENTRY_POINT

// ������������ ������ ApplicationEntryPoint
#include <Covellite\App\ApplicationEntryPoint.hpp>

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
  using ApplicationProxy_t = ::mock::covellite::app::Application::Proxy;
  ApplicationProxy_t ApplicationProxy;
  ApplicationProxy_t::GetInstance() = &ApplicationProxy;

  char Params[] = "Params1710191038";

  using namespace ::testing;

  EXPECT_CALL(ApplicationProxy, Main(Eq(typeid(Application).name()), Params))
    .Times(1);

#pragma warning(push)
#pragma warning(disable: 6387)

  const auto Result = WinMain(0, 0, Params, 0);
  EXPECT_EQ(0, Result);

#pragma warning(pop)
}

} // namespace covellite
