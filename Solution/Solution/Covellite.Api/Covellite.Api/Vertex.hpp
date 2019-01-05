
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
*  Класс-контейнер доступных форматов вертексных буферов.
* \details
*  - Форматы вложенных классов должны соответствовать фоматам входных
*  параметров в файле VertexInput.fx.
*  
* \version
*  1.0.0.0        \n
* \date
*  27 Ноябрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Vertex final
{
  using String_t = ::alicorn::extension::std::String;

public:
  /// \brief
  ///  Описание одной вершины меша для рендеринга Gui.
  /// \details
  ///  - Формат вершины для минимизации операций копирования данных буфера 
  /// (должнен соответствовать формату Rocket::Core::Vertex).
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
  ///  Описание одной вершины для рендеринга простых текстурированных мешей.
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
