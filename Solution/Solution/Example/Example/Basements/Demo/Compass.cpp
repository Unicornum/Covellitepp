
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
  GameObject(Type::Compass)
{

}

Object_t Compass::GetObject(const Any_t & _Value) const /*override*/
{
  auto & DbComponents = *::boost::any_cast<const IDbComponents *>(_Value);

  static const ::std::vector<Vertex_t> VertexData =
  {
    { -0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, },  // 0
    { -0.5f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, },  // 1
    {  0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, },  // 2
    {  0.5f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, },  // 3
  };

  static const ::std::vector<int> IndexData =
  {
     0,  2,  1,   2,  3,  1,
  };

  using namespace ::alicorn::extension::std;

  return 
    GetCommonObject(VertexData, IndexData) +
    LoadTexture("demo.compass.png") +
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
    };
}

} // namespace model

} // namespace basement
