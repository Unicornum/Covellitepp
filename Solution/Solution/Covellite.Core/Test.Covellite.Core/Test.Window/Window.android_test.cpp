
#include "stdafx.h"
#include <alicorn\platform\app-info.hpp>
#include <Platform\Android.mock.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Window.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define Window_test Window_android_test
#define Window Window_android

// Расположение класса Window
#include "..\..\Covellite.Core\Window.android.cpp"

// Общий тестовый класс класса Window
class Window_test :
  public ::testing::Test
{
protected:
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  using Tested_t = ::covellite::core::Window;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс Window 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Window_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetStatusBarHeight)
{
  using EnvironmentProxy_t = ::alicorn::system::platform::proxy::Environment;
  EnvironmentProxy_t EnvironmentProxy;
  EnvironmentProxy_t::GetInstance() = &EnvironmentProxy;

  const ::mock::Id_t EnvironmentId = 1711011250;
  const auto StatusBarHeight = 1711011251;

  const Tested_t Example;

  using namespace ::testing;

  EXPECT_CALL(EnvironmentProxy, Constructor())
    .Times(1)
    .WillOnce(Return(EnvironmentId));

  EXPECT_CALL(EnvironmentProxy, GetStatusBarHeight(EnvironmentId))
    .Times(1)
    .WillOnce(Return(StatusBarHeight));

  const auto Result = Example.GetStatusBarHeight();
  EXPECT_EQ(StatusBarHeight, Result);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Exit)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ANativeActivity Activity;
  auto * pActivity = &Activity;

  const AppInfo_t Info{ &pActivity };

  const Tested_t Example;

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, Finish(&Activity))
    .Times(1);

  Example.Exit();
}
