
#pragma once
#include "Layer.hpp"

namespace layers
{

/**
* \ingroup ExampleGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� ���� ������ ������ � ������ ��������.
*  
* \version
*  1.0.0.0        \n
* \date
*  19 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class Text final :
  public Layer
{
public:
  explicit Text(IWindowGui_t &);
};

} // namespace layers
