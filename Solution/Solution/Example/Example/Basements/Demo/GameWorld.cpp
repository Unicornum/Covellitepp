
#include "stdafx.h"
#include "GameWorld.hpp"
#include <random>
#include <alicorn/boost/filesystem.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/image.hpp>
#include <alicorn/logger.hpp>
#include <Covellite/Api/Vertex.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/App/Settings.hpp>
#include "DbComponents.hpp"
#include "GameScene.hpp"
#include "Constants.hpp"

// 17 Март 2019 11:45 (unicornum.verum@gmail.com)
TODO("Недопустимая ссылка на заголовочный файл!");
#include "../../Layers/Demo.hpp"

namespace basement
{

namespace model
{

namespace math = ::alicorn::extension::cpp::math;
using Vertex_t = ::covellite::api::Vertex::Polyhedron;

GameWorld::GameWorld(const Events_t & _Events, DbComponents & _DbComponents) :
  m_Events(_Events),
  m_DbComponents(_DbComponents)
{
  LOGGER(Trace) << "Create GameWorld.";

  const auto LoadScene = [=](const IntPtr_t & _pLoadingPercent)
  {
    PrepareScene(_pLoadingPercent);
  };

  m_Events[::events::Demo.Auto].Connect(LoadScene);
  m_Events[::events::Demo.Manual].Connect(LoadScene);
  m_Events[::events::Demo.Exit].Connect([=](void) { RemoveAllObjects(); });

  m_Events[::covellite::events::Application.Update].Connect([=](void) 
  { 
    m_pGameScene->CallForEach([=](const Id_t _Id) 
    { 
      m_DbUpdaters.CallUpdater(_Id);
    }, nullptr);
  });

  ActivateProcessMovingEvents();
}

GameWorld::~GameWorld(void)
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
  return GetHeight(0.5f * Position.first, 0.5f * Position.second);
}

size_t GameWorld::GetGameObjectType(const CubeCoords & _CellPosition) const /*override*/
{
  const auto Random = [](const int _From, const int _To)
  {
    static ::std::mt19937 Generator{ ::std::random_device{}() };
    return ::std::uniform_int_distribution<>{ _From, _To }(Generator);
  };

  if (GetLandscapeHeight(_CellPosition) < 0.2f)
  {
    return GameObject::Type::Sand;
  }

  const auto Type = Random(0, 100);

  if (Type > 80) return GameObject::Type::None;
  if (Type > 75) return GameObject::Type::Rock;
  if (Type > 70) return GameObject::Type::Bush;
  if (Type > 65) return GameObject::Type::Tree;
  return GameObject::Type::Grass;
}

void GameWorld::PrepareScene(const IntPtr_t & _pLoadingPercent)
{
  LOGGER(Trace) << "Create game objects...";

  PrepareLoader(_pLoadingPercent);
  m_pGameScene->Complete();

  m_Position = CubeCoords{};
  m_Orientation = CubeCoords{ 0, 1 };

  Step Step;
  Step.m_ChangePosition = CubeCoords{};
  Step.m_Orientation = m_Orientation;
  m_Steps.push(Step);

  PrepareCamera();
  PreparePlane();

  m_LoadingQueue.push([=](void)
  {
    LoadObject(GameObject::Create(GameObject::Type::Water, *this));
  });

  const IDbComponents * pDbComponents = &m_DbComponents;

  m_LoadingQueue.push([=](void)
  {
    LoadObject(GameObject::Create(GameObject::Type::Compass, *this), pDbComponents);
  });
}

void GameWorld::PrepareLoader(const IntPtr_t & _pLoadingPercent)
{
  const auto pLoadedObjects = ::std::make_shared<size_t>(0);

  const Updater_t LoaderUpdater = [=](void)
  {
    if (m_LoadingQueue.empty())
    {
      *_pLoadingPercent = 101; // 101 для того, чтобы отрисовалось 100%
      m_pGameScene->Complete();
      return;
    }

    using namespace ::std::chrono;

    const auto Begin = system_clock::now();

    while (!m_LoadingQueue.empty() &&
      duration_cast<milliseconds>(system_clock::now() - Begin) < milliseconds{ 50 })
    {
      m_LoadingQueue.front()();
      m_LoadingQueue.pop();
      (*pLoadedObjects)++;
    }

    *_pLoadingPercent = static_cast<int>(100 * (*pLoadedObjects) /
      (*pLoadedObjects + m_LoadingQueue.size()));
  };

  LoadObject(GameObject::Create(GameObject::Type::Loader, *this), LoaderUpdater);
}

void GameWorld::RemoveAllObjects(void)
{
  m_pGameScene->CallForEach(
    [=](const Id_t _Id)
    {
      m_DbUpdaters.RemoveUpdater(_Id);
    }, 
    [=](const Id_t _Id)
    {
      m_DbComponents.RemoveGameObject(_Id);
    });

  m_pGameScene->Complete();
}

void GameWorld::ActivateProcessMovingEvents(void)
{
  const auto PushStep = 
    [=](const CubeCoords & _Position, const CubeCoords & _Orientation)
  {
    using namespace ::std::chrono;

    Step Step;
    Step.m_ChangePosition = _Position;
    Step.m_Orientation = _Orientation;

    if (m_Steps.empty())
    {
      Step.m_BeginTime = duration<float>{ 
        system_clock::now() - Constant::BeginTime }.count();
    }
    else
    {
      Step.m_BeginTime = m_Steps.back().m_BeginTime + Constant::TimeStepSecond;
    }

    m_Steps.push(Step);
  };

  m_Events[::events::Demo.MoveForward].Connect([=](void)
  {
    PushStep(m_Orientation, m_Orientation);
  });

  m_Events[::events::Demo.MoveBackward].Connect([=](void)
  {
    PushStep(CubeCoords{ 0, 0 } - m_Orientation, m_Orientation);
  });

  m_Events[::events::Demo.MoveLeftForward].Connect([=](void)
  {
    PushStep(m_Orientation.TurnLeft(), m_Orientation);
  });

  m_Events[::events::Demo.MoveLeftBackward].Connect([=](void)
  {
    PushStep(CubeCoords{ 0, 0 } - m_Orientation.TurnRight(), m_Orientation);
  });

  m_Events[::events::Demo.MoveRightForward].Connect([=](void)
  {
    PushStep(m_Orientation.TurnRight(), m_Orientation);
  });

  m_Events[::events::Demo.MoveRightBackward].Connect([=](void)
  {
    PushStep(CubeCoords{ 0, 0 } -m_Orientation.TurnLeft(), m_Orientation);
  });

  m_Events[::events::Demo.TurnLeft].Connect([=](void)
  {
    PushStep(CubeCoords{}, m_Orientation.TurnLeft());
  });

  m_Events[::events::Demo.TurnRight].Connect([=](void)
  {
    PushStep(CubeCoords{}, m_Orientation.TurnRight());
  });
}

void GameWorld::LoadObject(const GameObject::IGameObjectPtr_t & _pObject, 
  const Any_t & _Param)
{
  const auto Id = m_DbComponents.AddGameObject(_pObject->GetObject(_Param));

  m_pGameScene->Add(_pObject->GetType(), Id);
}

void GameWorld::LoadObject(const GameObject::IGameObjectPtr_t & _pObject,
  const Updater_t & _Updater)
{
  const auto Id = m_DbComponents.AddGameObject(_pObject->GetObject());

  m_pGameScene->Add(_pObject->GetType(), Id, true);
  m_DbUpdaters.AddUpdater(Id, _Updater);
}

void GameWorld::PrepareCamera(void)
{
  const Updater_t CameraUpdater = [=](void)
  {
    auto pPosition =
      m_DbComponents.GetComponent(Constant::Player::ComponentPositionId);
    auto pRotation =
      m_DbComponents.GetComponent(Constant::Player::ComponentRotationId);

    using namespace ::std::chrono;

    const duration<float> Time = system_clock::now() - Constant::BeginTime;

    if (m_Steps.empty()) return;

    const auto & Step = m_Steps.front();

    auto T = (Time.count() - Step.m_BeginTime) / Constant::TimeStepSecond;

    const auto NewPosition = m_Position + Step.m_ChangePosition;
    const auto NewWorldPosition = NewPosition.ToPlane();
    const auto NewHeight = GetLandscapeHeight(NewPosition);
    const auto NewDirection = Step.m_Orientation.ToPlane();

    if (NewHeight < 0)
    {
      m_Steps.pop();
      return;
    }

    if (T > 1.0f)
    {
      T = 0.0f;
      m_Position = m_Position + Step.m_ChangePosition;
      m_Orientation = Step.m_Orientation;
      m_Steps.pop();
    }

    const auto OldWorldPosition = m_Position.ToPlane();
    const auto OldHeight = GetLandscapeHeight(m_Position);
    const auto OldDirection = m_Orientation.ToPlane();

    const auto Interpolation = [=](const float _Old, const float _New)
    {
      return _Old + T * (_New - _Old);
    };

    pPosition->SetValue(uT("x"), 
      0.5f * Interpolation(OldWorldPosition.first, NewWorldPosition.first));
    pPosition->SetValue(uT("y"), 
      0.5f * Interpolation(OldWorldPosition.second, NewWorldPosition.second));
    pPosition->SetValue(uT("z"),
      Constant::Player::Height + Interpolation(OldHeight, NewHeight));

    pRotation->SetValue(uT("z"), math::radian::ArcTan(
      -Interpolation(OldDirection.first, NewDirection.first),
      -Interpolation(OldDirection.second, NewDirection.second)));
  };

  m_LoadingQueue.push([=](void)
  {
    LoadObject(GameObject::Create(GameObject::Type::Camera, *this), CameraUpdater);
  });
}

void GameWorld::PreparePlane(void)
{
  using LanscapeObjects_t = ::std::map<size_t, GameObject::IGameObjectPtr_t>;
  const auto pLandscapeObjects = ::std::make_shared<LanscapeObjects_t>();

  for (int y = -Constant::CellRadius; y <= Constant::CellRadius; y++)
  {
    for (int x = -Constant::CellRadius; x <= Constant::CellRadius; x++)
    {
      const CubeCoords CellPosition{ x, y };

      if (CellPosition.GetZ() >= -Constant::CellRadius &&
          CellPosition.GetZ() <= Constant::CellRadius)
      {
        m_LoadingQueue.push([=](void)
        {
          const auto ObjectType = GetGameObjectType(CellPosition);
          if (pLandscapeObjects->find(ObjectType) == pLandscapeObjects->end())
          {
            const auto pPrototype =
              GameObject::Create(static_cast<Type_t::Value>(ObjectType), *this);

            LoadObject(pPrototype);
            (*pLandscapeObjects)[ObjectType] = pPrototype;
          }

          LoadObject((*pLandscapeObjects)[ObjectType], CellPosition);
        });
      }
    }
  }
}

float GameWorld::GetHeight(const float _X, const float _Y) const
{
# ifdef _DEBUG
  const auto K = 1.5f;
# else
  const auto K = 0.75f;
# endif

  const auto Distance = K * math::Root<2>(_X * _X + _Y * _Y);
  if (Distance > 12.0f) return -0.1f;
  if (Distance > 10.0f) return 0.03f;

  return 0.7f + 0.5f * math::radian::Cos(Distance);
}

} // namespace model

} // namespace basement
