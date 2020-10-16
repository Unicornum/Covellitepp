
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
  m_Events[::covellite::events::Drawing.Do].Connect([=](void)
  {
    using namespace ::std::chrono;

    const auto GetTime = [Begin = std::chrono::system_clock::now()](void)
    {
      return duration_cast<milliseconds>(std::chrono::system_clock::now() - Begin);
    };

    while (!m_LoadingQueue.empty() && GetTime() < milliseconds{ 10 })
    {
      m_LoadingQueue.front()();
      m_LoadingQueue.pop();
    }

    m_pScene->m_GameScene.Update();
    m_pScene->m_3DScene.Render();
  });
}

Window::~Window(void) = default; // ����� ��� ��������������� ��������� �������.

Window::operator Events_t (void) const noexcept /*override*/
{
  return m_Events;
}

/**
* \brief
*  ������� �������� 3D �������.
* \details
*  - ������� ������� 3D ������ ��� ���������� �������� �������, ����� ���� 
*  ���������� ��� �������������, ������� ������������ ��� ����������
*  (���������� ��������� I3DScene) � �������� ������� �������� RemoveObject().
*  - ���� � �������� ������ ����������� ����� ��������� \b Updater, ��� ����
*  ����� ������ ��������� ������, ������� ����� ���������� ��� ���������� �����
*  ����� ����������� �����.
*  
* \param [in] _Object
*  ������� ������.
*  
* \return
*  ������������� ���������� �������.
*  
* \exception std::exception
*  - �������� ���������� (��������� ��. �������� ����������).
*/
ObjectId_t Window::CreateObject(const GameObject_t & _Object) /*override*/
{
  return m_pScene->m_GameScene.CreateObject(_Object);
}

/**
* \brief
*  ������� �������� 3D �������.
* \details
*  - ������� �������� ������� �������� 3D ������� ��� ���������� ��������
*  �������, ����� ���� ���������� ����������� (��� �������� ����������� �����
*  ������, ������� ����� ������� ����� ����� ��������� ��� ������� �����,
*  ����� ���� ��������� �� �� ���� ��������).
*  - ���������� ������ ����� ������ ����� (������� �������� ��������
*  �������������� ����� �������, ����� ����� ������ �� �������� ����������),
*  ����� ���� ����� ������� ������� ��������� ������ � ���������������
*  ���������� �������.
*  - ���������� ������������� ����� �������������� ��� ���������� (����������
*  ��������� I3DScene) � �������� ������� �������� RemoveObject().
*  - ���� � �������� ������ ����������� ����� ��������� \b Updater, ��� ����
*  ����� ������ ��������� ������, ������� ����� ���������� ��� ���������� �����
*  ����� ����������� �����.
*
* \param [in] _fnObjectCreation
*  ������� �������� �������� �������.
* \param [in] _Callback
*  ������� ��������� ������, ������� ���������� ����� �������� �������.
*/
void Window::DeferredCreateObject(
  const fnObjectCreation_t & _fnObjectCreation,
  const fnObjectCreationCompleted_t & _Callback) /*override*/
{
  m_LoadingQueue.emplace([=](void)
  {
    _Callback(m_pScene->m_GameScene.CreateObject(_fnObjectCreation()));
  });
}

/**
* \brief
*  ������� �������� �������.
*
* \param [in] _Id
*  ������������� ����� ���������� �������.
*
* \exception std::exception
*  - �������� ���������� (��������� ��. �������� ����������).
*/
void Window::RemoveObject(const ObjectId_t _Id) /*override*/
{
  m_pScene->m_GameScene.RemoveObject(_Id);
}

/**
* \brief
*  ������� ���������� �������������� ������� � ������� ����������.
* \details
*  - ���������������, ��� ��������� ����� ������������� � ������� ����������
*  �� ���������������.
*
* \param [in] _Id
*  ������������� �������.
*/
void Window::Add(const ObjectId_t _Id) /*override*/
{
  m_pScene->m_3DScene.Add(_Id);
}
