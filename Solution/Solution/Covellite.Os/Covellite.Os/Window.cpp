
#include "stdafx.h"
#include <Covellite/Os/Window.hpp>
#include <Covellite/Events/Events.hpp>

using namespace covellite::os;

void Window::Subscribe(const EventHandlerPtr_t &) /*override*/
{
}

Window::operator Window::Events_t (void) const /*override*/
{ 
  return m_Events; 
}

/**
* \brief
*  Функция получения хэндла окна для конкретной операционной системы.
*/
Handle_t Window::GetHandle(void) const /*override*/
{
  return m_Handle;
}

/**
* \brief
*  Функция получения объекта доступа к параметрам системы.
*/
const Window::Configuration_t & Window::GetConfiguration(void) const /*override*/
{
  return m_Configuration;
}
