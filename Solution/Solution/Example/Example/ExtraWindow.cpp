
#include "stdafx.h"
#include "ExtraWindow.hpp"
//#include <Covellite\Api\GL.hpp>

ExtraWindow::ExtraWindow(const WindowApi_t & _WindowApi) :
  m_WindowApi(_WindowApi),
  m_Events(_WindowApi)
{
  //m_Events[::covellite::events::Drawing.Do].Connect([]()
  //{
  //  glClearColor(0.4f, 0.0f, 1.0f, 1.0f);
  //  glClear(GL_COLOR_BUFFER_BIT);
  //});
}

ExtraWindow::operator const WindowApi_t & (void) const
{
  return m_WindowApi;
}
