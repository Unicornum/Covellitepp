
#pragma once
#include <Covellite/Predefined.hpp>
#include <Covellite/Events/Events.hpp>
#include <Covellite/App/IWindow.hpp>
#include <Covellite/Os/IWindow.hpp>

namespace covellite
{

namespace app { class IApplication; }

namespace os
{

/**
* \ingroup CovelliteOsGroup
* \brief
*  Класс входит в проект \ref CovelliteOsPage \n
*  Класс окна операционной системы программы.
*
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
*  2.1.0.0        \n
*  2.2.0.0        \n
*  2.3.0.0        \n
* \date
*  17 Ноябрь 2016    \n
*  26 Сентябрь 2017    \n
*  02 Октябрь 2017    \n
*  24 Октябрь 2017    \n
*  06 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2018
*/
class Window final :
  public ::covellite::app::IWindow,
  public ::covellite::os::IWindow
{
  using Configuration_t = covellite::os::Configuration;

public:
  // Интерфейс events::IEvents:
  operator Events_t (void) const override;

public:
  // Интерфейс os::IWindow:
  Any_t GetHandle(void) const override;
  Rect GetClientRect(void) const override;
  const Configuration_t & GetConfiguration(void) const override;

private:
  void ActivateApplicationEvents(void);
  void ActivateResizeEvents(void);
  void ActivateMouseEvents(void);
  void ActivateKeyEvents(void);

private:
  Events_t        m_Events;
  long            m_MinWindowWidth = 0;
  long            m_MinWindowHeight = 0;
  Any_t           m_Handle;
  Configuration_t m_Configuration;
  size_t          m_LastTypeSizeMessage;

public:
  explicit Window(const ::covellite::app::IApplication &);
  ~Window(void);

private:
  FRIEND_TEST(Window_windows_test, /*DISABLED_*/Test_SetWindowLongPtr);
};

} // namespace os

} // namespace covellite
