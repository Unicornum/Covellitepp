
#pragma once
#include <Covellite/Gui/Layer.hpp>

namespace covellite
{

namespace gui
{

/**
* \brief
*  ������� ������� ������ ����.
* \details
*  - ������� ������� �������� ��� ������� ������ �������� � ������� ����
*  (������� � ����������� ���� �������������� ������������� ��� �������
*  ������ '�����').
*  - ��� ���������� ������ �������� '������' ��������� ���� ������� ��������
*  ���� �������� ���� ���������, ������������ �� ������� ��� ������ �������
*  �������� ����� � ����� ������� Back() �� ������� ������ �������� �������
*  ��������� ����.
*  - ��� ���������� ������ ��������� � ���� ������� (����� ����� ���� ��������
*  ����� ����������) ������� ������� ������ ������� ����, �������� ������, �
*  ����� ��� ������� ������ �������� � ���������� ���� �������� �������
*  Back() � ������� ������� ������ ����.
*
* \tparam TLayer
*  ����� ����, ������ �������� ������ ���� ������. \n
*  ������ ������������� �� covellite::gui::Layer.
*/
template<class TLayer>
TLayer & Window::PushLayer(void)
{
  static_assert(::std::is_convertible<TLayer &, covellite::gui::Layer &>::value,
    "TLayer must inherit covellite::gui::Layer.");

  const auto pLayer = ::std::make_shared<TLayer>(*this);
  PushLayer(pLayer);

  // 16 ���� 2019 14:30 (unicornum.verum@gmail.com)
  TODO("���� �������� ������ �� ��������� ������ ����");
  return *pLayer;
}

} // namespace gui

} // namespace covellite
