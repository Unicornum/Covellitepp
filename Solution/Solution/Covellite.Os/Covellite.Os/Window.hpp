
#pragma once
#include <Covellite\Core\IWindow.hpp>
#include <Covellite\Os\IWindow.hpp>

namespace covellite
{

namespace os
{

/**
* \ingroup gCovelliteOsClasses
* \brief
*  Класс входит в проект \ref CovelliteOsPage \n
*  Класс окна операционной системы программы.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
*  1.3.0.0        \n
*  2.0.0.0        \n
*  2.1.0.0        \n
*  2.2.0.0        \n
* \date
*  17 Ноябрь 2016    \n
*  19 Ноябрь 2016    \n
*  09 Декабрь 2016    \n
*  19 Сентябрь 2017    \n
*  26 Сентябрь 2017    \n
*  02 Октябрь 2017    \n
*  24 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/
class Window final :
  public ::covellite::core::IWindow,
  public ::covellite::os::IWindow
{
  using Configuration_t = covellite::os::Configuration;

public:
  // Интерфейс core::IWindow:
  void Subscribe(const EventHandlerPtr_t &) override;

public:
  // Интерфейс os::IWindow:
  Handle_t GetHandle(void) const override;
  const Configuration_t & GetConfiguration(void) const override;

private:
  Handle_t        m_Handle;
  Configuration_t m_Configuration;

public:
  Window(void);
  ~Window(void) noexcept;
};

} // namespace os

} // namespace covellite
