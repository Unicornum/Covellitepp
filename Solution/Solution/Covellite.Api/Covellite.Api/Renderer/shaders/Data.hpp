
#pragma once

/**
* \defgroup CovelliteApiFxGroup Fx
* \ingroup CovelliteApiGroup
*  Группа структур для передачи данных шейдерам.
*/

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура для передачи матриц камеры шейдеру.
* \details
*  - Перемножение матриц и формирование матрицы WorldViewProjection на ЦП
*  не влияет на производительность по сравнению с перемножением матриц
*  в шейдере (проверялось на 600 кубах с 96 вершинами).
*
* \version
*  1.0.0.0        \n
* \date
*  27 Август 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
struct Camera;

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура для передачи мировой матрицы шейдеру.
* \details
*  - Перемножение матриц и формирование матрицы WorldViewProjection на ЦП
*  не влияет на производительность по сравнению с перемножением матриц
*  в шейдере (проверялось на 600 кубах с 96 вершинами).
*
* \version
*  1.0.0.0        \n
* \date
*  10 Сентябрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
struct Matrices;

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура для передачи шейдеру информации об источниках света.
*
* \version
*  1.0.0.0        \n
* \date
*  10 Декабрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
struct Lights;

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура для передачи шейдеру данных тумана.
*
* \version
*  1.0.0.0        \n
* \date
*  30 Июль 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
struct Fog;

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура для передачи шейдеру информации о фоновой подсветке.
*
* \version
*  1.0.0.0        \n
* \date
*  03 Декабрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
struct Ambient_t;

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура для передачи шейдеру информации о направленном источнике света.
*
* \version
*  1.0.0.0        \n
* \date
*  04 Декабрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
struct Direction_t;

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура, содержащая информацию об одном точечном источнике света.
*
* \version
*  1.0.0.0        \n
* \date
*  04 Декабрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
struct Point_t;

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура для передачи шейдеру информации о точечных источниках света.
*
* \version
*  1.0.0.0        \n
* \date
*  10 Декабрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
struct Points_t;

/**
* \ingroup CovelliteApiFxGroup
* \def MAX_LIGHT_POINT_COUNT
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Макрос максимального количества точечных источников света.
* \details
*  - Увеличение значения в 10 раз приводит к падению fps в ~2 раза.
*/

#ifdef GLM_VERSION

using float4 = ::glm::vec4;
using float4x4 = ::glm::mat4;

#elif defined DIRECTX_MATH_VERSION

// Структуры, используемые шейдерами HLSL, выравниваются по 16 байт, поэтому
// для корректной передачи данных здесь следует использовать только выровненные
// структуры.
using float4 = ::DirectX::XMFLOAT4A;
using float4x4 = ::DirectX::XMMATRIX;

#endif // GLM_VERSION, DIRECTX_MATH_VERSION

#define mediump
#define lowp
using color_t = float4;
