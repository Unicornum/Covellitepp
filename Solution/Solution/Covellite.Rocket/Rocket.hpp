
#pragma once

/**
* \file
*  ����� ������������ ���� ��� ���� ������������ ������ ������� 
*  \ref CovelliteRocketPage.
* \defgroup CovelliteRocketGroup Rocket
* \ingroup CovelliteGroup
*  ������ ������� ������� \ref CovelliteRocketPage.
*/

#include "Gui\Events.hpp"
#include "Rocket\Layer.hpp"
#include "Rocket\Window.hpp"
#include "Rocket\Vfs.hpp"
#include "Covellite.Rocket.lib.hpp"

namespace covellite
{

#if BOOST_COMP_MSVC
namespace[[deprecated("Deprecated namespace. Use classes from �ovellite::gui.")]] rocket{}
#endif

} // namespace covellite
