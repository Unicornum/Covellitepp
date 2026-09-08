
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Using.
*/

// Общий тестовый класс класса Using
class Using_test :
  public ::testing::Test
{
protected:
  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) noexcept override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) noexcept override
  {
  }
};

ALICORN_DISABLE_GTEST_WARNINGS

// Образец макроса для подстановки в класс Using 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Using_test, Test_Function);

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_Function)
{
  FAIL() << u8"Добавить тесты";
}

ALICORN_RESTORE_WARNINGS
