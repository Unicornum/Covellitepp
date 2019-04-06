
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
* \date
*  30 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Landscape final :
  public GameObject
{
public:
  Object_t GetObject(const Any_t &) const override;

private:
  void BuildLowerObject(void) const;
  void BuildUpperObject(void) const;
  void BuildTriplexObject(const Point &, const float, const float, const Rect &) const;
  Path_t GetTexturePath(void) const;
  uint32_t GetBright(const CubeCoords &) const;

private:
  static Point GetPoint(const float, const float = 0.5f);

private:
  const IGameWorld & m_GameWorld;
  const Object_t m_Texture;
  mutable ::std::vector<Vertex_t> m_VertexData;
  mutable ::std::vector<int>      m_IndexData;

private:
  Landscape(const Type::Value, const IGameWorld &);
  friend GameObject;
};

} // namespace model

} // namespace basement
