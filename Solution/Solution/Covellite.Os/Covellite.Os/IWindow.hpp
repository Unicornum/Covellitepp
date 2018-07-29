
#pragma once
#include <Covellite/Os/Predefined.hpp>
#include <Covellite/Os/Configuration.hpp>
#include <Covellite/Events/IEvents.hpp>

namespace covellite
{

namespace os
{

/**
* \ingroup gCovelliteOsClasses
* \brief
*  Класс входит в проект \ref CovelliteOsPage \n
*  Интерфейсный класс окна операционной системы программы.
*
* \version
*  1.0.0.0        \n
* \date
*  02 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class IWindow :
  public ::covellite::events::IEvents
{
  using Configuration_t = covellite::os::Configuration;

public:
  virtual Handle_t GetHandle(void) const = 0;
  virtual const Configuration_t & GetConfiguration(void) const = 0;

public:
  virtual ~IWindow(void) = default;
};

} // namespace os

} // namespace covellite
