
#pragma once
#include <Covellite/Expanse/Defines.hpp>

namespace covellite
{

namespace expanse
{

/**
* \ingroup CovelliteExpanseGroup
* \brief
*   ласс входит в проект \ref CovelliteExpansePage \n
*  »нтерфейс класса игровой сцены.
*  
* \version
*  1.0.0.0        \n
* \date
*  18 »юнь 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2020
*/
class IGameScene
{
public:
  virtual ObjectId_t CreateObject(const GameObject_t &) = 0;
  virtual void RemoveObject(const ObjectId_t) = 0;

public:
  virtual ~IGameScene(void) = default;
};

} // namespace expanse

} // namespace covellite
