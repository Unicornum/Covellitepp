
#pragma once
#include <cstdint>

namespace covellite
{

namespace api
{

namespace vertex
{

/// \deprecated
///  Класс устарел и будет удален в следующей стабильной версии, вместо него
///  следует использовать класс covellite::api::Vertex.
/// \brief
///  Описание одной вершины меша для рендеринга плоских объектов.
/// \details
///  - Формат вершины для минимизации операций копирования данных буфера 
/// (должнен соответствовать формату Rocket::Core::Vertex).
class [[deprecated]] Polygon
{
public:
  float x, y;
  uint32_t ABGRColor;
  float u, v;
};

/// \deprecated
///  Класс устарел и будет удален в следующей стабильной версии, вместо него
///  следует использовать класс covellite::api::Vertex.
/// \brief
///  Описание одной вершины для рендеринга объемных моделей.
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
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс формата вертексного буфера.
* \details
*  - Формат класса вершин должен соответствовать фомату входных параметров 
*  вертексного шейдера в файле Input.fx.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  2.0.0.0        \n
* \date
*  27 Ноябрь 2018    \n
*  23 Февраль 2019    \n
*  19 Август 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018 - 2019
*/
class Vertex final
{
public:
  /**
  *\deprecated
  *  Класс устарел и будет удален в следующей стабильной версии, вместо
  *  них следует использовать класс covellite::api::Vertex.
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
  *  Класс устарел и будет удален в следующей стабильной версии, вместо
  *  них следует использовать класс covellite::api::Vertex.
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
