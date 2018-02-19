
#include "stdafx.h"
#include <Covellite\Core\Window.hpp>

using namespace covellite::core;

/**
* \brief
*  Функция получения высоты статус-бара окна программы.
*  
* \return
*  - Windows: 0.
*  - Android: высота (в пикселях) панели уведомлений.
*/
int32_t Window::GetStatusBarHeight(void) const
{
  return covellite::jni::Environment().GetStatusBarHeight();
}

/**
* \brief
*  Функция завершения работы программы.
*  
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
void Window::Exit(void) const
{
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  ANativeActivity_finish(AppInfo_t::Get<ANativeActivity *>());
}
