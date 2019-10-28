
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
auto GraphicApi::CapturingServiceComponent::Get(
  const Expected_t & _Expected) -> ::std::vector<ComponentPtr_t>
{
  ::std::vector<ComponentPtr_t> Result;

  for (const auto & Info : _Expected)
  {
    auto itResult = ::std::find_if(m_Components.rbegin(), m_Components.rend(),
      [&](const ComponentPtr_t & _pComponent)
    { return Info.first == _pComponent->Kind; });

    if (itResult != m_Components.rend())
    {
      Result.push_back(*itResult);
      m_Components.erase(::std::next(itResult).base());
    }
    else
    {
      Result.push_back(Info.second);
    }
  }

  if (!m_Components.empty())
  {
    // 14 Февраль 2019 16:29 (unicornum.verum@gmail.com)
    TODO("Писать в лог warning'и о лишних компонентах.");

    m_Components.clear();
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
void GraphicApi::CapturingServiceComponent::Process(
  const Handlers_t & _PreRenders)
{
  for (; !m_Components.empty(); m_Components.pop_front())
  {
    const auto pComponent = m_Components.front();

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
}

} // namespace renderer

} // namespace api

} // namespace covellite
