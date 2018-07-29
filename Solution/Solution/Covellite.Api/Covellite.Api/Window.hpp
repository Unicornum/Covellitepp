
#pragma once
#include <memory>
#include <alicorn\std\memory\unique-ptr.hpp>
#include <Covellite\Core\IWindow.hpp>
#include <Covellite\App\IWindow.hpp>
#include <Covellite\Api\IWindow.hpp>

namespace covellite
{

namespace os { class IWindow; }

namespace api
{

/**
* \ingroup gCovelliteApiClasses
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс окна-фасада для скрытия реализаций конкретных графических API.
*  
* \tparam TApiImpl
*  Класс реализации окна для конкретного графического API:
*  - Android:
*   + OpenGLES.
*  - Windows:
*   + OpenGL.
*
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  16 Октябрь 2017    \n
*  10 Июнь 2018    \n
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
  using IWindow_t = ::covellite::api::IWindow;
  using IApiPtr_t = ::alicorn::extension::std::unique_ptr<IWindow_t>;

public:
  // Интерфейс core::IWindow:
  void Subscribe(const EventHandlerPtr_t &) override;

public:
  // Интерфейс events::IEvents:
  operator Events_t (void) const override;

public:
  // Интерфейс api::IWindow:
  String_t GetUsingApi(void) const override;
  int32_t GetWidth(void) const override;
  int32_t GetHeight(void) const override;
  Rect GetClientRect(void) const override;
  RenderInterfacePtr_t MakeRenderInterface(void) const override;

private:
  static IApiPtr_t MakeApiImpl(const WindowOs_t &);

private:
  IApiPtr_t m_pImpl;

public:
  explicit Window(const WindowOs_t &);
  explicit Window(const WindowOsPtr_t &);
  ~Window(void);
};

} // namespace api

} // namespace covellite
