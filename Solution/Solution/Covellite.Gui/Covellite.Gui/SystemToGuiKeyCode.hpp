
// Так нужно - для тестов.
#ifndef COVELLITE_ROCKET_SYSTEMTORGUIKEYCODE_HPP
#define COVELLITE_ROCKET_SYSTEMTOGUIKEYCODE_HPP

namespace covellite
{

namespace gui
{

using KeyCode_t = CovelliteGui::Core::Input::KeyIdentifier;

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Функция входит в проект \ref CovelliteGuiPage \n
*  Функция преобразования системных кодов нажатых кнопок в коды используемой
*  библиотеки GUI.
*/
KeyCode_t SystemToGuiKeyCode(int32_t);

} // namespace gui

} // namespace covellite

#endif // COVELLITE_ROCKET_SYSTEMTORGUIKEYCODE_HPP
