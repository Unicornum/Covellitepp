
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса IWindow.
*/

// Расположение класса IWindow
#include <Covellite/Expanse/IWindow.hpp>
#include <Covellite/Events/Events.hpp>

ALICORN_DISABLE_GTEST_WARNINGS

// Общий тестовый класс класса IWindow
class IWindow_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::IWindow;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс IWindow 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(IWindow_test, Test_Function);

// ************************************************************************** //
TEST_F(IWindow_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(IWindow_test, /*DISABLED_*/Test_Interfaces)
{
  EXPECT_TRUE((::std::is_convertible<Tested_t &,
    ::covellite::events::IEvents &>::value));
  EXPECT_TRUE((::std::is_convertible<Tested_t &,
    ::covellite::expanse::IGameScene &>::value));
  EXPECT_TRUE((::std::is_convertible<Tested_t &,
    ::covellite::expanse::I3DScene &>::value));
}

ALICORN_RESTORE_WARNINGS
