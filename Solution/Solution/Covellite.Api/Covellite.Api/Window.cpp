
#include "stdafx.h"
#include <Covellite/Api/Window.hpp>
#include <alicorn/std/string.hpp>
#include <Covellite/Core/EventHandler.hpp>
#include <Covellite/Events.hpp>
#include <Covellite/App/Events.hpp>
#include <Covellite/App/Settings.hpp>
#include <Covellite/Os/IWindow.hpp>
#include <Covellite/Os/Events.hpp>
#include <Covellite/Api/Events.hpp>
#include "Renderer/Renderer.hpp"

using namespace covellite::api;

Window::Window(const WindowOs_t & _Window) :
  m_WindowOs(_Window),
  m_Events(_Window),
  m_pImpl(MakeRender(_Window))
{
  m_Events[events::Application.Update].Connect([&]()
  {
    m_pImpl->StartDrawingFrame();
    m_Events[events::Drawing.Do]();
    m_pImpl->PresentFrame();
  });

  m_Events[events::Window.Resize].Connect([&]()
  {
    const auto Rect = GetClientRect();
    m_pImpl->ResizeWindow(Rect.Width, Rect.Height);
  });
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

Window::operator Window::Events_t (void) const /*override*/
{
  return m_Events;
}

auto Window::GetClientRect(void) const /*override*/ -> Rect_t
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
  // 19 Ноябрь 2018 12:51 (unicornum.verum@gmail.com)
  TODO("При выпуске стабильной версии добавить создание нового объекта и тест функции.");
  //return ::std::make_shared<Component::Renders>(m_pImpl->GetCreators());

  return m_pImpl->GetRenders();
}

auto Window::GetRenderInterface(void) const -> RenderInterfacePtr_t /*override*/
{
  return m_pImpl;
}

int32_t Window::GetWidth(void) const /*override*/
{
  return m_WindowOs.GetClientRect().Width;
}

int32_t Window::GetHeight(void) const /*override*/
{
  return m_WindowOs.GetClientRect().Height;
}

auto Window::MakeRenderInterface(void) const -> RenderInterfacePtr_t /*override*/
{
  return m_pImpl;
}

void Window::Subscribe(const EventHandlerPtr_t & _pEvents) /*override*/
{
#if BOOST_COMP_MSVC
# pragma warning(push)
# pragma warning(disable: 4996)
# pragma warning(disable: 26444)
#endif

  using namespace ::covellite::core;

#if BOOST_COMP_MSVC
# pragma warning(pop)
#endif

  (*_pEvents)[Event::StartDrawing].connect([&](const Params &) 
  { 
    m_pImpl->StartDrawingFrame();
  });

  (*_pEvents)[Event::FinishDrawing].connect([&](const Params &) 
  { 
    m_pImpl->PresentFrame(); 
  });

  (*_pEvents)[Event::Resize].connect([&](const Params &) 
  { 
    const auto Rect = GetClientRect();
    m_pImpl->ResizeWindow(Rect.Width, Rect.Height);
  });
}

/*static*/ Window::RendererPtr_t Window::MakeRender(const WindowOs_t & _Window)
{
  Renderer_t::Data Data;
  Data.Handle = _Window.GetHandle();
  Data.Top = _Window.GetClientRect().Top;

  using String_t = ::alicorn::extension::std::String;

  const auto MainSection = ::covellite::app::Settings_t::GetInstance();
  const auto WindowSection = MainSection[uT("Window")];

# if BOOST_OS_WINDOWS
  Data.IsFullScreen = WindowSection.Get<bool>(uT("IsFullScreen"));
# endif

  auto BkColorSection = WindowSection[uT("BackgroundColor")];

  Data.BkColor = 
  {
    BkColorSection.Get<float>(uT("R")) / 255.0f,
    BkColorSection.Get<float>(uT("G")) / 255.0f,
    BkColorSection.Get<float>(uT("B")) / 255.0f,
    BkColorSection.Get<float>(uT("A")) / 255.0f
  };

  const auto NameOfApiClass = WindowSection.Get<String_t>(uT("GraphicsApi"));

  return ::std::make_shared<Renderer_t>(NameOfApiClass, Data);
}
