
#include "stdafx.h"
#include "OpenGLES.hpp"
#include <Covellite.Egl\Egl.hpp>

namespace covellite
{

namespace api
{

namespace renderer
{

OpenGLES::OpenGLES(const Renderer::Data & _Data) :
  OpenGLCommon(_Data, uT("")),
  m_pDisplay(::std::make_unique<covellite::egl::Display>()),
  m_pConfig(::std::make_unique<covellite::egl::Config>(*m_pDisplay, _Data.Handle)),
  m_pSurface(::std::make_unique<covellite::egl::Surface>(*m_pDisplay, *m_pConfig, _Data.Handle)),
  m_pContext(::std::make_unique<covellite::egl::Context>(*m_pDisplay, *m_pConfig))
{
  m_pContext->MakeCurrent(*m_pSurface);
}

OpenGLES::~OpenGLES(void) = default;

void OpenGLES::PresentFrame(void) /*override*/
{
  m_pSurface->SwapBuffers();
}

} // namespace renderer

} // namespace api

} // namespace covellite
