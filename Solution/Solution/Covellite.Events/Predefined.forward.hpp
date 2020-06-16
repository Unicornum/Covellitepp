
#pragma once
#include <boost/predef/os.h>
#include <boost/predef/platform.h>

/**
* \file
*  Файл предварительных определений, которые могут использоваться во всех 
*  проектах фреймворка.
*/

namespace std { class any; }
namespace boost { class any; }

namespace covellite
{
#if BOOST_OS_WINDOWS
namespace any_location = ::std;
#elif BOOST_PLAT_ANDROID
namespace any_location = ::boost;
#endif

using Any_t = any_location::any;

} // namespace covellite
