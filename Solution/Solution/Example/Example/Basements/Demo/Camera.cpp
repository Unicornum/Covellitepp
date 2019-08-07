
#include "stdafx.h"
#include "Camera.hpp"
#include <Covellite/Api/Component.inl>
#include "Constants.hpp"

namespace basement
{

namespace model
{

Camera::Camera(void) :
  GameObject(Support::Camera)
{

}

auto Camera::GetObject(const Any_t &) const /*override*/ -> Objects_t
{
  namespace math = ::alicorn::extension::cpp::math;

  const auto Far = 0.5f * math::Root<2>(Constant::Camera::FarClippingPlane);

  return
  {
    {
      Component_t::Make(
      {
        { uT("id"), Constant::Player::ComponentPositionId },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
      }),
      Component_t::Make(
      {
        { uT("id"), Constant::Player::ComponentRotationId },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Rotation") },
        { uT("y"), Constant::Camera::Pitch },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Camera") },
        { uT("type"), uT("Camera") },
        { uT("kind"), uT("Perspective") },
        { uT("distance"), Constant::Camera::Distance },
        { uT("fov"), Constant::Camera::Fov * math::Constant<float>::RadianToGreed },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.State.Depth") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("clear"), true },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.State.Sampler") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Sampler") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.State.Blend") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Blend") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Vertex.Default") },
        { uT("type"), uT("Shader") },
        { uT("version"), uT("vs_4_0") },
        { uT("entry"), uT("vsTextured") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Pixel.Default") },
        { uT("type"), uT("Shader") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psTextured") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Light.Ambient") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Ambient") },
        { uT("color"), 0xFF8080A0 }, // ARGB
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Direction") },
        { uT("x"), 0.0f },
        { uT("y"), -1.0f },
        { uT("z"), 0.33f },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Light.Direction") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Direction") },
        { uT("color"), 0xFFF0F0B0 }, // ARGB
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Fog") },
        { uT("type"), uT("Fog") },
        { uT("style"), uT("linear") },
        { uT("color"), 0xFF87CEFA }, // ARGB
        { uT("near"), 0.75f * Far },
        { uT("far"), Far },
        { uT("density"), 0.1f },
      }),
    }
  };
}

} // namespace model

} // namespace basement
