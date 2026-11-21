
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
*  Класс входит в проект \ref CovelliteExpansePage \n
*  Интерфейс класса окна.
*
* \version
*  1.0.0.0        \n
*  1.0.1.0        \n
* \date
*  18 Июнь 2020    \n
*  22 Ноябрь 2026    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2020 - 2026
*/
class IWindow :
  public ::covellite::events::IEvents,
  public ::covellite::expanse::IGameScene,
  public ::covellite::expanse::I3DScene
{
protected:
  using fnObjectCreation_t = ::std::function<GameObject_t(void)>;
  using fnObjectCreationCompleted_t = ::std::function<void(ObjectId_t)>;
  using fnObjectCreationCompletedTime_t = ::std::function<void(ObjectId_t, float)>;

public:
  virtual void DeferredCreateObject(const fnObjectCreation_t &, 
    const fnObjectCreationCompleted_t &) = 0;
  virtual void DeferredCreateObject(const fnObjectCreation_t &,
    const fnObjectCreationCompletedTime_t &) = 0;
};

} // namespace expanse

} // namespace covellite
