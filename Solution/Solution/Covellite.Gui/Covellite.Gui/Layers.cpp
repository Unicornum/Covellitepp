
#include "stdafx.h"
#include <Covellite\Gui\Layers.hpp>
#include <Covellite\Gui\ILayer.hpp>
#include <alicorn\std\exception.hpp>

using namespace covellite::gui;

/**
* \brief
*  ������� ������� ���� �� ������� �����.
* \details
*  - ������������ ���� �� ������� �����, ��������� ��������� ����, ����� ����
*  ���������� ���.
*  
* \param [in] _pLayer
*  ����������� ����.
*/
void Layers::Push(const LayerPtr_t & _pLayer) noexcept
{
  if (!m_Layers.empty()) m_Layers.back()->Hide();

  m_Layers.push_back(_pLayer);
  m_Layers.back()->Show();
}

/**
* \brief
*  ������� �������� ���� � ������� �����.
* \details
*  - ������������ � ������� ���� �� ������� �����, ����� ���� ���������� ����, 
*  ����������� �� ������� �����.
*  
* \return \b true
*  - �������� ��������� ������� (� ����� ��� ���� ���� �� ���� ����).
* \return \b false
*  - ��� ������ ��������� ����, ���� ����.
*  
* \exception std::exception
*  - ������� ������� ���� �� ������� �����.
*/
bool Layers::Pop(void)
{
  if (m_Layers.empty()) throw STD_EXCEPTION << "Empty stack.";

  m_Layers.back()->Hide();
  m_Layers.pop_back();

  if (m_Layers.empty()) return false;

  m_Layers.back()->Show();
  return true;
}
