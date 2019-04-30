
#include "stdafx.h"
#include "3DScene.hpp"
#include <alicorn/logger.hpp>
#include "GameScene.hpp"
#include "DbRenders.hpp"

namespace basement
{

namespace view
{

C3DScene::C3DScene(DbRenders & _DbRenders) :
  m_DbRenders(_DbRenders),
  m_Time(::std::chrono::seconds::zero())
{
  LOGGER(Trace) << "Create 3DScene.";
}

void C3DScene::SetGameScene(const GameScenePtr_t & _pGameScene) /*override*/
{
  LOGGER(Trace) << "Set GameScene.";

  m_pGameScene = _pGameScene;
}

void C3DScene::Render(void) /*override*/
{
  RenderToLog(::std::chrono::seconds{ 3 });

  m_pGameScene->Render([=](const Id_t _Id)
  {
    for (const auto & Render : m_DbRenders.GetRenders(_Id)) Render();
  });
}

void C3DScene::RenderToLog(const ::std::chrono::seconds & _Period)
{
  using namespace ::std::chrono;

  if (duration_cast<seconds>(system_clock::now() - m_Time) < _Period) return;
  m_Time = system_clock::now();

  LOGGER(Trace) << "Render GameScene.";
}

} // namespace view

} // namespace basement
