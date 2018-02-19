
#pragma once
#include <Covellite\Core\EventHandler.hpp>
#include <Covellite\Core\Params.hpp>

namespace covellite
{

namespace core
{

/**
* \brief
*  Специализация оператора доступа к сигналам событий Event::System.
*
* \param [in] _Event
*  Идентификатор требуемого события.
*
* \return
*  Объект сигнала для указанного события.
*/
template<>
inline EventHandler::Signal_t & EventHandler::operator[] (Event::System _Event)
{
  return OnSystem[_Event];
}

/**
* \brief
*  Специализация оператора доступа к сигналам событий Event::Window.
*
* \param [in] _Event
*  Идентификатор требуемого события.
*
* \return
*  Объект сигнала для указанного события.
*/
template<>
inline EventHandler::Signal_t & EventHandler::operator[] (Event::Window _Event)
{
  return OnWindow[_Event];
}

/**
* \brief
*  Специализация оператора доступа к сигналам событий Event::Input.
*
* \param [in] _Event
*  Идентификатор требуемого события.
*
* \return
*  Объект сигнала для указанного события.
*/
template<>
inline EventHandler::Signal_t & EventHandler::operator[] (Event::Input _Event)
{
  return OnInput[_Event];
}

/**
* \brief
*  Специализация оператора доступа к сигналам событий Event::Framework.
*
* \param [in] _Event
*  Идентификатор требуемого события.
*
* \return
*  Объект сигнала для указанного события.
*/
template<>
inline EventHandler::Signal_t & EventHandler::operator[] (Event::Framework _Event)
{
  return OnFramework[_Event];
}

/**
* \brief
*  Оператор доступа к сигналам событий Event::Rocket.
*
* \param [in] _Event
*  Идентификатор требуемого события.
*
* \return
*  Набор сигналов для указанного события.
*/
inline EventHandler::OnElement_t & EventHandler::operator[] (Event::Rocket _Event)
{
  return OnRocket[_Event];
}

/**
* \brief
*  Функция активации события.
* \details
*  - Если во время активации указанного события происходит исключение,
*  оно будет перехвачено и будет активирован сигнал Event::Error, в качестве
*  парамера которому будет передано описание возникшего исключения.
*
* \param [in] _Event
*  Идентификатор активируемого события.
* \param [in] _Params
*  Параметры, передаваемые активируемому событию.
*
* \return \b true
*  - Событие было активировано удачно.
* \return \b false
*  - Во время обработки события произошло исключение.
*/
template<class TEventType>
inline bool EventHandler::DoCommand(TEventType _Event, const Params & _Params)
{
  try
  {
    (*this)[_Event](_Params);
    return true;
  }
  catch (const ::std::exception & _Ex)
  {
    (*this)[Event::Error](params::Error{ _Ex.what() });
  }

  return false;
}

/**
* \brief
*  Функция подписки на события элементов управления libRocket.
* \details
*  Типы событий см. http://librocket.com/wiki/documentation/RML/Events
*
* \param [in] _pSource
*  Указатель на объект источника событий (Rocket::Core::Context или
*  Rocket::Core::ElementDocument).
*/
template<class TSource>
inline void EventHandler::Subscribe(TSource * _pSource)
{
  _pSource->AddEventListener("click", m_pClickEventListener.get(), false);
}

/**
* \brief
*  Функция отписки от событий элементов управления libRocket.
* \details
*  Типы событий см. http://librocket.com/wiki/documentation/RML/Events
*
* \param [in] _pSource
*  Указатель на объект источника событий (Rocket::Core::Context или
*  Rocket::Core::ElementDocument).
*/
template<class TSource>
inline void EventHandler::Unsubscribe(TSource * _pSource)
{
  _pSource->RemoveEventListener("click", m_pClickEventListener.get(), false);
}

} // namespace core

} // namespace covellite
