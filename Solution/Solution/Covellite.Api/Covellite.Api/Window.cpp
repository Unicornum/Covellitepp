
#include "stdafx.h"
#include <Covellite/Api/Window.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/patterns/factory.hpp>
#include <Covellite/Core/Settings.hpp>
#include <Covellite/Events.hpp>

#ifndef __USING_GTEST

#include <alicorn/logger.hpp>

# if BOOST_OS_WINDOWS
#include "OpenGL.hpp"
# elif BOOST_OS_ANDROID
#include "OpenGLES.hpp"
# endif

#endif

using namespace covellite::api;

Window::Window(const WindowOs_t & _Window) :
  m_pImpl(MakeApiImpl(_Window))
{
  LOGGER(Info) << uT("Using graphics API: ") << m_pImpl->GetUsingApi();
}

/**
* \deprecated
*  Устаревший конструктор, используемый совместно с классами из проекта
*  Covellite.Core, вместо них использовать классы из Covellite.Events и 
*  Covellite.App.
*/
Window::Window(const WindowOsPtr_t & _pWindow) :
  Window(*_pWindow)
{
}

Window::~Window(void) = default;

void Window::Subscribe(const EventHandlerPtr_t & _pEvents) /*override*/
{
  dynamic_cast<::covellite::core::IWindow &>(*m_pImpl).Subscribe(_pEvents);
}

Window::operator Window::Events_t (void) const /*override*/
{
  return *m_pImpl;
}

auto Window::GetUsingApi(void) const -> String_t /*override*/
{
  throw STD_EXCEPTION << "Implementation is not required.";
}

int32_t Window::GetWidth(void) const /*override*/
{
  return m_pImpl->GetWidth();
}

int32_t Window::GetHeight(void) const /*override*/
{
  return m_pImpl->GetHeight();
}

Window::Rect Window::GetClientRect(void) const /*override*/
{
  return m_pImpl->GetClientRect();
}

auto Window::MakeRenderInterface(void) const -> RenderInterfacePtr_t /*override*/
{
  return m_pImpl->MakeRenderInterface();
}

/*static*/ auto Window::MakeApiImpl(const WindowOs_t & _Window) -> IApiPtr_t
{
  const auto CovelliteppSection = ::covellite::core::Settings_t::GetInstance();

  const auto NameOfImplClass =
    CovelliteppSection[uT("Window")].Get<String_t>(uT("GraphicsApi"));

  using namespace ::alicorn::modules::patterns;

  return factory::make_unique<IWindow_t>(NameOfImplClass, _Window);
}
