
#pragma once
#include "targetver.h"
#include <memory>
#include <chrono>
#include <alicorn/requirements.hpp>
#include <boost/predef/platform.h>
#include <alicorn/cpp/disable-warnings.hpp>
ALICORN_DISABLE_ALL_WARNINGS
#include <glm/glm.force.hpp>
#include <boost/algorithm/string.hpp>
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/singleton.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/std.fast/unordered-map.hpp>
#include <alicorn/patterns/factory.hpp>

#ifndef __USING_GTEST
#include <alicorn/logger.hpp>
#endif

#include "Renderer/GL.hpp"
ALICORN_RESTORE_WARNINGS
