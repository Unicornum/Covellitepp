
// Так нужно - для тестов.
#ifndef COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP
#define COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP

// 15.03.2019:
// Избавиться не удалось, предварительное объявление для enum не прокатывает.
#include <Rocket/Core/Input.h>

namespace Rocket
{ 

namespace Core
{ 

template<typename> class StringBase;
using String = StringBase<char>;
template<typename> class Vector2;
using Vector2i = Vector2<int>;
class ElementDocument;
class Element;
class Context;

} // namespace Core

} // namespace Rocket

namespace covellite
{

namespace gui
{

using RocketKeyCode_t = Rocket::Core::Input::KeyIdentifier;

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Функция входит в проект \ref CovelliteGuiPage \n
*  Функция преобразования системных кодов нажатых кнопок в коды libRocket.
*/
RocketKeyCode_t SystemToRocketKeyCode(int32_t);

} // namespace gui

} // namespace covellite

#endif // COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP
