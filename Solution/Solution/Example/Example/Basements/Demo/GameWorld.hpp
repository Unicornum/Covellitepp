
#pragma once
#include <queue>
#include <alicorn/std/string.forward.hpp>
#include <alicorn/boost/filesystem.forward.hpp>
#include <Covellite/Covellite.hpp>
#include "IGameWorld.hpp"
#include "GameObject.hpp"
#include "CubeCoords.hpp"

namespace basement
{

namespace model
{

class DbComponents;

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс реализации логики игрового мира.
*
* \version
*  1.0.0.0        \n
* \date
*  10 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class GameWorld final :
  public IGameWorld
{
  using Updater_t = ::covellite::api::Updater_t;
  using Events_t = ::covellite::events::Events;
  using IntPtr_t = int *;
  using Type_t = size_t;

public:
  // Интерфейс IGameWorld
  GameScenePtr_t CreateGameScene(void) override;
  float GetLandscapeHeight(const CubeCoords &) const override;
  IGameObject::Landscape::Value GetGameObjectType(const CubeCoords &) const override;
  const PointLights & GetPointLights(void) const override;
  SoundDevice & GetSoundDevice(void) override;
  const IDbComponents & GetDbComponents(void) const override;

private:
  void PrepareLanscapeScene(const IntPtr_t &);
  void PrepareAnimationScene(const IntPtr_t &);
  void PrepareShadowsScene(const IntPtr_t &);
  void RemoveAllObjects(void);
  Updater_t GetAutoProcessMoving(void);
  Updater_t GetManualProcessMoving(void);

private:
  void LoadObject(const GameObject::IGameObjectPtr_t &, const Any_t & = Any_t{});
  void LoadObject(const GameObject::IGameObjectPtr_t &, const Updater_t &);
  void LoadObject(const GameObject::IGameObjectPtr_t &, const CubeCoords &);
  void PrepareLoader(const IntPtr_t &);
  void PrepareLoader(void);
  void PrepareCamera(void);
  void PreparePlane(void);
  void PrepareExtensionPlane(const CubeCoords &);
  void PrepareCompressionPlane(const CubeCoords &);
  Updater_t GetCellLoader(const CubeCoords &);
  Updater_t GetCellRemover(const CubeCoords &);
  float GetHeight(const float, const float) const;

private:
  Events_t                m_Events;
  DbComponents &          m_DbComponents;
  GameScenePtr_t          m_pGameScene;
  ::std::queue<Updater_t> m_LoadingQueue;
  Updater_t               m_ProcessingMode = [](const float) {};
  float m_WindowX = 100.0f;
  float m_WindowY = 100.0f;

private:
  ::std::shared_ptr<PointLights> m_pPointLights;
  ::std::shared_ptr<SoundDevice> m_pSoundDevice;

private:
  class Step final
  {
  public:
    CubeCoords m_ChangePosition;
    CubeCoords m_Orientation;
    float      m_Pitch = 0.0f;
    float      m_BeginTime = 0.0f;
  };

  CubeCoords m_Position;
  CubeCoords m_Orientation{ 0, 1 };
  float      m_Pitch = 0.0f;
  ::std::queue<Step> m_Steps;
  ::std::map<size_t, GameObject::IGameObjectPtr_t> m_LandscapeObjects;

public:
  GameWorld(const Events_t &, DbComponents &);
  ~GameWorld(void) noexcept;
};

} // namespace model

} // namespace basement
