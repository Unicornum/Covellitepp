
#pragma once
#include "DbComponents.hpp"
#include "GameWorld.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� ���������� \b ������ �������� MVC.
*
* \version
*  1.0.0.0        \n
* \date
*  10 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class Model final :
  public IDbComponents,
  public IGameWorld
{
  using Events_t = ::covellite::events::Events;

public:
  // ��������� IDbComponents
  const Object_t & GetObject(const Id_t) const override;
  ComponentPtr_t GetComponent(const ComponentId_t &) const override;

public:
  // ��������� IGameWorld
  GameScenePtr_t CreateGameScene(void) override;
  float GetLandscapeHeight(const CubeCoords &) const override;
  size_t GetGameObjectType(const CubeCoords &) const override;

private:
  DbComponents  m_DbComponents;
  GameWorld     m_GameWorld;

public:
  explicit Model(const Events_t &);
};

} // namespace model

} // namespace basement