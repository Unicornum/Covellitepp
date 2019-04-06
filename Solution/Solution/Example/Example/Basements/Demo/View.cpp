
#include "stdafx.h"
#include "View.hpp"

namespace basement
{

namespace view
{

View::View(const RendersPtr_t & _pRenders) :
  m_DbRenders(_pRenders),
  m_3DScene(m_DbRenders)
{

}

void View::AddObject(const Id_t _Id, const Object_t & _Object) /*override*/
{
  m_DbRenders.AddObject(_Id, _Object);
}

void View::RemoveObject(const Id_t _Id, const Object_t & _Object) /*override*/
{
  m_DbRenders.RemoveObject(_Id, _Object);
}

void View::SetGameScene(const GameScenePtr_t & _pGameScene) /*override*/
{
  m_3DScene.SetGameScene(_pGameScene);
}

void View::Render(void) /*override*/
{
  m_3DScene.Render();
}

} // namespace view

} // namespace basement
