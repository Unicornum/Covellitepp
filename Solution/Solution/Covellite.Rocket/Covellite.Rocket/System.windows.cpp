
#include "stdafx.h"
#include <Covellite\Rocket\System.hpp>
#include <chrono>
#include <windows.h>

namespace covellite
{

namespace rocket
{

/**
* \brief
*  Функция получения времени, прошедшего с момента запуска программы.
*/
float System::GetElapsedTime(void) /*override*/
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
void System::ActivateKeyboard(void) /*override*/
{
}

/**
* \brief
*  Функция деактивации экранной клавиатуры.
*/
void System::DeactivateKeyboard(void) /*override*/
{
}

} // namespace rocket

} // namespace covallite
