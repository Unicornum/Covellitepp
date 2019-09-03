
#pragma once
#include "OpenGLCommonShader.hpp"

class OpenGLES3_test;

namespace covellite
{

namespace egl
{

class Display;
class Config;
class Surface;
class Context;

}

namespace api
{

namespace renderer
{

/**
* \ingroup CovelliteApiRendererGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Реализация рендера графического API для Android/OpenGL ES3.
*
* \version
*  1.0.0.0        \n
* \date
*  12 Август 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class OpenGLES3 final :
  public OpenGLCommonShader
{
  template<class TEgl>
  using EglPtr_t = ::std::shared_ptr<TEgl>;

public:
  // Интерфейс IGraphicApi:
  void PresentFrame(void) override;

private:
  EglPtr_t<covellite::egl::Display> m_pDisplay;
  EglPtr_t<covellite::egl::Config>  m_pConfig;
  EglPtr_t<covellite::egl::Surface> m_pSurface;
  EglPtr_t<covellite::egl::Context> m_pContext;

public:
  explicit OpenGLES3(const Data_t &);
  ~OpenGLES3(void);

private:
  friend OpenGLES3_test;
};

FACTORY_REGISTER_STRING_NAME(OpenGLES3);

} // namespace renderer

} // namespace api

} // namespace covellite
