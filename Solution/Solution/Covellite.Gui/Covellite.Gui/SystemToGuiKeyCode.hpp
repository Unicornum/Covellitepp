
// ��� ����� - ��� ������.
#ifndef COVELLITE_ROCKET_SYSTEMTOGUIKEYCODE_HPP
#define COVELLITE_ROCKET_SYSTEMTOGUIKEYCODE_HPP

namespace covellite
{

namespace gui
{

using KeyCode_t = CovelliteGui::Input::KeyIdentifier;

/**
* \ingroup CovelliteGuiGroup
* \brief
*  ������� ������ � ������ \ref CovelliteGuiPage \n
*  ������� �������������� ��������� ����� ������� ������ � ���� ������������
*  ���������� GUI.
*/
KeyCode_t SystemToGuiKeyCode(int32_t);

} // namespace gui

} // namespace covellite

#endif // COVELLITE_ROCKET_SYSTEMTOGUIKEYCODE_HPP
