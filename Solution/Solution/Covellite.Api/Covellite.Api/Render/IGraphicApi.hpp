
#pragma once
#include <alicorn/std/string.forward.hpp>
#include "IRender.hpp"

namespace covellite
{

namespace api
{

namespace render
{

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Интерфейсный класс реализации рендеринга GUI при помощи графических Api.
*  
* \version
*  1.0.0.0        \n
* \date
*  24 Август 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class IGraphicApi :
  public IRender
{
protected:
  using String_t = ::alicorn::extension::std::String;

public:
  /// \brief
  ///  Описание одной вершины меша.
  /// \details
  ///  - Структура должна соответствовать Rocket::Core::Vertex
  class Vertex final
  {
  public:
    float x, y;
    uint32_t Color;
    float u, v;
  };

  class ITexture
  {
  public:
    class Data final
    {
    public:
      const uint8_t * pData = nullptr;
      int Width = 0;
      int Height = 0;
    };

  public:
    virtual void Render(void) = 0;

  public:
    virtual ~ITexture(void) {}
  };

  class IGeometry
  {
  public:
    class Data
    {
    public:
      Vertex * pVertices = nullptr;
      int VerticesCount = 0;
      int * pIndices = nullptr;
      int IndicesCount = 0;
      ITexture * pTexture = nullptr;
    };

  public:
    virtual void Update(float, float) = 0;
    virtual void Render(void) = 0;

  public:
    virtual ~IGeometry(void) {}
  };

public:
  virtual String_t GetUsingApi(void) const = 0;

public:
  virtual ITexture * Create(const ITexture::Data &) = 0;
  virtual void Destroy(ITexture *) = 0;
  virtual IGeometry * Create(const IGeometry::Data &) = 0;
  virtual void Destroy(IGeometry *) = 0;
  virtual void EnableScissorRegion(int, int, int, int) = 0;
  virtual void DisableScissorRegion(void) = 0;
  virtual void Render(void) = 0;
};

} // namespace render

} // namespace api

} // namespace covellite
