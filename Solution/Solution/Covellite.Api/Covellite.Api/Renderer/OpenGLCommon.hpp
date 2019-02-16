
#pragma once
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"
#include "CapturingServiceComponent.hpp"

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
*  2.1.0.0        \n
* \date
*  06 Ноябрь 2017    \n
*  27 Август 2018    \n
*  26 Декабрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017 - 2018
*/
class OpenGLCommon :
  public Registator_t<IGraphicApi>
{
  using CameraId_t = String_t;
  using Color_t = ::std::vector<GLfloat>;

public:
  // Интерфейс IGraphicApi:
  String_t GetUsingApi(void) const final;
  void ClearFrame(void) final;
  void ResizeWindow(int32_t, int32_t) final;
  const Creators_t & GetCreators(void) const final;

private:
  Render_t CreateState(const ComponentPtr_t &);
  Render_t CreateLight(const ComponentPtr_t &);
  Render_t CreateMaterial(const ComponentPtr_t &);
  Render_t CreateTexture(const ComponentPtr_t &);
  Render_t CreateBuffer(const ComponentPtr_t &);
  Render_t CreatePresent(const ComponentPtr_t &);
  Render_t CreateCamera(const ComponentPtr_t &);
  Render_t GetDeptRender(const ComponentPtr_t &);
  Render_t GetLightsRender(const ComponentPtr_t &);
  Render_t GetCameraCommon(const ComponentPtr_t &);
  Render_t GetCameraGui(const ComponentPtr_t &);
  Render_t GetCameraFocal(const ComponentPtr_t &);
  Render_t CreateGeometry(const ComponentPtr_t &);
  static Color_t ARGBtoFloat4(uint32_t);

private:
  CapturingServiceComponent   m_ServiceComponents;
  const Renderer::Data::Color m_BackgroundColor;
  const int                   m_Top;
  const String_t              m_PreVersion;
  Creators_t                  m_Creators;
  Render_t                    m_DrawElements;

private:
  class Texture;
  class Light
  {
  public:
    Color_t Ambient;
    Color_t Diffuse;
    Color_t Specular;
    float Position[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
    float Const = 1.0f;
    float Linear = 0.0f;
    float Exponent = 0.0f;
  };

  ::std::map<CameraId_t, ::std::vector<Light>> m_Lights;
  CameraId_t m_CurrentCameraId = uT("Unknown");

public:
  OpenGLCommon(const Renderer::Data &, const String_t &);
};

} // namespace renderer

} // namespace api

} // namespace covellite
