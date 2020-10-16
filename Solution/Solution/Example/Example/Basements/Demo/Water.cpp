
#include "stdafx.h"
#include "Water.hpp"
#include <alicorn/std/vector.hpp>
#include <Covellite/Api/Component.inl>
#include "IDbComponents.hpp"
#include "Constants.hpp"

namespace basement
{

namespace model
{


Water::Water(void) :
  GameObject(Extra::Water)
{
  // Изменение размеров треугольника не влияет на fps!

  AddTexture("demo.water.png");

  Mesh::Triangle_t Triangle1;
  Triangle1.Vertexes[0].m_Vertex =
    { -570.0f, -330.0f, 0.0f, 1.0f,     0.0f, 200.0f,  0.0f, 0.0f, 1.0f, 0.0f };  // 0
  Triangle1.Vertexes[1].m_Vertex =
    {  570.0f, -330.0f, 0.0f, 1.0f,   200.0f, 200.0f,  0.0f, 0.0f, 1.0f, 0.0f };  // 2
  Triangle1.Vertexes[2].m_Vertex =
    {    0.0f,  660.0f, 0.0f, 1.0f,   100.0f,   0.0f,  0.0f, 0.0f, 1.0f, 0.0f };  // 1

  AddMesh<Mesh>(::std::vector<Mesh::Triangle_t>{ Triangle1 });
}

auto Water::GetObject(const Any_t &) const /*override*/ -> Objects_t
{
  using namespace ::alicorn::extension::std;

  const auto & Vfs = ::covellite::app::Vfs_t::GetInstance();

  return
  {
    Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shader.Vertex.Water") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("vsVolume") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shader.Pixel.Water") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("psTextured") },
        { uT("content"), Vfs.GetData("Data\\Shaders\\Example.fx") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Transform.Water") },
        { uT("type"), uT("Transform") },
      }),
    } +
    GetTexture(0).GetObject() +
    GetMesh(0).GetObject()
  };
}

} // namespace model

} // namespace basement
