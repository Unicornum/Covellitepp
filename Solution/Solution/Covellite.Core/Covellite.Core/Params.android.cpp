
#include "stdafx.h"
#include <Covellite\Core\Params.hpp>
#include <map>

using namespace covellite::core::params;

/**
* \brief
*  Функция для преобразования 'сырого' кода нажатой кнопки в код libRocket.
*  
* \return
*  Код клавиши libRocket для кода AsciiKeyCode.
* \return \b KI_UNKNOWN
*  Код клавиши не обрабатывается.
*/
auto KeyCode::ToRocketKeyCode(void) const -> RocketKeyCode_t
{
  static const ::std::map<int32_t, RocketKeyCode_t> Keys =
  {
    { AKEYCODE_DEL, Rocket::Core::Input::KI_BACK }, // BackSpace
  };

  auto itKey = Keys.find(AsciiKeyCode);
  if (itKey == Keys.end()) return RocketKeyCode_t::KI_UNKNOWN;

  return itKey->second;
}
