
#pragma once
#include "IBasement.hpp"

namespace basement
{

/**
* \ingroup ExampleBasementGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  �����-��������, �� ����������� ������� ���������.
*
* \version
*  1.0.0.0        \n
* \date
*  28 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class Dummy final :
  public IBasement
{
public:
  void Render(void) override {}
};

} // namespace basement
