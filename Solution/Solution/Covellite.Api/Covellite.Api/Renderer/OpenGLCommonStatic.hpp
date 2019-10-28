
#pragma once
#include "OpenGLCommon.hpp"

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
*  Вспомогательный класс, содержащий общий для Windows и Android код OpenGL
*  (статический конвеер). 
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
*  2.1.0.0        \n
*  2.2.0.0        \n
* \date
*  06 Ноябрь 2017    \n
*  27 Август 2018    \n
*  26 Декабрь 2018    \n
*  11 Август 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017 - 2019
*/
class OpenGLCommonStatic :
  public OpenGLCommon
{
protected:
  // Интефейс GraphicApi
  Render_t CreateCamera(const ComponentPtr_t &) override;
  Render_t CreateState(const ComponentPtr_t &) override;
  Render_t CreateFog(const ComponentPtr_t &) override;
  Render_t CreateMaterial(const ComponentPtr_t &) override;
  Render_t CreateTexture(const ComponentPtr_t &) override;
  Render_t CreateBuffer(const ComponentPtr_t &) override;
  Render_t CreateTransform(const ComponentPtr_t &) override;
  Render_t CreatePresentBuffer(const ComponentPtr_t &) override;

private:
  // deprecated
  Render_t CreateLight(const ComponentPtr_t &) override;
  Render_t CreateGeometry(const ComponentPtr_t &) override;

private:
  Render_t GetCameraCommon(void);
  Render_t GetCameraOrthographic(const ComponentPtr_t &);
  Render_t GetCameraPerspective(const ComponentPtr_t &);

private:
  template<class>
  class ConstantBuffer;
  Render_t m_DrawElements = [](void) {};

public:
  OpenGLCommonStatic(const Data_t &, const String_t &);
};

} // namespace renderer

} // namespace api

} // namespace covellite
