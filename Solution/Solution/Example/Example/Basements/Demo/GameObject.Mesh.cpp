
#include "stdafx.h"
#include "GameObject.hpp"

namespace basement
{

namespace model
{

static size_t Index = 0;

GameObject::Mesh::Mesh(const ::std::vector<Triangle_t> & _Data) :
  m_MeshId(uT("Demo.Mesh.%ID%").Replace(uT("%ID%"), ++Index))
{
  for (const auto & Triangle : _Data)
  {
    Add(Triangle);
  }
}

/*virtual*/ Object_t GameObject::Mesh::GetObject(const Any_t &) const
{
  // Компонент Data.Buffer не используется, т.к. это приводит к глюкам
  // на экране загрузки при использовании общего Present'a для разных уровней
  // LOD.

  const auto & Indices = m_MaterialIndices[uT("Unknown")];

  return
  {
    Component_t::Make(
    {
      { uT("id"), m_MeshId + uT(".Vertex") },
      { uT("type"), uT("Buffer") },
      { uT("data"), static_cast<const Vertex_t *>(m_Vertexes.data()) },
      { uT("count"), m_Vertexes.size() },
    }),
    Component_t::Make(
    {
      { uT("id"), m_MeshId + uT(".Index") },
      { uT("type"), uT("Buffer") },
      { uT("data"), static_cast<const int *>(Indices.data()) },
      { uT("count"), Indices.size() },
    }),
  };
}

void GameObject::Mesh::Add(
  const Triangle_t & _Triangle, 
  const cbVertex_t & _cbVertex)
{
  auto & Indices = m_MaterialIndices[_Triangle.MaterialName];

  for (const auto & Vertex : _Triangle.Vertexes)
  {
    if (Vertex.Hash != static_cast<Vertex::Hash_t>(-1))
    {
      const auto itExistsVertex = m_iVertexes.find(Vertex.Hash);
      if (itExistsVertex != m_iVertexes.end())
      {
        // Такая вершина уже есть в m_Vertexes, ссылаемся на нее.
        Indices.push_back(itExistsVertex->second);
        continue;
      }

      // Такой вершины еще нет в m_AllVertexes, добавляем ее.
      m_iVertexes[Vertex.Hash] = static_cast<Index_t>(m_Vertexes.size());
    }

    Indices.push_back(static_cast<Index_t>(m_Vertexes.size()));
    m_Vertexes.push_back(Vertex.m_Vertex);

    if (_cbVertex) _cbVertex(Vertex, m_Vertexes.size() - 1);
  }
}

void GameObject::Mesh::BuildBasementObject(const float _TextureRatioXY)
{
  const auto kX = 1.0f / (4.0f * _TextureRatioXY);

  ::std::vector<Vertex_t> Points;

  for (float i = 0.0f; i < 6.0f; i += 1.0f)
  {
    const auto HexPoint = GetPoint(i);

    Points.push_back(
      {
        HexPoint.x, HexPoint.y, 0.0f,
        0.0f, 0.0f, 1.0f,
        kX * (0.5f + HexPoint.x), 0.25f * (0.5f + HexPoint.y),
      });
  }

  for (float i = 0.0f; i < 6.0f; i += 1.0f)
  {
    const auto HexPoint = GetPoint(i);
    const auto HexPointNext = GetPoint(i + 1);
    const auto Normal = GetPoint(i, 1.0f);

    Points.push_back(
      {
        HexPoint.x, HexPoint.y, 0.0f,
        Normal.x, Normal.y, 0.0f,
        0.0f, 0.25f,
      });

    Points.push_back(
      {
        HexPoint.x, HexPoint.y, -3.0f,
        Normal.x, Normal.y, 0.0f,
        0.0f, 1.0f,
      });

    Points.push_back(
      {
        HexPointNext.x, HexPointNext.y, 0.0f,
        Normal.x, Normal.y, 0.0f,
        kX, 0.25f,
      });

    Points.push_back(
      {
        HexPointNext.x, HexPointNext.y, -3.0f,
        Normal.x, Normal.y, 0.0f,
        kX, 1.0f,
      });
  }

  const auto AddTriangle =
    [this, &Points](const size_t _i1, const size_t _i2, const size_t _i3)
  {
    Triangle_t Triangle;
    Triangle.Vertexes[0].m_Vertex = Points[_i1];
    Triangle.Vertexes[1].m_Vertex = Points[_i2];
    Triangle.Vertexes[2].m_Vertex = Points[_i3];

    Add(Triangle);
  };

  AddTriangle(0, 1, 2);
  AddTriangle(0, 2, 3);
  AddTriangle(0, 3, 4);
  AddTriangle(0, 4, 5);

  for (size_t i = 0; i < 6; i++)
  {
    AddTriangle(6 + 4 * i, 7 + 4 * i, 9 + 4 * i);
    AddTriangle(9 + 4 * i, 8 + 4 * i, 6 + 4 * i);
  }
}

/*static*/ auto GameObject::Mesh::GetPoint(const float _Index, const float _Radius) -> Point
{
  namespace math = ::alicorn::extension::cpp::math;

  return Point
  {
    _Radius * math::degree::Cos(60.0f * _Index),
    _Radius * math::degree::Sin(60.0f * _Index)
  };
}

} // namespace model

} // namespace basement
