
#pragma once
#include <Covellite/Covellite.hpp>

namespace layers
{

/**
* \ingroup ExampleGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� �������� ������ ���������.
*  
* \version
*  1.0.0.0        \n
* \date
*  01 ������� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
/// [Layer example]
class MainScreen final :
  public ::covellite::gui::Layer
{
  /// [Layer example]

public:
  explicit MainScreen(IWindowGui_t &);
};

} // namespace layers
