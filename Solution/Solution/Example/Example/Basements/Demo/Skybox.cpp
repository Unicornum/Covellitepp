
#include "stdafx.h"
#include "Skybox.hpp"
#include <alicorn/std/vector.hpp>
#include <Covellite/Api/Component.inl>
#include "Constants.hpp"
#include "IDbComponents.hpp"

namespace basement
{

namespace model
{

Skybox::Skybox(void) :
  GameObject(Type::Skybox)
{
  AddTexture("demo.skybox.png");
  AddMesh<Mesh>(Mesh::Data
    {
      {
        { -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  0.499f, 0.499f, },  // 0
        {  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  0.499f, 0.001f, },  // 1
        { -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  0.251f, 0.499f, },  // 2
        {  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  0.251f, 0.001f, },  // 3

        {  0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f,  0.5f, },  // 4
        {  0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  0.75f,  0.5f, },  // 5
        {  0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f,  1.0f, },  // 6
        {  0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  0.75f,  1.0f, },  // 7

        { -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  0.25f, 0.5f, },  // 8
        { -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  0.50f, 0.5f, },  // 9
        { -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.25f, 1.0f, },  // 10
        { -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.50f, 1.0f, },  // 11

        { -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  0.50f, 0.5f, },  // 12
        {  0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  0.75f, 0.5f, },  // 13
        { -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  0.50f, 1.0f, },  // 14
        {  0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  0.75f, 1.0f, },  // 15

        { -0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  0.25f, 0.5f, },  // 16
        {  0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  0.00f, 0.5f, },  // 17
        { -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  0.25f, 1.0f, },  // 18
        {  0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  0.00f, 1.0f, },  // 19
      },
      {
         0,  1,  2,   2,  1,  3,
         4,  5,  6,   6,  5,  7,
         8, 10,  9,  10, 11,  9,
        12, 14, 13,  14, 15, 13,
        16, 17, 18,  18, 17, 19,
      }
    });
}

auto Skybox::GetObject(const Any_t & _Value) const /*override*/ -> Objects_t
{
  namespace math = ::alicorn::extension::cpp::math;
  using namespace ::alicorn::extension::std;

  auto & DbComponents = *::boost::any_cast<const IDbComponents *>(_Value);

  return
  {
    Object_t
    {
      DbComponents.GetComponent(Constant::Player::ComponentRotationId),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Camera.Skybox") },
        { uT("type"), uT("Camera") },
        { uT("kind"), uT("Perspective") },
        { uT("distance"), 0.0f },
        { uT("fov"), Constant::Camera::Fov * math::Constant<float>::RadianToGreed },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Light.Skybox") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Ambient") },
        { uT("color"), 0xFFFFFFFF }, // ARGB
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Material.Skybox") },
        { uT("type"), uT("Material") },
        { uT("ambient"), 0xFFFFFFFF },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.State.Depth") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Depth") },
        { uT("enabled"), false },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.State.Sampler") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Sampler") },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psTextured") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shader.Pixel.Skybox") },
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
        { uT("id"), uT("Demo.Present.Skybox") },
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Geometry") },
      })
    }
  };
}

} // namespace model

} // namespace basement
