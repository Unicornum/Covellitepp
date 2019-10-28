
#include "stdafx.h"
#include "OpenGL.hpp"
#include <alicorn/platform/winapi-check.hpp>
//#include <alicorn/logger.hpp>

#pragma comment(lib, "opengl32.lib")

namespace covellite
{

namespace api
{

namespace renderer
{

//static void GLDebugCallback(GLenum /*source*/, GLenum type, GLuint id,
//  GLenum severity, GLsizei /*length*/, const GLchar *message, const void * /*userParam*/)
//{
//  const auto strType = 
//    (type == GL_DEBUG_TYPE_ERROR) ? "ERROR" :
//    (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) ? "DEPRECATED BEHAVIOR" :
//    (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR) ? "UDEFINED BEHAVIOR" :
//    (type == GL_DEBUG_TYPE_PORTABILITY) ? "PORTABILITY" :
//    (type == GL_DEBUG_TYPE_PERFORMANCE) ? "PERFORMANCE" :
//    (type == GL_DEBUG_TYPE_OTHER) ? "OTHER" :
//    (type == GL_DEBUG_TYPE_MARKER) ? "MARKER" : "UNKNOWN";
//
//  if (severity == GL_DEBUG_SEVERITY_HIGH)
//  {
//    LOGGER(Error) << strType << ": " << message << " [" << id << "]";
//  }
//  else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
//  {
//    LOGGER(Warning) << strType << ": " << message << " [" << id << "]";
//  }
//  else if (severity == GL_DEBUG_SEVERITY_LOW)
//  {
//    LOGGER(Info) << strType << ": " << message << " [" << id << "]";
//  }
//  else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
//  {
//    LOGGER(Trace) << strType << ": " << message << " [" << id << "]";
//  }
//  else
//  {
//    LOGGER(Trace) << strType << ": " << message << " [" << id << "]";
//  }
//}

static PIXELFORMATDESCRIPTOR PixelFormatDescriptor = { 0 };

OpenGL::OpenGL(const Data_t & _Data) :
  OpenGLCommonShader(_Data, uT("OpenGL "), 
    "#version 330 core\r\n"
    "#define COVELLITE_SHADER_DESKTOP\r\n"),
  m_hWnd(::covellite::any_cast<HWND>(_Data.Handle)),
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

  // Игнорирование предупреждений при анализе кода (анализатор не понимает,
  // что макрос WINAPI_CHECK вызывает исключение при нулевом значении 
  // m_hDeviceContex и последующий код не может быть выполнен).
# pragma warning(push)
# pragma warning(disable: 6387)

  const auto PixelFormat = USING_MOCK ::ChoosePixelFormat(m_hDeviceContex, 
    &PixelFormatDescriptor);
  WINAPI_CHECK (PixelFormat != 0);

  WINAPI_CHECK USING_MOCK ::SetPixelFormat(m_hDeviceContex, PixelFormat, 
    &PixelFormatDescriptor);

# pragma warning(pop)

  m_hRenderContex = USING_MOCK ::wglCreateContext(m_hDeviceContex);
  WINAPI_CHECK (m_hRenderContex != NULL);

  WINAPI_CHECK USING_MOCK ::wglMakeCurrent(m_hDeviceContex, m_hRenderContex);

  RECT ClientRect = { 0 };
  WINAPI_CHECK USING_MOCK ::GetClientRect(m_hWnd, &ClientRect);

  ResizeWindow(ClientRect.right - ClientRect.left,
    ClientRect.bottom - ClientRect.top);

  //glEnable(GL_DEBUG_OUTPUT);
  //glDebugMessageCallback(&GLDebugCallback, NULL);
  //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
}

OpenGL::~OpenGL(void) noexcept
{
  USING_MOCK ::wglMakeCurrent(NULL, NULL);
  USING_MOCK ::wglDeleteContext(m_hRenderContex);

  USING_MOCK ::ReleaseDC(m_hWnd, m_hDeviceContex);
}

void OpenGL::PresentFrame(void) /*override*/
{
  WINAPI_CHECK USING_MOCK ::SwapBuffers(m_hDeviceContex);

  OpenGLCommonShader::PresentFrame();
}

} // namespace renderer

} // namespace api

} // namespace covellite
