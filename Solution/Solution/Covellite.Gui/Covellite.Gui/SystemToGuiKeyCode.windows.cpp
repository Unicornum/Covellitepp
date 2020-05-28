
#include "stdafx.h"
#include "SystemToGuiKeyCode.hpp"
#include <alicorn/platform/windows.hpp>
#include <windef.h>
#include <winuser.h>

namespace covellite
{

namespace gui
{

KeyCode_t SystemToGuiKeyCode(int32_t _AsciiKeyCode)
{
  static const ::std::map<int32_t, KeyCode_t> Keys =
  {
    { VK_BACK,    CovelliteGui::Core::Input::KI_BACK }, // Backspace
    { VK_END,     CovelliteGui::Core::Input::KI_END }, // End
    { VK_HOME,    CovelliteGui::Core::Input::KI_HOME }, // Home
    { VK_LEFT,    CovelliteGui::Core::Input::KI_LEFT }, // Left
    { VK_UP,      CovelliteGui::Core::Input::KI_UP }, // Up
    { VK_RIGHT,   CovelliteGui::Core::Input::KI_RIGHT }, // Right
    { VK_DOWN,    CovelliteGui::Core::Input::KI_DOWN }, // Down
    { VK_DELETE,  CovelliteGui::Core::Input::KI_DELETE }, // Del
  };

  // 01 ������ 2017 10:59 (unicornum.verum@gmail.com)
  TODO("�������������� ��������� ����������� �����.");

  auto itKey = Keys.find(_AsciiKeyCode);
  if (itKey == Keys.end()) return KeyCode_t::KI_UNKNOWN;

  return itKey->second;
}

} // namespace gui

} // namespace covellite