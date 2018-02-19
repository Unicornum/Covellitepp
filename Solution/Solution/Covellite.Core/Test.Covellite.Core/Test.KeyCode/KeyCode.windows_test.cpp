
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса KeyCode.
*/

#define KeyCode_test KeyCode_windows_test
#define KeyCode KeyCode_windows

// Расположение класса Window
#include "..\..\Covellite.Core\Params.windows.cpp"

// Общий тестовый класс класса KeyCode
class KeyCode_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::params::KeyCode;
  using RocketKeyCode_t = ::mock::Rocket::Core::Input::KeyIdentifier;

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
TEST_F(KeyCode_test, /*DISABLED_*/Test_ToRocketKeyCode_Unknown)
{
  const Tested_t Example{ 1711011232 };

  const auto Result = Example.ToRocketKeyCode();
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_UNKNOWN, Result);
}

// ************************************************************************** //
TEST_F(KeyCode_test, /*DISABLED_*/Test_ToRocketKeyCode)
{
  static const ::std::map<int32_t, RocketKeyCode_t> Keys =
  {
    { VK_BACK, ::mock::Rocket::Core::Input::KI_BACK }, // Backspace
    { VK_END, ::mock::Rocket::Core::Input::KI_END }, // End
    { VK_HOME, ::mock::Rocket::Core::Input::KI_HOME }, // Home
    { VK_LEFT, ::mock::Rocket::Core::Input::KI_LEFT }, // Left
    { VK_UP, ::mock::Rocket::Core::Input::KI_UP }, // Up
    { VK_RIGHT, ::mock::Rocket::Core::Input::KI_RIGHT }, // Right
    { VK_DOWN, ::mock::Rocket::Core::Input::KI_DOWN }, // Down
    { VK_DELETE, ::mock::Rocket::Core::Input::KI_DELETE }, // Del
  };

  for (const auto Key : Keys)
  {
    const Tested_t Example{ Key.first };

    const auto Result = Example.ToRocketKeyCode();
    EXPECT_EQ(Key.second, Result);
  }
}
