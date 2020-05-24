
#pragma once
#include "targetver.h"
#include <alicorn/requirements.hpp>
#include <vector>
#include <map>
#include <functional>
#include <glm/glm.force.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/std/exception.hpp>

// Использование здесь включений отдельных файлов Covellite++ вместо
// единого файла позволяет ускорить сборку всего на пару секунд.
#include <Covellite/Covellite.hpp>

#if BOOST_OS_WINDOWS

#include <alicorn/platform/windows.hpp>
#include <windef.h>
#include <wingdi.h>
#undef GetObject

#endif
