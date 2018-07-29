﻿
#include <Covellite\Rocket\Rocket.forward.hpp>
#include <map>

namespace covellite
{

namespace rocket
{

RocketKeyCode_t SystemToRocketKeyCode(int32_t _AsciiKeyCode)
{
  static const ::std::map<int32_t, RocketKeyCode_t> Keys =
  {
    { AKEYCODE_DEL, Rocket::Core::Input::KI_BACK }, // BackSpace
  };

  // 01 Ноябрь 2017 10:59 (unicornum.verum@gmail.com)
  TODO("Преобразование остальных виртуальных кодов.");

  auto itKey = Keys.find(_AsciiKeyCode);
  if (itKey == Keys.end()) return RocketKeyCode_t::KI_UNKNOWN;

  return itKey->second;
}

} // namespace rocket

} // namespace covellite
