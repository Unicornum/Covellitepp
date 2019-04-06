
#include "stdafx.h"
#include <Platform\Android.mock.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса KeyCode.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define KeyCode_test KeyCode_android_test
#define KeyCode KeyCode_android

// Расположение класса Window
#include "..\..\Covellite.Core\Params.android.cpp"

// Общий тестовый класс класса KeyCode
class KeyCode_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::params::KeyCode;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс KeyCode 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(KeyCode_test, Test_Function);

// ************************************************************************** //
TEST_F(KeyCode_test, /*DISABLED_*/Test_ToRocketKeyCode_UnknownCode)
{
  const Tested_t Example{ 1711011211 };

  const auto Result = Example.ToRocketKeyCode();
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_UNKNOWN, Result);
}

// ************************************************************************** //
TEST_F(KeyCode_test, /*DISABLED_*/Test_ToRocketKeyCode)
{
  const Tested_t Example{ AKEYCODE_DEL };

  const auto Result = Example.ToRocketKeyCode();
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_BACK, Result);
}
