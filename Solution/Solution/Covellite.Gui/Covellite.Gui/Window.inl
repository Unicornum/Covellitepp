
#pragma once
#include <Covellite/Gui/Layer.hpp>

namespace covellite
{

namespace gui
{

/**
* \deprecated
*  ������� �������� � ����� ������� � ��������� ���������� ������. \n
*  ������ ��� ����������� ������� PushLayer().
* \brief
*  ������� ������� ������ ����.
* \details
*  - ������� ������� �������� ��� ������� ������ �������� � ������� ����
*  (������� � ����������� ���� �������������� ������������� ��� ������� 
*  ������ '�����').
*  
* \tparam TLayer
*  ����� ����, ������ �������� ������ ���� ������. \n
*  ������ ������������� �� covellite::gui::Layer.
*  
* \return
*  ��������� ������ ����.
*/
template<class TLayer>
::std::shared_ptr<TLayer> Window::AddLayer(void)
{
  auto pLayer = ::std::make_shared<TLayer>(m_pContext.get());
  m_Layers.Push(pLayer);
  pLayer->Subscribe(m_pEvents);
  return pLayer;
}

/**
* \brief
*  ������� ������� ������ ����.
* \details
*  - ������� ������� �������� ��� ������� ������ �������� � ������� ����
*  (������� � ����������� ���� �������������� ������������� ��� �������
*  ������ '�����').
*
* \tparam TLayer
*  ����� ����, ������ �������� ������ ���� ������. \n
*  ������ ������������� �� covellite::gui::Layer.
*/
template<class TLayer>
void Window::PushLayer(void)
{
  static_assert(::std::is_convertible<TLayer &, covellite::gui::Layer &>::value,
    "TLayer must inherit covellite::gui::Layer.");

  m_Layers.Push(::std::make_shared<TLayer>(*this));
}

} // namespace gui

} // namespace covellite
