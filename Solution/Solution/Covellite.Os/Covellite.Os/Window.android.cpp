
#include "stdafx.h"
#include <Covellite\Os\Window.hpp>

using AppInfo_t = ::alicorn::system::platform::AppInfo;

using namespace covellite::os;

Window::Window(void) :
  m_Handle(AppInfo_t::Get<ANativeWindow *>())
{
}

Window::~Window(void) noexcept = default;
