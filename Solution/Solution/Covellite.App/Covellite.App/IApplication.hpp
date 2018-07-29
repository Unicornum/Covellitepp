
#pragma once
#include <Covellite/Events/IEvents.hpp>

namespace covellite
{

namespace app
{

/**
* \ingroup CovelliteAppGroup
* \brief
*  ����� ������ � ������ \ref CovelliteAppPage \n
*  ������������ ����� ����������.
*  
* \version
*  1.0.0.0        \n
* \date
*  16 ���� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class IApplication :
  public ::covellite::events::IEvents
{
};

} // namespace app

} // namespace covellite
