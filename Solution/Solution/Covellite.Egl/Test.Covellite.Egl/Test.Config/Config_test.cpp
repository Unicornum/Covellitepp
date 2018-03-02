
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Config.
*/

// Расположение класса Config
#include "..\..\Covellite.Egl\Config.hpp"

// Общий тестовый класс класса Config
class Config_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::egl::Config;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс Config 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Config_test, Test_Function);

// ************************************************************************** //
TEST_F(Config_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_FALSE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

namespace covellite
{

namespace egl
{

// ************************************************************************** //
TEST_F(Config_test, /*DISABLED_*/Test_ANativeWindow_Nullptr)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  ::covellite::egl::EGLDisplay MockDisplay = (::covellite::egl::EGLDisplay)1611182238;

  using namespace ::testing;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;

  EXPECT_THROW(Tested_t Example(Display, nullptr), ::std::exception);
}

// ************************************************************************** //
// cppcheck-suppress syntaxError
TEST_F(Config_test, /*DISABLED_*/Test_Constructor)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  ::covellite::egl::EGLDisplay MockDisplay = (::covellite::egl::EGLDisplay)1611182202;
  ::covellite::egl::EGLConfig MockConfig = (::covellite::egl::EGLConfig)1611182215;
  const ::covellite::egl::EGLint Format = 1611182232;

  const ::covellite::egl::EGLint Attributes[] =
  {
    ::covellite::egl::EGL_SURFACE_TYPE, ::covellite::egl::EGL_WINDOW_BIT,
    ::covellite::egl::EGL_BLUE_SIZE,    8,
    ::covellite::egl::EGL_GREEN_SIZE,   8,
    ::covellite::egl::EGL_RED_SIZE,     8,
    ::covellite::egl::EGL_NONE
  };

  EglProxy.m_pConfig = &MockConfig;
  EglProxy.m_Value = Format;
  EglProxy.m_pAttributes = Attributes;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ANativeWindow NativeWindow;

  EXPECT_CALL(EglProxy, ChooseConfig(MockDisplay, _, 1, Ne(nullptr)))
    .Times(1)
    .WillOnce(Return(1611182205));

  EXPECT_CALL(EglProxy, GetConfigAttrib(MockDisplay, MockConfig,
    ::covellite::egl::EGL_NATIVE_VISUAL_ID))
    .Times(1)
    .WillOnce(Return(1611182223));

  EXPECT_CALL(EglProxy, SetBuffersGeometry(&NativeWindow, 0, 0, Format))
    .Times(1);

  Tested_t Example(Display, &NativeWindow);
  EXPECT_EQ(MockConfig, Example.m_Config);
}

} // namespace egl

} // namespace covellite

// ************************************************************************** //
TEST_F(Config_test, /*DISABLED_*/Test_eglChooseConfig_Fail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  ::covellite::egl::EGLDisplay MockDisplay = (::covellite::egl::EGLDisplay)1611182202;
  ::covellite::egl::EGLConfig MockConfig = (::covellite::egl::EGLConfig)1611182244;

  const ::covellite::egl::EGLint Attributes[] =
  {
    ::covellite::egl::EGL_SURFACE_TYPE, ::covellite::egl::EGL_WINDOW_BIT,
    ::covellite::egl::EGL_BLUE_SIZE,    8,
    ::covellite::egl::EGL_GREEN_SIZE,   8,
    ::covellite::egl::EGL_RED_SIZE,     8,
    ::covellite::egl::EGL_NONE
  };

  EglProxy.m_pConfig = &MockConfig;
  EglProxy.m_pAttributes = Attributes;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ANativeWindow NativeWindow;

  EXPECT_CALL(EglProxy, ChooseConfig(MockDisplay, _, 1, Ne(nullptr)))
    .Times(1)
    .WillOnce(Return(0));

  EXPECT_CALL(EglProxy, GetConfigAttrib(_, _, _))
    .Times(0);

  EXPECT_CALL(EglProxy, SetBuffersGeometry(_, _, _, _))
    .Times(0);

  EXPECT_THROW(Tested_t Example(Display, &NativeWindow), ::std::exception);
}

// ************************************************************************** //
TEST_F(Config_test, /*DISABLED_*/Test_eglGetConfigAttrib_Fail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  ::covellite::egl::EGLDisplay MockDisplay = (::covellite::egl::EGLDisplay)1611182245;
  ::covellite::egl::EGLConfig MockConfig = (::covellite::egl::EGLConfig)1611182246;
  const ::covellite::egl::EGLint Format = 1611182247;

  const ::covellite::egl::EGLint Attributes[] =
  {
    ::covellite::egl::EGL_SURFACE_TYPE, ::covellite::egl::EGL_WINDOW_BIT,
    ::covellite::egl::EGL_BLUE_SIZE,    8,
    ::covellite::egl::EGL_GREEN_SIZE,   8,
    ::covellite::egl::EGL_RED_SIZE,     8,
    ::covellite::egl::EGL_NONE
  };

  EglProxy.m_pConfig = &MockConfig;
  EglProxy.m_Value = Format;
  EglProxy.m_pAttributes = Attributes;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ANativeWindow NativeWindow;

  EXPECT_CALL(EglProxy, ChooseConfig(MockDisplay, _, 1, Ne(nullptr)))
    .Times(1)
    .WillOnce(Return(1611182248));

  EXPECT_CALL(EglProxy, GetConfigAttrib(MockDisplay, MockConfig,
    ::covellite::egl::EGL_NATIVE_VISUAL_ID))
    .Times(1)
    .WillOnce(Return(0));

  EXPECT_CALL(EglProxy, SetBuffersGeometry(_, _, _, _))
    .Times(0);

  EXPECT_THROW(Tested_t Example(Display, &NativeWindow), ::std::exception);
}
