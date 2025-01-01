
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса SystemToGuiKeyCode.
*/

// Общий тестовый класс класса SystemToGuiKeyCode
class SystemToGuiKeyCode_test :
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

// Образец макроса для подстановки в класс SystemToGuiKeyCode 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(SystemToGuiKeyCode_test, Test_Function);

#ifdef SystemToGuiKeyCode
#undef SystemToGuiKeyCode
#endif
#define SystemToGuiKeyCode SystemToGuiKeyCode_windows
#undef COVELLITE_ROCKET_SYSTEMTOGUIKEYCODE_HPP
#include <Covellite/Gui/SystemToGuiKeyCode.hpp>

// ************************************************************************** //
TEST_F(SystemToGuiKeyCode_test, /*DISABLED_*/Test_Windows_UnknownCode)
{
  using namespace ::covellite::gui;

  const auto Result = SystemToGuiKeyCode(1807241220);
  EXPECT_EQ(::mock::CovelliteGui::Input::KI_UNKNOWN, Result);
}

// ************************************************************************** //
TEST_F(SystemToGuiKeyCode_test, /*DISABLED_*/Test_Windows)
{
  using namespace ::covellite::gui;

  static const ::std::map<int32_t, KeyCode_t> Keys =
  {
    { VK_BACK, ::mock::CovelliteGui::Input::KI_BACK }, // Backspace
    { VK_END, ::mock::CovelliteGui::Input::KI_END }, // End
    { VK_HOME, ::mock::CovelliteGui::Input::KI_HOME }, // Home
    { VK_LEFT, ::mock::CovelliteGui::Input::KI_LEFT }, // Left
    { VK_UP, ::mock::CovelliteGui::Input::KI_UP }, // Up
    { VK_RIGHT, ::mock::CovelliteGui::Input::KI_RIGHT }, // Right
    { VK_DOWN, ::mock::CovelliteGui::Input::KI_DOWN }, // Down
    { VK_DELETE, ::mock::CovelliteGui::Input::KI_DELETE }, // Del
  };

  for (const auto Key : Keys)
  {
    const auto Result = SystemToGuiKeyCode(Key.first);
    EXPECT_EQ(Key.second, Result);
  }
}

#include "..\..\Covellite.Gui\SystemToGuiKeyCode.windows.cpp"

#ifdef SystemToGuiKeyCode
#undef SystemToGuiKeyCode
#endif
#define SystemToGuiKeyCode SystemToGuiKeyCode_android
#undef COVELLITE_ROCKET_SYSTEMTOGUIKEYCODE_HPP
#include <Covellite/Gui/SystemToGuiKeyCode.hpp>

// ************************************************************************** //
TEST_F(SystemToGuiKeyCode_test, /*DISABLED_*/Test_Android_UnknownCode)
{
  using namespace ::covellite::gui;

  // cppcheck-suppress ConfigurationNotChecked
  const auto Result = SystemToGuiKeyCode(1807241224);
  EXPECT_EQ(::mock::CovelliteGui::Input::KI_UNKNOWN, Result);
}

// ************************************************************************** //
TEST_F(SystemToGuiKeyCode_test, /*DISABLED_*/Test_Android)
{
  using namespace ::covellite::gui;

  // cppcheck-suppress ConfigurationNotChecked
  const auto Result = SystemToGuiKeyCode(AKEYCODE_DEL);
  EXPECT_EQ(::mock::CovelliteGui::Input::KI_BACK, Result);
}

#include "..\..\Covellite.Gui\SystemToGuiKeyCode.android.cpp"
