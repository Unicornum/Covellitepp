
#pragma once
#include "targetver.h"
#include <alicorn/requirements.hpp>
#include <memory>
#include <alicorn/platform/app-info.hpp>
#include <alicorn/platform/windows.hpp>

#if BOOST_COMP_MSVC
#include <winnls.h>
#endif

#include <alicorn/cpp/disable-warnings.hpp>
ALICORN_DISABLE_ALL_WARNINGS
#include <boost/lexical_cast.hpp>
#include <alicorn/std/message.hpp>
#include <alicorn/std/singleton.hpp>
ALICORN_RESTORE_WARNINGS
