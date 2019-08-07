
#ifdef __cplusplus

#pragma once

using uint = uint32_t;

#ifdef GLM_VERSION

using float4 = ::glm::vec4;
using float4x4 = ::glm::mat4;

#elif defined DIRECTX_MATH_VERSION

// ���������, ������������ ��������� HLSL, ������������� �� 16 ����, �������
// ��� ���������� �������� ������ ����� ������� ������������ ������ �����������
// ���������.
using float4 = ::DirectX::XMFLOAT4A;
using float4x4 = ::DirectX::XMMATRIX;

#endif // DIRECTX_MATH_VERSION

#if BOOST_COMP_MSVC

#pragma warning(push)
#pragma warning(disable: 4324)

#endif // BOOST_COMP_MSVC

#endif // __cplusplus

#define BUFFER_VALUE(letter, index) letter##index
#define BUFFER_INDEX(index) BUFFER_VALUE(b, index)

#define MATRICES_BUFFER_INDEX 0
#define MATERIAL_BUFFER_INDEX 5
#define LIGHTS_BUFFER_INDEX 6
#define FOG_BUFFER_INDEX 7

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ������ ������������� ���������� �������� ���������� �����.
* \details
*  - ���������� �������� � 10 ��� �������� � ������� fps � ~2 ����.
*/
#define MAX_LIGHT_POINT_COUNT 16

/**
* \defgroup CovelliteApiFxGroup Fx
* \ingroup CovelliteApiGroup
*  ������ �������� ��� �������� ������ ��������.
*/

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������ �������, ������� ������������ DirectX.
* \details
*  - ������������ ������ � ������������ ������� WorldViewProjection �� ��
*  �� ������ �� ������������������ �� ��������� � ������������� ������
*  � ������� (����������� �� 600 ����� � 96 ���������).
*  
* \version
*  1.0.0.0        \n
* \date
*  10 �������� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
struct Matrices
{
  float4x4 World;
  float4x4 View;
  float4x4 Projection;
  float4x4 ViewInverse;
};

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������� ������ ���������.
*
* \version
*  1.0.0.0        \n
* \date
*  27 ������� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
struct Material
{
  uint ARGBAmbient;
  uint ARGBDiffuse;
  uint ARGBSpecular;
  uint ARGBEmission;
  float Shininess;
  float Align16bytes[3];
};

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ������ � ������ \ref CovelliteApiPage \n
*  ������������ ���� �������� ��� �������� ������� ���������� �� ���������� 
*  �����.
*
* \version
*  1.0.0.0        \n
* \date
*  03 ������� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
namespace Light
{

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������� ���������� � ������� ���������.
*
* \version
*  1.0.0.0        \n
* \date
*  03 ������� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
struct Ambient
{
  uint IsValid;
  uint ARGBColor;
};

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������� ���������� � ������������ ��������� �����.
*
* \version
*  1.0.0.0        \n
* \date
*  04 ������� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
struct Direction
{
  uint IsValid;
  uint ARGBColor;
  float4 Direction;
};

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ���������, ���������� ���������� �� ����� �������� ��������� �����.
*
* \version
*  1.0.0.0        \n
* \date
*  04 ������� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
struct Point
{
  uint ARGBColor;
  uint Align16bytes;
  float4 Position;
  float4 Attenuation; // Const, Linear, Exponent;
};

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������� ���������� � �������� ���������� �����.
*
* \version
*  1.0.0.0        \n
* \date
*  10 ������� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
struct Points
{
  Point Lights[MAX_LIGHT_POINT_COUNT];
  uint UsedSlotCount;
};

}; // namespace Light

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������� ���������� �� ���������� �����.
*
* \version
*  1.0.0.0        \n
* \date
*  10 ������� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
struct Lights
{
  ::Light::Ambient Ambient;
  ::Light::Direction Direction;
  ::Light::Points Points;
};

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������� ������ ������.
*
* \version
*  1.0.0.0        \n
* \date
*  30 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
struct Fog
{
  uint ARGBColor;
  float Near;
  float Far;
  float Density;
};

#ifdef __cplusplus

#if BOOST_COMP_MSVC
#pragma warning(pop)
#endif // BOOST_COMP_MSVC

#endif // __cplusplus
