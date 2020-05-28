
#pragma once
#include <Covellite/Gui/SystemToGuiKeyCode.hpp>

namespace covellite
{

namespace gui
{

class SystemToGuiKeyCodeProxy :
  public ::alicorn::extension::testing::Proxy<SystemToGuiKeyCodeProxy>
{
public:
  MOCK_METHOD1(Convert, KeyCode_t(int32_t));
};

inline KeyCode_t SystemToGuiKeyCode(int32_t _Code)
{
  return SystemToGuiKeyCodeProxy::GetInstance()->Convert(_Code);
}

} // namespace gui

} // namespace covellite
