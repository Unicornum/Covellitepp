
#pragma once

#if BOOST_OS_WINDOWS

#include <windows.h>
using Handle_t = HWND;

#elif BOOST_OS_ANDROID

using Handle_t = ANativeWindow *;

#endif
