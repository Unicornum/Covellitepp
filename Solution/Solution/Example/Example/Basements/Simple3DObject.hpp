
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

public:
  void Notify(int, const ::boost::any &) override;

private:
  Updater_t GetUpdater(void);

private:
  ObjectId_t BuildCamera(void);
  ObjectId_t BuildShader(int, bool);
  ObjectId_t BuildLights(int);
  void BuildSimpleCubes(int, float);
  void BuildInstanceCubes(int, int, float);
  void BuildCubeData(int);
  ObjectId_t BuildSimpleCubeObject(float, float, float);

private:
  Component_t::ComponentPtr_t m_pCubeRotation;
  ObjectId_t m_CameraId;
  ObjectId_t m_ShaderId;
  ObjectId_t m_LightsId;
  ::std::vector<Vertex_t> m_VertexData;
  ::std::vector<int> m_IndexData;
  ::std::vector<ObjectId_t> m_Cubes;
  ::std::vector<float> m_InstanceData;

public:
  Simple3DObject(WindowExpanse_t &, const int, const int);
  ~Simple3DObject(void);
};

} // namespace basement
