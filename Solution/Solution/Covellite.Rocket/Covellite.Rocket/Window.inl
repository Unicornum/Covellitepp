
#pragma once

namespace covellite
{

namespace rocket
{

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

} // namespace rocket

} // namespace covellite
