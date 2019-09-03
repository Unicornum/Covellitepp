
#include "stdafx.h"
#include "OpenGLES2.hpp"
#include <Covellite.Egl\Egl.hpp>
#include "Component.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

OpenGLES2::OpenGLES2(const Data_t & _Data) :
  OpenGLCommonShader(_Data, uT(""), ""),
  m_pDisplay(::std::make_unique<covellite::egl::Display>()),
  m_pConfig(::std::make_unique<covellite::egl::Config>(*m_pDisplay, ::covellite::any_cast<ANativeWindow *>(_Data.Handle))),
  m_pSurface(::std::make_unique<covellite::egl::Surface>(*m_pDisplay, *m_pConfig, ::covellite::any_cast<ANativeWindow *>(_Data.Handle))),
  m_pContext(::std::make_unique<covellite::egl::Context>(*m_pDisplay, *m_pConfig, 2))
{
  m_pContext->MakeCurrent(*m_pSurface);
}

OpenGLES2::~OpenGLES2(void) = default;

void OpenGLES2::PresentFrame(void) /*override*/
{
  m_pSurface->SwapBuffers();

  OpenGLCommonShader::PresentFrame();
}

} // namespace renderer

} // namespace api

} // namespace covellite
