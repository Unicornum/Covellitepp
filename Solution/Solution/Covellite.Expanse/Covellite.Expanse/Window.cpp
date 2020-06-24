
#include "stdafx.h"
#include <Covellite/Expanse/Window.hpp>
#include <Covellite/Events/Events.inl>
#include <Covellite/Api/Events.hpp>
#include "GameScene.impl.hpp"
#include "3DScene.impl.hpp"

using namespace covellite::expanse;

class Window::Scene
{
public:
  covellite::expanse::C3DScene  m_3DScene;
  covellite::expanse::GameScene m_GameScene;

public:
  explicit Scene(const WindowApi_t & _WindowApi) :
    m_GameScene(_WindowApi.GetRenders(), m_3DScene)
  {

  }
  Scene(const Scene &) = delete;
  Scene(Scene &&) = delete;
  Scene & operator= (const Scene &) = delete;
  Scene & operator= (Scene &&) = delete;
  ~Scene(void) = default;
};

Window::Window(const WindowApi_t & _WindowApi) :
  m_Events(_WindowApi),
  m_pScene(::std::make_unique<Scene>(_WindowApi))
{
  m_Events[::covellite::events::Drawing.Do].Connect([&](void)
  {
    m_pScene->m_GameScene.Update();
    m_pScene->m_3DScene.Render();
  });
}

Window::~Window(void) = default; // Нужно для компилируемости тестового проекта.

Window::operator Events_t (void) const noexcept /*override*/
{
  return m_Events;
}

ObjectId_t Window::CreateObject(const GameObject_t & _Object) /*override*/
{
  return m_pScene->m_GameScene.CreateObject(_Object);
}

void Window::RemoveObject(const ObjectId_t _Id) /*override*/
{
  m_pScene->m_GameScene.RemoveObject(_Id);
}

void Window::Add(const size_t _Pass, const ObjectId_t _Id, const size_t _Hash) /*override*/
{
  m_pScene->m_3DScene.Add(_Pass, _Id, _Hash);
}
