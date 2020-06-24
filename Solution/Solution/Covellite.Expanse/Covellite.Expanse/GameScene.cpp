
#include "stdafx.h"
#include "GameScene.impl.hpp"
#include <Covellite/Api/Renders.hpp>
#include "IProduce3DObject.impl.hpp"

using namespace ::covellite::expanse;

/**
* \brief
*  Конструктор класса.
*  
* \param [in] _pRenders
*  Объект, полученный от \ref CovelliteApiPage для создания наборов рендеров
*  3D объектов из наборов компонентов игровых объектов.
* \param [in] _Produce3DObject
*  Объект для добавления и удаления 3D объектов в 3D cцену.
*/
GameScene::GameScene(
  const RendersPtr_t & _pRenders, 
  IProduce3DObject & _Produce3DObject) noexcept :
  m_pRenders(_pRenders),
  m_Produce3DObject(_Produce3DObject)
{

}

GameScene::~GameScene(void)
{
  for (size_t i = 0; i < m_GameObjects.size(); i++)
  {
    if (!m_GameObjects[i].empty())
    {
      LOGGER(Warning) << "GameScene: game object was not deleted [id=" << i << "]:";

      for (const auto & pComponent : m_GameObjects[i])
      {
        LOGGER(Warning) << "  > component id=" << pComponent->Id << " type=" << pComponent->Type;
      }
    }
  }
}

/**
* \brief
*  Функция создания объекта.
* \details
*  - Для указанного игрового объекта будет создан набор рендеров 3D объекта и
*  возвращен уникальный идентификатор, который можно использовать для добавления
*  в очередь рендеринга.
*  - Если в исходном наборе компонентов будет компонент \b Updater, для него
*  будет создан отдельный рендер, который будет вызываться при обновлении сцены
*  перед рендерингом кадра.
*  
* \param [in] _Object
*  Набор компонентов игрового объекта.
*  
* \return
*  Идентификатор созданного объекта.
*  
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
ObjectId_t GameScene::CreateObject(const GameObject_t & _Object) /*override*/
{
  const auto Id = m_Id.GetFreeId();

  if (Id >= m_GameObjects.size())
  {
    m_GameObjects.resize(Id + 1);
  }

  C3DObject_t o3DObject;

  for (const auto & pComponent : _Object)
  {
    if (pComponent->Type == uT("Updater"))
    {
      m_Updaters.push_back({ Id, m_pRenders->Obtain(pComponent) });
    }
    else
    {
      o3DObject.push_back(m_pRenders->Obtain(pComponent));
    }
  }

  m_Produce3DObject.Add(Id, o3DObject);

  m_GameObjects[Id] = _Object;
  return Id;
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
void GameScene::RemoveObject(const ObjectId_t _Id) /*override*/
{
  if (_Id >= m_GameObjects.size() || m_GameObjects[_Id].empty()) return;

  m_Produce3DObject.Remove(_Id);

  for (const auto & pComponent : m_GameObjects[_Id])
  {
    m_pRenders->Remove(pComponent);
  }

  const auto itRemove = ::std::remove_if(
    ::std::begin(m_Updaters), ::std::end(m_Updaters),
    [&](const ::std::pair<ObjectId_t, Render_t> & _Object) 
    { 
      return _Object.first == _Id;
    });

  m_Updaters.erase(itRemove, ::std::end(m_Updaters));
  m_GameObjects[_Id].clear();
  m_Id.AddRemovedObjectId(_Id);
}

/**
* \brief
*  Функция обновления сцены.
* \details
*  - Функция активирует рендеры компонента \b Updater для всех созданных
*  объектов, у которых он был указан.
*  - Подразумевается, что функция будет вызываться в начале рендеринга кадра и
*  что updater'ы объектов будут добавлять идентификаторы объектов, которые
*  должны рендерится в текущем кадре в очередь рендеринга.
*/
void GameScene::Update(void) const
{
  for (const auto & Updater : m_Updaters) Updater.second();
}
