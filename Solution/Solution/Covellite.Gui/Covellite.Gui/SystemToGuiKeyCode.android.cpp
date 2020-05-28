
#include "SystemToGuiKeyCode.hpp"
#include <map>

namespace covellite
{

namespace gui
{

// cppcheck-suppress ConfigurationNotChecked
KeyCode_t SystemToGuiKeyCode(int32_t _AsciiKeyCode)
{
  static const ::std::map<int32_t, KeyCode_t> Keys =
  {
    { AKEYCODE_DEL, KeyCode_t::KI_BACK }, // BackSpace
  };

  // 01 Ноябрь 2017 10:59 (unicornum.verum@gmail.com)
  TODO("Преобразование остальных виртуальных кодов.");

  auto itKey = Keys.find(_AsciiKeyCode);
  if (itKey == Keys.end()) return KeyCode_t::KI_UNKNOWN;

  return itKey->second;
}

} // namespace gui

} // namespace covellite
