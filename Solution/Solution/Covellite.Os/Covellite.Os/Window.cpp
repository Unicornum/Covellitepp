
#include "stdafx.h"
#include <Covellite\Os\Window.hpp>

using namespace covellite::os;

void Window::Subscribe(const EventHandlerPtr_t &) /*override*/
{
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
