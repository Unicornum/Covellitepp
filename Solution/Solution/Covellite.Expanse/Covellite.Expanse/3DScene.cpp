
#include "stdafx.h"
#include "3DScene.impl.hpp"
#include <alicorn/std/exception.hpp>

using namespace ::covellite::expanse;

C3DScene::C3DScene(void)
{
  m_RenderQueue.reserve(1000u);
}

/**
* \brief
*  Функция добавления 3D объекта в 3D сцену.
* \details
*  - После добавления идентификатор объекта можно будет добавлять в очередь
*  рендеринга, тогда при вызове функции рендеринга будут вызваны его рендеры.
*  
* \param [in] _Id
*  Идентификатор объекта.
* \param [in] _Object
*  Набор рендеров 3D объекта.
*/
void C3DScene::Add(const ObjectId_t _Id, const C3DObject_t & _Object) /*override*/
{
  if (_Id >= m_Renders.size())
  {
    m_Renders.resize(_Id + 1,
      { [](void) { throw STD_EXCEPTION << "Using render for not created object."; } });
  }

  m_Renders[_Id] = _Object;
}

/**
* \brief
*  Функция удаления 3D объекта из 3D cцены.
* \details
*  - При добавлении идентификатора удаленного объекта в очередь
*  рендеринга при попытке отрендерить сцену будет брошено исключение.
*  
* \param [in] _Id
*  Идентификатор ранее созданного объекта.
*/
void C3DScene::Remove(const ObjectId_t _Id) /*override*/
{
  if (_Id >= m_Renders.size()) return;

  m_Renders[_Id] =
  {
    [=](void)
    {
      throw STD_EXCEPTION << "Using render for removed object: " << _Id;
    }
  };
}

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо
*  нее следует использовать Add() с одним параметром и сортировку на стороне
*  клиентского кода.
* \brief
*  Функция добавления идентификатора объекта в очередь рендеринга указанного
*  прохода.
* \details
*  - Подразумевается, что рендеринг будет производиться в несколько проходов
*  (в порядке возрастания их номеров), каждый из которых представляет собой
*  очередь рендеринга, в которой объекты отсортированы в порядке возрастания
*  их хешей.
*
* \param [in] _Pass
*  Номер прохода.
* \param [in] _Id
*  Идентификатор объекта.
* \param [in] _HashForSort
*  Хеш для сортировки.
*/
void C3DScene::Add(
  const size_t _Pass,
  const ObjectId_t _Id,
  const size_t _HashForSort) /*override*/
{
  if (_Pass >= m_RenderQueueDepracated.size()) m_RenderQueueDepracated.resize(_Pass + 1);

  // Если использовать vector<Object_t>, то простое добавление объекта в конец
  // работает гораздо быстрее, но для реалистичного количества ~500 ВИДИМЫХ
  // объектов в сцене - накладные расходы совершенно мизерные, а при 50к
  // объектах на ноутбуке 2013-го года выпуска эта вставка занимает ~33%
  // времени при рендеринге пустых рендеров (при этом Pass.clear() занимает 10%).
  // Телефон 2016-го года выпуска отстает всего лишь в 2 раза.
  m_RenderQueueDepracated[_Pass].emplace(Object_t{ _HashForSort, _Id });
}

/**
* \brief
*  Функция добавления идентификатора объекта в очередь рендеринга указанного
*  прохода.
* \details
*  - Подразумевается, что рендеринг будет производиться в несколько проходов
*  (в порядке возрастания их номеров), каждый из которых представляет собой
*  очередь рендеринга, в которой объекты отсортированы в порядке возрастания
*  их хешей.
*
* \param [in] _Pass
*  Номер прохода.
* \param [in] _Id
*  Идентификатор объекта.
* \param [in] _HashForSort
*  Хеш для сортировки.
*/
void C3DScene::Add(const ObjectId_t _Id) /*override*/
{
  m_RenderQueue.push_back(_Id);
}

/**
* \brief
*  Функция рендеринга 3D сцены.
* \details
*  - Функция предназначена для рендеринга всех объектов, предварительно
*  добавленных в очередь рендеринга.
*  - После завершения рендеринга сцены все очереди будут очищены, поэтому
*  в начале каждого кадра их следует заполнить заново.
*  
* \exception std::exception
*  - Попытка рендеринга несуществующего или удаленного объекта.
*/
void C3DScene::Render(void)
{
  for (auto & Pass : m_RenderQueueDepracated)
  {
    for (const auto & Object : Pass)
    {
      for (const auto & Render : Get(Object.second))
      {
        Render();
      }
    }

    Pass.clear();
  }

  for (const auto & Id : m_RenderQueue)
  {
    for (const auto & Render : Get(Id))
    {
      Render();
    }
  }

  m_RenderQueue.clear();
}

/**
* \brief
*  Функция получения набора рендеров 3D объекта по его идентификатору.
*  
* \param [in] _Id
*  Идентификатор объекта.
*  
* \exception std::exception
*  - Объект с указанным идентификатором не был добавлен в 3D сцену.
*/
const C3DObject_t & C3DScene::Get(const ObjectId_t _Id) const
{
  if (_Id >= m_Renders.size())
  {
    throw STD_EXCEPTION << "Unexpected object id: " << _Id;
  }

  return m_Renders[_Id];
}
