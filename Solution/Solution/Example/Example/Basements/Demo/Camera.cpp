
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
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<::Lights_t>;

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

  Object_t Result =
  {
    pCameraPosition, pCameraRotation, // должны присутствовать, т.к. впоследствии 
                                      // они извлекаютс€ из базы данных компонентов  
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
      { uT("entry"), uT("psLightened") },
    }),
  };

  const auto ARGBtoFloat4 = [](uint32_t _HexColor)
  {
    return ::glm::vec4
    {
      ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
      ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
      ((_HexColor & 0x000000FF) >> 0) / 255.0f,
      ((_HexColor & 0xFF000000) >> 24) / 255.0f
    };
  };

  BufferMapper_t Mapper;

  if (Constant::GetSettings<bool>(uT("IsNightMode")))
  {
    Mapper = [=](::Lights_t * _pLights)
    {
      // «ахватывать здесь ::glm::vec4 нельз€, программа упадет в Release версии

      _pLights->Ambient.IsValid = 1;
      _pLights->Ambient.Color = ARGBtoFloat4(0xFF202050);

      _pLights->Direction.IsValid = 0;
      _pLights->Points.UsedSlotCount = 0;

      return false;
    };
  }
  else
  {
    Mapper = [=](::Lights_t * _pLights)
    {
      // «ахватывать здесь ::glm::vec4 нельз€, программа упадет в Release версии

      _pLights->Ambient.IsValid = 1;
      _pLights->Ambient.Color = ARGBtoFloat4(0xFF8080A0);

      _pLights->Direction.IsValid = 1;
      _pLights->Direction.Color = ARGBtoFloat4(0xFFF0F0B0);
      _pLights->Direction.Direction = { 0.0f, -1.0f, 0.33f, 1.0f };

      _pLights->Points.UsedSlotCount = 0;

      return false;
    };
  }

  using namespace ::alicorn::extension::std;

  Result += Object_t
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Light.Ambient.Direction") },
      { uT("type"), uT("Buffer") },
      { uT("mapper"), Mapper },
    }),
  };

  return { Result };
}

} // namespace model

} // namespace basement
