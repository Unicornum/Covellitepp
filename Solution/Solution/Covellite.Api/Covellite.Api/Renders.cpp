
#include "stdafx.h"
#include <Covellite/Api/Renders.hpp>
#include <Covellite/Api/Defines.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Vertex.hpp>
//#include <boost/thread/lock_guard.hpp>

namespace covellite
{

namespace api
{

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
*  Функция получения рендера для указанного компонента.
* \details
*  - Если для компонента (с его идентификатором) рендер уже существует, будет
*  возвращен существующий (с увеличением счетчика ссылок, см. описание функции
*  Remove()), если нет - будет создан новый.
*  - Для компонентов неподдерживаемых типов будет сделана запись в лог и
*  возвращен рендер, содержащий функцию-заглушку, не выполняющую никакого
*  действия.
*  - Для компонентов, для которых создается render, содержащий nullptr, будет
*  сделана запись в лог и возвращен рендер, содержащий функцию-заглушку,
*  не выполняющую никакого действия.
*  - Если при создании рендера компонента будет брошено исключение, будет
*  сделана запись в лог и возвращен рендер, содержащий функцию-заглушку,
*  не выполняющую никакого действия.
*
* \param [in] _pComponent
*  Исходный компонент.
*
* \return
*  Рендер для входного компонента.
*/
auto Component::Renders::Obtain(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto itRender = m_AllExistingRenders.find(_pComponent->Id);
  if (itRender != ::std::end(m_AllExistingRenders))
  {
    itRender->second.first++;
    return itRender->second.second;
  }

  auto itCreator = m_Creators.find(_pComponent->Type);
  if (itCreator == ::std::end(m_Creators))
  {
    LOGGER(Warning) << "Creating a component of an unknown type: " <<
      _pComponent->Type << " [" << "id: " << _pComponent->Id << "].";

    return [](void) {};
  }

  Render_t Result;

  try
  {
    Result = itCreator->second(_pComponent);
  }
  catch (const ::std::exception & _Ex)
  {
    LOGGER(Error) << "Create render fail: " << _Ex.what() << " ["
      << "id: " << _pComponent->Id << ", "
      << "type: " << _pComponent->Type << "].";

    return [](void) {};
  }

  if (Result == nullptr)
  {
    LOGGER(Warning) << "Received an empty render: " << "["
      << "id: " << _pComponent->Id << ", "
      << "type: " << _pComponent->Type << "].";

    return [](void) {};
  }

  m_AllExistingRenders[_pComponent->Id] = { 1, Result };
  return Result;
}

/**
* \brief
*  Функция удаления рендера для указанного компонента.
* \details
*  - Для каждого компонента функция уменьшает на 1 счетчик соответствующего ему
*  рендера; когда счетчик уменьшается до нуля, рендер удаляется.
*/
void Component::Renders::Remove(const ComponentPtr_t & _pComponent)
{
  auto itRender = m_AllExistingRenders.find(_pComponent->Id);
  if (itRender == m_AllExistingRenders.end()) return;

  itRender->second.first--;

  if (itRender->second.first == 0)
  {
    m_AllExistingRenders.erase(itRender);
  }
}

} // namespace api

} // namespace covellite
