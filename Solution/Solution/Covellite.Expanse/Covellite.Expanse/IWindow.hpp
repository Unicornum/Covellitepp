
#pragma once
#include <Covellite/Events/IEvents.hpp>
#include <Covellite/Expanse/IGameScene.hpp>
#include <Covellite/Expanse/I3DScene.hpp>

namespace covellite
{

namespace expanse
{

/**
* \ingroup CovelliteExpanseGroup
* \brief
*  ����� ������ � ������ \ref CovelliteExpansePage \n
*  ��������� ������ ����.
*
* \version
*  1.0.0.0        \n
* \date
*  18 ���� 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2020
*/
class IWindow :
  public ::covellite::events::IEvents,
  public ::covellite::expanse::IGameScene,
  public ::covellite::expanse::I3DScene
{
};

} // namespace expanse

} // namespace covellite
