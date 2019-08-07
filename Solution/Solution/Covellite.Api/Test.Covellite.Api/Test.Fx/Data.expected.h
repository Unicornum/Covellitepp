
#ifdef __cplusplus

#pragma once

using uint = uint32_t;

#ifdef GLM_VERSION

using float4 = ::glm::vec4;
using float4x4 = ::glm::mat4;

#elif defined DIRECTX_MATH_VERSION

// Структуры, используемые шейдерами HLSL, выравниваются по 16 байт, поэтому
// для корректной передачи данных здесь следует использовать только выровненные
// структуры.
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
*  Класс входит в проект \ref CovelliteApiPage \n
*  Макрос максимального количества точечных источников света.
* \details
*  - Увеличение значения в 10 раз приводит к падению fps в ~2 раза.
*/
#define MAX_LIGHT_POINT_COUNT 16

/**
* \defgroup CovelliteApiFxGroup Fx
* \ingroup CovelliteApiGroup
*  Группа структур для передачи данных шейдерам.
*/

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура для передачи данных шейдеру, который используется DirectX.
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
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура для передачи шейдеру данных материала.
*
* \version
*  1.0.0.0        \n
* \date
*  27 Декабрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
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
*  Входит в проект \ref CovelliteApiPage \n
*  Пространство имен структур для передачи шейдеру информации об источниках 
*  света.
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
namespace Light
{

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
struct Ambient
{
  uint IsValid;
  uint ARGBColor;
};

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
struct Direction
{
  uint IsValid;
  uint ARGBColor;
  float4 Direction;
};

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
struct Points
{
  Point Lights[MAX_LIGHT_POINT_COUNT];
  uint UsedSlotCount;
};

}; // namespace Light

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
struct Lights
{
  ::Light::Ambient Ambient;
  ::Light::Direction Direction;
  ::Light::Points Points;
};

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
