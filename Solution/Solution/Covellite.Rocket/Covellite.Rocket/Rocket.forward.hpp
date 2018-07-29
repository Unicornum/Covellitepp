
// Так нужно - для тестов.
#ifndef COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP
#define COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP
#include <Rocket/Core/Input.h>

namespace Rocket
{ 

namespace Core
{ 

class ElementDocument;
class Element;
class Context;

template<typename> class Vector2;
using Vector2i = Vector2<int>;

} // namespace Core

} // namespace Rocket

namespace covellite
{

namespace rocket
{

using RocketKeyCode_t = Rocket::Core::Input::KeyIdentifier;

/**
* \ingroup gCovelliteRocketClasses
* \brief
*  Функция входит в проект \ref CovelliteRocketPage \n
*  Функция преобразования системных кодов нажатых кнопок в коды libRocket.
*/
RocketKeyCode_t SystemToRocketKeyCode(int32_t);

} // namespace rocket

} // namespace covellite

#endif // COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP
