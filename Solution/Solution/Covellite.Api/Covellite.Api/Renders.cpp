
#include "stdafx.h"
#include <Covellite/Api/Renders.hpp>

using namespace covellite::api;

/**
* \brief
*  Конструктор класса.
*  
* \param [in] _Creators
*  Набор функций создающих рендеры для соответствующих типов компонентов.
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
*  в результат будет добавлен существующий, если нет - будет создан новый.
*  - Компоненты неподдерживаемых типов будут проигнорированы.
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
  Renders_t Result;

  for (const auto & _pComponent : _Object)
  {
    auto itComponent = m_AllExistingRenders.find(_pComponent->Id);
    if (itComponent != m_AllExistingRenders.end())
    {
      Result.push_back(itComponent->second);
      continue;
    }

    auto itCreator = m_Creators.find(_pComponent->Type);
    if (itCreator != m_Creators.end())
    {
      auto ComponentRender = itCreator->second(_pComponent);
      m_AllExistingRenders[_pComponent->Id] = ComponentRender;
      Result.push_back(ComponentRender);
    }
  }

  return Result;
}

/**
* \brief
*  Функция удаления рендеров для указанных компонентов.
*/
void Component::Renders::Remove(const Object_t & _Object)
{
  for (const auto & _pComponent : _Object)
  {
    m_AllExistingRenders.erase(_pComponent->Id);
  }
}
