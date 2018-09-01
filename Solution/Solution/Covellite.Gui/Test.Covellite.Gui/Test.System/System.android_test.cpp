
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса System.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define System_test System_android_test
#define System System_android

// Расположение класса System
#include "..\..\Covellite.Gui\System.android.cpp"
#include "..\..\Covellite.Gui\System.cpp"

#include <Covellite\Gui\IStringTranslator.hpp>

// Общий тестовый класс класса System
class System_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::System;
  using ITested_t = ::mock::Rocket::Core::SystemInterface;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
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

// Образец макроса для подстановки в класс System 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(System_test, Test_Function);

// ************************************************************************** //
TEST_F(System_test, /*DISABLED_*/Test_GetElapsedTime)
{
  using Proxy_t = ::mock::TimeProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const ::mock::timeval Value0 = { 123L, 456789L };
  const ::mock::timeval Value1 = { 234L, 567899L };
  const ::mock::timeval Value2 = { 345L, 678999L };

  const auto pStringTranslator = ::std::make_shared<StringTranslator>();

  Tested_t Example{ pStringTranslator };
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Proxy, Get(nullptr))
    .Times(3)
    .WillOnce(Return(Value0))
    .WillOnce(Return(Value1))
    .WillOnce(Return(Value2));

  const auto Result0 = IExample.GetElapsedTime();
  EXPECT_EQ(0.0f, Result0);

  const auto Result1 = IExample.GetElapsedTime();
  EXPECT_EQ(111.111110f, Result1);

  const auto Result2 = IExample.GetElapsedTime();
  EXPECT_EQ(222.222210f, Result2);
}

// ************************************************************************** //
TEST_F(System_test, /*DISABLED_*/Test_ActivateKeyboard)
{
  using EnvironmentProxy_t = ::alicorn::system::platform::proxy::Environment;
  EnvironmentProxy_t EnvironmentProxy;
  EnvironmentProxy_t::GetInstance() = &EnvironmentProxy;

  const ::mock::Id_t Id = 1705252038;

  const auto pStringTranslator = ::std::make_shared<StringTranslator>();

  Tested_t Example{ pStringTranslator };
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EnvironmentProxy, Constructor())
    .Times(1)
    .WillOnce(Return(Id));
  
  EXPECT_CALL(EnvironmentProxy, ShowSoftKeyboard(Id))
    .Times(1);

  IExample.ActivateKeyboard();
}

// ************************************************************************** //
TEST_F(System_test, /*DISABLED_*/Test_DeactivateKeyboard)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ANativeActivity Activity;
  auto * pActivity = &Activity;

  const AppInfo_t Info{ &pActivity };

  const auto pStringTranslator = ::std::make_shared<StringTranslator>();

  Tested_t Example{ pStringTranslator };
  ITested_t & IExample = Example;

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, HideSoftInput(&Activity, 0))
    .Times(1);

  IExample.DeactivateKeyboard();
}
