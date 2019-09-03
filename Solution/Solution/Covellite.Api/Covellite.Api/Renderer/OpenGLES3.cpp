
#include "stdafx.h"
#include "OpenGLES3.hpp"
#include <Covellite.Egl\Egl.hpp>

namespace covellite
{

namespace api
{

namespace renderer
{

OpenGLES3::OpenGLES3(const Data_t & _Data) :
  OpenGLCommonShader(_Data, uT(""), 
    "#version 300 es\r\n"
    "#define COVELLITE_SHADER_MOBILE\r\n"),
  m_pDisplay(::std::make_unique<covellite::egl::Display>()),
  m_pConfig(::std::make_unique<covellite::egl::Config>(*m_pDisplay, ::covellite::any_cast<ANativeWindow *>(_Data.Handle))),
  m_pSurface(::std::make_unique<covellite::egl::Surface>(*m_pDisplay, *m_pConfig, ::covellite::any_cast<ANativeWindow *>(_Data.Handle))),
  m_pContext(::std::make_unique<covellite::egl::Context>(*m_pDisplay, *m_pConfig, 3))
{
  m_pContext->MakeCurrent(*m_pSurface);
}

OpenGLES3::~OpenGLES3(void) = default;

void OpenGLES3::PresentFrame(void) /*override*/
{
  m_pSurface->SwapBuffers();

  OpenGLCommonShader::PresentFrame();
}

} // namespace renderer

} // namespace api

} // namespace covellite
