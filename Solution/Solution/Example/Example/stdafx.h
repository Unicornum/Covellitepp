
#pragma once
#include "targetver.h"
#include <alicorn/requirements.hpp>
#include <vector>
#include <chrono>
#include <alicorn/std/string.hpp>

#if BOOST_COMP_MSVC

#include <alicorn/logger.hpp>

#pragma warning(push)
#pragma warning(disable: 4996)
#include <Covellite/Covellite.hpp>
#pragma warning(pop)

#endif
