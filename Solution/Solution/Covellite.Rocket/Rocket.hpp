
#pragma once

/**
* \file
*  Общий заголовочный файл для всех заголовочных файлов проекта 
*  \ref CovelliteRocketPage.
* \defgroup CovelliteRocketGroup Rocket
* \ingroup CovelliteGroup
*  Группа классов проекта \ref CovelliteRocketPage.
*/

#include "Gui\Events.hpp"
#include "Rocket\Layer.hpp"
#include "Rocket\Window.hpp"
#include "Rocket\Vfs.hpp"
#include "Covellite.Rocket.lib.hpp"

namespace covellite
{

#if BOOST_COMP_MSVC
namespace[[deprecated("Deprecated namespace. Use classes from сovellite::gui.")]] rocket{}
#endif

} // namespace covellite
