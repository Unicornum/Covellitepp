
#include "stdafx.h"
#include "Camera.hpp"
#include <alicorn/std/vector.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Constant.hpp>
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

  const auto pCameraPosition = Component_t::Make(
    {
      { uT("id"), Constant::Player::ComponentPositionId },
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
    });

  const auto pCameraRotation = Component_t::Make(
    {
      { uT("id"), Constant::Player::ComponentRotationId },
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
      { uT("y"), Constant::Camera::Pitch },
    });

  return {
  {
    pCameraPosition, pCameraRotation, // должны присутствовать, т.к. впоследствии 
                                      // они извлекаются из базы данных компонентов  
                                      // updater'ом камеры по из идентификаторам
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Camera") },
      { uT("type"), uT("Camera") },
      { uT("kind"), uT("Perspective") },
      { uT("distance"), Constant::Camera::Distance },
      { uT("fov"), Constant::Camera::Fov * math::Constant<float>::RadianToDegree },
      { uT("service"), Object_t{ pCameraPosition, pCameraRotation } },
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
      { uT("entry"), uT("vsVolume") },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Example.Shader.Pixel.Default") },
      { uT("type"), uT("Shader") },
      { uT("entry"), uT("psTextured") },
    }),
  } };
}

} // namespace model

} // namespace basement
