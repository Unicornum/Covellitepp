
#pragma once
#include <Covellite/Covellite.hpp>
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
    PBR,
  };
};

/**
* \ingroup ExampleBasementGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� ��������� �������� 3D ������� ��� ���� Draw3DObject.
*
* \version
*  1.0.0.0        \n
* \date
*  28 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class Simple3DObject final :
  public Common
{
  using Updater_t = ::covellite::api::Updater_t;

public:
  void Notify(int, const ::boost::any &) override;

private:
  Updater_t GetUpdater(void);

private:
  Id BuildCamera(void);
  Id BuildShader(int);
  Id BuildLights(int);
  Id BuildCube(int, float, float, float);

private:
  Component_t::ComponentPtr_t m_pCubeRotation;
  Id m_Camera;
  ::std::vector<Id> m_Cubes;

public:
  Simple3DObject(const RendersPtr_t &, const int, const int);
  ~Simple3DObject(void);
};

} // namespace basement
