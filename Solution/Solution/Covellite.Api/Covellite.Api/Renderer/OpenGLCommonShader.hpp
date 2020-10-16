
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
*  (шейдерная реализация).
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
class OpenGLCommonShader :
  public OpenGLCommon
{
public:
  // Интерфейс IGraphicApi:
  String_t GetUsingApi(void) const override;
  void PresentFrame(void) override;

protected:
  // Интефейс GraphicApi
  Render_t CreateCamera(const ComponentPtr_t &) override;
  Render_t CreateBkSurface(const ComponentPtr_t &) override;
  Render_t CreateState(const ComponentPtr_t &) override;
  Render_t CreateTexture(const ComponentPtr_t &) override;
  Render_t CreateShader(const ComponentPtr_t &) override;
  Render_t CreateBuffer(const ComponentPtr_t &) override;
  Render_t CreateTransform(const ComponentPtr_t &) override;
  Render_t CreatePresentBuffer(const ComponentPtr_t &) override;

private:
  Render_t GetCameraCommon(const ComponentPtr_t &);
  Render_t GetCameraOrthographic(const ComponentPtr_t &);
  Render_t GetCameraPerspective(const ComponentPtr_t &);

private:
  class Buffer;

private:
  template<class>
  class ConstantBuffer;
  Render_t m_DrawElements = [](void) {};

private:
  class Programs;
  using ProgramsPtr_t = ::std::shared_ptr<Programs>;
  const ProgramsPtr_t m_pPrograms;

public:
  OpenGLCommonShader(const Data_t &, const String_t &, const ::std::string &);
  OpenGLCommonShader(const OpenGLCommonShader &) = delete;
  OpenGLCommonShader(OpenGLCommonShader &&) = delete;
  OpenGLCommonShader & operator= (const OpenGLCommonShader &) = delete;
  OpenGLCommonShader & operator= (OpenGLCommonShader &&) = delete;
  ~OpenGLCommonShader(void) = default;
};

} // namespace renderer

} // namespace api

} // namespace covellite
