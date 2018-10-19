
#pragma once

/**
* \file
*  Файл предварительных определений, которые могут использоваться во всех 
*  проектах фреймворка.
*
* \todo
*  Заменить boost::any на std::any когда это будет возможно (для Android - 
*  перейти на новую версию Android NDK, в Windows - включить в настройках 
*  проекта последнюю версию стандарта).
*/

namespace boost { class any; }

namespace covellite
{

using Any_t = ::boost::any;

} // namespace covellite
