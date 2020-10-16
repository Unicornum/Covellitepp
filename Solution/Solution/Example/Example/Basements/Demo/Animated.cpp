
#include "stdafx.h"
#include "Animated.hpp"
#include <omp.h>
#include <alicorn/std/vector.hpp>
#include <alicorn/std/string/encoding.hpp>
#include <alicorn/logger.hpp>
#include <Covellite/Covellite.hpp>
#include <Covellite/Api/Constant.hpp>
#include "Constants.hpp"
#include "GameScene.hpp"
#include "Lights.hpp"

namespace basement
{

namespace model
{

Animated::Animated(const GameScenePtr_t & _pGameScene) :
  GameObject(Another::Animated),
  m_pGameScene(_pGameScene)
{
  const auto ProcessorsCount = omp_get_num_procs();

  // ������������� ���� ��������� ����������� �������� � �������������
  // ������� ������.
  omp_set_num_threads((ProcessorsCount > 1) ? ProcessorsCount - 1 : 1);

  LOGGER(Info) << "OpenMP: processors count=" << ProcessorsCount;

  // .fbx �����, ���������� �� Mixamo (� ������ � ��������) ������� 
  // ������������� (������-������� ��� ������ Blender).
  // ??? ��� �������� � .dae ������ �������� �� Mixamo ����� ��������� 
  // ��������� (� ���������!) ��������/�������/�������, �� ��� ���� 
  // � ���������� ��� ����� ���������� ���� �� ��������� � ����� ����� ������ 
  // ������.

  AddMesh<Mesh>("Human"
    //".obj");
    ".fbx");
    //".dae"); // !!! ����� .dae, ����������� Visual Studio, �� �������� !!!
              // !!!      (������������ ������-������� ����� Bender)    !!!

  Animation AnimationWalking{ "Human.walking.fbx"};

  const ::std::vector<::std::string> Textures =
  {
    "Human.Face.png",
    "Human.Hand.Foot.png",
    "Human.Head.png",
    "Human.Outerwear.png",
    "demo.stone.jpg"
  };

  for (const auto & Name : Textures)
  {
    m_Textures[Name] = AddTexture(Name);
  }

  m_Object =
  {
    AnimationWalking.GetObject()
  };
}

auto Animated::GetObject(const Any_t & _Value) const /*override*/ -> Objects_t
{
  using namespace ::alicorn::extension::std;

  if (_Value.empty())
  {
    return   
    {
      GetCameraObject(m_pGameScene) +
      GetShaderObject(true) +
      GetLightsObject()
    };
  }

  auto & Human = GetMesh(0);
  const auto CellPosition = ::boost::any_cast<const CubeCoords &>(_Value);

  const cbMaterial_t MaterialProcess = 
    [=](const Material_t::value_type & _Material)
  {
    const auto TextureName = (_Material.first == uT("Unknown")) ?
      ::std::string{ "demo.stone.jpg" } :
      string_cast<::std::string, Encoding::UTF8>(_Material.first);

    return
      GetTexture(m_Textures[TextureName]).GetObject() +
      GetPresentObject(CellPosition, _Material.second);
  };

  return 
  { 
    Human.GetObject(Value_t{ MaterialProcess, CellPosition, m_Object[0] })
  };
}

/*static*/ Object_t Animated::GetCameraObject(const GameScenePtr_t & _pGameScene)
{
  using Updater_t = ::covellite::api::Updater_t;

  const auto pCameraPosition = Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.Camera.Position") },
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
      { uT("z"), Constant::Player::Height },
    });

  const auto pCameraRotation = Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.Camera.Rotation") },
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
      { uT("y"), Constant::Camera::Pitch },
      { uT("z"), 2.0f },
    });

  const Updater_t CameraUpdater = [=](const float _Time)
  {
    namespace math = ::alicorn::extension::cpp::math;

    const float CameraDirection = math::Constant<float>::Pi * 
      (fmod(_Time / 10.0f, 2.0f) - 1.0f);

    (*pCameraRotation)[uT("z")] = CameraDirection;

    _pGameScene->SetCameraInfo(
      support::GameScene::Camera{ 0.0f, 0.0f, CameraDirection });
  };

  return     
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.Camera.Updater") },
      { uT("type"), uT("Updater") },
      { uT("function"), CameraUpdater },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.Camera") },
      { uT("type"), uT("Camera") },
      { uT("kind"), uT("Perspective") },
      { uT("distance"), Constant::Camera::Distance },
      { uT("fov"), Constant::Camera::Fov * math::Constant<float>::RadianToDegree },
      { uT("service"), Object_t{ pCameraPosition, pCameraRotation } }
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
      { uT("id"), uT("Demo.Animated.State.Clear") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Clear") },
      { uT("color"), 0xFF7F7FA0 },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.State.Sampler") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Sampler") },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.State.Blend") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Blend") },
    }),
  };
}

/*static*/ Object_t Animated::GetLightsObject(void)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  struct Fog
  {
    float Color[4];
    float Near;
    float Far;
    float Density;
    // cppcheck-suppress unusedStructMember
    float Dummy;
  };

  struct UserConstantBuffer
  {
    Fog Fog;
    Lights_t Lights;
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

  const BufferMapper_t Mapper = [=](void * _pUserConstBuffer)
  {
    // ����������� ����� ::glm::vec4 ������, ��������� ������ � Release ������

    auto * pUserConstBuffer =
      reinterpret_cast<UserConstantBuffer *>(_pUserConstBuffer);
    auto & Ambient = pUserConstBuffer->Lights.Ambient;
    auto & Direction = pUserConstBuffer->Lights.Direction;
    auto & Points = pUserConstBuffer->Lights.Points;

    Ambient.IsValid = 1;
    Ambient.Color = ARGBtoFloat4(0xFF8080A0);

    Direction.IsValid = 1;
    Direction.Color = ARGBtoFloat4(0xFFF0F0B0);
    Direction.Direction = ::glm::vec4{ 0.0f, 1.0f, 0.33f, 1.0f };

    Points.UsedSlotCount = 0;

    return false;
  };

  return
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.Lights") },
      { uT("type"), uT("Buffer") },
      { uT("mapper"), Mapper },
      { uT("size"), sizeof(UserConstantBuffer) },
    }),
  };
}

/*static*/ Object_t Animated::GetPresentObject(
  const CubeCoords & _Coords,
  const ::std::vector<Index_t> & _IndexBuffer)
{
  static size_t Index = 0;

  const auto Id = String_t{ uT("%ID%") }.Replace(uT("%ID%"), Index++);

  const Object_t Transform =
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
      { uT("x"), _Coords.ToPlane().first },
      { uT("y"), _Coords.ToPlane().second },
    })
  };

  return     
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.Transform.") + Id },
      { uT("type"), uT("Transform") },
      { uT("service"), Transform },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.Present.") + Id },
      { uT("type"), uT("Present") },
      { uT("content"), _IndexBuffer },
    })
  };
}

} // namespace model

} // namespace basement
