
#include "stdafx.h"
#include "Compass.hpp"
#include <alicorn/std/vector.hpp>
#include <Covellite/Api/Component.inl>
#include "IDbComponents.hpp"
#include "Constants.hpp"

namespace basement
{

namespace model
{


Compass::Compass(void) :
  GameObject(Extra::Compass)
{
  AddTexture("demo.compass.png");

  Mesh::Triangle_t Triangle1;
  Triangle1.Vertexes[0].m_Vertex =
    { -0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, };  // 0
  Triangle1.Vertexes[1].m_Vertex =
    {  0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, };  // 2
  Triangle1.Vertexes[2].m_Vertex =
    { -0.5f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, };  // 1

  Mesh::Triangle_t Triangle2;
  Triangle2.Vertexes[0].m_Vertex =
    {  0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, };  // 2
  Triangle2.Vertexes[1].m_Vertex =
    {  0.5f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, };  // 3
  Triangle2.Vertexes[2].m_Vertex =
    { -0.5f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, };  // 1

  AddMesh<Mesh>(::std::vector<Mesh::Triangle_t>{ Triangle1, Triangle2 });
}

auto Compass::GetObject(const Any_t & _Value) const /*override*/ -> Objects_t
{
  auto & DbComponents = *::boost::any_cast<const IDbComponents *>(_Value);

  using namespace ::alicorn::extension::std;

  return
  {
    GetShaderObject() +
    GetTexture(0).GetObject() +
    GetMesh(0).GetObject() +
    Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Material.Compass") },
        { uT("type"), uT("Material") },
        { uT("ambient"), 0xFFFFFFFF },
        { uT("diffuse"), 0xFFFFFFFF },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("z"), -1.0f },
      }),
      DbComponents.GetComponent(Constant::Player::ComponentPositionId),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Present.Compass") },
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Geometry") },
      })
    }
  };
}

} // namespace model

} // namespace basement
