
#pragma once
#include <Covellite/Events/IEvents.hpp>
#include <Covellite/Rocket/Rocket.forward.hpp>

namespace covellite
{

namespace rocket
{

/**
* \ingroup CovelliteRocketGroup
* \brief
*  Класс входит в проект \ref CovelliteRocketPage \n
*  Интерфесный класс окна, отображающего элементы GUI.
*  
* \version
*  1.0.0.0        \n
* \date
*  23 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class IWindow :
  public ::covellite::events::IEvents
{
protected:
  using Document_t = Rocket::Core::ElementDocument;
  using PathToFile_t = Rocket::Core::String;

public:
  virtual Document_t * LoadDocument(const PathToFile_t &) = 0;
};

} // namespace rocket

} // namespace covellite
