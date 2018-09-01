
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Fx.
*/

// Общий тестовый класс класса Fx
class Fx_test :
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

// Образец макроса для подстановки в класс Fx 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Fx_test, Test_Function);

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_Render)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"Render.expected.fx", 
    THIS_DIRECTORY / L"../../Covellite.Api/Render/fx/Render.fx");
}
