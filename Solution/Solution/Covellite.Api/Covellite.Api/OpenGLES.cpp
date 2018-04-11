
#include "stdafx.h"
#include <Covellite\Api\OpenGLES.hpp>
#include <alicorn\std\string.hpp>
#include <alicorn\platform\environment.hpp>
#include <Covellite\Core.hpp>
#include <Covellite.Egl\Egl.hpp>
#include <Covellite\Os\Window.hpp>
#include <Covellite\Api\RenderOpenGL.hpp>

using namespace covellite::api;

OpenGLES::OpenGLES(const WindowOsPtr_t & _pWindow) :
  m_pDisplay(::std::make_unique<covellite::egl::Display>()),
  m_pConfig(::std::make_unique<covellite::egl::Config>(*m_pDisplay, _pWindow->GetHandle())),
  m_pSurface(::std::make_unique<covellite::egl::Surface>(*m_pDisplay, *m_pConfig, _pWindow->GetHandle())),
  m_pContext(::std::make_unique<covellite::egl::Context>(*m_pDisplay, *m_pConfig))
{
  m_pContext->MakeCurrent(*m_pSurface);
}

OpenGLES::~OpenGLES(void) noexcept = default;

/**
* \brief
*  Функция подписки на события приложения.
*/
void OpenGLES::Subscribe(const EventHandlerPtr_t & _pEvents) /*override*/
{
  using namespace ::covellite::core;

  (*_pEvents)[Event::Resize]
    .connect([&](const Params &) { UpdateWindow(GetWidth(), GetHeight()); });
  (*_pEvents)[Event::StartDrawing]
    .connect([&](const Params &) { ClearWindow(); });
  (*_pEvents)[Event::FinishDrawing]
    .connect([&](const Params &) { m_pSurface->SwapBuffers(); });
}

/**
* \brief
*  Функция получения строки текущей версии и типа используемого графического 
*  API.
*/
auto OpenGLES::GetUsingApi(void) const -> String_t /*override*/
{
  using namespace ::alicorn::extension::std;

  auto Version = reinterpret_cast<const char *>(glGetString(GL_VERSION));

  return uT("OpenGL ES %VERSION%").Replace(uT("%VERSION%"),
    string_cast<String, Locale::Default>(::std::string{ Version }));
}

/**
* \brief
*  Функция получения текущей ширины окна.
*/
int32_t OpenGLES::GetWidth(void) const /*override*/
{
  return m_pSurface->GetWidth();
}

/**
* \brief
*  Функция получения текущей высоты окна.
*/
int32_t OpenGLES::GetHeight(void) const /*override*/
{
  return m_pSurface->GetHeight();
}

/**
* \brief
*  Функция создания объекта Rocket::Core::RenderInterface.
*/
OpenGLES::RenderInterfacePtr_t OpenGLES::MakeRenderInterface(void) const /*override*/
{
  return ::std::make_shared<covellite::api::RenderOpenGL>(
    ::alicorn::system::platform::Environment{}.GetStatusBarHeight());
}

void OpenGLES::GlOrtho(int32_t _Width, int32_t _Height) /*override*/
{
  glOrthof(0, static_cast<GLfloat>(_Width),
    static_cast<GLfloat>(_Height), 0, -1, 1);
}
