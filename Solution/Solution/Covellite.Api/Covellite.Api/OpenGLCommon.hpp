
#pragma once
#include <Covellite/Api/GL.hpp>

namespace covellite
{

namespace api
{

/**
* \ingroup gCovelliteApiClasses
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Вспомогательный класс, содержащий код OpenGL, общий для Windows и Android
*  версий. 
*  
* \version
*  1.0.0.0        \n
* \date
*  06 Ноябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class OpenGLCommon
{
public:
  class Color
  {
  public:
    GLclampf R;
    GLclampf G;
    GLclampf B;
    GLclampf A;
  };

protected:
  virtual void GlOrtho(int32_t, int32_t) = 0;

protected:
  void ClearWindow(void);
  void UpdateWindow(int32_t, int32_t);

private:
  const Color m_BackgroundColor;

public:
  OpenGLCommon(void);
  virtual ~OpenGLCommon(void) = default;
};

} // namespace api

} // namespace covellite
