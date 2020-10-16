
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
*  Структура для передачи шейдеру информации об объекте.
* \details
*  - Структура должна содержать мировую матрицу объекта и список источников
*  света, которые его освещают.
*
* \version
*  1.0.0.0        \n
* \date
*  10 Сентябрь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
struct Object;
