
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
  GameObject(Type::Water)
{

}

Object_t Water::GetObject(const Any_t &) const /*override*/
{
  static const ::std::vector<Vertex_t> VertexData =
  {
    { -57.0f, -33.0f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 20.0f, },  // 0
    {    0.0f, 66.0f,  0.0f,   0.0f, 0.0f, 1.0f,  10.0f, 0.0f, },  // 1
    {  57.0f, -33.0f,  0.0f,   0.0f, 0.0f, 1.0f,  20.0f, 20.0f, },  // 2
  };

  static const ::std::vector<int> IndexData = { 0,  2,  1 };

  using namespace ::alicorn::extension::std;

  return 
    GetCommonObject(VertexData, IndexData) +
    LoadTexture("demo.water.png") +
    Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Material.Water") },
        { uT("type"), uT("Material") },
        { uT("ambient"), 0xFFFFFFFF },
        { uT("diffuse"), 0xFFFFFFFF },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Present.Water") },
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Geometry") },
      })
    };
}

} // namespace model

} // namespace basement
