
#pragma once
#include <cstdint>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ����� ������� ����������� ������.
* \details
*  - ������ ������ ������ ������ ��������������� ������ ������� ���������� 
*  ����������� ������� � ����� Input.fx.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  2.0.0.0        \n
* \date
*  27 ������ 2018    \n
*  23 ������� 2019    \n
*  19 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018 - 2019
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
