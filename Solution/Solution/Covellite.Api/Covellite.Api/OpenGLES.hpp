
#pragma once
#include <memory>
#include <Covellite\Core\IWindow.hpp>
#include <Covellite\Api\IWindow.hpp>
#include <Covellite\Api\OpenGLCommon.hpp>

namespace covellite
{

namespace egl
{

class Display;
class Config;
class Surface;
class Context;

}

namespace os { class IWindow; }

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
*  1.1.0.0        \n
*  1.2.0.0        \n
*  1.3.0.0        \n
*  2.0.0.0        \n
*  2.1.0.0        \n
* \date
*  17 Ноябрь 2016    \n
*  19 Ноябрь 2016    \n
*  09 Декабрь 2016    \n
*  19 Сентябрь 2017    \n
*  26 Сентябрь 2017    \n
*  28 Сентябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/
class OpenGLES final :
  public ::covellite::core::IWindow,
  public ::covellite::api::IWindow,
  public OpenGLCommon
{
  template<class TEgl>
  using EglPtr_t = ::std::unique_ptr<TEgl>;
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

protected:
  // Интерфейс OpenGLCommon:
  void GlOrtho(int32_t, int32_t) override;

private:
  EglPtr_t<covellite::egl::Display> m_pDisplay;
  EglPtr_t<covellite::egl::Config>  m_pConfig;
  EglPtr_t<covellite::egl::Surface> m_pSurface;
  EglPtr_t<covellite::egl::Context> m_pContext;

public:
  explicit OpenGLES(const WindowOsPtr_t &);
  ~OpenGLES(void) noexcept;
};

} // namespace api

} // namespace covellite
