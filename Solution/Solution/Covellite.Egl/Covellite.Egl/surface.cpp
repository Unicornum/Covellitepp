
#include "stdafx.h"
#include "Surface.hpp"
#include "Display.hpp"
#include "Config.hpp"
#include "CallProxy.hpp"

namespace covellite
{

namespace egl
{

Surface::Surface(const covellite::egl::Display & _Display,
  const covellite::egl::Config & _Config, ANativeWindow * _pNativeWindow) :
  m_Display(_Display.m_Display),
  m_Surface(eglCreateWindowSurface(m_Display, _Config.m_Config, _pNativeWindow,
    nullptr))
{
  if (m_Surface == EGL_NO_SURFACE) EGL_CALL 0;
}

Surface::~Surface(void) noexcept
{
  eglDestroySurface(m_Display, m_Surface);
}

void Surface::SwapBuffers(void) const
{
  EGL_CALL eglSwapBuffers(m_Display, m_Surface);
}

int32_t Surface::GetWidth(void) const
{
  return GetValue(EGL_WIDTH);
}

int32_t Surface::GetHeight(void) const
{
  return GetValue(EGL_HEIGHT);
}

int32_t Surface::GetValue(int32_t _Attribute) const
{
  EGLint Value;
  EGL_CALL eglQuerySurface(m_Display, m_Surface, _Attribute, &Value);
  return Value;
}

} // namespace egl

} // namespace covellite
