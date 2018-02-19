
#include "stdafx.h"
#include <Covellite\Core\Window.hpp>
#include <windows.h>

using namespace covellite::core;

int32_t Window::GetStatusBarHeight(void) const
{
  return 0;
}

void Window::Exit(void) const
{
  PostQuitMessage(0);
}
