
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса OpenGL.
*/

#include <Covellite\Os.mock.hpp>

#define OpenGLCommon OpenGLCommon_windows

// Расположение класса OpenGL
#include "..\..\Covellite.Api\OpenGL.cpp"
#include "..\..\Covellite.Api\OpenGLCommon.cpp"

// Общий тестовый класс класса OpenGL
class OpenGL_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::OpenGL;
  using ITestedApi_t = ::covellite::api::IWindow;
  using ITestedCore_t = ::covellite::core::IWindow;
  using WindowOs_t = ::mock::covellite::os::Window;
  using String_t = ::alicorn::extension::std::String;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<int>::Set(1710282125);
    ::testing::DefaultValue<HGLRC>::Set((HGLRC)1710282204);
    ::testing::DefaultValue<RECT>::Set({ 0, 0, 1, 1 });
    ::testing::DefaultValue<String_t>::Set(uT("0"));
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<int>::Clear();
    ::testing::DefaultValue<HGLRC>::Clear();
    ::testing::DefaultValue<RECT>::Clear();
    ::testing::DefaultValue<String_t>::Clear();
  }
};

// Образец макроса для подстановки в класс OpenGL 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(OpenGL_test, Test_Function);

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_EmptyFormatDescriptor)
{
  EXPECT_EQ(0, PixelFormatDescriptor.nSize);
  EXPECT_EQ(0, PixelFormatDescriptor.nVersion);
  EXPECT_EQ(0, PixelFormatDescriptor.dwFlags);
  EXPECT_EQ(0, PixelFormatDescriptor.iPixelType);
  EXPECT_EQ(0, PixelFormatDescriptor.cColorBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cRedBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cRedShift);
  EXPECT_EQ(0, PixelFormatDescriptor.cGreenBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cGreenShift);
  EXPECT_EQ(0, PixelFormatDescriptor.cBlueBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cBlueShift);
  EXPECT_EQ(0, PixelFormatDescriptor.cAlphaBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cAlphaShift);
  EXPECT_EQ(0, PixelFormatDescriptor.cAccumBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cAccumRedBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cAccumGreenBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cAccumBlueBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cAccumAlphaBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cDepthBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cStencilBits);
  EXPECT_EQ(0, PixelFormatDescriptor.cAuxBuffers);
  EXPECT_EQ(0, PixelFormatDescriptor.iLayerType);
  EXPECT_EQ(0, PixelFormatDescriptor.bReserved);
  EXPECT_EQ(0, PixelFormatDescriptor.dwLayerMask);
  EXPECT_EQ(0, PixelFormatDescriptor.dwVisibleMask);
  EXPECT_EQ(0, PixelFormatDescriptor.dwDamageMask);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_DeviceContex)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  const ::mock::Id_t WindowOsId = 1710281230;
  const HWND hWnd = (HWND)1710281231;
  const HDC hDC = (HDC)1710281243;

  using namespace ::testing;

  {
    EXPECT_CALL(WindowOsProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowOsId));

    EXPECT_CALL(WindowOsProxy, GetHandle(WindowOsId))
      .Times(1)
      .WillOnce(Return(hWnd));

    EXPECT_CALL(WindowsProxy, GetDC(hWnd))
      .Times(1)
      .WillOnce(Return(hDC));

    Tested_t Example(::std::make_shared<WindowOs_t>());

    EXPECT_CALL(WindowsProxy, ReleaseDC(hWnd, hDC))
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_GetDeviceContex_Error)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return((HDC)NULL));

  EXPECT_THROW(Tested_t{ ::std::make_shared<WindowOs_t>() }, ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_ChoosePixelFormat)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  EXPECT_EQ(sizeof(PIXELFORMATDESCRIPTOR), PixelFormatDescriptor.nSize);
  EXPECT_EQ(1, PixelFormatDescriptor.nVersion);
  EXPECT_EQ(PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 
    PixelFormatDescriptor.dwFlags);
  EXPECT_EQ(PFD_TYPE_RGBA, PixelFormatDescriptor.iPixelType);
  EXPECT_EQ(32, PixelFormatDescriptor.cColorBits);
  EXPECT_EQ(8, PixelFormatDescriptor.cRedBits);
  EXPECT_EQ(8, PixelFormatDescriptor.cGreenBits);
  EXPECT_EQ(8, PixelFormatDescriptor.cBlueBits);
  EXPECT_EQ(8, PixelFormatDescriptor.cAlphaBits);
  EXPECT_EQ(24, PixelFormatDescriptor.cDepthBits);
  EXPECT_EQ(8, PixelFormatDescriptor.cStencilBits);

  const HDC hDC = (HDC)1710282115;
  const int PixelFormat = 1710282129;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .WillRepeatedly(Return(hDC));

  EXPECT_CALL(WindowsProxy, ChoosePixelFormat(hDC, &PixelFormatDescriptor))
    .WillOnce(Return(PixelFormat))
    .WillOnce(Return(0));

  Tested_t Example(::std::make_shared<WindowOs_t>());
  EXPECT_THROW(Tested_t{ ::std::make_shared<WindowOs_t>() }, ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_SetPixelFormat)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const HDC hDC = (HDC)1710282138;
  const int PixelFormat = 1710282139;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .WillRepeatedly(Return(hDC));

  EXPECT_CALL(WindowsProxy, ChoosePixelFormat(_, _))
    .WillOnce(Return(PixelFormat))
    .WillOnce(Return(PixelFormat));

  EXPECT_CALL(WindowsProxy, SetPixelFormat(hDC, PixelFormat, &PixelFormatDescriptor))
    .WillOnce(Return(TRUE))
    .WillOnce(Return(FALSE));

  Tested_t Example(::std::make_shared<WindowOs_t>());
  EXPECT_THROW(Tested_t{ ::std::make_shared<WindowOs_t>() }, ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_RenderContext)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const HDC hDC = (HDC)1710282144;
  const HGLRC hRC = (HGLRC)1710282145;

  using namespace ::testing;

  {
    InSequence Dummy;

    EXPECT_CALL(WindowsProxy, GetDC(_))
      .Times(1)
      .WillOnce(Return(hDC));

    EXPECT_CALL(WindowsProxy, wglCreateContext(hDC))
      .Times(1)
      .WillOnce(Return(hRC));

    EXPECT_CALL(WindowsProxy, wglMakeCurrent(hDC, hRC))
      .Times(1)
      .WillOnce(Return(TRUE));

    Tested_t Example(::std::make_shared<WindowOs_t>());

    EXPECT_CALL(WindowsProxy, wglMakeCurrent(NULL, NULL))
      .Times(1);

    EXPECT_CALL(WindowsProxy, wglDeleteContext(hRC))
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_wglCreateContext_Error)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const HDC hDC = (HDC)1710282200;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return(hDC));

  EXPECT_CALL(WindowsProxy, wglCreateContext(_))
    .Times(1)
    .WillOnce(Return((HGLRC)NULL));

  EXPECT_THROW(Tested_t{ ::std::make_shared<WindowOs_t>() }, ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_wglMakeCurrent_Error)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const HDC hDC = (HDC)1710282144;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return(hDC));

  EXPECT_CALL(WindowsProxy, wglMakeCurrent(_, _))
    .Times(1)
    .WillOnce(Return(FALSE));

  EXPECT_THROW(Tested_t{ ::std::make_shared<WindowOs_t>() }, ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_Constructor_UpdateScreen)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const HWND hWnd = (HWND)1710291025;
  const int Left = 10291020;
  const int Top = 10291021;
  const int Width = 1710291013;
  const int Height = 1710291014;
  const RECT ClientRect = { Left, Top, Left + Width, Top + Height };
  const auto pWindowOs = ::std::make_shared<WindowOs_t>();

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, BuildClientRect())
    .WillRepeatedly(Return(ClientRect));

  EXPECT_CALL(WindowsProxy, GetClientRect(hWnd))
    .WillRepeatedly(Return(TRUE));

  InSequence Dummy;

  EXPECT_CALL(WindowOsProxy, GetHandle(_))
    .Times(1)
    .WillOnce(Return(hWnd));

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return((HDC)1710290957));

  EXPECT_CALL(WindowsProxy, ChoosePixelFormat(_, _))
    .Times(1);

  EXPECT_CALL(WindowsProxy, wglCreateContext(_))
    .Times(1);

  EXPECT_CALL(GLProxy, Viewport(0, 0, Width, Height))
    .Times(1);

  EXPECT_CALL(GLProxy, MatrixMode(GL_PROJECTION))
    .Times(1);

  EXPECT_CALL(GLProxy, LoadIdentity())
    .Times(1);

  EXPECT_CALL(GLProxy, MatrixMode(GL_MODELVIEW))
    .Times(1);

  EXPECT_CALL(GLProxy, LoadIdentity())
    .Times(1);

  EXPECT_CALL(GLProxy, Ortho(0, Width, Height, 0, -1, 1))
    .Times(1);

  Tested_t Example(pWindowOs);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_GetUsingApi)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  using namespace ::alicorn::extension::std;

  const char * Version = "Version1710311100";
  const auto Expected = uT("OpenGL %VERSION%").Replace(uT("%VERSION%"), 
    string_cast<String, Locale::Default>(::std::string{ Version }));

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return((HDC)1710311052));

  Tested_t Example{ ::std::make_shared<WindowOs_t>() };
  ITestedApi_t & IExample = Example;

  EXPECT_CALL(GLProxy, GetString(GL_VERSION))
    .Times(1)
    .WillOnce(Return(reinterpret_cast<const unsigned char *>(Version)));

  const auto Result = IExample.GetUsingApi();
  EXPECT_EQ(Expected, Result);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_GetWidth)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const HWND hWnd = (HWND)1710291042;
  const int Left = 10291043;
  const int Width = 1710291045;
  const RECT ClientRect = { Left, 0, Left + Width, 0 };

  using namespace ::testing;

  EXPECT_CALL(WindowOsProxy, GetHandle(_))
    .Times(1)
    .WillOnce(Return(hWnd));

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return((HDC)1710291047));

  Tested_t Example{ ::std::make_shared<WindowOs_t>() };
  ITestedApi_t & IExample = Example;

  EXPECT_CALL(WindowsProxy, BuildClientRect())
    .WillRepeatedly(Return(ClientRect));

  EXPECT_CALL(WindowsProxy, GetClientRect(hWnd))
    .Times(2)
    .WillOnce(Return(FALSE))
    .WillOnce(Return(TRUE));

  EXPECT_THROW(IExample.GetWidth(), ::std::exception);
  const auto Result = IExample.GetWidth();
  EXPECT_EQ(Width, Result);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_GetHeight)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const HWND hWnd = (HWND)1710291049;
  const int Top = 10291043;
  const int Height = 1710291045;
  const RECT ClientRect = { 0, Top, 0, Top + Height };

  using namespace ::testing;

  EXPECT_CALL(WindowOsProxy, GetHandle(_))
    .Times(1)
    .WillOnce(Return(hWnd));

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return((HDC)1710291047));

  Tested_t Example{ ::std::make_shared<WindowOs_t>() };
  ITestedApi_t & IExample = Example;

  EXPECT_CALL(WindowsProxy, BuildClientRect())
    .WillRepeatedly(Return(ClientRect));

  EXPECT_CALL(WindowsProxy, GetClientRect(hWnd))
    .Times(2)
    .WillOnce(Return(FALSE))
    .WillOnce(Return(TRUE));

  EXPECT_THROW(IExample.GetHeight(), ::std::exception);
  const auto Result = IExample.GetHeight();
  EXPECT_EQ(Height, Result);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_MakeRenderInterface)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using RenderOpenGLProxy_t = ::mock::covellite::api::RenderOpenGL::Proxy;
  RenderOpenGLProxy_t RenderOpenGLProxy;
  RenderOpenGLProxy_t::GetInstance() = &RenderOpenGLProxy;

  const ::mock::Id_t RenderOpenGLId = 1710291052;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return((HDC)1710291054));

  const Tested_t Example{ ::std::make_shared<WindowOs_t>() };
  const ITestedApi_t & IExample = Example;

  EXPECT_CALL(RenderOpenGLProxy, Constructor(0))
    .Times(1)
    .WillOnce(Return(RenderOpenGLId));

  const auto pResult = IExample.MakeRenderInterface();

  const auto & RenderOpenGL =
    dynamic_cast<const ::mock::covellite::api::RenderOpenGL &>(*pResult);
  EXPECT_EQ(RenderOpenGLId, RenderOpenGL.m_Id);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_DoResize)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const int Left = 10291057;
  const int Top = 10291058;
  const int Width = 1710291059;
  const int Height = 1710291100;
  const RECT ClientRect = { Left, Top, Left + Width, Top + Height };

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, BuildClientRect())
    .WillRepeatedly(Return(ClientRect));

  EXPECT_CALL(WindowsProxy, GetClientRect(_))
    .WillRepeatedly(Return(TRUE));

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return((HDC)1710291054));

  Tested_t Example{ ::std::make_shared<WindowOs_t>() };
  ITestedCore_t & IExample = Example;

  IExample.Subscribe(pEventHandler);

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Viewport(0, 0, Width, Height))
    .Times(1);

  EXPECT_CALL(GLProxy, MatrixMode(GL_PROJECTION))
    .Times(1);

  EXPECT_CALL(GLProxy, LoadIdentity())
    .Times(1);

  EXPECT_CALL(GLProxy, MatrixMode(GL_MODELVIEW))
    .Times(1);

  EXPECT_CALL(GLProxy, LoadIdentity())
    .Times(1);

  EXPECT_CALL(GLProxy, Ortho(0, Width, Height, 0, -1, 1))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::Resize, params::Empty{});
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_DoStartDrawing)
{
  using SectionImplProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SectionImplProxy_t SectionImplProxy;
  SectionImplProxy_t::GetInstance() = &SectionImplProxy;

  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::mock::Id_t WindowSectionId = 1711061132;
  const ::mock::Id_t BackgroundColorSectionId = 1711061133;

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

  using namespace ::testing;

  {
    InSequence Dummy;

    EXPECT_CALL(SectionImplProxy, GetChildSectionImpl(_, uT("Window")))
      .Times(1);

    EXPECT_CALL(SectionImplProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowSectionId));

    EXPECT_CALL(SectionImplProxy,
      GetChildSectionImpl(WindowSectionId, uT("BackgroundColor")))
      .Times(1);

    EXPECT_CALL(SectionImplProxy, Constructor())
      .Times(1)
      .WillOnce(Return(BackgroundColorSectionId));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("R")))
      .Times(1)
      .WillOnce(Return(uT("25.5")));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("G")))
      .Times(1)
      .WillOnce(Return(uT("51")));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("B")))
      .Times(1)
      .WillOnce(Return(uT("76.5")));

    EXPECT_CALL(SectionImplProxy, GetValue(BackgroundColorSectionId, uT("A")))
      .Times(1)
      .WillOnce(Return(uT("102")));
  }

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return((HDC)1710291103));

  Tested_t Example{ ::std::make_shared<WindowOs_t>() };
  ITestedCore_t & IExample = Example;

  IExample.Subscribe(pEventHandler);

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Disable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, ClearColor(0.1f, 0.2f, 0.3f, 0.4f))
    .Times(1);

  EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::StartDrawing, params::Empty{});
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_DoFinishDrawing)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const auto hDC = (HDC)1710291106;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return(hDC));

  Tested_t Example{ ::std::make_shared<WindowOs_t>() };
  ITestedCore_t & IExample = Example;

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

  IExample.Subscribe(pEventHandler);

  EXPECT_CALL(WindowsProxy, SwapBuffers(hDC))
    .WillOnce(Return(FALSE))
    .WillOnce(Return(TRUE));

  using namespace ::covellite::core;

  auto DoFinishDrawing = [&]()
    { pEventHandler->DoCommand(Event::FinishDrawing, params::Empty{}); };

  EXPECT_THROW(DoFinishDrawing(), ::std::exception);
  DoFinishDrawing();
}
