
#include "stdafx.h"
#include "GameObject.hpp"

namespace basement
{

namespace model
{

static size_t Index = 0;

GameObject::Mesh::Mesh(const Data & _Data) :
  m_Data(_Data),
  m_VertexBufferId(uT("Demo.Buffer.Vertex.%ID%").Replace(uT("%ID%"), ++Index)),
  m_IndexBufferId(uT("Demo.Buffer.Index.%ID%").Replace(uT("%ID%"), Index))
{

}

Object_t GameObject::Mesh::GetObject(void) const
{
  return
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Buffer") },
      { uT("data"), static_cast<const Vertex_t *>(m_Data.Vertex.data()) },
      { uT("count"), m_Data.Vertex.size() },
    }),
    Component_t::Make(
    {
      { uT("id"), m_VertexBufferId },
      { uT("type"), uT("Buffer") },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Buffer") },
      { uT("data"), static_cast<const int *>(m_Data.Index.data()) },
      { uT("count"), m_Data.Index.size() },
    }),
    Component_t::Make(
    {
      { uT("id"), m_IndexBufferId },
      { uT("type"), uT("Buffer") },
    }),
  };
}

void GameObject::Mesh::Add(const Triangle_t & _Triangle)
{
  for (const auto & Vertex : _Triangle)
  {
    m_Data.Index.push_back(static_cast<int>(m_Data.Vertex.size()));
    m_Data.Vertex.push_back(Vertex);
  }
}

} // namespace model

} // namespace basement
