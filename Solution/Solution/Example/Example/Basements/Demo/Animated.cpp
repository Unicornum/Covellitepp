
#include "stdafx.h"
#include "Animated.hpp"
#include <omp.h>
#include <alicorn/boost/format.inl>
#include <alicorn/std/vector.hpp>
#include <alicorn/logger.hpp>
#include <Covellite/Covellite.hpp>
#include "Constants.hpp"
#include "GameScene.hpp"

namespace basement
{

namespace model
{

Animated::Animated(const GameScenePtr_t & _pGameScene) :
  GameObject(Another::Animated),
  m_pGameScene(_pGameScene)
{
  const auto ProcessorsCount = omp_get_num_procs();

  // Использование всех доступных процессоров приводит к периодическим
  // жестким фризам.
  omp_set_num_threads((ProcessorsCount > 1) ? ProcessorsCount - 1 : 1);

  LOGGER(Info) << "OpenMP: processors count=" << ProcessorsCount;

  // .fbx файлы, полученные от Mixamo (и модель и анимации) следует 
  // преобразовать (импорт-экспорт при помощи Blender).
  // ??? При экспорте в .dae файлов анимаций от Mixamo нужно правильно 
  // настроить (и применить!) смещение/поворот/масштаб, но при этом 
  // в результате все равно получается каша из сложенных в одной точке частей 
  // модели.

  AddMesh<Mesh>("Human"
    //".obj");
    ".fbx");
    //".dae"); // !!! Файлы .dae, сохраненные Visual Studio, не читаются !!!
              // !!!      (использовать импорт-экспорт через Bender)    !!!

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
      GetLightsObject() +
      GetShaderObject(true)
    };
  }

  auto & Human = GetMesh(0);
  const auto CellPosition = ::boost::any_cast<const CubeCoords &>(_Value);

  const cbMaterial_t MaterialProcess = [=](const String_t & _MaterialName)
  {
    const auto TextureName = (_MaterialName == uT("Unknown")) ?
      ::std::string{ "demo.stone.jpg" } :
      string_cast<::std::string, Locale::UTF8>(_MaterialName);

    return
      GetTexture(m_Textures[TextureName]).GetObject() +
      GetPresentObject(CellPosition);
  };

  return 
  { 
    Human.GetObject(Value_t{ MaterialProcess, CellPosition, m_Object[0] })
  };
}

/*static*/ Object_t Animated::GetCameraObject(const GameScenePtr_t & _pGameScene)
{
  using Updater_t = ::covellite::api::Updater_t;

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

    pCameraRotation->SetValue(uT("z"), CameraDirection);

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
      { uT("id"), uT("Demo.Animated.Camera.Position") },
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
      { uT("z"), Constant::Player::Height },
    }),
    pCameraRotation,
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.Camera") },
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
  return
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.Light.Ambient") },
      { uT("type"), uT("Light") },
      { uT("kind"), uT("Ambient") },
      { uT("color"), 0xFF8080A0 }, // ARGB
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Direction") },
      { uT("x"), 0.0f },
      { uT("y"), 1.0f },
      { uT("z"), 0.33f },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.Light.Direction") },
      { uT("type"), uT("Light") },
      { uT("kind"), uT("Direction") },
      { uT("color"), 0xFFF0F0B0 }, // ARGB
    })
  };
}

/*static*/ Object_t Animated::GetPresentObject(const CubeCoords & _Coords)
{
  static int Index = 0;
  Index++;

  using ::alicorn::extension::boost::Format;

  // Использование здесь String::Replace() увеличивает время формирования
  // идентификатора в 10(!) раз, что существенно сказывается
  // на производительности.
  const auto Id =
    (Format{ uT("Demo.Animated.Present.%1%") } % Index).ToString();

  return     
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
      { uT("x"), _Coords.ToPlane().first },
      { uT("y"), _Coords.ToPlane().second },
    }),
    Component_t::Make(
    {
      { uT("id"), Id },
      { uT("type"), uT("Present") },
      { uT("kind"), uT("Geometry") },
    })
  };
}

} // namespace model

} // namespace basement
