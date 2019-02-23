
#pragma once
#include "Layer.hpp"

namespace layers
{

/**
* \ingroup ExampleGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс слоя отображения 3D объектов.
*  
* \version
*  1.0.0.0        \n
* \date
*  15 Январь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Draw3DObject final :
  public Layer
{
private:
  int m_Fps = 0;
  int m_LastFps = 999;
  int m_Polygons = 12;
  int m_CubesCount = 1;
  int m_LightFlags = 0xFFFFFFFF; // Все включено

public:
  explicit Draw3DObject(IWindowGui_t &);
  ~Draw3DObject(void);
};

} // namespace layers
