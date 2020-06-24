
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса I3DScene.
*/

// Расположение класса I3DScene
#include <Covellite\Expanse\I3DScene.hpp>

ALICORN_DISABLE_GTEST_WARNINGS

// Общий тестовый класс класса I3DScene
class I3DScene_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::I3DScene;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс I3DScene 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(I3DScene_test, Test_Function);

// ************************************************************************** //
TEST_F(I3DScene_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

ALICORN_RESTORE_WARNINGS
