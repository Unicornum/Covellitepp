
#pragma once
#include <Covellite\Core\EventHandler.hpp>
#include <Covellite\Core\Params.hpp>

namespace covellite
{

namespace core
{

/**
* \brief
*  ������������� ��������� ������� � �������� ������� Event::System.
*
* \param [in] _Event
*  ������������� ���������� �������.
*
* \return
*  ������ ������� ��� ���������� �������.
*/
template<>
inline EventHandler::Signal_t & EventHandler::operator[] (Event::System _Event)
{
  return OnSystem[_Event];
}

/**
* \brief
*  ������������� ��������� ������� � �������� ������� Event::Window.
*
* \param [in] _Event
*  ������������� ���������� �������.
*
* \return
*  ������ ������� ��� ���������� �������.
*/
template<>
inline EventHandler::Signal_t & EventHandler::operator[] (Event::Window _Event)
{
  return OnWindow[_Event];
}

/**
* \brief
*  ������������� ��������� ������� � �������� ������� Event::Input.
*
* \param [in] _Event
*  ������������� ���������� �������.
*
* \return
*  ������ ������� ��� ���������� �������.
*/
template<>
inline EventHandler::Signal_t & EventHandler::operator[] (Event::Input _Event)
{
  return OnInput[_Event];
}

/**
* \brief
*  ������������� ��������� ������� � �������� ������� Event::Framework.
*
* \param [in] _Event
*  ������������� ���������� �������.
*
* \return
*  ������ ������� ��� ���������� �������.
*/
template<>
inline EventHandler::Signal_t & EventHandler::operator[] (Event::Framework _Event)
{
  return OnFramework[_Event];
}

/**
* \brief
*  �������� ������� � �������� ������� Event::Rocket.
*
* \param [in] _Event
*  ������������� ���������� �������.
*
* \return
*  ����� �������� ��� ���������� �������.
*/
inline EventHandler::OnElement_t & EventHandler::operator[] (Event::Rocket _Event)
{
  return OnRocket[_Event];
}

/**
* \brief
*  ������� ��������� �������.
* \details
*  - ���� �� ����� ��������� ���������� ������� ���������� ����������,
*  ��� ����� ����������� � ����� ����������� ������ Event::Error, � ��������
*  �������� �������� ����� �������� �������� ���������� ����������.
*
* \param [in] _Event
*  ������������� ������������� �������.
* \param [in] _Params
*  ���������, ������������ ������������� �������.
*
* \return \b true
*  - ������� ���� ������������ ������.
* \return \b false
*  - �� ����� ��������� ������� ��������� ����������.
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
*  ������� �������� �� ������� ��������� ���������� libRocket.
* \details
*  ���� ������� ��. http://librocket.com/wiki/documentation/RML/Events
*
* \param [in] _pSource
*  ��������� �� ������ ��������� ������� (Rocket::Core::Context ���
*  Rocket::Core::ElementDocument).
*/
template<class TSource>
inline void EventHandler::Subscribe(TSource * _pSource)
{
  _pSource->AddEventListener("click", m_pClickEventListener.get(), false);
}

/**
* \brief
*  ������� ������� �� ������� ��������� ���������� libRocket.
* \details
*  ���� ������� ��. http://librocket.com/wiki/documentation/RML/Events
*
* \param [in] _pSource
*  ��������� �� ������ ��������� ������� (Rocket::Core::Context ���
*  Rocket::Core::ElementDocument).
*/
template<class TSource>
inline void EventHandler::Unsubscribe(TSource * _pSource)
{
  _pSource->RemoveEventListener("click", m_pClickEventListener.get(), false);
}

} // namespace core

} // namespace covellite
