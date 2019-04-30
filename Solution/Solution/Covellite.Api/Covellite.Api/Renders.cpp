
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
    else if (_pComponent->Type == uT("Updater"))
    {
      Result = CreateUpdater(_pComponent);
    }
  }
  catch (const ::std::exception & _Ex)
  {
    LOGGER(Error) << "Create render fail: " << _Ex.what() << " ["
      << "id: " << _pComponent->Id << ", "
      << "type: " << _pComponent->Type << "].";
    return nullptr;
  }

  if (Result != nullptr)
  {
    m_AllExistingRenders[_pComponent->Id] = { 1, Result };
  }
  else
  {
    // 10 Декабрь 2018 19:11 (unicornum.verum@gmail.com)
    TODO("Писать в лог??? А если он и должен игнорироваться, как шейдер для OpenGL?");
  }

  return Result;
}

auto Component::Renders::CreateUpdater(const ComponentPtr_t & _pComponent) -> Render_t
{
  // Реализация компонента обновления; реализация рабочая (см. реализацию
  // класса Simple3DObject), но есть проблемы:
  // - Рендерятся объекты, попадающие в поле зрения камеры, а обновлять
  // нужно объекты рядом с камерой (включая те, что позади).
  // - Все updater'ы всех объектов в рамках рендеринга одного кадра должны
  // получать одно и то же значение времени.

  using Updater_t = ::std::function<void(const float)>;
  const Updater_t Empty = [](const float) {};

  const auto Updater = _pComponent->GetValue(uT("function"), Empty);

  return [Updater](void)
  {
    static const auto Begin = ::std::chrono::system_clock::now();

    const ::std::chrono::duration<float> Time = 
      (::std::chrono::system_clock::now() - Begin);

    Updater(Time.count());
  };
}
