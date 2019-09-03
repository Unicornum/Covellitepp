
#pragma once
#include "OpenGLCommonShader.hpp"

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
*  Реализация рендера графического API для Android/OpenGL ES2.
* \details
*  - В принципе, это работает, но синтаксис шейдеров v100es заметно отличается
*  от v300es, поэтому потребуется серьезная доработка, которая будет
*  бессмысленной тратой времени.
*  - Вызов glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &InfoLogLenght) для
*  получения длины сообщения об ошибках компиляции шейдера работает неправильно.
*
* \version
*  1.0.0.0        \n
* \date
*  09 Август 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class OpenGLES2 final :
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
  explicit OpenGLES2(const Data_t &);
  ~OpenGLES2(void);
};

//FACTORY_REGISTER_STRING_NAME(OpenGLES2);

} // namespace renderer

} // namespace api

} // namespace covellite
