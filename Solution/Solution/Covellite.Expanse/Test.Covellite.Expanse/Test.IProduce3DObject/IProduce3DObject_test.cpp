
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса IProduce3DObject.
*/

// Расположение класса IProduce3DObject
#include "..\..\Covellite.Expanse\IProduce3DObject.impl.hpp"

// Общий тестовый класс класса IProduce3DObject
class IProduce3DObject_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::IProduce3DObject;

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

// Образец макроса для подстановки в класс IProduce3DObject 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(IProduce3DObject_test, Test_Function);

// ************************************************************************** //
TEST_F(IProduce3DObject_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

ALICORN_RESTORE_WARNINGS
