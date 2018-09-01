
#pragma once
#include "targetver.h"
#include <alicorn\requirements.hpp>
#include <alicorn\platform\app-info.hpp>

#if BOOST_OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if BOOST_COMP_MSVC 
# pragma warning(push)
# pragma warning(disable: 4996)
#endif

#include <memory>

#if BOOST_COMP_MSVC 
# pragma warning(pop)
#endif
