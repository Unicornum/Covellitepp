
#pragma once
#include "Defines.hpp"

namespace basement
{

namespace model
{

class CubeCoords;

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
  virtual size_t GetGameObjectType(const CubeCoords &) const = 0;

public:
  virtual ~IGameWorld(void) = default;
};

} // namespace model

} // namespace basement
