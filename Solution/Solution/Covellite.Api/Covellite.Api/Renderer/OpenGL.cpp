
#include "stdafx.h"
#include "OpenGL.hpp"
#include <alicorn\platform\winapi-check.hpp>

namespace covellite
{

namespace api
{

namespace renderer
{

static PIXELFORMATDESCRIPTOR PixelFormatDescriptor = {0};

OpenGL::OpenGL(const Renderer::Data & _Data) :
  OpenGLCommon(_Data, uT("OpenGL ")),
  m_hWnd(_Data.Handle),
  m_hDeviceContex(USING_MOCK ::GetDC(m_hWnd))
{
  WINAPI_CHECK (m_hDeviceContex != NULL);

  PixelFormatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  PixelFormatDescriptor.nVersion = 1;
  PixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  PixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
  PixelFormatDescriptor.cColorBits = 32;
  PixelFormatDescriptor.cAlphaBits = 8;
  PixelFormatDescriptor.cDepthBits = 32;
  PixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;

  // ������������� �������������� ��� ������� ���� (���������� �� ��������,
  // ��� ������ WINAPI_CHECK �������� ���������� ��� ������� �������� 
  // m_hDeviceContex � ����������� ��� �� ����� ���� ��������).
# pragma warning(push)
# pragma warning(disable: 6387)

  const auto PixelFormat = USING_MOCK ::ChoosePixelFormat(m_hDeviceContex, 
    &PixelFormatDescriptor);
  WINAPI_CHECK (PixelFormat != 0);

  WINAPI_CHECK USING_MOCK ::SetPixelFormat(m_hDeviceContex, PixelFormat, 
    &PixelFormatDescriptor);

# pragma warning(pop)
//
  m_hRenderContex = USING_MOCK ::wglCreateContext(m_hDeviceContex);
  WINAPI_CHECK (m_hRenderContex != NULL);

  WINAPI_CHECK USING_MOCK ::wglMakeCurrent(m_hDeviceContex, m_hRenderContex);

  RECT ClientRect = { 0 };
  WINAPI_CHECK USING_MOCK ::GetClientRect(m_hWnd, &ClientRect);

  ResizeWindow(ClientRect.right - ClientRect.left,
    ClientRect.bottom - ClientRect.top);
}

OpenGL::~OpenGL(void)
{
  USING_MOCK ::wglMakeCurrent(NULL, NULL);
  USING_MOCK ::wglDeleteContext(m_hRenderContex);

  USING_MOCK ::ReleaseDC(m_hWnd, m_hDeviceContex);
}

void OpenGL::PresentFrame(void) /*override*/
{
  WINAPI_CHECK USING_MOCK ::SwapBuffers(m_hDeviceContex);
}

} // namespace renderer

} // namespace api

} // namespace covellite