
#define ALIGN(Value)

#ifdef __cplusplus

#pragma once
#include <directxmath.h>
using uint = uint32_t;
using float3 = ::DirectX::XMFLOAT3;
using float4x4 = ::DirectX::XMMATRIX;

#undef ALIGN
#define ALIGN(Value) \
  alignas(Value)

#pragma warning(push)
#pragma warning(disable: 4324)

#endif // __cplusplus

#define BUFFER_VALUE(letter, index) letter##index
#define BUFFER_INDEX(index) BUFFER_VALUE(b, index)

#define MATRICES_BUFFER_INDEX 0
#define MATERIAL_BUFFER_INDEX 5
#define LIGHTS_BUFFER_INDEX 6

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
struct ALIGN(16) Matrices
{
  float4x4 World;
  float4x4 View;
  float4x4 Projection;
};

/**
* \ingroup CovelliteApiFxGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура для передачи шейдеру компонентов цвета.
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
struct ALIGN(16) Color
{
  uint ARGBAmbient;
  uint ARGBDiffuse;
  uint ARGBSpecular;
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
struct ALIGN(16) Material
{
  Color Color;
  uint ARGBEmission;
  float Shininess;
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
struct ALIGN(16) Ambient
{
  Color Color;
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
struct ALIGN(16) Direction
{
  Color Color;
  float3 Direction;
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
struct ALIGN(16) Point
{
  Color Color;
  float3 Position;
  float3 Attenuation; // Const, Linear, Exponent;
  uint Dummy;
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
struct ALIGN(16) Points
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
struct ALIGN(16) Lights
{
  ::Light::Ambient Ambient;
  ::Light::Direction Direction;
  ::Light::Points Points;
};

#undef ALIGN

#ifdef __cplusplus
#pragma warning(pop)
#endif // __cplusplus
