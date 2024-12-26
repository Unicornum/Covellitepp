
#include "stdafx.h"
#include <Covellite\Gui\System.hpp>
#include <chrono>

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
  const auto Now = std::chrono::system_clock::now();

  static const auto StartTime = Now;

  using namespace ::std::chrono;
  return duration_cast<microseconds>(Now - StartTime).count() / 1000000.0f;
}

/**
* \brief
*  Функция активации экранной клавиатуры.
*/
void System::ActivateKeyboard(Rml::Vector2f, float) /*override*/
{
}

/**
* \brief
*  Функция деактивации экранной клавиатуры.
*/
void System::DeactivateKeyboard(void) /*override*/
{
}

} // namespace gui

} // namespace covallite
