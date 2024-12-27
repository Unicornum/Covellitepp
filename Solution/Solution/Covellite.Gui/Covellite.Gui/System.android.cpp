
#include "stdafx.h"
#include <Covellite\Gui\System.hpp>
#include <alicorn\platform\app-info.hpp>
#include <alicorn\platform\environment.hpp>
#include <alicorn\std.memory\trivial-ptr.inl>

namespace covellite
{

namespace gui
{

/**
* \brief
*  Функция получения времени, прошедшего с момента запуска программы.
*/
CovelliteGuiTime_t System::GetElapsedTime(void) /*override*/
{
  struct timeval Now;
  gettimeofday(&Now, nullptr);

  static auto StartTime = Now;

  auto sec = Now.tv_sec - StartTime.tv_sec;
  auto usec = Now.tv_usec - StartTime.tv_usec;
  return sec + (usec / 1000000.0f);
}

/**
* \brief
*  Функция активации экранной клавиатуры.
*/
void System::ActivateKeyboard(CovelliteGui::Vector2f, float) /*override*/
{
  ::alicorn::system::platform::Environment{}.ShowSoftKeyboard();
}

/**
* \brief
*  Функция деактивации экранной клавиатуры.
*/
void System::DeactivateKeyboard(void) /*override*/
{
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  ANativeActivity_hideSoftInput(&AppInfo_t::Get<ANativeActivity>(), 0);
}

} // namespace gui

} // namespace covallite
