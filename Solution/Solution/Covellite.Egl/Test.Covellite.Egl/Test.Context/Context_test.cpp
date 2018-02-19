
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Context.
*/

// Расположение класса Context
#include "..\..\Covellite.Egl\Context.hpp"

// Общий тестовый класс класса Context
class Context_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::egl::Context;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс Context 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Context_test, Test_Function);

namespace covellite
{

namespace egl
{

// ************************************************************************** //
TEST_F(Context_test, /*DISABLED_*/Test_Using)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191236;
  const auto MockConfig = (::covellite::egl::EGLConfig)1611191237;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);
  Config.m_Config = MockConfig;

  const auto Context = (::covellite::egl::EGLContext)1611191238;

  EXPECT_CALL(EglProxy, CreateContext(MockDisplay, MockConfig, EGL_NO_CONTEXT,
    nullptr))
    .Times(1)
    .WillOnce(Return(Context));

  {
    Tested_t Example(Display, Config);
    EXPECT_EQ(Context, Example.m_Context);

    EXPECT_CALL(EglProxy, MakeCurrent(Display.m_Display, 
      ::covellite::egl::EGL_NO_SURFACE, ::covellite::egl::EGL_NO_SURFACE, 
      ::covellite::egl::EGL_NO_CONTEXT))
      .Times(1)
      .WillOnce(Return(1611181415));

    EXPECT_CALL(EglProxy, DestroyContext(MockDisplay, Context))
      .Times(1);
  }
}

// ************************************************************************** //
// cppcheck-suppress syntaxError
TEST_F(Context_test, /*DISABLED_*/Test_CreateContext_Fail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191248;
  const auto MockConfig = (::covellite::egl::EGLConfig)1611191249;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);
  Config.m_Config = MockConfig;

  EXPECT_CALL(EglProxy, CreateContext(MockDisplay, MockConfig, EGL_NO_CONTEXT,
    nullptr))
    .Times(1)
    .WillOnce(Return(EGL_NO_CONTEXT));

  {
    EXPECT_THROW(Tested_t Example(Display, Config), ::std::exception);

    EXPECT_CALL(EglProxy, DestroyContext(_, _))
      .Times(0);
  }
}

// ************************************************************************** //
TEST_F(Context_test, /*DISABLED_*/Test_MakeCurrent)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191938;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);

  const auto Context = (::covellite::egl::EGLContext)1611191941;

  EXPECT_CALL(EglProxy, CreateContext(MockDisplay, _, ::covellite::egl::EGL_NO_CONTEXT,
    nullptr))
    .Times(1)
    .WillOnce(Return(Context));

  const Tested_t Example(Display, Config);

  const auto MockSurface = (::covellite::egl::EGLSurface)1611191942;
  ::mock::covellite::egl::Surface Surface(Display, Config, nullptr);
  Surface.m_Surface = MockSurface;

  EXPECT_CALL(EglProxy, MakeCurrent(MockDisplay, MockSurface, MockSurface,
    Context))
    .Times(1)
    .WillOnce(Return(1611191943));

  {
    Example.MakeCurrent(Surface);

    EXPECT_CALL(EglProxy, MakeCurrent(_, _, _, _))
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Context_test, /*DISABLED_*/Test_MakeCurrent_Fail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191947;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);

  const auto Context = (::covellite::egl::EGLContext)1611191948;

  EXPECT_CALL(EglProxy, CreateContext(MockDisplay, _, ::covellite::egl::EGL_NO_CONTEXT,
    nullptr))
    .Times(1)
    .WillOnce(Return(Context));

  {
    const Tested_t Example(Display, Config);

    const auto MockSurface = (::covellite::egl::EGLSurface)1611191949;
    ::mock::covellite::egl::Surface Surface(Display, Config, nullptr);
    Surface.m_Surface = MockSurface;

    EXPECT_CALL(EglProxy, MakeCurrent(MockDisplay, MockSurface, MockSurface,
      Context))
      .Times(1)
      .WillOnce(Return(0));

    EXPECT_THROW(Example.MakeCurrent(Surface), ::std::exception);

    EXPECT_CALL(EglProxy, MakeCurrent(_, _, _, _))
      .Times(1);
  }
}

} // namespace egl

} // namespace covellite

