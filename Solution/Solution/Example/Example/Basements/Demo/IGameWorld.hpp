
#pragma once
#include "Defines.hpp"
#include "IGameObject.hpp"

class SoundDevice;

namespace basement
{

namespace model
{

class CubeCoords;
class PointLights;
class IDbComponents;

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Интерфейсный класс игрового мира.
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
class IGameWorld
{
public:
  virtual GameScenePtr_t CreateGameScene(void) = 0;
  virtual float GetLandscapeHeight(const CubeCoords &) const = 0;
  virtual IGameObject::Landscape::Value GetGameObjectType(const CubeCoords &) const = 0;
  virtual const PointLights & GetPointLights(void) const = 0;
  virtual SoundDevice & GetSoundDevice(void) = 0;
  virtual const IDbComponents & GetDbComponents(void) const = 0;

public:
  virtual ~IGameWorld(void) = default;
};

} // namespace model

} // namespace basement
