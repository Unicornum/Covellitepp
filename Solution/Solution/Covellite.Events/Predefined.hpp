
#pragma once

/**
* \file
*  Файл определений, которые могут использоваться во всех проектах фреймворка.
*/

#include <Covellite/Predefined.forward.hpp>

#if BOOST_OS_WINDOWS
#include <any>
#elif BOOST_PLAT_ANDROID
#include <boost/any.hpp>
#endif

namespace covellite
{

template<class T>
inline T any_cast(const Any_t & _Value)
{
  return any_location::any_cast<T>(_Value);
}

template<class T>
inline T any_cast(Any_t & _Value)
{
  return any_location::any_cast<T>(_Value);
}

inline bool has_value(const Any_t & _Value) noexcept
{
# if BOOST_OS_WINDOWS
  return _Value.has_value();
# elif BOOST_PLAT_ANDROID
  return !_Value.empty();
# endif
}

} // namespace covellite
