
#pragma once
#include <memory>
#include <Covellite\Core\IWindow.hpp>
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
*
* \version
*  1.0.0.0        \n
* \date
*  16 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
template<class TApiImpl>
class Window final :
  public ::covellite::core::IWindow,
  public ::covellite::api::IWindow
{
  using WindowOsPtr_t = ::std::shared_ptr<::covellite::os::IWindow>;

public:
  // Интерфейс core::IWindow:
  void Subscribe(const EventHandlerPtr_t &) override;

public:
  // Интерфейс api::IWindow:
  String_t GetUsingApi(void) const override;
  int32_t GetWidth(void) const override;
  int32_t GetHeight(void) const override;
  RenderInterfacePtr_t MakeRenderInterface(void) const override;

private:
  ::std::unique_ptr<TApiImpl> m_pImpl;

public:
  explicit Window(const WindowOsPtr_t &);
  ~Window(void) noexcept;
};

#if BOOST_OS_WINDOWS

// Реализации, доступные в Windows:
extern template class Window<class OpenGL>;

#elif BOOST_OS_ANDROID

// Реализации, доступные в Android:
extern template class Window<class OpenGLES>;

#endif

} // namespace api

} // namespace covellite
