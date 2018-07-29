
#include "stdafx.h"
#include <alicorn/patterns/factory.hpp>

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ OpenGL.
*/

#include <Covellite\Os.mock.hpp>
#include <Covellite\Os\Events.hpp>
#include <Covellite\App\Events.hpp>
#include <Covellite\Api\Events.hpp>

#define OpenGLCommon OpenGLCommon_windows

// ������������ ������ OpenGL
#include "..\..\Covellite.Api\OpenGL.cpp"
#include "..\..\Covellite.Api\OpenGLCommon.cpp"

// ����� �������� ����� ������ OpenGL
class OpenGL_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::OpenGL;
  using ITestedApi_t = ::covellite::api::IWindow;
  using ITestedCore_t = ::covellite::core::IWindow;
  using WindowOs_t = ::mock::covellite::os::Window;
  using String_t = ::alicorn::extension::std::String;
  using Events_t = ::covellite::events::Events;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
    ::testing::DefaultValue<int>::Set(1710282125);
    ::testing::DefaultValue<HDC>::Set((HDC)1806101721);
    ::testing::DefaultValue<HGLRC>::Set((HGLRC)1710282204);
    ::testing::DefaultValue<RECT>::Set({ 0, 0, 1, 1 });
    ::testing::DefaultValue<String_t>::Set(uT("0"));
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
    ::testing::DefaultValue<int>::Clear();
    ::testing::DefaultValue<HGLRC>::Clear();
    ::testing::DefaultValue<HDC>::Clear();
    ::testing::DefaultValue<RECT>::Clear();
    ::testing::DefaultValue<String_t>::Clear();
  }
};

// ������� ������� ��� ����������� � ����� OpenGL 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(OpenGL_test, Test_Function);

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_EmptyFormatDescriptor)
{
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ����� !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
  // ��� ������� ������ ����������� �� �������� ������� �������!
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ����� !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //

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
TEST_F(OpenGL_test, /*DISABLED_*/Test_RegisterIntoFactory)
{
  using namespace ::alicorn::modules::patterns;

  const WindowOs_t WindowOs;
  const ::covellite::os::IWindow & IWindowOs = WindowOs;
  auto pExample = factory::make_unique<ITestedApi_t>(uT("OpenGL"), IWindowOs);
  EXPECT_NO_THROW(dynamic_cast<Tested_t &>(*pExample));
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

    const WindowOs_t WindowOs;

    EXPECT_CALL(WindowsProxy, GetDC(hWnd))
      .Times(1)
      .WillOnce(Return(hDC));

    const Tested_t Example{ WindowOs };

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

  const WindowOs_t WindowOs;
  EXPECT_THROW(Tested_t{ WindowOs }, ::std::exception);
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
  EXPECT_EQ(8, PixelFormatDescriptor.cAlphaBits);
  EXPECT_EQ(32, PixelFormatDescriptor.cDepthBits);
  EXPECT_EQ(PFD_MAIN_PLANE, PixelFormatDescriptor.iLayerType);

  const HDC hDC = (HDC)1710282115;
  const int PixelFormat = 1710282129;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .WillRepeatedly(Return(hDC));

  EXPECT_CALL(WindowsProxy, ChoosePixelFormat(hDC, &PixelFormatDescriptor))
    .WillOnce(Return(PixelFormat))
    .WillOnce(Return(0));

  const WindowOs_t WindowOs;
  Tested_t Example{ WindowOs };
  EXPECT_THROW(Tested_t{ WindowOs }, ::std::exception);
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

  const WindowOs_t WindowOs;
  Tested_t Example{ WindowOs };
  EXPECT_THROW(Tested_t{ WindowOs }, ::std::exception);
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

    const WindowOs_t WindowOs;
    Tested_t Example{ WindowOs };

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

  const WindowOs_t WindowOs;
  EXPECT_THROW(Tested_t{ WindowOs }, ::std::exception);
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

  const WindowOs_t WindowOs;
  EXPECT_THROW(Tested_t{ WindowOs }, ::std::exception);
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

  const WindowOs_t WindowOs;
  Tested_t Example{ WindowOs };
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_GetEvents)
{
  class Proxy
  {
  public:
    MOCK_METHOD0(DoEvent, void(void));
  };

  Proxy oProxy;
  static const int Id = 1807221121;

  const WindowOs_t WindowOs;
  Events_t SourceEvents = WindowOs;
  SourceEvents[Id].Connect([&]() { oProxy.DoEvent(); });
  SourceEvents[::covellite::events::Error.Exception].Connect([]() {});

  const Tested_t Example{ WindowOs };

  using namespace ::testing;

  EXPECT_CALL(oProxy, DoEvent())
    .Times(1);

  Events_t ExampleEvents = Example;
  ExampleEvents[Id]();
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_WindowResize)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  class Tested :
    public Tested_t
  {
  public:
    MOCK_CONST_METHOD0(GetClientRect, Rect(void));

  public:
    explicit Tested(const WindowOs_t & _Window) :
      Tested_t(_Window)
    {

    }
  };

  const int Left = 10291057;
  const int Top = 10291058;
  const int Width = 1710291059;
  const int Height = 1710291100;
  const Tested::Rect Rect = { Left, Top, Width, Height };

  const WindowOs_t WindowOs;
  const Tested Example{ WindowOs };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, GetClientRect())
    .Times(1)
    .WillOnce(Return(Rect));

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

  Events_t Events = Example;
  Events[::covellite::events::Window.Resize]();
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_ApplicationUpdate)
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

  class Tested :
    public Tested_t
  {
  public:
    MOCK_CONST_METHOD0(DoDrawing, void(void));
    MOCK_CONST_METHOD0(DoException, void(void));

  public:
    explicit Tested(const WindowOs_t & _Window) :
      Tested_t(_Window)
    {
    }
  };

  const auto hDC = (HDC)1710291106;
  const ::mock::Id_t WindowSectionId = 1711061132;
  const ::mock::Id_t BackgroundColorSectionId = 1711061133;

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
    .WillOnce(Return(hDC));

  const WindowOs_t WindowOs;
  const Tested Example{ WindowOs };
  Events_t Events = Example;

  using namespace ::covellite::events;

  Events[Drawing.Do].Connect([&]() { Example.DoDrawing(); });
  Events[Error.Exception].Connect([&]() { Example.DoException(); });

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Disable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, ClearColor(0.1f, 0.2f, 0.3f, 0.4f))
    .Times(1);

  EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
    .Times(1);

  EXPECT_CALL(Example, DoDrawing())
    .Times(1);

  EXPECT_CALL(WindowsProxy, SwapBuffers(hDC))
    .Times(1)
    .WillOnce(Return(TRUE));

  EXPECT_CALL(Example, DoException())
    .Times(0);

  Events[::covellite::events::Application.Update]();
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_ApplicationUpdate_SwapBuffersFail)
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

  class Tested :
    public Tested_t
  {
  public:
    MOCK_CONST_METHOD0(DoDrawing, void(void));
    MOCK_CONST_METHOD0(DoException, void(void));

  public:
    explicit Tested(const WindowOs_t & _Window) :
      Tested_t(_Window)
    {
    }
  };

  const auto hDC = (HDC)1710291106;
  const ::mock::Id_t WindowSectionId = 1711061132;
  const ::mock::Id_t BackgroundColorSectionId = 1711061133;

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
    .WillOnce(Return(hDC));

  const WindowOs_t WindowOs;
  const Tested Example{ WindowOs };
  Events_t Events = Example;

  using namespace ::covellite::events;

  Events[Drawing.Do].Connect([&]() { Example.DoDrawing(); });
  Events[Error.Exception].Connect([&]() { Example.DoException(); });

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Disable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, ClearColor(0.1f, 0.2f, 0.3f, 0.4f))
    .Times(1);

  EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
    .Times(1);

  EXPECT_CALL(Example, DoDrawing())
    .Times(1);

  EXPECT_CALL(WindowsProxy, SwapBuffers(hDC))
    .Times(1)
    .WillOnce(Return(FALSE));

  EXPECT_CALL(Example, DoException())
    .Times(1);

  Events[::covellite::events::Application.Update]();
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

  const WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };
  const ITestedApi_t & IExample = Example;

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

  const WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };
  const ITestedApi_t & IExample = Example;

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

  const WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };
  const ITestedApi_t & IExample = Example;

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
TEST_F(OpenGL_test, /*DISABLED_*/Test_GetClientRect)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const HWND hWnd = (HWND)1807211305;
  const int Left = 07211310;
  const int Width = 1807211311;
  const int Top = 07211307;
  const int Height = 1807211308;
  const RECT ClientRect = { Left, Top, Left + Width, Top + Height };

  using namespace ::testing;

  EXPECT_CALL(WindowOsProxy, GetHandle(_))
    .Times(1)
    .WillOnce(Return(hWnd));

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return((HDC)1807211309));

  const WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };
  const ITestedApi_t & IExample = Example;

  EXPECT_CALL(WindowsProxy, BuildClientRect())
    .WillRepeatedly(Return(ClientRect));

  EXPECT_CALL(WindowsProxy, GetClientRect(hWnd))
    .Times(2)
    .WillOnce(Return(FALSE))
    .WillOnce(Return(TRUE));

  EXPECT_THROW(IExample.GetClientRect(), ::std::exception);
  const auto Result = IExample.GetClientRect();
  EXPECT_EQ(0, Result.Left);
  EXPECT_EQ(0, Result.Top);
  EXPECT_EQ(Width, Result.Width);
  EXPECT_EQ(Height, Result.Height);
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

  class Tested :
    public Tested_t
  {
  public:
    MOCK_CONST_METHOD0(GetClientRect, Rect (void));

  public:
    explicit Tested(const WindowOs_t & _Window) :
      Tested_t(_Window)
    {

    }
  };

  const ::mock::Id_t RenderOpenGLId = 1710291052;
  const auto Top = 1807221057;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return((HDC)1710291054));

  const WindowOs_t WindowOs;
  const Tested Example{ WindowOs };
  const ITestedApi_t & IExample = Example;

  EXPECT_CALL(Example, GetClientRect())
    .Times(1)
    .WillOnce(Return(Tested::Rect{ 0, Top, 0, 0 }));

  EXPECT_CALL(RenderOpenGLProxy, Constructor(Top))
    .Times(1)
    .WillOnce(Return(RenderOpenGLId));

  const auto pResult = IExample.MakeRenderInterface();

  const auto & RenderOpenGL =
    dynamic_cast<const ::mock::covellite::api::RenderOpenGL &>(*pResult);
  EXPECT_EQ(RenderOpenGLId, RenderOpenGL.m_Id);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_DoResize_Deprecated)
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

  const WindowOs_t WindowOs;
  Tested_t Example{ WindowOs };
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
TEST_F(OpenGL_test, /*DISABLED_*/Test_DoStartDrawing_Deprecated)
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

  const WindowOs_t WindowOs;
  Tested_t Example{ WindowOs };
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
TEST_F(OpenGL_test, /*DISABLED_*/Test_DoFinishDrawing_Deprecated)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const auto hDC = (HDC)1710291106;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return(hDC));

  const WindowOs_t WindowOs;
  Tested_t Example{ WindowOs };
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
