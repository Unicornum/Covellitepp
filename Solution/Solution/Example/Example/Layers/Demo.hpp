
#pragma once
#include "Layer.hpp"

namespace layers
{

// 15 ������ 2019 18:30 (unicornum.verum@gmail.com)
TODO("�������� �������� ������.");
/**
* \ingroup ExampleGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  < ������� �������� ������ >
* \details
*  - ��������� �������� ������.
*  
* \version
*  1.0.0.0        \n
* \date
*  15 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*  
*  ������ �������������:
*  ---------------------
*  
* ### ������ ������
*  
* \code
// ����� ������ ���� ������ �������������...
// ...
* \endcode
*/
class Demo final :
  public Layer
{
public:
  explicit Demo(IWindowGui_t &);
};

} // namespace layers
