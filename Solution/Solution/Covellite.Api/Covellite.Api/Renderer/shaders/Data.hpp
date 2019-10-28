
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
*  ��������� ��� �������� ������� ���������� �� �������.
* \details
*  - ��������� ������ ��������� ������� ������� ������� � ������ ����������
*  �����, ������� ��� ��������.
*
* \version
*  1.0.0.0        \n
* \date
*  10 �������� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
struct Object;

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
*  ��������� ��� �������� ������� ���������� � �������� ���������� ����� �������.
*
* \version
*  1.0.0.0        \n
* \date
*  14 �������� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
struct Points_t;

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������� ���������� �� ���������� ����� �������.
*
* \version
*  1.0.0.0        \n
* \date
*  14 �������� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
struct Lights_t;

/**
* \deprecated
*  ��������� �������� � ����� ������� � ��������� ���������� ������.
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������� ���������� � �������� ���������� ����� �����.
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
struct ScenePoints;

/**
* \deprecated
*  ��������� �������� � ����� ������� � ��������� ���������� ������; ������
*  ��� ������������ ��������� Object, ����������� ����� ��������� ������������
*  ������.
* \ingroup CovelliteApiFxGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������� ��� �������� ������� ���������� �� ���������� ����� �����.
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
struct SceneLights;

/**
* \deprecated
*  ��������� �������� � ����� ������� � ��������� ���������� ������; ������
*  ��� ������������ ��������� Object, ����������� ����� ��������� ������������
*  ������.
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
* \def COVELLITE_MAX_LIGHT_POINT_SCENE_COUNT
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ������ ������������� ���������� �������� ���������� �����.
* \details
*  - ���������� �������� � 10 ��� �������� � ������� fps � ~2 ����.
*/
