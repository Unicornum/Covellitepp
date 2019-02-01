
#pragma once
#include "Layer.hpp"

namespace layers
{

/**
* \ingroup ExampleGroup
* \brief
*   ласс входит в проект \ref ExamplePage \n
*   ласс сло€ вывода текста в разных форматах.
*  
* \version
*  1.0.0.0        \n
* \date
*  19 январь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Text final :
  public Layer
{
public:
  explicit Text(IWindowGui_t &);
};

} // namespace layers
