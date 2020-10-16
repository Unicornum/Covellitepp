
#pragma once
#include <cstdint>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс формата вертексного буфера.
* \details
*  - Формат класса вершин должен соответствовать фомату входных параметров 
*  вертексного шейдера в файле Input.fx.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  2.0.0.0        \n
* \date
*  27 Ноябрь 2018    \n
*  23 Февраль 2019    \n
*  19 Август 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018 - 2019
*/
class Vertex final
{
public:
  float px, py, pz, pw; ///< Position
  float tu, tv;         ///< Texture coords
  float ex, ey, ez, ew; ///< Extra parameters (color, normal, etc)
};

} // namespace api

} // namespace covellite
