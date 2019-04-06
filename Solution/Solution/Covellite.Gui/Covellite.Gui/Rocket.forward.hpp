
// ��� ����� - ��� ������.
#ifndef COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP
#define COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP

// 15.03.2019:
// ���������� �� �������, ��������������� ���������� ��� enum �� �����������.
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
*  ������� ������ � ������ \ref CovelliteGuiPage \n
*  ������� �������������� ��������� ����� ������� ������ � ���� libRocket.
*/
RocketKeyCode_t SystemToRocketKeyCode(int32_t);

} // namespace gui

} // namespace covellite

#endif // COVELLITE_ROCKET_SYSTEMTOROCKETKEYCODE_HPP
