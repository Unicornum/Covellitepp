
#pragma once
#include <GLMath.hpp>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"
#include "CapturingServiceComponent.hpp"
#include "Updater.hpp"

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
*  ��������������� �����, ���������� ��� OpenGL, ����� ��� Windows � Android
*  ������. 
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
*  2.1.0.0        \n
* \date
*  06 ������ 2017    \n
*  27 ������ 2018    \n
*  26 ������� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017 - 2018
*/
class OpenGLCommon :
  public Registator_t<IGraphicApi>
{
  using Color_t = ::std::vector<GLfloat>;
  using MatrixBuilder_t = ::std::function<void(::glm::mat4 &)>;

protected:
  using Data_t = ::covellite::api::renderer::SettingsData;

public:
  // ��������� IGraphicApi:
  String_t GetUsingApi(void) const final;
  void ResizeWindow(int32_t, int32_t) final;
  const Creators_t & GetCreators(void) const final;

private:
  Render_t CreateCamera(const ComponentPtr_t &);
  Render_t CreateState(const ComponentPtr_t &);
  Render_t CreateFog(const ComponentPtr_t &);
  Render_t CreateLight(const ComponentPtr_t &);
  Render_t CreateMaterial(const ComponentPtr_t &);
  Render_t CreateTexture(const ComponentPtr_t &);
  Render_t CreateBuffer(const ComponentPtr_t &);
  Render_t CreatePresent(const ComponentPtr_t &);

private:
  Render_t GetDepthRender(const bool, const bool, const bool);
  Render_t GetCameraCommon(void);
  Render_t GetCameraOrthographic(const ComponentPtr_t &);
  Render_t GetCameraPerspective(const ComponentPtr_t &);
  Render_t CreateGeometry(const ComponentPtr_t &);
  MatrixBuilder_t GetPreRenderGeometry(void);
  MatrixBuilder_t GetPreRenderBillboardGeometry(void);
  static Color_t ARGBtoFloat4(uint32_t);

private:
  const int       m_Top;
  const String_t  m_PreVersion;
  Creators_t      m_Creators;
  Render_t        m_DrawElements = [](void) {};
  Render_t        m_SampleState = [](void) {};

private:
  class Data;
  class Texture;

private:
  ::std::shared_ptr<Data>   m_pData;
  CapturingServiceComponent m_ServiceComponents;

protected:
  Updater                   m_Updater;

public:
  OpenGLCommon(const Data_t &, const String_t &);
};

} // namespace renderer

} // namespace api

} // namespace covellite
