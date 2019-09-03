
#pragma once

/**
* \defgroup CovelliteApiFxGroup Fx
* \ingroup CovelliteApiGroup
*  ������ �������� ��� �������� ������ ��������.
*/

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������ ������ �������.
* \details
*  - ������������ ������ � ������������ ������� WorldViewProjection �� ��
*  �� ������ �� ������������������ �� ��������� � ������������� ������
*  � ������� (����������� �� 600 ����� � 96 ���������).
*
* \version
*  1.0.0.0        \n
* \date
*  27 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
struct Camera;

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������� ������� �������.
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
struct Matrices;

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
struct Lights;

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
struct Fog;

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
struct Ambient_t;

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
struct Direction_t;

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
struct Point_t;

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
struct Points_t;

/**
* \ingroup CovelliteApiFxGroup
* \def MAX_LIGHT_POINT_COUNT
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ������ ������������� ���������� �������� ���������� �����.
* \details
*  - ���������� �������� � 10 ��� �������� � ������� fps � ~2 ����.
*/

#ifdef GLM_VERSION

using float4 = ::glm::vec4;
using float4x4 = ::glm::mat4;

#elif defined DIRECTX_MATH_VERSION

// ���������, ������������ ��������� HLSL, ������������� �� 16 ����, �������
// ��� ���������� �������� ������ ����� ������� ������������ ������ �����������
// ���������.
using float4 = ::DirectX::XMFLOAT4A;
using float4x4 = ::DirectX::XMMATRIX;

#endif // GLM_VERSION, DIRECTX_MATH_VERSION

#define mediump
#define lowp
using color_t = float4;
