
#include "stdafx.h"
#include "Config.hpp"
#include <alicorn\std\exception.hpp>
#include "Display.hpp"
#include "CallProxy.hpp"

namespace covellite
{

namespace egl
{

Config::Config(const covellite::egl::Display & _Display, 
  ANativeWindow * _pNativeWindow)
{
  if (_pNativeWindow == nullptr)
  {
    throw STD_EXCEPTION << "Nullptr native window pointer.";
  }

  const EGLint Attributes[] =
  {
    //EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT, // Ќужно дл€ ES2???
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    //EGL_ALPHA_SIZE,   8, // ѕриводит к проблемам с отрисовкой при переходе 
                           // к другому документу.
    EGL_BLUE_SIZE,    8,
    EGL_GREEN_SIZE,   8,
    EGL_RED_SIZE,     8,
    EGL_DEPTH_SIZE,   24,
    EGL_NONE
  };

  EGLint ConfigCount = 0;
  EGL_CALL eglChooseConfig(_Display.m_Display, Attributes,
    &m_Config, 1, &ConfigCount);

  EGLint Format;
  EGL_CALL eglGetConfigAttrib(_Display.m_Display, m_Config,
    EGL_NATIVE_VISUAL_ID, &Format);

  ANativeWindow_setBuffersGeometry(_pNativeWindow, 0, 0, Format);
}

} // namespace egl

} // namespace covellite
