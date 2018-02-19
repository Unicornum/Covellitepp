
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Display.
*/

// Расположение класса Display
#include "..\..\Covellite.Egl\Display.hpp"

// Общий тестовый класс класса Display
class Display_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::egl::Display;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс Display 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Display_test, Test_Function);

// ************************************************************************** //
TEST_F(Display_test, /*DISABLED_*/Test_InitializeSuccess)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  int intDisplay = 1611181409;
  ::covellite::egl::EGLDisplay Display = &intDisplay;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(Display));

  EXPECT_CALL(EglProxy, Initialize(Display, nullptr, nullptr))
    .Times(1)
    .WillOnce(Return(1611181415));

  {
    Tested_t Example;

    EXPECT_CALL(EglProxy, Terminate(Display))
      .Times(1)
      .WillOnce(Return(1611181437));
  }
}

// ************************************************************************** //
TEST_F(Display_test, /*DISABLED_*/Test_InitializeFail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  int intDisplay = 1611181416;
  ::covellite::egl::EGLDisplay Display = &intDisplay;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(Display));

  EXPECT_CALL(EglProxy, Initialize(Display, nullptr, nullptr))
    .Times(1)
    .WillOnce(Return(0));

  {
    EXPECT_THROW(Tested_t Example, ::std::exception);

    EXPECT_CALL(EglProxy, MakeCurrent(Display,
      ::covellite::egl::EGL_NO_SURFACE, ::covellite::egl::EGL_NO_SURFACE, ::covellite::egl::EGL_NO_CONTEXT))
      .Times(0);

    EXPECT_CALL(EglProxy, Terminate(Display))
      .Times(0);
  }
}
