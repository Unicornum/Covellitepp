
#include "stdafx.h"
#include <Covellite/Api/Renders.hpp>
#include <Covellite/Api/Component.inl>
//#include <boost/thread/lock_guard.hpp>

using namespace covellite::api;

/**
* \brief
*  Конструктор класса.
*  
* \param [in] _Creators
*  Набор функций создающих рендеры для соответствующих типов компонентов
*  (функции могут возвращать nullptr).
*/
Component::Renders::Renders(const Creators_t & _Creators) :
  m_Creators(_Creators)
{
}

/**
* \brief
*  Функция получения набора рендеров для заданного набора компонентов.
* \details
*  - Для каждого компонента в исходном наборе будет добавлен рендер 
*  соответствующего типа.
*  - Если для компонента (с его идентификатором) рендер уже существует,
*  в результат будет добавлен существующий (с увеличением счетчика ссылок,
*  см. описание функции Remove()), если нет - будет создан новый.
*  - Компоненты неподдерживаемых типов будут проигнорированы.
*  - Пустые рендеры (nullptr) будут проигнорированы.
*  - Порядок следования рендеров будет соответствовать порядку следования
*  компонентов в исходном наборе.
*
* \param [in] _Object
*  Исходный набор компонентов.
*
* \return
*  Набор рендеров для входного набора компонентов.
*/
auto Component::Renders::Obtain(const Object_t & _Object) -> Renders_t
{
  // 23 Март 2019 18:20 (unicornum.verum@gmail.com)
  TODO("Тест работы в многопоточном режиме.");
  // Когда будут использоваться разные объекты этого класса, синхронизация
  // должная быть реализована общая.
  //::boost::lock_guard<::boost::mutex> Lock(m_Mutex);

  Renders_t Result;
  Result.reserve(m_MaxRendersCount);

  for (const auto & _pComponent : _Object)
  {
    auto itRender = m_AllExistingRenders.find(_pComponent->Id);

    const auto GetExistsRender = [&](void)
    {
      itRender->second.first++;
      return itRender->second.second;
    };

    const auto ComponentRender = (itRender == m_AllExistingRenders.end()) ?
      Create(_pComponent) : GetExistsRender();
    if (ComponentRender) Result.push_back(ComponentRender);
  }

  m_MaxRendersCount = ::std::max(m_MaxRendersCount, Result.size());

  return Result;
}

/**
* \brief
*  Функция удаления рендеров для указанных компонентов.
* \details
*  - Для каждого компонента в наборе функция уменьшает на 1 счетчик 
*  соответсвующего ему рендера; когда счетчик уменьшается до нуля, рендер
*  удаляется.
*/
void Component::Renders::Remove(const Object_t & _Object)
{
  //::boost::lock_guard<::boost::mutex> Lock(m_Mutex);

  for (const auto & _pComponent : _Object)
  {
    auto itRender = m_AllExistingRenders.find(_pComponent->Id);
    if (itRender == m_AllExistingRenders.end()) continue;

    itRender->second.first--;

    if (itRender->second.first == 0)
    {
      m_AllExistingRenders.erase(itRender);
    }
  }
}

auto Component::Renders::Create(const ComponentPtr_t & _pComponent) -> Render_t
{
  Render_t Result = nullptr;

  try
  {
    auto itCreator = m_Creators.find(_pComponent->Type);
    if (itCreator != m_Creators.end())
    {
      Result = itCreator->second(_pComponent);
    }
  }
  catch (const ::std::exception & _Ex)
  {
    LOGGER(Error) << "Create render fail: " << _Ex.what() << " ["
      << "id: " << _pComponent->Id << ", "
      << "type: " << _pComponent->Type << "].";
  }

  if (Result != nullptr)
  {
    m_AllExistingRenders[_pComponent->Id] = { 1, Result };
  }

  return Result;
}
