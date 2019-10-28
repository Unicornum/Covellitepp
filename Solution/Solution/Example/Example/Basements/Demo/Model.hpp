
#pragma once
#include "GameWorld.hpp"
#include "DbComponents.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс реализации \b модели паттерна MVC.
*
* \version
*  1.0.0.0        \n
* \date
*  10 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Model final :
  public IDbComponents,
  public IGameWorld
{
  using Events_t = ::covellite::events::Events;

public:
  // Интерфейс IDbComponents
  const Object_t & GetObject(const Id_t) const override;
  ComponentPtr_t GetComponent(const ComponentId_t &) const override;

public:
  // Интерфейс IGameWorld
  GameScenePtr_t CreateGameScene(void) override;
  float GetLandscapeHeight(const CubeCoords &) const override;
  IGameObject::Landscape::Value GetGameObjectType(const CubeCoords &) const override;
  const PointLights & GetPointLights(void) const override;
  SoundDevice & GetSoundDevice(void) override;
  const IDbComponents & GetDbComponents(void) const override;

private:
  DbComponents  m_DbComponents;
  GameWorld     m_GameWorld;

public:
  explicit Model(const Events_t &);
};

} // namespace model

} // namespace basement
