
#include "stdafx.h"
#include "OpenGLES3.hpp"
#include <Covellite.Egl\Egl.hpp>

namespace covellite::api::renderer
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
  if (m_Top > 0)
  {
    // Для Android очищаем заголовок, иначе остается мусор под заголовком
    // при повороте устройства.

    glEnable(GL_SCISSOR_TEST);

    // (0, 0) - левый нижний угол!
    glScissor(0, m_Height - m_Top, m_Width, m_Top);

    // 22 Май 2020 10:58 (unicornum.verum@gmail.com)
    TODO("Цвет заголовка Android - в настройках?");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_SCISSOR_TEST);
  }

  m_pSurface->SwapBuffers();

  OpenGLCommonShader::PresentFrame();
}

} // nnamespace covellite::api::renderer
