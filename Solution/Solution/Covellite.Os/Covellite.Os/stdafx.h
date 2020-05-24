
#pragma once
#include "targetver.h"
#include <alicorn/requirements.hpp>
#include <memory>
#include <alicorn/platform/app-info.hpp>
#include <alicorn/platform/windows.hpp>

#if BOOST_COMP_MSVC
#include <winnls.h>
#endif

#include <alicorn/cpp/warnings.hpp>

#if BOOST_COMP_MSVC

# pragma warning(push, 0)
# pragma warning(disable: ALICORN_ALL_WARNINGS)

#endif

// 16 Май 2020 17:56 (unicornum.verum@gmail.com)
TODO("Почему не работает включение заголовочного файла и работает явное включение его содержимого???");
//#include <alicorn/cpp/disable-all-warnings.hpp>

#include <boost/lexical_cast.hpp>
#include <alicorn/std/message.hpp>
#include <alicorn/std/singleton.hpp>
#include <alicorn/cpp/restore-warnings.hpp>
