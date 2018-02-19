
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Environment.
*/

// Расположение класса Environment
#include "..\..\Covellite.Jni\Environment.hpp"

// Общий тестовый класс класса Environment
class Environment_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::jni::Environment;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс Environment 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Environment_test, Test_Function);

// ************************************************************************** //
TEST_F(Environment_test, /*DISABLED_*/Test_Inheritance)
{
  using EnvironmentProxy_t = ::mock::alicorn::system::platform::Environment::Proxy;
  EnvironmentProxy_t EnvironmentProxy;
  EnvironmentProxy_t::GetInstance() = &EnvironmentProxy;

  using namespace ::testing;

  EXPECT_CALL(EnvironmentProxy, Constructor())
    .Times(1);

  Tested_t Example;
}
