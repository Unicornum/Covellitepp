
#pragma once
#include "Layer.hpp"

namespace layers
{

class Help_t
{
public:
  // �������������� ������� ������ ������� �������.
  enum Id
  {
    Title = 0,
    Text,
  };
};

namespace { Help_t Help; }

/**
* \ingroup ExampleGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� ���� �������� ����.
* \details
*  ��������� � ����� �������� ����� ������� �������� ����� ��������� (� �����
*  ������� � �������� ��������� ��������� ������ alicorn::extension::std::String):
*  - Help.Title - ���������.
*  - Help.Text - �����.
*
* \version
*  1.0.0.0        \n
* \date
*  14 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class Description final :
  public Layer
{
public:
  explicit Description(IWindowGui_t &);
};

} // namespace layers
