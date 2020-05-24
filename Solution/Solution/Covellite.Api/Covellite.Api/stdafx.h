
#pragma once
#include "targetver.h"
#include <memory>
#include <chrono>
#include <alicorn/requirements.hpp>
#include <boost/predef/platform.h>

#include <alicorn/cpp/warnings.hpp>

#if BOOST_COMP_MSVC

# pragma warning(push, 0)
# pragma warning(disable: ALICORN_ALL_WARNINGS)

#endif

// 16 Май 2020 17:56 (unicornum.verum@gmail.com)
TODO("Почему не работает включение заголовочного файла и работает явное включение его содержимого???");
//#include <alicorn/cpp/disable-all-warnings.hpp>

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

#include <Rocket/Rocket.hpp>
#include "Renderer/GL.hpp"
#include <alicorn/cpp/restore-warnings.hpp>
