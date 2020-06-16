
#include "stdafx.h"
#include "GraphicApi.hpp"
#include "Component.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

GraphicApi::CapturingServiceComponent::CapturingServiceComponent(
  Components_t & _Components) :
  m_Components(_Components)
{

}

/**
* \brief
*  Функция получения набора вспомогательных компонентов.
* \details
*  - Функция предназначена для получения набора, гарантированно содержащего
*  требуемое количество вспомогательных компонент.
*  - Найденные компоненты (добавленные в выходной набор) удаляются из очереди.
*  - После вызова функции очередь компонентов будет пустой.
*
* \param [in] _Expected
*  Информация об ожидаемом наборе компонентов, в котором каждый элемент
*  должен содержать значение \b kind искомого компонента и компонент, который
*  будет помещен в выходной набор, если компонента с указанным \b kind
*  в очереди вспомогательных компонентов не существует.
*
* \return
*  Набор компонентов, в котором:
*  - Количество компонентов в точности совпадает с количеством элементов
*  входного набора.
*  - Компоненты расположены в том порядке значений \b kind, что и во входном
*  наборе.
*/
/*static*/ auto GraphicApi::CapturingServiceComponent::Get(
  const ComponentPtr_t & _pMainComponent,
  const Expected_t & _Expected) -> Services_t
{
  Services_t & Source = (*_pMainComponent)[uT("service")].Default(Services_t{});
  Services_t Result;

  for (const auto & Info : _Expected)
  {
    auto itResult = ::std::find_if(Source.rbegin(), Source.rend(),
      [&](const ComponentPtr_t & _pComponent)
    { return Info.first == _pComponent->Kind; });

    if (itResult != Source.rend())
    {
      Result.push_back(*itResult);
      Source.erase(::std::next(itResult).base());
    }
    else
    {
      Result.push_back(Info.second);
    }
  }

  if (!Source.empty())
  {
    // 14 Февраль 2019 16:29 (unicornum.verum@gmail.com)
    TODO("Писать в лог warning'и о лишних компонентах.");

    Source.clear();
  }

  return Result;
}

/**
* \brief
*  Функция для последовательной обработки очереди вспомогательных компонентов.
* \details
*  - Функция проходит по очереди вспомогательных компонентов и для каждого
*  компонента вызывает указанный для него (по параметру \b kind) обработчик,
*  передавая ему в качестве параметра этот компонент.
*  - Обработчики будут вызываться в том порядке, в котором вспомогательные
*  компоненты были добавлены в очередь.
*  - После вызова функции очередь компонентов будет пустой.
*
* \param [in] _PreRenders
*  Исходный набор обработчиков компонентов.
*/
/*static*/ void GraphicApi::CapturingServiceComponent::Process(
  const ComponentPtr_t & _pMainComponent,
  const Handlers_t & _PreRenders)
{
  Services_t & Source = (*_pMainComponent)[uT("service")].Default(Services_t{});

  for (const auto & pComponent : Source)
  {
    auto itComponent = _PreRenders.find(pComponent->Kind);
    if (itComponent != _PreRenders.end())
    {
      itComponent->second(pComponent);
    }
    else
    {
      // 13 Декабрь 2018 14:22 (unicornum.verum@gmail.com)
      TODO("Писать в лог warning о лишнем компоненте.");
    }
  }

  Source.clear();
}

} // namespace renderer

} // namespace api

} // namespace covellite
