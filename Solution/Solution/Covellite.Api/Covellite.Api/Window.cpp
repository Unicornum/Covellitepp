
#include "stdafx.h"
#include <Covellite/Api/Window.hpp>
#include <alicorn/std/string.hpp>
#include <Covellite/Events.hpp>
#include <Covellite/App/Events.hpp>
#include <Covellite/App/Settings.hpp>
#include <Covellite/Os/IWindow.hpp>
#include <Covellite/Os/Events.hpp>
#include <Covellite/Api/Events.hpp>
#include "Renderer/IGraphicApi.hpp"

#ifndef __USING_GTEST

# if BOOST_OS_WINDOWS

#include "Renderer/OpenGL.hpp"
#include "Renderer/DirectX10.hpp"
#include "Renderer/DirectX11.hpp"

# elif BOOST_PLAT_ANDROID

#if __ANDROID_API__ >= 18
#include "Renderer/OpenGLES3.hpp"
#endif

#include "Renderer/OpenGLES.hpp"

# endif // BOOST_OS_...

#endif // __USING_GTEST

namespace covellite
{

namespace api
{

Window::Window(const WindowOs_t & _Window) :
  m_WindowOs(_Window),
  m_Events(_Window),
  m_pImpl(MakeImpl(_Window))
{
  LOGGER(Info) << uT("Using graphics API: ") << m_pImpl->GetUsingApi();

  m_Events[events::Application.Update].Connect([this](void)
  {
    m_Events[events::Drawing.Do]();
    m_pImpl->PresentFrame();
  });

  m_Events[events::Window.Resize].Connect([this](void)
  {
    m_pImpl->ResizeWindow(GetClientRect());
  });
}

Window::~Window(void) = default;

Window::operator Window::Events_t(void) const /*override*/
{
  return m_Events;
}

Rect Window::GetClientRect(void) const /*override*/
{
  return m_WindowOs.GetClientRect();
}

/**
* \brief
*  Функция получения объекта для созданий рендеров  компонентов объектов 3D сцены.
* \details
*  - При каждом вызове создается новый объект; допустимо использовать
*  несколько объектов одновременно, но при этом следует следить, чтобы
*  удаление рендеров для компонентов производилось в том же объекте, в котором
*  они создавались.
*
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
auto Window::GetRenders(void) const /*override*/ -> RendersPtr_t
{
  return ::std::make_shared<Component::Renders>(m_pImpl->GetCreators());
}

/*static*/ auto Window::MakeImpl(const WindowOs_t & _Window) -> IGraphicApiPtr_t
{
  renderer::SettingsData oSettingsData;
  oSettingsData.Handle = _Window.GetHandle();
  oSettingsData.ClientRect = _Window.GetClientRect();

  using String_t = ::alicorn::extension::std::String;

  const auto MainSection = ::covellite::app::Settings_t::GetInstance();
  const auto WindowSection = MainSection[uT("Window")];

# if BOOST_OS_WINDOWS
  oSettingsData.IsFullScreen = WindowSection.Get<bool>(uT("IsFullScreen"));
# endif

  const auto NameOfApiClass = WindowSection.Get<String_t>(uT("GraphicsApi"));

  return MakeImpl(NameOfApiClass, oSettingsData);
}

} // namespace api

} // namespace covellite
