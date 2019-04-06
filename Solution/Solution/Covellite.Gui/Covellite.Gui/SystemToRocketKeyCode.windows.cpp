
#include "stdafx.h"
#include <Covellite/Gui/Rocket.forward.hpp>
#include <Rocket/Core/Input.h>
#include <windows.h>

namespace covellite
{

namespace gui
{

RocketKeyCode_t SystemToRocketKeyCode(int32_t _AsciiKeyCode)
{
  static const ::std::map<int32_t, RocketKeyCode_t> Keys =
  {
    { VK_BACK,    Rocket::Core::Input::KI_BACK }, // Backspace
    { VK_END,     Rocket::Core::Input::KI_END }, // End
    { VK_HOME,    Rocket::Core::Input::KI_HOME }, // Home
    { VK_LEFT,    Rocket::Core::Input::KI_LEFT }, // Left
    { VK_UP,      Rocket::Core::Input::KI_UP }, // Up
    { VK_RIGHT,   Rocket::Core::Input::KI_RIGHT }, // Right
    { VK_DOWN,    Rocket::Core::Input::KI_DOWN }, // Down
    { VK_DELETE,  Rocket::Core::Input::KI_DELETE }, // Del
  };

  // 01 Ноябрь 2017 10:59 (unicornum.verum@gmail.com)
  TODO("Преобразование остальных виртуальных кодов.");

  auto itKey = Keys.find(_AsciiKeyCode);
  if (itKey == Keys.end()) return RocketKeyCode_t::KI_UNKNOWN;

  return itKey->second;
}

} // namespace gui

} // namespace covellite
