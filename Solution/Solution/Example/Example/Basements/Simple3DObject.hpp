
#pragma once
#include "Common.hpp"

namespace events
{

class Simple3DObject_t
{
public:
  enum Id
  {
    Start = 0,
  };
};

namespace { Simple3DObject_t Simple3DObject; }

} // namespace events

namespace basement
{

/**
* \ingroup ExampleBasementGroup
* \brief
*   ласс входит в проект \ref ExamplePage \n
*   ласс отрисовки простого 3D объекта дл€ сло€ Draw3DObject.
*
* \version
*  1.0.0.0        \n
* \date
*  28 январь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Simple3DObject final :
  public Common
{
public:
  void Render(void) override;

private:
  Id BuildCamera(void);
  Id BuildCube(int, float, float, float);
  Id BuildLights(void);

private:
  Component_t::ComponentPtr_t m_pCubeRotation;

public:
  Simple3DObject(const RendersPtr_t &, int, int);
};

} // namespace basement
