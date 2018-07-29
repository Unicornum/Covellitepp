
#pragma once
#include <windows.h>
#include <alicorn/std/string.hpp>
#include <Covellite/Core/IWindow.hpp>
#include <Covellite/Events/Events.hpp>
#include <Covellite/Api/Api.forward.hpp>
#include <Covellite/Api/IWindow.hpp>
#include <Covellite/Api/OpenGLCommon.hpp>

namespace covellite
{

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
*  1.0.1.0        \n
* \date
*  16 Октябрь 2017    \n
*  10 Июнь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017 - 2018
*/
class OpenGL final :
  public Registator_t<::covellite::api::IWindow>,
  public ::covellite::core::IWindow,
  public OpenGLCommon
{
  using Events_t = ::covellite::events::Events;

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
  Events_t  m_Events;
  HWND      m_hWnd;
  HDC       m_hDeviceContex;
  HGLRC     m_hRenderContex;

public:
  explicit OpenGL(const WindowOs_t &);
  ~OpenGL(void) noexcept;
};

FACTORY_REGISTER_STRING_NAME(OpenGL);

} // namespace api

} // namespace covellite
