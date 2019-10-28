
#include "stdafx.h"
#include "Skybox.hpp"
#include <alicorn/std/vector.hpp>
#include <SoundDevice.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Constant.hpp>
#include "Constants.hpp"
#include "IDbComponents.hpp"
#include "IGameWorld.hpp"

namespace basement
{

namespace model
{

Skybox::Skybox(void) :
  GameObject(Support::Skybox),
  m_IsNightMode{ Constant::GetSettings<bool>(uT("IsNightMode")) }
{
  AddTexture(m_IsNightMode ? "demo.skybox.night.jpg" : "demo.skybox.day.jpg");

  const ::std::vector<Vertex_t> Vertexes =
  {
    { -0.5f,  0.5f,  0.5f, 1.0f,   0.499f, 0.499f,  0.0f, 0.0f, 1.0f, 0.0f, },  // 0
    {  0.5f,  0.5f,  0.5f, 1.0f,   0.499f, 0.001f,  0.0f, 0.0f, 1.0f, 0.0f, },  // 1
    { -0.5f, -0.5f,  0.5f, 1.0f,   0.251f, 0.499f,  0.0f, 0.0f, 1.0f, 0.0f, },  // 2
    {  0.5f, -0.5f,  0.5f, 1.0f,   0.251f, 0.001f,  0.0f, 0.0f, 1.0f, 0.0f, },  // 3
                                  
    {  0.5f, -0.5f,  0.5f, 1.0f,    1.0f,  0.5f,    1.0f, 0.0f, 0.0f, 0.0f, },  // 4
    {  0.5f,  0.5f,  0.5f, 1.0f,   0.75f,  0.5f,    1.0f, 0.0f, 0.0f, 0.0f, },  // 5
    {  0.5f, -0.5f, -0.5f, 1.0f,    1.0f,  1.0f,    1.0f, 0.0f, 0.0f, 0.0f, },  // 6
    {  0.5f,  0.5f, -0.5f, 1.0f,   0.75f,  1.0f,    1.0f, 0.0f, 0.0f, 0.0f, },  // 7
                                  
    { -0.5f, -0.5f,  0.5f, 1.0f,   0.25f,  0.5f,   -1.0f, 0.0f, 0.0f, 0.0f, },  // 8
    { -0.5f,  0.5f,  0.5f, 1.0f,   0.50f,  0.5f,   -1.0f, 0.0f, 0.0f, 0.0f, },  // 9
    { -0.5f, -0.5f, -0.5f, 1.0f,   0.25f,  1.0f,   -1.0f, 0.0f, 0.0f, 0.0f, },  // 10
    { -0.5f,  0.5f, -0.5f, 1.0f,   0.50f,  1.0f,   -1.0f, 0.0f, 0.0f, 0.0f, },  // 11
                                  
    { -0.5f,  0.5f,  0.5f, 1.0f,   0.50f,  0.5f,    0.0f, 1.0f, 0.0f, 0.0f, },  // 12
    {  0.5f,  0.5f,  0.5f, 1.0f,   0.75f,  0.5f,    0.0f, 1.0f, 0.0f, 0.0f, },  // 13
    { -0.5f,  0.5f, -0.5f, 1.0f,   0.50f,  1.0f,    0.0f, 1.0f, 0.0f, 0.0f, },  // 14
    {  0.5f,  0.5f, -0.5f, 1.0f,   0.75f,  1.0f,    0.0f, 1.0f, 0.0f, 0.0f, },  // 15
                                  
    { -0.5f, -0.5f,  0.5f, 1.0f,   0.25f,  0.5f,   0.0f, -1.0f, 0.0f, 0.0f, },  // 16
    {  0.5f, -0.5f,  0.5f, 1.0f,   0.00f,  0.5f,   0.0f, -1.0f, 0.0f, 0.0f, },  // 17
    { -0.5f, -0.5f, -0.5f, 1.0f,   0.25f,  1.0f,   0.0f, -1.0f, 0.0f, 0.0f, },  // 18
    {  0.5f, -0.5f, -0.5f, 1.0f,   0.00f,  1.0f,   0.0f, -1.0f, 0.0f, 0.0f, },  // 19
  };

  const ::std::vector<int> Indexes =
  {
    0,  1,  2,   2,  1,  3,
    4,  5,  6,   6,  5,  7,
    8, 10,  9,  10, 11,  9,
    12, 14, 13,  14, 15, 13,
    16, 17, 18,  18, 17, 19,
  };

  ::std::vector<Mesh::Triangle_t> Triangles;

  for (::std::size_t i = 0; i < Indexes.size(); i += 3)
  {
    Mesh::Triangle_t Triangle;
    Triangle.Vertexes[0].m_Vertex = Vertexes[Indexes[i]];
    Triangle.Vertexes[1].m_Vertex = Vertexes[Indexes[i + 1]];
    Triangle.Vertexes[2].m_Vertex = Vertexes[Indexes[i + 2]];

    Triangles.push_back(Triangle);
  }

  AddMesh<Mesh>(Triangles);
}

auto Skybox::GetObject(const Any_t & _Value) const /*override*/ -> Objects_t
{
  namespace math = ::alicorn::extension::cpp::math;
  using namespace ::alicorn::extension::std;
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<::Lights_t>;

  auto & GameWorld = *::boost::any_cast<IGameWorld *>(_Value);
  auto & DbComponents = GameWorld.GetDbComponents();

  const auto GetAmbientSound = [&](void)
  {
    using Path_t = ::boost::filesystem::path;
    using ::covellite::app::Settings_t;

    const auto PathToSoundsDirectory =
      Settings_t::GetInstance().Get<Path_t>(uT("PathToSoundsDirectory"));
    const auto PathToAmbientFile = PathToSoundsDirectory / 
      (m_IsNightMode ? "Ambient.night.mp3" : "Ambient.day.mp3");

    return GameWorld.GetSoundDevice().Make(
      ::covellite::app::Vfs_t::GetInstance().GetData(PathToAmbientFile));
  };

  const auto pAmbient = GetAmbientSound();

  const BufferMapper_t LightMapper = [=](Lights_t * _pLights)
  {
    pAmbient->Dummy();

    _pLights->Ambient.IsValid = 1;
    _pLights->Ambient.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

    _pLights->Direction.IsValid = 0;
    _pLights->Points.UsedSlotCount = 0;

    return false;
  };

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
        { uT("type"), uT("Buffer") },
        { uT("mapper"), LightMapper },
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
        { uT("id"), uT("Demo.Shader.Vertex.Skybox") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("vsVolume") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shader.Pixel.Skybox") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("psLightened") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Transform.Skybox") },
        { uT("type"), uT("Transform") },
      }),
    } +
    GetTexture(0).GetObject() +
    GetMesh(0).GetObject()
  };
}

} // namespace model

} // namespace basement
