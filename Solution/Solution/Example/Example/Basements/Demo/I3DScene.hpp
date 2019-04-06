
#pragma once
#include "Defines.hpp"

namespace basement
{

namespace view
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ������������ ����� 3D �����.
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
class I3DScene
{
public:
  virtual void SetGameScene(const GameScenePtr_t &) = 0;
  virtual void Render(void) = 0;

public:
  virtual ~I3DScene(void) = default;
};

} // namespace view

} // namespace basement
