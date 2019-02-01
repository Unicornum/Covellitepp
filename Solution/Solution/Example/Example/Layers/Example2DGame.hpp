
#pragma once
#include "Layer.hpp"

namespace layers
{

/**
* \ingroup ExampleGroup
* \brief
*   ласс входит в проект \ref ExamplePage \n
*   ласс сло€ простой 2D игры.
*  
* \version
*  1.0.0.0        \n
* \date
*  29 январь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Simple2DGame final :
  public Layer
{
private:
  int m_Fps = 0;
  int m_LastFps = 999;

public:
  explicit Simple2DGame(IWindowGui_t &);
  ~Simple2DGame(void);
};

} // namespace layers
