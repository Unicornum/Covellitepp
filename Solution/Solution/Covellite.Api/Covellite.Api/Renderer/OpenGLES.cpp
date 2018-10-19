
#include "stdafx.h"
#include "OpenGLES.hpp"
#include <Covellite.Egl\Egl.hpp>

namespace covellite
{

namespace api
{

namespace renderer
{

OpenGLES::OpenGLES(const Data & _Data) :
  OpenGLCommon(_Data, uT("")),
  m_pDisplay(::std::make_unique<covellite::egl::Display>()),
  m_pConfig(::std::make_unique<covellite::egl::Config>(*m_pDisplay, _Data.Handle)),
  m_pSurface(::std::make_unique<covellite::egl::Surface>(*m_pDisplay, *m_pConfig, _Data.Handle)),
  m_pContext(::std::make_unique<covellite::egl::Context>(*m_pDisplay, *m_pConfig))
{
  m_pContext->MakeCurrent(*m_pSurface);
}

OpenGLES::~OpenGLES(void) = default;

void OpenGLES::Present(void) /*override*/
{
  m_pSurface->SwapBuffers();
}

void OpenGLES::DoOrtho(int32_t _Width, int32_t _Height) /*override*/
{
  glOrthof(0, static_cast<GLfloat>(_Width),
    static_cast<GLfloat>(_Height), 0, -1, 1);
}

} // namespace renderer

} // namespace api

} // namespace covellite
