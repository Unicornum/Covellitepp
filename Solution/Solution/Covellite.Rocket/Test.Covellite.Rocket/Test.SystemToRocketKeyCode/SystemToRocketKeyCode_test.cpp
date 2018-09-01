
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса SystemToRocketKeyCode.
*/

// Общий тестовый класс класса SystemToRocketKeyCode
class SystemToRocketKeyCode_test :
  public ::testing::Test
{
protected:
  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс SystemToRocketKeyCode 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(SystemToRocketKeyCode_test, Test_Function);

#ifdef SystemToRocketKeyCode
#undef SystemToRocketKeyCode
#endif
#define SystemToRocketKeyCode SystemToRocketKeyCode_windows
#undef COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP
#include <Covellite\Rocket\Rocket.forward.hpp>

// ************************************************************************** //
TEST_F(SystemToRocketKeyCode_test, /*DISABLED_*/Test_Windows_UnknownCode)
{
  using namespace ::covellite::rocket;

  const auto Result = SystemToRocketKeyCode(1807241220);
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_UNKNOWN, Result);
}

// ************************************************************************** //
TEST_F(SystemToRocketKeyCode_test, /*DISABLED_*/Test_Windows)
{
  using namespace ::covellite::rocket;

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
    const auto Result = SystemToRocketKeyCode(Key.first);
    EXPECT_EQ(Key.second, Result);
  }
}

#include "..\..\Covellite.Rocket\SystemToRocketKeyCode.windows.cpp"

#ifdef SystemToRocketKeyCode
#undef SystemToRocketKeyCode
#endif
#define SystemToRocketKeyCode SystemToRocketKeyCode_android
#undef COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP
#include <Covellite\Rocket\Rocket.forward.hpp>

// ************************************************************************** //
TEST_F(SystemToRocketKeyCode_test, /*DISABLED_*/Test_Android_UnknownCode)
{
  using namespace ::covellite::rocket;

  // cppcheck-suppress ConfigurationNotChecked
  const auto Result = SystemToRocketKeyCode(1807241224);
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_UNKNOWN, Result);
}

// ************************************************************************** //
TEST_F(SystemToRocketKeyCode_test, /*DISABLED_*/Test_Android)
{
  using namespace ::covellite::rocket;

  // cppcheck-suppress ConfigurationNotChecked
  const auto Result = SystemToRocketKeyCode(AKEYCODE_DEL);
  EXPECT_EQ(::mock::Rocket::Core::Input::KI_BACK, Result);
}

#include "..\..\Covellite.Rocket\SystemToRocketKeyCode.android.cpp"
