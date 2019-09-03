
#pragma once
#include "OpenGLCommonStatic.hpp"

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
*  Реализация рендера графического API для Android/OpenGL ES1.
*
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
*  3.0.0.0        \n
* \date
*  17 Ноябрь 2016    \n
*  26 Сентябрь 2017    \n
*  27 Август 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2018
*/
class OpenGLES final :
  public OpenGLCommonStatic
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
  explicit OpenGLES(const Data_t &);
  ~OpenGLES(void);
};

FACTORY_REGISTER_STRING_NAME(OpenGLES);

} // namespace renderer

} // namespace api

} // namespace covellite
