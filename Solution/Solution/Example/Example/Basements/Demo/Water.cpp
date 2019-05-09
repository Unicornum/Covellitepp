
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
  AddMesh<Mesh>(Mesh::Data
    {
      {
        { -570.0f, -330.0f,  0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 200.0f, },  // 0
        {    0.0f,  660.0f,  0.0f,   0.0f, 0.0f, 1.0f,  100.0f,   0.0f, },  // 1
        {  570.0f, -330.0f,  0.0f,   0.0f, 0.0f, 1.0f,  200.0f, 200.0f, },  // 2
      },
      { 
        0,  2,  1
      }
    });
}

auto Water::GetObject(const Any_t &) const /*override*/ -> Objects_t
{
  using namespace ::alicorn::extension::std;

  return
  {
    Object_t
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psTextured") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shader.Pixel.Water") },
        { uT("type"), uT("Shader") },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("vs_4_0") },
        { uT("entry"), uT("vsTextured") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shader.Vertex") },
        { uT("type"), uT("Shader") },
      }),
    } +
    GetTexture(0).GetObject() +
    GetMesh(0).GetObject() +
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
    }
  };
}

} // namespace model

} // namespace basement
