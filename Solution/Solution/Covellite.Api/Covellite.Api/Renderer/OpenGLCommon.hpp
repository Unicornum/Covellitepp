
#pragma once
#include <alicorn/std/string.hpp>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

/**
* \ingroup CovelliteApiRendererGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Вспомогательный класс, содержащий код OpenGL, общий для Windows и Android
*  версий. 
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  06 Ноябрь 2017    \n
*  27 Август 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017 - 2018
*/
class OpenGLCommon :
  public Registator_t<IGraphicApi>
{
public:
  // Интерфейс IRender:
  void ClearWindow(void) final;
  void ResizeWindow(int32_t, int32_t) final;

public:
  // Интерфейс IGraphicApi:
  String_t GetUsingApi(void) const final;

public:
  // Интерфейс IGraphicApi:
  ITexture * Create(const ITexture::Data &) final;
  void Destroy(ITexture *) final;
  IGeometry * Create(const IGeometry::Data &) final;
  void Destroy(IGeometry *) final;
  void EnableScissorRegion(int, int, int, int) final;
  void DisableScissorRegion(void) final;
  void Render(void) final;

protected:
  virtual void DoOrtho(int32_t, int32_t) = 0;

private:
  const Data::Color m_BackgroundColor;
  const int         m_Top;
  const String_t    m_PreVersion;

public:
  OpenGLCommon(const Data &, const String_t &);
};

} // namespace renderer

} // namespace api

} // namespace covellite
