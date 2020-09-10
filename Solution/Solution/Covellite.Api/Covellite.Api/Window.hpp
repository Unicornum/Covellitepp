
#pragma once
#include <memory>
#include <Covellite/Events/Events.hpp>
#include <Covellite/App/IWindow.hpp>
#include <Covellite/Api/IWindow.hpp>

namespace covellite
{

namespace os { class IWindow; }

namespace api
{

namespace renderer { class IGraphicApi; }
namespace renderer { class SettingsData; }

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
  public ::covellite::app::IWindow,
  public ::covellite::api::IWindow
{
  using String_t = ::alicorn::extension::std::String;
  using WindowOs_t = ::covellite::os::IWindow;
  using IGraphicApiPtr_t = 
    ::alicorn::extension::std::unique_ptr<renderer::IGraphicApi>;

public:
  // Интерфейс events::IEvents:
  operator Events_t (void) const override;

public:
  // Интерфейс api::IWindow:
  Rect GetClientRect(void) const override;
  RendersPtr_t GetRenders(void) const override;

private:
  static IGraphicApiPtr_t MakeImpl(const WindowOs_t &);
  static IGraphicApiPtr_t MakeImpl(const String_t &, const renderer::SettingsData &);
  static ::std::vector<String_t> GetRenderers(void);

private:
  const WindowOs_t &      m_WindowOs;
  Events_t                m_Events;
  const IGraphicApiPtr_t  m_pImpl;

public:
  explicit Window(const WindowOs_t &);
  ~Window(void);
};

} // namespace api

} // namespace covellite
