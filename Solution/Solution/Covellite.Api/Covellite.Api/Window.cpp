
#include "stdafx.h"
#include <Covellite\Api\Window.hpp>
#include <alicorn\std\string.hpp>

using namespace covellite::api;

#ifdef __USING_GTEST

template class Window<::mock::ApiImpl>;

#else

#include <alicorn\logger.hpp>

#if BOOST_OS_WINDOWS

// Реализации, доступные в Windows:
#include <Covellite\Api\OpenGL.hpp>

template class Window<OpenGL>;

#elif BOOST_OS_ANDROID

// Реализации, доступные в Android:
#include <Covellite\Api\OpenGLES.hpp>

template class Window<OpenGLES>;

#endif

#endif

template<class TApiImpl>
Window<TApiImpl>::Window(const WindowOsPtr_t & _pWindow) :
  m_pImpl(::std::make_unique<TApiImpl>(_pWindow))
{
  LOGGER(Info) << uT("Using graphics API: ") << m_pImpl->GetUsingApi();
}

template<class TApiImpl>
Window<TApiImpl>::~Window(void) noexcept = default;

template<class TApiImpl>
void Window<TApiImpl>::Subscribe(const EventHandlerPtr_t & _pEvents) /*override*/
{
  m_pImpl->Subscribe(_pEvents);
}

template<class TApiImpl>
auto Window<TApiImpl>::GetUsingApi(void) const -> String_t /*override*/
{
  throw STD_EXCEPTION << "Implementation is not required.";
}

template<class TApiImpl>
int32_t Window<TApiImpl>::GetWidth(void) const /*override*/
{
  return m_pImpl->GetWidth();
}

template<class TApiImpl>
int32_t Window<TApiImpl>::GetHeight(void) const /*override*/
{
  return m_pImpl->GetHeight();
}

template<class TApiImpl>
auto Window<TApiImpl>::MakeRenderInterface(void) const -> RenderInterfacePtr_t /*override*/
{
  return m_pImpl->MakeRenderInterface();
}
