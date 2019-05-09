
#include "stdafx.h"
#include <Covellite/Events/Signal.hpp>
#include <Covellite/Events/Params.hpp>
#include <Covellite/Events/Error.hpp>
#include <Covellite/Events/Events.inl>
#include "Signals.hpp"

using namespace covellite::events;

Events::Signal::Signal(Events & _Events, size_t _EventType, size_t _EventValue) :
  m_Events(_Events),
  m_Signal((*m_Events.m_pSignals).m_Signals[_EventType][_EventValue])
{

}

/**
* \brief
*  Функция подписки на событие.
* \details
*  - Функция будет вызвана при активации события.
*  - Одному событию может быть назначено несколько функций (они будут вызваны
*  в порядке установки).
*
* \param [in] _Fn
*  Функция-обработчик выбранного события (без параметров).
*/
void Events::Signal::Connect(const FnVoid_t & _Fn)
{
  auto Fn = [=](const Params &) { _Fn(); };
  m_Events.m_Connections.push_back(m_Signal.connect(Fn));
}

/**
* \brief
*  Функция подписки на событие.
* \details
*  - Функция будет вызвана при активации события.
*  - Одному событию может быть назначено несколько функций (они будут вызваны
*  в порядке установки).
*  
* \param [in] _Fn
*  Функция-обработчик выбранного события (с одним параметром).
*/
void Events::Signal::Connect(const FnParams_t & _Fn)
{
  m_Events.m_Connections.push_back(m_Signal.connect(_Fn));
}

/**
* \brief
*  Оператор активации события без параметров.
* \details
*  - Будут вызваны все функции, назначенные событию.
*  - Функция вернет управление, когда работа всех обработчиков будет завершена.
*
* \return \b true
*  Вызов обработчиков события прошел успешно.
* \return \b false
*  При вызове одного из обработчиков события произошло исключение.
*/
bool Events::Signal::operator() (void)
{
  return Activate(0);
}

/**
* \brief
*  Оператор активации события.
* \details
*  - Будут вызваны все функции, назначенные событию.
*  - Функция вернет управление, когда работа всех обработчиков будет завершена.
*
* \param [in] _Value
*  Параметр, передаваемый функциям-обработчикам, подписанным на выбранное
*  событие.
*  
* \return \b true
*  Вызов обработчиков события прошел успешно.
* \return \b false
*  - При вызове одного из обработчиков события произошло исключение.
*  - Активирован слот, которому не назначено ни одного обработчика.
*/
bool Events::Signal::operator() (const Params & _Value)
{
  return Activate(_Value);
}

bool Events::Signal::Activate(const Params & _Value)
{
  try
  {
    m_Signal(_Value);
  }
  catch (const ::std::exception & _Ex)
  {
    m_Events[Error.Exception](Error_t::Description{ _Ex.what() });
    return false;
  }

  // При активации слота, которому не назначены обработчики нужно вернуть
  // false, т.к. для событий Windows, которые не обрабатываются, необходимо
  // вызывать обработчик по умолчанию.
  return !m_Signal.empty();
}
