
#pragma once
#include <memory>
#include <windows.h>
#include <Covellite\Core\IWindow.hpp>
#include <Covellite\Api\IWindow.hpp>
#include <Covellite\Api\OpenGLCommon.hpp>

namespace covellite
{

namespace os { class IWindow; }

namespace api
{

/**
* \ingroup gCovelliteApiClasses
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Реализация окна графического API для Windows/OpenGL.
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
class OpenGL final :
  public ::covellite::core::IWindow,
  public ::covellite::api::IWindow,
  public OpenGLCommon
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

protected:
  // Интерфейс OpenGLCommon:
  void GlOrtho(int32_t, int32_t) override;

private:
  HWND  m_hWnd;
  HDC   m_hDeviceContex;
  HGLRC m_hRenderContex;

public:
  explicit OpenGL(const WindowOsPtr_t &);
  ~OpenGL(void) noexcept;
};

} // namespace api

} // namespace covellite
