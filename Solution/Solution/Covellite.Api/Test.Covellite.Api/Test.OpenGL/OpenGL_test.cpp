
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса OpenGL.
*/

#define OpenGLCommon OpenGLCommon_windows

// Расположение класса OpenGL
#include "../../Covellite.Api/Renderer/OpenGL.cpp"
#include "../../Covellite.Api/Renderer/OpenGLCommon.cpp"

// Общий тестовый класс класса OpenGL
class OpenGL_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::OpenGL;
  using ITested_t = ::covellite::api::renderer::IGraphicApi;
  using String_t = ::alicorn::extension::std::String;
  using Component_t = ::covellite::api::Component;
  using Render_t = ::std::function<void(void)>;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    static const ::mock::GLfloat Viewport[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    ::testing::DefaultValue<const ::mock::GLfloat *>::Set(Viewport);

    ::testing::DefaultValue<int>::Set(1710282125);
    ::testing::DefaultValue<HDC>::Set((HDC)1806101721);
    ::testing::DefaultValue<HGLRC>::Set((HGLRC)1710282204);
    ::testing::DefaultValue<RECT>::Set({ 0, 0, 1, 1 });
    ::testing::DefaultValue<String_t>::Set(uT("0"));
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<int>::Clear();
    ::testing::DefaultValue<HGLRC>::Clear();
    ::testing::DefaultValue<HDC>::Clear();
    ::testing::DefaultValue<RECT>::Clear();
    ::testing::DefaultValue<String_t>::Clear();
    ::testing::DefaultValue<const ::mock::GLfloat *>::Clear();
  }

protected:
  const String_t m_UsingApi = uT("OpenGL");

protected:
  static const int m_Top = 0;

  class Data :
    public ::covellite::api::renderer::Renderer::Data
  {
  public:
    Data(void)
    {
      memset(this, 0, sizeof(Data));
      Top = m_Top;
    }
  };

  using Data_t = Data;
};

// Образец макроса для подстановки в класс OpenGL 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(OpenGL_test, Test_Function);

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_EmptyFormatDescriptor)
{
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ВАЖНО !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //
  // Эта функция должна выполняться до создания первого объекта!
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ВАЖНО !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! //

  using ::covellite::api::renderer::PixelFormatDescriptor;

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

  Data_t Data;
  Data.Handle = (HWND)1710281231;

  const HDC hDC = (HDC)1710281243;

  using namespace ::testing;

  {
    EXPECT_CALL(WindowsProxy, GetDC(Data.Handle))
      .Times(1)
      .WillOnce(Return(hDC));

    const Tested_t Example{ Data };

    EXPECT_CALL(WindowsProxy, ReleaseDC(Data.Handle, hDC))
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

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_ChoosePixelFormat)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using ::covellite::api::renderer::PixelFormatDescriptor;

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

  Tested_t Example{ Data_t{} };
  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_SetPixelFormat)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using ::covellite::api::renderer::PixelFormatDescriptor;

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

  Tested_t Example{ Data_t{} };
  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
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

    Tested_t Example{ Data_t{} };

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

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
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

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_Constructor_UpdateScreen)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Data_t Data;
  Data.Handle = (HWND)1710291025;
  const int Left = 10291020;
  const int Top = 10291021;
  const int Width = 1710291013;
  const int Height = 1710291014;
  const RECT ClientRect = { Left, Top, Left + Width, Top + Height };

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, BuildRect())
    .WillRepeatedly(Return(ClientRect));

  EXPECT_CALL(WindowsProxy, GetClientRect(Data.Handle))
    .WillRepeatedly(Return(TRUE));

  InSequence Dummy;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return((HDC)1710290957));

  EXPECT_CALL(WindowsProxy, ChoosePixelFormat(_, _))
    .Times(1);

  EXPECT_CALL(WindowsProxy, wglCreateContext(_))
    .Times(1);

  EXPECT_CALL(GLProxy, Viewport(0, 0, Width, Height))
    .Times(1);

  Tested_t Example{ Data };
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

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  EXPECT_CALL(GLProxy, GetString(GL_VERSION))
    .Times(1)
    .WillOnce(Return(reinterpret_cast<const unsigned char *>(Version)));

  const auto Result = IExample.GetUsingApi();
  EXPECT_EQ(Expected, Result);
}

// ************************************************************************** //
TEST_F(OpenGL_test, /*DISABLED_*/Test_PresentFrame)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const HDC hDC = (HDC)1808261819;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetDC(_))
    .Times(1)
    .WillOnce(Return(hDC));

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  EXPECT_CALL(WindowsProxy, SwapBuffers(hDC))
    .WillOnce(Return(TRUE))
    .WillOnce(Return(FALSE));

  IExample.PresentFrame();
  EXPECT_THROW(IExample.PresentFrame(), ::std::exception);
}

#define OpenGLCommon_test OpenGL_test

#include "../OpenGLCommon_test.hpp"
