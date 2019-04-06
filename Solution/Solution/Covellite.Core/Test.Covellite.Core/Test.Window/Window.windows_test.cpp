
#include "stdafx.h"
#include <Platform\Windows.mock.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Window.
*/

#define Window_test Window_windows_test
#define Window Window_windows

// Расположение класса Window
#include "..\..\Covellite.Core\Window.windows.cpp"

// Общий тестовый класс класса Window
class Window_test :
  public ::testing::Test
{
protected:
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
  const Tested_t Example;
  EXPECT_EQ(0, Example.GetStatusBarHeight());
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Exit)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const Tested_t Example;

  using namespace ::testing;

  EXPECT_CALL(Proxy, PostQuitMessage(0))
    .Times(1);

  Example.Exit();
}
