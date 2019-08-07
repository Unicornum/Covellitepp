
#pragma once
#include <cstdint>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \deprecated
*  ����� ������� � ����� ������ � ��������� ���������� ������, ������ ����
*  ������������ ������ �� ������������ ���� vertex.
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  �����-��������� �������� ���������� �������.
* \details
*  - ������� ������� ������ ������ ��������������� ������� ������� ���������� 
*  � ����� Input.fx.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  27 ������ 2018    \n
*  23 ������� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018 - 2019
*/
class [[deprecated]] Vertex final
{
public:
  /// \brief
  ///  �������� ����� ������� ���� ��� ���������� ������� ��������.
  /// \details
  ///  - ������ ������� ��� ����������� �������� ����������� ������ ������ 
  /// (������� ��������������� ������� Rocket::Core::Vertex).
  class Polygon final
  {
  public:
    float x, y;
    uint32_t ABGRColor;
    float u, v;
  };

  /// \brief
  ///  �������� ����� ������� ��� ���������� �������� �������.
  class Polyhedron final
  {
  public:
    float x, y, z;
    float nx, ny, nz;
    float tu, tv;
  };
};

namespace vertex
{

using Polygon = Vertex::Polygon;
using Polyhedron = Vertex::Polyhedron;

} // namespace vertex

} // namespace api

} // namespace covellite
