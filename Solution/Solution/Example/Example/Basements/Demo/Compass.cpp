
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
  AddMesh<Mesh>(Mesh::Data
    {
      {
        { -0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, },  // 0
        { -0.5f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, },  // 1
        {  0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, },  // 2
        {  0.5f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, },  // 3
      },
      {
         0,  2,  1,   2,  3,  1,
      }
    });
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
