
#pragma once
#include <alicorn/std/string.hpp>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  �����-��������� ��������� �������� ���������� �������.
* \details
*  - ������� ��������� ������� ������ ��������������� ������� �������
*  ���������� � ����� VertexInput.fx.
*  
* \version
*  1.0.0.0        \n
* \date
*  27 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class Vertex final
{
  using String_t = ::alicorn::extension::std::String;

public:
  /// \brief
  ///  �������� ����� ������� ���� ��� ���������� Gui.
  /// \details
  ///  - ������ ������� ��� ����������� �������� ����������� ������ ������ 
  /// (������� ��������������� ������� Rocket::Core::Vertex).
  class Gui final
  {
  public:
    float x, y;
    uint32_t ABGRColor;
    float u, v;

  public:
    inline static String_t GetName(void) { return uT("Vertex.Gui"); }
  };

  /// \brief
  ///  �������� ����� ������� ��� ���������� ������� ���������������� �����.
  class Textured final
  {
  public:
    float x, y, z;
    float nx, ny, nz;
    float tu, tv;

  public:
    inline static String_t GetName(void) { return uT("Vertex.Textured"); }
  };
};

} // namespace api

} // namespace covellite
