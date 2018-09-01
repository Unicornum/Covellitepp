
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ System.
*/

#define System_test System_windows_test
#define System System_windows

// ������������ ������ System
#include "..\..\Covellite.Gui\System.windows.cpp"
#include "..\..\Covellite.Gui\System.cpp"

#include <Covellite\Gui\IStringTranslator.hpp>

// ����� �������� ����� ������ System
class System_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::System;
  using ITested_t = ::mock::Rocket::Core::SystemInterface;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }

protected:
  class StringTranslator :
    public ::covellite::gui::IStringTranslator
  {
  public:
    MOCK_METHOD1(Translate, Utf8String_t(const Utf8String_t &));
  };
};

// ������� ������� ��� ����������� � ����� System 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(System_test, Test_Function);

// ************************************************************************** //
TEST_F(System_test, /*DISABLED_*/Test_GetElapsedTime)
{
  using Time_t = ::std::chrono::microseconds;

  using Proxy_t = ::mock::std::chrono::system_clock::Proxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const Time_t Value0{ 1222333 };
  const Time_t Value1{ 4555666 };
  const Time_t Value2{ 7888999 };

  const auto pStringTranslator = ::std::make_shared<StringTranslator>();

  Tested_t Example{ pStringTranslator };
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Proxy, Now())
    .Times(3)
    .WillOnce(Return(Value0))
    .WillOnce(Return(Value1))
    .WillOnce(Return(Value2));

  const auto Result0 = IExample.GetElapsedTime();
  EXPECT_EQ(0.0f, Result0);

  const auto Result1 = IExample.GetElapsedTime();
  EXPECT_EQ(3.333333f, Result1);

  const auto Result2 = IExample.GetElapsedTime();
  EXPECT_EQ(6.666666f, Result2);
}
