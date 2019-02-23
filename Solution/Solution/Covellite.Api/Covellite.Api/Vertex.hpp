
#pragma once
#include <alicorn/std/string.hpp>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс-контейнер форматов вертексных буферов.
* \details
*  - Форматы классов вершин должны соответствовать фоматам входных параметров 
*  в файле Input.fx.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  27 Ноябрь 2018    \n
*  23 Февраль 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018 - 2019
*/
class Vertex final
{
public:
  /// \brief
  ///  Описание одной вершины меша для рендеринга плоских объектов.
  /// \details
  ///  - Формат вершины для минимизации операций копирования данных буфера 
  /// (должнен соответствовать формату Rocket::Core::Vertex).
  class Polygon final
  {
  public:
    float x, y;
    uint32_t ABGRColor;
    float u, v;
  };

  /// \brief
  ///  Описание одной вершины для рендеринга объемных моделей.
  class Polyhedron final
  {
  public:
    float x, y, z;
    float nx, ny, nz;
    float tu, tv;
  };
};

} // namespace api

} // namespace covellite
