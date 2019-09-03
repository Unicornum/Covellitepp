
#include "stdafx.h"
#include "OpenGLES.hpp"
#include <Covellite.Egl\Egl.hpp>

namespace covellite
{

namespace api
{

namespace renderer
{

OpenGLES::OpenGLES(const Data_t & _Data) :
  OpenGLCommonStatic(_Data, uT("")),
  m_pDisplay(::std::make_unique<covellite::egl::Display>()),
  m_pConfig(::std::make_unique<covellite::egl::Config>(*m_pDisplay, ::covellite::any_cast<ANativeWindow *>(_Data.Handle))),
  m_pSurface(::std::make_unique<covellite::egl::Surface>(*m_pDisplay, *m_pConfig, ::covellite::any_cast<ANativeWindow *>(_Data.Handle))),
  m_pContext(::std::make_unique<covellite::egl::Context>(*m_pDisplay, *m_pConfig, 1))
{
  m_pContext->MakeCurrent(*m_pSurface);
}

OpenGLES::~OpenGLES(void) = default;

void OpenGLES::PresentFrame(void) /*override*/
{
  m_pSurface->SwapBuffers();

  OpenGLCommonStatic::PresentFrame();
}

} // namespace renderer

} // namespace api

} // namespace covellite
