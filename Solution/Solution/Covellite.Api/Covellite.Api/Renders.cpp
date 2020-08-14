
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

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо
*  нее следует использовать функцию получения одного рендера для одного
*  компонента, передавая вспомогательные компоненты как параметр service.
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
  using Service_t = ::std::vector<ComponentPtr_t>;

  // 23 Март 2019 18:20 (unicornum.verum@gmail.com)
  TODO("Тест работы в многопоточном режиме.");
  // Когда будут использоваться разные объекты этого класса, синхронизация
  // должная быть реализована общая.
  //::boost::lock_guard<::boost::mutex> Lock(m_Mutex);

  Renders_t Result;
  Result.reserve(m_MaxRendersCount);

  Service_t Service;
  BinaryData_t TextureData;
  BinaryData_t ShaderData;
  Buffer_t<::covellite::api::Vertex> VertexBufferData;
  Buffer_t<int> IndexBufferData;

  const auto IsExistsData = [](const Component & _Component)
  {
    try
    {
      _Component[uT("data")];
      return true;
    }
    catch (const ::std::exception &) {}

    return false;
  };

  for (const auto & _pComponent : _Object)
  {
    if (_pComponent->Type == uT("Data"))
    {
      Service.push_back(_pComponent);
    }
    else if (!Service.empty())
    {
      (*_pComponent)[uT("service")] = Service;
      Service.clear();
    }

    Render_t ComponentRender;

    auto itRender = m_AllExistingRenders.find(_pComponent->Id);
    if (itRender != ::std::end(m_AllExistingRenders))
    {
      itRender->second.first++;
      ComponentRender = itRender->second.second;
    }
    else
    {
      if (IsExistsData(*_pComponent))
      {
        // Преобразование способа хранения данных из пары (data, count)
        // в (content); нужно для обеспечения обратной совместимости, потом все
        // это будет удалено вместе со всей функцией.
        // Сюда перенесено для того, чтобы копия данных хранилась только
        // в тех компонентах, для которых создается рендер (иначе программа
        // занимает огромное количество памяти).

        using Vertex_t = ::covellite::api::Vertex;

        if ((*_pComponent)[uT("data")].IsType<const uint8_t *>())
        {
          if (_pComponent->Type == uT("Texture") || _pComponent->Kind == uT("Texture"))
          {
            const uint8_t * pData = (*_pComponent)[uT("data")];
            const int Width = (*_pComponent)[uT("width")];
            const int Height = (*_pComponent)[uT("height")];
            (*_pComponent)[uT("content")] = BinaryData_t{ pData, 
              pData + (static_cast<size_t>(Width) * Height * 4) };
          }
          else
          {
            const uint8_t * pData = (*_pComponent)[uT("data")];
            const ::std::size_t Count = (*_pComponent)[uT("count")];
            (*_pComponent)[uT("content")] = BinaryData_t{ pData, pData + Count };
          }
        }
        else if ((*_pComponent)[uT("data")].IsType<const Vertex_t *>())
        {
          const Vertex_t * pData = (*_pComponent)[uT("data")];
          const ::std::size_t Count = (*_pComponent)[uT("count")];
          (*_pComponent)[uT("content")] = Buffer_t<Vertex_t>{ pData, pData + Count };
        }
        else if ((*_pComponent)[uT("data")].IsType<const int *>())
        {
          const int * pData = (*_pComponent)[uT("data")];
          const ::std::size_t Count = (*_pComponent)[uT("count")];
          (*_pComponent)[uT("content")] = Buffer_t<int>{ pData, pData + Count };
        }
      }

      ComponentRender = Create(_pComponent);
    }

    if (ComponentRender) Result.push_back(ComponentRender);
  }

  m_MaxRendersCount = ::std::max(m_MaxRendersCount, Result.size());

  return Result;
}

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо
*  нее следует использовать функцию удаления одного рендера для одного
*  компонента.
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

} // namespace api

} // namespace covellite
