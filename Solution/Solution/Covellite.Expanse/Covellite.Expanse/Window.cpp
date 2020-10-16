
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

Window::~Window(void) = default; // Нужно для компилируемости тестового проекта.

Window::operator Events_t (void) const noexcept /*override*/
{
  return m_Events;
}

/**
* \brief
*  Функция создания 3D объекта.
* \details
*  - Функция создает 3D объект для указанного игрового объекта, после чего 
*  возвращает его идентификатор, который используется для рендеринга
*  (реализация интерфеса I3DScene) и удаления объекта функцией RemoveObject().
*  - Если в исходном наборе компонентов будет компонент \b Updater, для него
*  будет создан отдельный рендер, который будет вызываться при обновлении сцены
*  перед рендерингом кадра.
*  
* \param [in] _Object
*  Игровой объект.
*  
* \return
*  Идентификатор созданного объекта.
*  
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
ObjectId_t Window::CreateObject(const GameObject_t & _Object) /*override*/
{
  return m_pScene->m_GameScene.CreateObject(_Object);
}

/**
* \brief
*  Функция создания 3D объекта.
* \details
*  - Функция помещает задание создания 3D объекта для указанного игрового
*  объекта, после чего немедленно завершается (это действие выполняется очень
*  быстро, поэтому таким образом можно сразу загрузить все объекты сцены,
*  после чего рендерить их по мере создания).
*  - Фактически объект будет создан позже (очередь создания объектов
*  обрабатывается таким образом, чтобы слабо влиять на скорость рендеринга),
*  после чего будет вызвана функция обратного вызова с идентификатором
*  созданного объекта.
*  - Полученный идентификатор может использоваться для рендеринга (реализация
*  интерфеса I3DScene) и удаления объекта функцией RemoveObject().
*  - Если в исходном наборе компонентов будет компонент \b Updater, для него
*  будет создан отдельный рендер, который будет вызываться при обновлении сцены
*  перед рендерингом кадра.
*
* \param [in] _fnObjectCreation
*  Функция создания игрового объекта.
* \param [in] _Callback
*  Функция обратного вызова, которая вызывается после создания объекта.
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
*  Функция удаления объекта.
*
* \param [in] _Id
*  Идентификатор ранее созданного объекта.
*
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
void Window::RemoveObject(const ObjectId_t _Id) /*override*/
{
  m_pScene->m_GameScene.RemoveObject(_Id);
}

/**
* \brief
*  Функция добавления идентификатора объекта в очередь рендеринга.
* \details
*  - Подразумевается, что рендеринг будет производиться в порядке добавления
*  их идентификаторов.
*
* \param [in] _Id
*  Идентификатор объекта.
*/
void Window::Add(const ObjectId_t _Id) /*override*/
{
  m_pScene->m_3DScene.Add(_Id);
}
