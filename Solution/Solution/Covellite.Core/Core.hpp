
#pragma once

/**
* \file
*  ����� ������������ ���� ��� ���� ������������ ������ ������� 
*  \ref CovelliteCorePage.
* \defgroup CovelliteCoreGroup Core
* \ingroup CovelliteGroup
*  ������ ������� ������� \ref CovelliteCorePage.
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
namespace[[deprecated("Deprecated namespace. Use classes from �ovellite::events and covellite::app.")]] core{}
#endif

} // namespace covellite
