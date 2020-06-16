
#include "stdafx.h"
#include "GameWorld.hpp"
#include <random>
#include <chrono>
#include <alicorn/std/vector.hpp>
#include <alicorn/logger.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/Os/Events.hpp>
#include "DbComponents.hpp"
#include "GameScene.hpp"
#include "PointLights.hpp"
#include "Constants.hpp"
#include "SoundDevice.hpp"

// 17 Март 2019 11:45 (unicornum.verum@gmail.com)
TODO("Недопустимая ссылка на заголовочный файл!");
#include "../../Layers/Demo.hpp"

namespace basement
{

namespace model
{

namespace math = ::alicorn::extension::cpp::math;

const auto Random = [](const int _From, const int _To)
{
  static ::std::mt19937 Generator{ ::std::random_device{}() };
  return ::std::uniform_int_distribution<>{ _From, _To }(Generator);
};

GameWorld::GameWorld(const Events_t & _Events, DbComponents & _DbComponents) :
  m_Events(_Events),
  m_DbComponents(_DbComponents),
  m_pPointLights(::std::make_shared<PointLights>()),
  m_pSoundDevice(::std::make_shared<SoundDevice>())
{
  LOGGER(Trace) << "Create GameWorld.";

  m_Events[::events::Demo.Resize].Connect(
    [this](const ::std::pair<int, int> & _Size)
  {
    m_WindowX = _Size.first;
    m_WindowY = _Size.second;
  });

  m_Events[::covellite::events::Application.Update].Connect([this](void)
  {
    m_ProcessingMode(0.0f);
  });

  m_Events[::events::Demo.Landscape].Connect([this](const IntPtr_t & _pLoadingPercent)
  {
    PrepareLanscapeScene(_pLoadingPercent);

    m_ProcessingMode = Constant::GetSettings<bool>(uT("IsAutoRun")) ?
      GetAutoProcessMoving() : GetManualProcessMoving();
  });

  m_Events[::events::Demo.Animation].Connect([this](const IntPtr_t & _pLoadingPercent)
  {
    PrepareAnimationScene(_pLoadingPercent);
  });

  m_Events[::events::Demo.Shadows].Connect([this](const IntPtr_t & _pLoadingPercent)
  {
    PrepareShadowsScene(_pLoadingPercent);
  });

  m_Events[::events::Demo.Exit].Connect([this](void) 
  { 
    RemoveAllObjects(); 
  });

  m_Events[::covellite::events::Key.Back].Connect([this](void)
  {
    RemoveAllObjects();
  });
}

GameWorld::~GameWorld(void) noexcept
{
  RemoveAllObjects();

  LOGGER(Trace) << "Destroy GameWorld.";
}

GameScenePtr_t GameWorld::CreateGameScene(void) /*override*/
{
  LOGGER(Trace) << "Create GameScene.";

  m_pGameScene = ::std::make_shared<basement::support::GameScene>();
  return m_pGameScene;
}

float GameWorld::GetLandscapeHeight(const CubeCoords & _CellPosition) const /*override*/
{
  const auto Position = _CellPosition.ToPlane();
  return GetHeight(Position.first, Position.second);
}

auto GameWorld::GetGameObjectType(const CubeCoords & _CellPosition) const /*override*/
  -> IGameObject::Landscape::Value
{
  if (_CellPosition.GetX() == -3 &&
    _CellPosition.GetY() == -3)
  {
    return GameObject::Landscape::Well;
  }

  if (GetLandscapeHeight(_CellPosition) < 0.05f)
  {
    return GameObject::Landscape::Sand;
  }

  const auto Type = Random(0, 100);

  if (Type > (100 - Constant::GetSettings<int>(uT("PointLightsPercent"))))
  {
    return GameObject::Landscape::None;
  }

  if (Type > 27) return GameObject::Landscape::Grass;
  if (Type > 25) return GameObject::Landscape::Rock;
  if (Type > 10) return GameObject::Landscape::Bush;
  return GameObject::Landscape::Tree;
}

const PointLights & GameWorld::GetPointLights(void) const /*override*/
{
  return *m_pPointLights;
}

SoundDevice & GameWorld::GetSoundDevice(void) /*override*/
{
  return *m_pSoundDevice;
}

const IDbComponents & GameWorld::GetDbComponents(void) const /*override*/
{
  return m_DbComponents;
}

void GameWorld::PrepareLanscapeScene(const IntPtr_t & _pLoadingPercent)
{
  LOGGER(Trace) << "Create game objects...";

  PrepareLoader(_pLoadingPercent);
  m_pGameScene->CompleteReplace();

  // Порядок добавления объектов не важен, рендерится они будут в порядке
  // увеличения значений типа объектов.

  PrepareLoader();
  PrepareCamera();
  PreparePlane();

  m_LoadingQueue.push([this](const float)
  {
    // Скайбокс должен быть добавлен после камеры, т.к. он извлекает
    // компонент положения камеры.
    LoadObject(GameObject::Create(GameObject::Support::Skybox),
      static_cast<IGameWorld *>(this));
  });

  m_LoadingQueue.push([this](const float)
  {
    LoadObject(GameObject::Create(GameObject::Extra::Water));
  });

  m_LoadingQueue.push([this](const float)
  {
    LoadObject(GameObject::Create(GameObject::Another::Particles, m_pGameScene));
  });
}

void GameWorld::PrepareAnimationScene(const IntPtr_t & _pLoadingPercent)
{
  LOGGER(Trace) << "Load object...";

  PrepareLoader(_pLoadingPercent);
  m_pGameScene->CompleteReplace();

  const auto pPrototype = 
    GameObject::Create(GameObject::Another::Animated, m_pGameScene);

  m_LoadingQueue.push([=](const float)
  {
    LoadObject(pPrototype);
  });

  const auto CellRadius = 
    ::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION ? 0 : Constant::CellRadius;

  for (int y = -CellRadius; y <= CellRadius; y++)
  {
    for (int x = -CellRadius; x <= CellRadius; x++)
    {
      const CubeCoords CellPosition{ x, y };

      if (abs(CellPosition.GetZ()) <= CellRadius)
      {
        m_LoadingQueue.push([=](const float)
        {
          LoadObject(pPrototype, CellPosition);
        });
      }
    }
  }

  m_ProcessingMode = [](float) {};
}

void GameWorld::PrepareShadowsScene(const IntPtr_t & _pLoadingPercent)
{
  const auto pCursorData = ::std::make_shared<Cursor>();

  LOGGER(Trace) << "Load object...";

  PrepareLoader(_pLoadingPercent);
  m_pGameScene->CompleteReplace();

  const auto pPrototype = 
    GameObject::Create(GameObject::Another::Shadows, m_pGameScene);

  m_LoadingQueue.push([=](const float)
  {
    LoadObject(pPrototype, pCursorData);
  });

  m_ProcessingMode = [](float) {};

  using Cursor_t = ::covellite::events::Cursor_t;

  m_Events[::covellite::events::Cursor.Touch].Connect([=](void)
  {
    pCursorData->IsClick = true;
  });

  m_Events[::covellite::events::Cursor.Motion].Connect(
    [=](const Cursor_t::Position & _Position)
  {
    pCursorData->X = _Position.X;
    pCursorData->Y = _Position.Y;
    pCursorData->WindowX = m_WindowX;
    pCursorData->WindowY = m_WindowY;
  });
}

void GameWorld::RemoveAllObjects(void)
{
  m_pGameScene->ProcessAll([this](const Id_t _Id)
    {
      m_DbComponents.RemoveGameObject(_Id);
    });

  m_pGameScene->CompleteReplace();
  m_LandscapeObjects.clear();
  m_ProcessingMode = [](const float) {};

  {
    ::std::queue<Updater_t> Empty;
    m_LoadingQueue.swap(Empty);
  }
}

auto GameWorld::GetAutoProcessMoving(void) -> Updater_t
{
  const auto PushStep =
    [this](const CubeCoords & _Position, const CubeCoords & _Orientation)
  {
    using namespace ::std::chrono;

    Step Step;
    Step.m_ChangePosition = _Position;
    Step.m_Orientation = _Orientation;
    Step.m_Pitch = m_Pitch;
    Step.m_BeginTime = m_Steps.empty() ?
      duration<float>{ system_clock::now() - Constant::BeginTime }.count() :
      m_Steps.back().m_BeginTime + Constant::TimeStepSecond;

    m_Steps.push(Step);
  };

  namespace events = ::covellite::events;

  return [this, PushStep](const float)
  {
    if (m_Steps.empty())
    {
      const auto ForwardStepCount = Random(10, 30);

      for (int i = 0; i < ForwardStepCount; i++)
      {
        PushStep(m_Orientation, m_Orientation);
      }

      const auto Turn = Random(-1, 1);

      if (Turn > 0)
      {
        PushStep(CubeCoords{}, m_Orientation.TurnRight());
      }
      else if (Turn < 0)
      {
        PushStep(CubeCoords{}, m_Orientation.TurnLeft());
      }
      else
      {
        PushStep(m_Orientation.TurnLeft(), m_Orientation);
        PushStep(m_Orientation.TurnLeft(), m_Orientation);
        PushStep(m_Orientation.TurnLeft(), m_Orientation);
      }
    }
  };
}

auto GameWorld::GetManualProcessMoving(void) -> Updater_t
{
  const auto PushStep = [this](
    const CubeCoords & _ChangePosition,
    const CubeCoords & _Orientation,
    const float _ChangePitch = 0.0f)
  {
    if (!m_Steps.empty()) return;

    using namespace ::std::chrono;

    Step Step;
    Step.m_ChangePosition = _ChangePosition;
    Step.m_Orientation = _Orientation;
    Step.m_Pitch = m_Pitch + _ChangePitch;
    Step.m_BeginTime =
      duration<float>{ system_clock::now() - Constant::BeginTime }.count();

    m_Steps.push(Step);
  };

  m_Events[::events::Demo.MoveForward].Connect([this, PushStep](void)
  {
    PushStep(m_Orientation, m_Orientation);
  });

  m_Events[::events::Demo.MoveBackward].Connect([this, PushStep](void)
  {
    PushStep(CubeCoords{ 0, 0 } - m_Orientation, m_Orientation);
  });

  m_Events[::events::Demo.MoveLeftForward].Connect([this, PushStep](void)
  {
    PushStep(m_Orientation.TurnLeft(), m_Orientation);
  });

  m_Events[::events::Demo.MoveLeftBackward].Connect([this, PushStep](void)
  {
    PushStep(CubeCoords{ 0, 0 } - m_Orientation.TurnRight(), m_Orientation);
  });

  m_Events[::events::Demo.MoveRightForward].Connect([this, PushStep](void)
  {
    PushStep(m_Orientation.TurnRight(), m_Orientation);
  });

  m_Events[::events::Demo.MoveRightBackward].Connect([this, PushStep](void)
  {
    PushStep(CubeCoords{ 0, 0 } -m_Orientation.TurnLeft(), m_Orientation);
  });

  m_Events[::events::Demo.TurnLeft].Connect([this, PushStep](void)
  {
    PushStep(CubeCoords{}, m_Orientation.TurnLeft());
  });

  m_Events[::events::Demo.TurnRight].Connect([this, PushStep](void)
  {
    PushStep(CubeCoords{}, m_Orientation.TurnRight());
  });

  m_Events[::events::Demo.TurnUp].Connect([this, PushStep](void)
  {
    if (m_Pitch > 0.1f) return;
    PushStep(CubeCoords{}, m_Orientation, - Constant::Camera::Pitch);
  });

  m_Events[::events::Demo.TurnDown].Connect([this, PushStep](void)
  {
    if (m_Pitch < -0.1f) return;
    PushStep(CubeCoords{}, m_Orientation, Constant::Camera::Pitch);
  });

  return [](const float) {};
}

void GameWorld::LoadObject(
  const GameObject::IGameObjectPtr_t & _pObject, 
  const Any_t & _Param)
{
  const auto Objects = _pObject->GetObject(_Param);
  ::std::vector<Id_t> ObjectIds;

  for (const auto & Object : Objects)
  {
    ObjectIds.push_back(m_DbComponents.AddGameObject(Object));
  }

  m_pGameScene->Add(_pObject->GetType(), ObjectIds);
}

void GameWorld::LoadObject(
  const GameObject::IGameObjectPtr_t & _pObject,
  const Updater_t & _Updater)
{
  static size_t Index = 0;
  Index++;

  using namespace ::alicorn::extension::std;

  const auto Object = 
    Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Updater.%ID%").Replace(uT("%ID%"), Index) },
        { uT("type"), uT("Updater") },
        { uT("function"), _Updater },
      })
    } +
    _pObject->GetObject()[0];

  const auto Id = m_DbComponents.AddGameObject(Object);

  m_pGameScene->Add(_pObject->GetType(), { Id });
}

void GameWorld::LoadObject(
  const GameObject::IGameObjectPtr_t & _pObject,
  const CubeCoords & _Coords)
{
  const auto Objects = _pObject->GetObject(_Coords);
  ::std::vector<Id_t> ObjectIds;

  for (const auto & Object : Objects)
  {
    ObjectIds.push_back(m_DbComponents.AddGameObject(Object));
  }

  m_pGameScene->Add(_pObject->GetType(), ObjectIds, _Coords);
}

void GameWorld::PrepareLoader(const IntPtr_t & _pLoadingPercent)
{
  using namespace ::std::chrono;
  const auto Start = system_clock::now();

  const auto pLoadedObjects = ::std::make_shared<size_t>(0);

  const Updater_t LoaderUpdater = [=](const float)
  {
    if (m_LoadingQueue.empty())
    {
      *_pLoadingPercent = 101; // 101 для того, чтобы отрисовалось 100%
      m_pGameScene->CompleteReplace();

      LOGGER(Info) << "Loading time: " << 
        duration<float>{system_clock::now() - Start}.count();

      return;
    }

    using namespace ::std::chrono;

    const auto GetTime = [Begin = system_clock::now()](void)
    {
      return duration_cast<milliseconds>(system_clock::now() - Begin);
    };

    while (!m_LoadingQueue.empty() && GetTime() < milliseconds{ 50 })
    {
      try
      {
        m_LoadingQueue.front()(0.0f);
      }
      catch (const ::std::exception & _Ex)
      {
        LOGGER(Error) << _Ex.what();
      }

      m_LoadingQueue.pop();
      (*pLoadedObjects)++;
    }

    *_pLoadingPercent = static_cast<int>(100 * (*pLoadedObjects) /
      (*pLoadedObjects + m_LoadingQueue.size()));
  };

  LoadObject(GameObject::Create(GameObject::Extra::Loader), LoaderUpdater);
}

void GameWorld::PrepareLoader(void)
{
  const Updater_t LoaderUpdater = [this](const float)
  {
    if (m_LoadingQueue.empty()) return;

    using namespace ::std::chrono;

    const auto GetTime = [Begin = system_clock::now()](void)
    {
      return duration_cast<milliseconds>(system_clock::now() - Begin);
    };

    while (!m_LoadingQueue.empty() && GetTime() < milliseconds{ 20 })
    {
      try
      {
        m_LoadingQueue.front()(0.0f);
      }
      catch (const ::std::exception & _Ex)
      {
        LOGGER(Error) << _Ex.what();
      }

      m_LoadingQueue.pop();
    }

    m_pGameScene->CompleteUpdate();
  };

  m_LoadingQueue.push([=](const float)
  {
    LoadObject(GameObject::Create(GameObject::Extra::Loader), LoaderUpdater);
  });
}

void GameWorld::PrepareCamera(void)
{
  m_Position = CubeCoords{};
  m_Orientation = CubeCoords{ 0, 1 };

  Step Step;
  Step.m_ChangePosition = CubeCoords{};
  Step.m_Orientation = m_Orientation;
  Step.m_Pitch = Constant::Camera::Pitch;
  m_Steps = ::std::queue<GameWorld::Step>{ { Step } };

  const Updater_t CameraUpdater = [=](const float)
  {
    if (m_Steps.empty()) return;

    const auto pPosition =
      m_DbComponents.GetComponent(Constant::Player::ComponentPositionId);
    const auto pRotation =
      m_DbComponents.GetComponent(Constant::Player::ComponentRotationId);

    using namespace ::std::chrono;

    const duration<float> Time = system_clock::now() - Constant::BeginTime;

    const auto & Step = m_Steps.front();

    auto T = (Time.count() - Step.m_BeginTime) / Constant::TimeStepSecond;

    const auto NewPosition = m_Position + Step.m_ChangePosition;
    const auto NewWorldPosition = NewPosition.ToPlane();
    const auto NewHeight = GetLandscapeHeight(NewPosition);
    const auto NewDirection = Step.m_Orientation.ToPlane();
    const auto NewPitch = Step.m_Pitch;

    //if (NewHeight < 0)
    //{
    //  m_Steps.pop();
    //  return;
    //}

    if (T > 1.0f)
    {
      T = 0.0f;

      // До изменения m_Position
      PrepareCompressionPlane(CubeCoords{ 0, 0 } - Step.m_ChangePosition);

      m_Position = m_Position + Step.m_ChangePosition;
      m_Orientation = Step.m_Orientation;
      m_Pitch = Step.m_Pitch;
      m_Steps.pop();

      // После изменения m_Position
      PrepareExtensionPlane(Step.m_ChangePosition);
    }

    const auto OldWorldPosition = m_Position.ToPlane();
    const auto OldHeight = GetLandscapeHeight(m_Position);
    const auto OldDirection = m_Orientation.ToPlane();
    const auto OldPitch = m_Pitch;

    const auto Interpolation = [T](const float _Old, const float _New)
    {
      return _Old + T * (_New - _Old);
    };

    const auto X = 
      Interpolation(OldWorldPosition.first, NewWorldPosition.first);
    const auto Y =
      Interpolation(OldWorldPosition.second, NewWorldPosition.second);

    pPosition->SetValue(uT("x"), X);
    pPosition->SetValue(uT("y"), Y);
    pPosition->SetValue(uT("z"), 
      Constant::Player::Height + Interpolation(OldHeight, NewHeight));

    const auto CameraPitch = Interpolation(OldPitch, NewPitch);

    const auto CameraDirection = math::radian::ArcTan(
      -Interpolation(OldDirection.first, NewDirection.first),
      -Interpolation(OldDirection.second, NewDirection.second));

    pRotation->SetValue(uT("y"), CameraPitch);
    pRotation->SetValue(uT("z"), CameraDirection);

    m_pGameScene->SetCameraInfo(
      support::GameScene::Camera{ X, Y, CameraDirection });
  };

  m_LoadingQueue.push([=](const float)
  {
    LoadObject(GameObject::Create(GameObject::Support::Camera), CameraUpdater);
  });
}

void GameWorld::PreparePlane(void)
{
  for (int y = -Constant::CellRadius; y <= Constant::CellRadius; y++)
  {
    for (int x = -Constant::CellRadius; x <= Constant::CellRadius; x++)
    {
      const CubeCoords CellPosition{ x, y };

      if (CellPosition.GetZ() >= -Constant::CellRadius &&
          CellPosition.GetZ() <= Constant::CellRadius)
      {
        m_LoadingQueue.push(GetCellLoader(CellPosition));
      }
    }
  }
}

void GameWorld::PrepareExtensionPlane(const CubeCoords & _Offset)
{
  if (_Offset.GetX() == 0 && _Offset.GetY() == 0) return;

  const auto CellStep = Constant::CellRadius;
  const auto NewFarCell = m_Position + _Offset * CellStep;
  const auto LeftOffset = _Offset.TurnLeft(2);
  const auto RightOffset = _Offset.TurnRight(2);

  m_LoadingQueue.push(GetCellLoader(NewFarCell));

  for (const auto & Offset : { LeftOffset, RightOffset })
  {
    for (int i = 1; i <= CellStep; i++)
    {
      const auto CellPosition = NewFarCell + Offset * i;

      m_LoadingQueue.push(GetCellLoader(CellPosition));
    }
  }
}

void GameWorld::PrepareCompressionPlane(const CubeCoords & _Offset)
{
  if (_Offset.GetX() == 0 && _Offset.GetY() == 0) return;

  const auto CellStep = Constant::CellRadius;
  const auto NewFarCell = m_Position + _Offset * CellStep;
  const auto LeftOffset = _Offset.TurnLeft(2);
  const auto RightOffset = _Offset.TurnRight(2);

  m_LoadingQueue.push(GetCellRemover(NewFarCell));

  for (const auto & Offset : { LeftOffset, RightOffset })
  {
    for (int i = 1; i <= CellStep; i++)
    {
      const auto CellPosition = NewFarCell + Offset * i;

      m_LoadingQueue.push(GetCellRemover(CellPosition));
    }
  }
}

auto GameWorld::GetCellLoader(const CubeCoords & _CellPosition) -> Updater_t
{
  return [=](const float)
  {
    LOGGER_DEBUG(Trace) << "Load cell [" <<
      _CellPosition.GetX() << ", " << _CellPosition.GetY() << "].";

    const auto ObjectType = GetGameObjectType(_CellPosition);
    if (m_LandscapeObjects.find(ObjectType) == m_LandscapeObjects.end())
    {
      const auto pPrototype = GameObject::Create(ObjectType, this);

      LoadObject(pPrototype);
      m_LandscapeObjects[ObjectType] = pPrototype;
    }

    LoadObject(m_LandscapeObjects[ObjectType], _CellPosition);

    if (ObjectType == GameObject::Landscape::None && 
      Constant::GetSettings<bool>(uT("IsNightMode")))
    {
      m_pPointLights->Add(_CellPosition, GetLandscapeHeight(_CellPosition));
    }
  };
}

auto GameWorld::GetCellRemover(const CubeCoords & _CellPosition) -> Updater_t
{
  return [=](const float)
  {
    LOGGER_DEBUG(Trace) << "Remove cell [" <<
      _CellPosition.GetX() << ", " << _CellPosition.GetY() << "].";

    const auto Objects = m_pGameScene->Remove(_CellPosition);
    m_pPointLights->Remove(_CellPosition);

    for (const auto & Object : Objects)
    {
      m_DbComponents.RemoveGameObject(Object);
    }
  };
}

float GameWorld::GetHeight(const float _X, const float _Y) const
{
  const auto Distance = 0.7f * math::Root<2>(_X * _X + _Y * _Y);
  if (Distance < 2.0f) return -0.1f;
  if (Distance < 3.0f) return 0.03f;

  const auto Value = ((Distance / math::Constant<float>::Pi) - 1.0f) / 8.0f;

  if (Value > 0.5f && 
    math::Abs(Value - math::Floor(Value + 0.5f)) < 0.025f)
  {
    return -0.05f;
  }

  return 0.55f + 0.5f * math::radian::Cos(Distance);
}

} // namespace model

} // namespace basement
