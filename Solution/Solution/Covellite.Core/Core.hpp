
#pragma once

/**
* \file
*  Общий заголовочный файл для всех заголовочных файлов проекта 
*  \ref CovelliteCorePage.
* \defgroup CovelliteCoreGroup Core
* \ingroup CovelliteGroup
*  Группа классов проекта \ref CovelliteCorePage.
*/

#include "Core\Event.hpp"
#include "Core\Params.hpp"
#include "Core\EventHandler.hpp"
#include "Core\Window.hpp"
#include "Core\Application.hpp"
#include "Core\ApplicationEntryPoint.hpp"
#include "Core\Settings.hpp"
#include "Covellite.Core.lib.hpp"

namespace covellite
{

#if BOOST_COMP_MSVC
namespace[[deprecated("Deprecated namespace. Use classes from сovellite::events and covellite::app.")]] core{}
#endif

} // namespace covellite
