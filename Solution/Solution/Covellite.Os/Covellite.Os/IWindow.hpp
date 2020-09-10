
#pragma once
#include <Covellite/Predefined.forward.hpp>
#include <Covellite/Events/IEvents.hpp>
#include <Covellite/Os/Configuration.hpp>

namespace covellite
{

namespace os
{

/**
* \ingroup CovelliteOsGroup
* \brief
*  Класс входит в проект \ref CovelliteOsPage \n
*  Интерфейсный класс окна операционной системы программы.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  02 Октябрь 2017    \n
*  02 Май 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017 - 2019
*/
class IWindow :
  public ::covellite::events::IEvents
{
  using Configuration_t = covellite::os::Configuration;

public:
  virtual Any_t GetHandle(void) const = 0;
  virtual Rect GetClientRect(void) const = 0;
  virtual const Configuration_t & GetConfiguration(void) const = 0;

public:
  virtual ~IWindow(void) = default;
};

} // namespace os

} // namespace covellite
