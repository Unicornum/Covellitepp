
#pragma once
#include "Defines.hpp"
#include "GameObject.hpp"
#include "CubeCoords.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс объекта одной ячейки игрового мира.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  30 Март 2019    \n
*  16 Апрель 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Landscape final :
  public GameObject
{
public:
  Objects_t GetObject(const Any_t &) const override;

private:
  class Mesh final :
    public GameObject::Mesh
  {
    class MeshStreamBuffer;

  private:
    void LoadMesh(const Path_t &, const Rect &);
    void BuildMesh(const Type::Value, const int, const float, const Rect &);
    void BuildBasementObject(const float);
    void BuildTriplex6Object(const Point &, const float, const float, 
      const float, const Rect &);
    void BuildTriplex12Object(const Point &, const float, const float, 
      const float, const Rect &);
    static Point GetPoint(const float, const float = 0.5f);

  public:
    Mesh(const Type::Value, const float, const Rect &);
    Mesh(const int, const float, const Rect &);
    Mesh(const Path_t &, const float, const Rect &);
  };

private:
  uint32_t GetBright(const CubeCoords &) const;

private:
  bool m_IsUsingRotate = true;
  float m_ScaleFactor = 1.25f;
  const IGameWorld & m_GameWorld;
  ::std::vector<::std::vector<::std::pair<size_t, size_t>>> m_Models;

private:
  Landscape(const Type::Value, const IGameWorld &);
  friend GameObject;
};

} // namespace model

} // namespace basement
