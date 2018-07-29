
#pragma once
#include <alicorn/std/string.hpp>
#include <Covellite/Core/IWindow.hpp>
#include <Covellite/Events/Events.hpp>
#include <Covellite/Api/IWindow.hpp>
#include <Covellite/Api/Api.forward.hpp>
#include <Covellite/Api/OpenGLCommon.hpp>

namespace covellite
{

namespace egl
{

class Display;
class Config;
class Surface;
class Context;

}

namespace api
{

/**
* \ingroup gCovelliteApiClasses
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс реализации OpenGLES окна программы.
*
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
*  2.1.0.0        \n
*  2.1.1.0        \n
* \date
*  17 Ноябрь 2016    \n
*  26 Сентябрь 2017    \n
*  28 Сентябрь 2017    \n
*  10 Июнь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2018
*/
class OpenGLES final :
  public Registator_t<::covellite::api::IWindow>,
  public ::covellite::core::IWindow,
  public OpenGLCommon
{
  using Events_t = ::covellite::events::Events;
  template<class TEgl>
  using EglPtr_t = ::std::shared_ptr<TEgl>;

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

protected:
  // Интерфейс OpenGLCommon:
  void GlOrtho(int32_t, int32_t) override;

private:
  Events_t                          m_Events;
  EglPtr_t<covellite::egl::Display> m_pDisplay;
  EglPtr_t<covellite::egl::Config>  m_pConfig;
  EglPtr_t<covellite::egl::Surface> m_pSurface;
  EglPtr_t<covellite::egl::Context> m_pContext;

public:
  explicit OpenGLES(const WindowOs_t &);
  ~OpenGLES(void);
};

FACTORY_REGISTER_STRING_NAME(OpenGLES);

} // namespace api

} // namespace covellite
