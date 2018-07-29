
#pragma once
#include <Covellite/Rocket/Layer.hpp>

namespace covellite
{

namespace rocket
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
*  ������ ������������� �� covellite::rocket::Layer.
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
*  ������ ������������� �� covellite::rocket::Layer.
*/
template<class TLayer>
void Window::PushLayer(void)
{
  static_assert(::std::is_convertible<TLayer &, covellite::rocket::Layer &>::value,
    "TLayer must inherit covellite::rocket::Layer.");

  m_Layers.Push(::std::make_shared<TLayer>(*this));
}

} // namespace rocket

} // namespace covellite
