
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
    LightsChanged,
  };
};

namespace { Simple3DObject_t Simple3DObject; }

} // namespace events

namespace basement
{

class Lights final
{
public:
  enum Type
  {
    Ambient = 0,
    Directional,
    Red,
    Green,
    Blue,
  };
};

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
  void Notify(int, const ::boost::any &) override;
  void Render(void) override;

private:
  Id BuildCamera(void);
  Id BuildLights(int);
  Id BuildCube(int, float, float, float);
  Id BuildCube2(float, float, float, float);

private:
  Component_t::ComponentPtr_t m_pCubeRotation;
  const float m_CenterX = 0.0f;
  const float m_CenterY = 1.6f;
  ::std::vector<Id> m_Lights;

public:
  Simple3DObject(const RendersPtr_t &, int, int);
};

} // namespace basement
