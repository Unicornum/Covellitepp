
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
  // Интерфейс IGraphicApi:
  String_t GetUsingApi(void) const final;
  void ClearFrame(void) final;
  void ResizeWindow(int32_t, int32_t) final;
  const Creators_t & GetCreators(void) const final;

protected:
  virtual void DoOrtho(int32_t, int32_t) = 0;

private:
  Render_t CreateCamera(const ComponentPtr_t &);
  Render_t CreateState(const ComponentPtr_t &) const;
  Render_t CreatePosition(const ComponentPtr_t &);
  Render_t CreateBuffer(const ComponentPtr_t &) const;
  Render_t CreateDrawCall(const ComponentPtr_t &) const;
  Render_t CreateTexture(const ComponentPtr_t &) const;

private:
  const Renderer::Data::Color m_BackgroundColor;
  const int                   m_Top;
  const String_t              m_PreVersion;
  Creators_t                  m_Creators;

private:
  class Texture;

public:
  OpenGLCommon(const Renderer::Data &, const String_t &);
};

} // namespace renderer

} // namespace api

} // namespace covellite
