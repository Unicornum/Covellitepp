
#include "stdafx.h"
#include "Camera.hpp"
#include <Covellite/Api/Component.inl>
#include "Constants.hpp"

namespace basement
{

namespace model
{

Camera::Camera(void) :
  GameObject(Type::Camera)
{

}

auto Camera::GetObject(const Any_t &) const /*override*/ -> Objects_t
{
  namespace math = ::alicorn::extension::cpp::math;

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
    }
  };
}

} // namespace model

} // namespace basement
