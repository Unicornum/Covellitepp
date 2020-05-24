
#pragma once
#include <glm/glm.force.hpp>
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
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������������� �����, ���������� ����� ��� Windows � Android ��� OpenGL.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
*  2.1.0.0        \n
*  2.2.0.0        \n
* \date
*  06 ������ 2017    \n
*  27 ������ 2018    \n
*  26 ������� 2018    \n
*  11 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017 - 2019
*/
class OpenGLCommon :
  public GraphicApi
{
public:
  class Texture;
  using MatrixBuilder_t = ::std::function<void(::glm::mat4 &)>;

public:
  // ��������� IGraphicApi:
  String_t GetUsingApi(void) const override;
  void ResizeWindow(int32_t, int32_t) final;

protected:
  // �������� GraphicApi
  Render_t CreateState(const ComponentPtr_t &) override;

protected:
  Render_t GetDepthRender(const bool, const bool, const bool);
  MatrixBuilder_t GetPreRenderGeometry(void);
  MatrixBuilder_t GetPreRenderBillboardGeometry(void);

protected:
  class TexParameters final
  {
  public:
    GLint MinFilter = 0;
    GLint MagFilter = 0;
    GLint WrapS = 0;
    GLint WrapT = 0;
  };

  TexParameters m_TexParameters;

protected:
  const int       m_Top;
  const String_t  m_PreVersion;

protected:
  OpenGLCommon(const Data_t &, const String_t &);
};

} // namespace renderer

} // namespace api

} // namespace covellite
