
#pragma once
#include "targetver.h"
#include <alicorn/requirements.hpp>
#include <vector>
#include <map>
#include <functional>
#include <GLMath.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/std/exception.hpp>

#if BOOST_COMP_MSVC 

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef GetObject
# undef GetObject
#endif

#endif

// Использование здесь включений отдельных файлов Covellite++ вместо
// единого файла позволяет ускорить сборку всего на пару секунд.
#include <Covellite/Covellite.hpp>
