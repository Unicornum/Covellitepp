
#include "stdafx.h"
#include <Covellite/Api/Renders.hpp>

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
*  в результат будет добавлен существующий, если нет - будет создан новый.
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
  Renders_t Result;

  for (const auto & _pComponent : _Object)
  {
    auto itRender = m_AllExistingRenders.find(_pComponent->Id);

    const auto ComponentRender = (itRender == m_AllExistingRenders.end()) ?
      Create(_pComponent) : itRender->second;
    if (ComponentRender) Result.push_back(ComponentRender);
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

auto Component::Renders::Create(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto itCreator = m_Creators.find(_pComponent->Type);
  if (itCreator == m_Creators.end())
  {
    // 10 Декабрь 2018 19:11 (unicornum.verum@gmail.com)
    TODO("Писать в лог??? А если он и должен игнорироваться, как шейдер для OpenGL?");

    return nullptr;
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
    return nullptr;
  }

  if (Result) m_AllExistingRenders[_pComponent->Id] = Result;

  return Result;
}
