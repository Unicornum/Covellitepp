
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
  /// [Vertex format]
  using Vertex_t = ::covellite::api::Vertex;
  /// [Vertex format]
  using Updater_t = ::covellite::api::Updater_t;

public:
  void Notify(int, const ::boost::any &) override;

private:
  Updater_t GetUpdater(void);

private:
  Id BuildCamera(void);
  Id BuildShader(int, bool);
  Id BuildLights(int);
  void BuildSimpleCubes(int, float);
  void BuildInstanceCubes(int, int, float);
  void BuildCubeData(int);
  Id BuildSimpleCubeObject(float, float, float);

private:
  Component_t::ComponentPtr_t m_pCubeRotation;
  Id m_Camera;
  ::std::vector<Vertex_t> m_VertexData;
  ::std::vector<int> m_IndexData;
  ::std::vector<Id> m_Cubes;
  ::std::vector<float> m_InstanceData;

public:
  Simple3DObject(const RendersPtr_t &, const int, const int);
  ~Simple3DObject(void) noexcept;
};

} // namespace basement
