
#pragma once
#include <memory>
#include <Covellite/Core/IWindow.hpp>
#include <Covellite/Events/Events.hpp>
#include <Covellite/App/IWindow.hpp>
#include <Covellite/Api/IWindow.hpp>

namespace covellite
{

namespace os { class IWindow; }

namespace api
{

namespace renderer { class Renderer; }

/**
* \ingroup CovelliteApiGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс окна-фасада для скрытия реализаций конкретных графических API.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
*  3.0.0.0        \n
*  3.1.0.0        \n
* \date
*  16 Октябрь 2017    \n
*  10 Июнь 2018    \n
*  20 Август 2018    \n
*  19 Ноябрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017 - 2018
*/
class Window final :
  public ::covellite::core::IWindow,
  public ::covellite::app::IWindow,
  public ::covellite::api::IWindow
{
  using WindowOs_t = ::covellite::os::IWindow;
  using WindowOsPtr_t = ::std::shared_ptr<WindowOs_t>;
  using Renderer_t = covellite::api::renderer::Renderer;
  using RendererPtr_t = ::std::shared_ptr<Renderer_t>;

public:
  // Интерфейс events::IEvents:
  operator Events_t (void) const override;

public:
  // Интерфейс api::IWindow:
  Rect_t GetClientRect(void) const override;
  RendersPtr_t GetRenders(void) const override;
  RenderInterfacePtr_t GetRenderInterface(void) const override;
  int32_t GetWidth(void) const override;
  int32_t GetHeight(void) const override;
  RenderInterfacePtr_t MakeRenderInterface(void) const override;

public:
  // Интерфейс core::IWindow:
  void Subscribe(const EventHandlerPtr_t &) override;

private:
  static RendererPtr_t MakeRender(const WindowOs_t &);

private:
  const WindowOs_t &  m_WindowOs;
  Events_t            m_Events;
  const RendererPtr_t m_pImpl;

public:
  explicit Window(const WindowOs_t &);
  explicit Window(const WindowOsPtr_t &);
  ~Window(void);
};

} // namespace api

} // namespace covellite
