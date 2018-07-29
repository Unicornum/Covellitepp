
#include "stdafx.h"
#include <Covellite\Api\OpenGLES.hpp>
#include <alicorn\std\string.hpp>
#include <alicorn\platform\environment.hpp>
#include <Covellite\Events.hpp>
#include <Covellite\Core.hpp>
#include <Covellite.Egl\Egl.hpp>
#include <Covellite\App\Events.hpp>
#include <Covellite\Os\IWindow.hpp>
#include <Covellite\Os\Events.hpp>
#include <Covellite\Api\Events.hpp>
#include <Covellite\Api\RenderOpenGL.hpp>

using namespace covellite::api;

OpenGLES::OpenGLES(const WindowOs_t & _Window) :
  m_Events(_Window),
  m_pDisplay(::std::make_unique<covellite::egl::Display>()),
  m_pConfig(::std::make_unique<covellite::egl::Config>(*m_pDisplay, _Window.GetHandle())),
  m_pSurface(::std::make_unique<covellite::egl::Surface>(*m_pDisplay, *m_pConfig, _Window.GetHandle())),
  m_pContext(::std::make_unique<covellite::egl::Context>(*m_pDisplay, *m_pConfig))
{
  m_pContext->MakeCurrent(*m_pSurface);

  using namespace ::covellite::events;

  m_Events[Application.Update].Connect([&](void)
  {
    ClearWindow();
    m_Events[Drawing.Do]();
    m_pSurface->SwapBuffers();
  });

  m_Events[Window.Resize].Connect([&](void)
  {
    const auto Rect = GetClientRect();
    UpdateWindow(Rect.Width, Rect.Height);
  });
}

OpenGLES::~OpenGLES(void) = default;

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
*  Оператор доступа к объекту событий фреймворка.
*/
OpenGLES::operator OpenGLES::Events_t (void) const /*override*/
{
  return m_Events;
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
*  Функция получения положения и размеров клинтской области окна программы.
*/
OpenGLES::Rect OpenGLES::GetClientRect(void) const /*override*/
{
  return { 0, ::alicorn::system::platform::Environment{}.GetStatusBarHeight(),
    m_pSurface->GetWidth(), m_pSurface->GetHeight() };
}

/**
* \brief
*  Функция создания объекта Rocket::Core::RenderInterface.
*/
OpenGLES::RenderInterfacePtr_t OpenGLES::MakeRenderInterface(void) const /*override*/
{
  return ::std::make_shared<covellite::api::RenderOpenGL>(GetClientRect().Top);
}

void OpenGLES::GlOrtho(int32_t _Width, int32_t _Height) /*override*/
{
  glOrthof(0, static_cast<GLfloat>(_Width),
    static_cast<GLfloat>(_Height), 0, -1, 1);
}
