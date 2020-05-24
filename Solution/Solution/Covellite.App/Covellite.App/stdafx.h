
#pragma once
#include "targetver.h"
#include <alicorn/requirements.hpp>
#include <memory>
#include <vector>

#include <alicorn/cpp/warnings.hpp>

#if BOOST_COMP_MSVC

# pragma warning(push, 0)
# pragma warning(disable: ALICORN_ALL_WARNINGS)

#endif

// 16 Май 2020 17:56 (unicornum.verum@gmail.com)
TODO("Почему не работает включение заголовочного файла и работает явное включение его содержимого???");
//#include <alicorn/cpp/disable-all-warnings.hpp>

#include <alicorn/std/message.hpp>
#include <alicorn/cpp/restore-warnings.hpp>
