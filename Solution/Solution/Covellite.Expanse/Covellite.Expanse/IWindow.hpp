
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
*   ласс входит в проект \ref CovelliteExpansePage \n
*  »нтерфейс класса окна.
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
class IWindow :
  public ::covellite::events::IEvents,
  public ::covellite::expanse::IGameScene,
  public ::covellite::expanse::I3DScene
{
protected:
  using fnObjectCreation_t = ::std::function<GameObject_t(void)>;
  using fnObjectCreationCompleted_t = ::std::function<void(ObjectId_t)>;

public:
  virtual void DeferredCreateObject(const fnObjectCreation_t &, 
    const fnObjectCreationCompleted_t &) = 0;
};

} // namespace expanse

} // namespace covellite
