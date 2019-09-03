
#pragma once
#include <GLMath.hpp>
#include "GraphicApi.hpp"

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
*  Вспомогательный класс, содержащий общий для Windows и Android код OpenGL.
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
class OpenGLCommon :
  public GraphicApi
{
protected:
  class Texture;
  using MatrixBuilder_t = ::std::function<void(::glm::mat4 &)>;

public:
  // Интерфейс IGraphicApi:
  String_t GetUsingApi(void) const override;
  void ResizeWindow(int32_t, int32_t) final;

protected:
  // Интефейс GraphicApi
  Render_t CreateState(const ComponentPtr_t &) override;

protected:
  Render_t GetDepthRender(const bool, const bool, const bool);
  MatrixBuilder_t GetPreRenderGeometry(void);
  MatrixBuilder_t GetPreRenderBillboardGeometry(void);

protected:
  Render_t        m_SamplerState = [](void) {};

private:
  const int       m_Top;
  const String_t  m_PreVersion;

protected:
  OpenGLCommon(const Data_t &, const String_t &);
};

} // namespace renderer

} // namespace api

} // namespace covellite
