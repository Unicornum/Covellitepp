
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Surface.
*/

// Расположение класса Surface
#include "..\..\Covellite.Egl\Surface.hpp"

// Общий тестовый класс класса Surface
class Surface_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::egl::Surface;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }

protected:
  void SetConfig(::mock::covellite::egl::Config & _Config, ::covellite::egl::EGLConfig _MockConfig)
  {
    _Config.m_Config = _MockConfig;
  }
};

// Образец макроса для подстановки в класс Surface 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Surface_test, Test_Function);

namespace covellite
{

namespace egl
{

// ************************************************************************** //
TEST_F(Surface_test, /*DISABLED_*/Test_CreateSurface)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191319;
  const auto MockConfig = (::covellite::egl::EGLConfig)1611191320;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);
  SetConfig(Config, MockConfig);
  ANativeWindow NativeWindow;

  const auto Surface = (::covellite::egl::EGLSurface)1611191321;

  EXPECT_CALL(EglProxy, CreateSurface(MockDisplay, MockConfig, &NativeWindow,
    nullptr))
    .Times(1)
    .WillOnce(Return(Surface));

  {
    Tested_t Example(Display, Config, &NativeWindow);
    EXPECT_EQ(Surface, Example.m_Surface);

    EXPECT_CALL(EglProxy, DestroySurface(MockDisplay, Surface))
      .Times(1);
  }
}

} // namespace egl

} // namespace covellite

// ************************************************************************** //
TEST_F(Surface_test, /*DISABLED_*/Test_CreateSurface_Fail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191332;
  const auto MockConfig = (::covellite::egl::EGLConfig)1611191333;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);
  SetConfig(Config, MockConfig);
  ANativeWindow NativeWindow;

  EXPECT_CALL(EglProxy, CreateSurface(MockDisplay, MockConfig, &NativeWindow,
    nullptr))
    .Times(1)
    .WillOnce(Return(::covellite::egl::EGL_NO_SURFACE));

  {
    EXPECT_THROW(Tested_t Example(Display, Config, &NativeWindow),
      ::std::exception);

    EXPECT_CALL(EglProxy, DestroySurface(_, _))
      .Times(0);
  }
}

// ************************************************************************** //
TEST_F(Surface_test, /*DISABLED_*/Test_GetWidth)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191338;
  const auto MockConfig = (::covellite::egl::EGLConfig)1611191339;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);
  SetConfig(Config, MockConfig);
  ANativeWindow NativeWindow;
  const ::covellite::egl::EGLint Value = 1611191344;

  EglProxy.m_Value = Value;

  const auto Surface = (::covellite::egl::EGLSurface)1611191340;

  EXPECT_CALL(EglProxy, CreateSurface(MockDisplay, MockConfig, &NativeWindow,
    nullptr))
    .Times(1)
    .WillOnce(Return(Surface));

  const Tested_t Example(Display, Config, &NativeWindow);

  EXPECT_CALL(EglProxy, QuerySurface(MockDisplay, Surface, ::covellite::egl::EGL_WIDTH))
    .Times(1)
    .WillOnce(Return(1611191343));

  const auto Result = Example.GetWidth();
  EXPECT_EQ(Value, Result);
}

// ************************************************************************** //
TEST_F(Surface_test, /*DISABLED_*/Test_GetWidth_Fail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191347;
  const auto MockConfig = (::covellite::egl::EGLConfig)1611191348;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);
  SetConfig(Config, MockConfig);
  ANativeWindow NativeWindow;

  const auto Surface = (::covellite::egl::EGLSurface)1611191350;

  EXPECT_CALL(EglProxy, CreateSurface(MockDisplay, MockConfig, &NativeWindow,
    nullptr))
    .Times(1)
    .WillOnce(Return(Surface));

  const Tested_t Example(Display, Config, &NativeWindow);

  EXPECT_CALL(EglProxy, QuerySurface(MockDisplay, Surface, ::covellite::egl::EGL_WIDTH))
    .Times(1)
    .WillOnce(Return(0));

  EXPECT_THROW(Example.GetWidth(), ::std::exception);
}

// ************************************************************************** //
TEST_F(Surface_test, /*DISABLED_*/Test_GetHeight)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191351;
  const auto MockConfig = (::covellite::egl::EGLConfig)1611191352;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);
  SetConfig(Config, MockConfig);
  ANativeWindow NativeWindow;
  const ::covellite::egl::EGLint Value = 1611191353;

  EglProxy.m_Value = Value;

  const auto Surface = (::covellite::egl::EGLSurface)1611191354;

  EXPECT_CALL(EglProxy, CreateSurface(MockDisplay, MockConfig, &NativeWindow,
    nullptr))
    .Times(1)
    .WillOnce(Return(Surface));

  const Tested_t Example(Display, Config, &NativeWindow);

  EXPECT_CALL(EglProxy, QuerySurface(MockDisplay, Surface, ::covellite::egl::EGL_HEIGHT))
    .Times(1)
    .WillOnce(Return(1611191355));

  const auto Result = Example.GetHeight();
  EXPECT_EQ(Value, Result);
}

// ************************************************************************** //
TEST_F(Surface_test, /*DISABLED_*/Test_GetHeight_Fail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191356;
  const auto MockConfig = (::covellite::egl::EGLConfig)1611191357;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);
  SetConfig(Config, MockConfig);
  ANativeWindow NativeWindow;

  const auto Surface = (::covellite::egl::EGLSurface)1611191358;

  EXPECT_CALL(EglProxy, CreateSurface(MockDisplay, MockConfig, &NativeWindow,
    nullptr))
    .Times(1)
    .WillOnce(Return(Surface));

  const Tested_t Example(Display, Config, &NativeWindow);

  EXPECT_CALL(EglProxy, QuerySurface(MockDisplay, Surface, ::covellite::egl::EGL_HEIGHT))
    .Times(1)
    .WillOnce(Return(0));

  EXPECT_THROW(Example.GetHeight(), ::std::exception);
}

// ************************************************************************** //
TEST_F(Surface_test, /*DISABLED_*/Test_SwapBuffers)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191955;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);

  const auto Surface = (::covellite::egl::EGLSurface)1611191956;

  EXPECT_CALL(EglProxy, CreateSurface(MockDisplay, _, _, nullptr))
    .Times(1)
    .WillOnce(Return(Surface));

  const Tested_t Example(Display, Config, nullptr);

  EXPECT_CALL(EglProxy, SwapBuffers(MockDisplay, Surface))
    .Times(1)
    .WillOnce(Return(1611191957));

  Example.SwapBuffers();
}

// ************************************************************************** //
TEST_F(Surface_test, /*DISABLED_*/Test_SwapBuffers_Fail)
{
  using EglProxy_t = ::mock::EglProxy;
  EglProxy_t EglProxy;
  EglProxy_t::GetInstance() = &EglProxy;

  const auto MockDisplay = (::covellite::egl::EGLDisplay)1611191958;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(EglProxy, GetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
    .Times(1)
    .WillOnce(Return(MockDisplay));

  const ::mock::covellite::egl::Display Display;
  ::mock::covellite::egl::Config Config(Display, nullptr);

  const auto Surface = (::covellite::egl::EGLSurface)1611191959;

  EXPECT_CALL(EglProxy, CreateSurface(MockDisplay, _, _, nullptr))
    .Times(1)
    .WillOnce(Return(Surface));

  const Tested_t Example(Display, Config, nullptr);

  EXPECT_CALL(EglProxy, SwapBuffers(MockDisplay, Surface))
    .Times(1)
    .WillOnce(Return(0));

  EXPECT_THROW(Example.SwapBuffers(), ::std::exception);
}
