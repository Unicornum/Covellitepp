
#include "stdafx.h"
#include "Display.hpp"
#include "CallProxy.hpp"

using namespace covellite::egl;

Display::Display(void) :
  m_Display(eglGetDisplay(EGL_DEFAULT_DISPLAY))
{
  EGL_CALL eglInitialize(m_Display, nullptr, nullptr);
}

Display::~Display(void) noexcept
{
  eglTerminate(m_Display);
}
