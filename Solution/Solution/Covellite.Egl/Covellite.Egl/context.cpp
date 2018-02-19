
#include "stdafx.h"
#include "Context.hpp"
#include "Display.hpp"
#include "Config.hpp"
#include "Surface.hpp"
#include "CallProxy.hpp"

namespace covellite
{

namespace egl
{

Context::Context(const covellite::egl::Display & _Display, 
  const covellite::egl::Config & _Config) :
  m_Display(_Display.m_Display),
  m_Context(eglCreateContext(m_Display, _Config.m_Config, EGL_NO_CONTEXT, nullptr))
{
  if (m_Context == EGL_NO_CONTEXT) EGL_CALL 0;
}

Context::~Context(void) noexcept
{
  eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  eglDestroyContext(m_Display, m_Context);
}

void Context::MakeCurrent(const covellite::egl::Surface & _Surface) const
{
  EGL_CALL eglMakeCurrent(m_Display, _Surface.m_Surface, _Surface.m_Surface,
    m_Context);
}

} // namespace egl

} // namespace covellite
