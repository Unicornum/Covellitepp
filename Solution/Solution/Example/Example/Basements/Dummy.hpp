
#pragma once
#include "IBasement.hpp"

namespace basement
{

/**
* \ingroup ExampleBasementGroup
* \brief
*   ласс входит в проект \ref ExamplePage \n
*   ласс-заглушка, не выполн€ющий никакой отрисовки.
*
* \version
*  1.0.0.0        \n
* \date
*  28 январь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Dummy final :
  public IBasement
{
public:
  void Render(void) override {}
};

} // namespace basement
