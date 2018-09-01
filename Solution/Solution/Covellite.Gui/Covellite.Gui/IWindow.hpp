
#pragma once
#include <Covellite/Events/IEvents.hpp>
#include <Covellite/Gui/Rocket.forward.hpp>

namespace covellite
{

namespace gui
{

/**
* \ingroup CovelliteGuiGroup
* \brief
*  ����� ������ � ������ \ref CovelliteGuiPage \n
*  ����������� ����� ����, ������������� �������� GUI.
*  
* \version
*  1.0.0.0        \n
* \date
*  23 ���� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
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

} // namespace gui

} // namespace covellite
