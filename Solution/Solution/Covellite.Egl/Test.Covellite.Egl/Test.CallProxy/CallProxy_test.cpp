
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса CallProxy.
*/

// Расположение класса CallProxy
#include "..\..\Covellite.Egl\CallProxy.hpp"

// Общий тестовый класс класса CallProxy
class CallProxy_test :
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

protected:
  ::covellite::egl::EGLBoolean Test(::covellite::egl::EGLBoolean _Result)
  {
    return _Result;
  }
};

// Образец макроса для подстановки в класс CallProxy 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(CallProxy_test, Test_Function);

// ************************************************************************** //
TEST_F(CallProxy_test, /*DISABLED_*/Test_Success)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetError())
    .Times(0);

  EXPECT_NO_THROW(EGL_CALL Test(1611181235));
}

// ************************************************************************** //
TEST_F(CallProxy_test, /*DISABLED_*/Test_Fail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetError())
    .Times(1)
    .WillOnce(Return(12345));

  EXPECT_STDEXCEPTION(EGL_CALL Test(0), 
    ".*callproxy_test.cpp \\(79\\): EGL error: 12345"); // 79 - это эта строка
}
