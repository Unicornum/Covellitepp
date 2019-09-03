
#pragma once
#include <cstdint>

namespace covellite
{

namespace api
{

namespace vertex
{

/// \deprecated
///  ����� ������� � ����� ������ � ��������� ���������� ������, ������ ����
///  ������� ������������ ����� covellite::api::Vertex.
/// \brief
///  �������� ����� ������� ���� ��� ���������� ������� ��������.
/// \details
///  - ������ ������� ��� ����������� �������� ����������� ������ ������ 
/// (������� ��������������� ������� Rocket::Core::Vertex).
class [[deprecated]] Polygon
{
public:
  float x, y;
  uint32_t ABGRColor;
  float u, v;
};

/// \deprecated
///  ����� ������� � ����� ������ � ��������� ���������� ������, ������ ����
///  ������� ������������ ����� covellite::api::Vertex.
/// \brief
///  �������� ����� ������� ��� ���������� �������� �������.
class [[deprecated]] Polyhedron
{
public:
  float x, y, z;
  float nx, ny, nz;
  float tu, tv;
};

} // namespace vertex

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
  /**
  *\deprecated
  *  ����� ������� � ����� ������ � ��������� ���������� ������, ������
  *  ��� ������� ������������ ����� covellite::api::Vertex.
  */
  class [[deprecated]] Polygon
  {
  public:
    float x, y;
    uint32_t ABGRColor;
    float u, v;
  };

  /**
  *\deprecated
  *  ����� ������� � ����� ������ � ��������� ���������� ������, ������
  *  ��� ������� ������������ ����� covellite::api::Vertex.
  */
  class [[deprecated]] Polyhedron
  {
  public:
    float x, y, z;
    float nx, ny, nz;
    float tu, tv;
  };

public:
  float px, py, pz, pw; ///< Position
  float tu, tv;         ///< Texture coords
  float ex, ey, ez, ew; ///< Extra parameters (color, normal, etc)
};

} // namespace api

} // namespace covellite
