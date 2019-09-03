
#pragma once

/**
* \file
*  Файл определений, которые могут использоваться во всех проектах фреймворка.
*  
* \todo
*  Заменить boost::any_cast на std::any_cast когда это будет возможно 
*  (для Android - перейти на новую версию Android NDK, в Windows - включить 
*  в настройках проекта последнюю версию стандарта).
*/

#include <boost/any.hpp>
#include <Covellite/Predefined.forward.hpp>

namespace covellite
{

template<class T>
inline T any_cast(const Any_t & _Value)
{
  return ::boost::any_cast<T>(_Value);
}

template<class T>
inline T any_cast(Any_t & _Value)
{
  return ::boost::any_cast<T>(_Value);
}

} // namespace covellite
