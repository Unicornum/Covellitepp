﻿
#include "stdafx.h"
#include "Model.hpp"

namespace basement
{

namespace model
{

Model::Model(const Events_t & _Events) :
  m_GameWorld(_Events, m_DbComponents)
{
  m_DbComponents.DoAddObject.connect([&](Id_t _Id)
  {
    DoAddObject(_Id);
  });

  m_DbComponents.DoRemoveObject.connect([&](Id_t _Id)
  {
    DoRemoveObject(_Id);
  });
}

// cppcheck-suppress passedByValue
const Object_t & Model::GetObject(const Id_t _Id) const /*override*/
{
  return m_DbComponents.GetObject(_Id);
}

ComponentPtr_t Model::GetComponent(const ComponentId_t & _Id) const /*override*/
{
  return m_DbComponents.GetComponent(_Id);
}

GameScenePtr_t Model::CreateGameScene(void) /*override*/
{
  return m_GameWorld.CreateGameScene();
}

float Model::GetLandscapeHeight(const CubeCoords & _CellPosition) const /*override*/
{
  return m_GameWorld.GetLandscapeHeight(_CellPosition);
}

auto Model::GetGameObjectType(const CubeCoords & _CellPosition) const /*override*/
  -> IGameObject::Landscape::Value
{
  return m_GameWorld.GetGameObjectType(_CellPosition);
}

const PointLights & Model::GetPointLights(void) const /*override*/
{
  return m_GameWorld.GetPointLights();
}

SoundDevice & Model::GetSoundDevice(void) /*override*/
{
  return m_GameWorld.GetSoundDevice();
}

const IDbComponents & Model::GetDbComponents(void) const /*override*/
{
  return m_GameWorld.GetDbComponents();
}

} // namespace model

} // namespace basement
