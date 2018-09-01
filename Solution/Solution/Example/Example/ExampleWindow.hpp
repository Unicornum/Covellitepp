
#pragma once
#include <Covellite\Covellite.hpp>

/**
* \ingroup ExampleGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс GUI окна программы.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  12 Декабрь 2016    \n
*  28 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2018
*/
class ExampleWindow final :
  public ::covellite::app::IWindow
{
  using Events_t = ::covellite::events::Events;
  using WindowRocket_t = ::covellite::gui::Window;
  using Path_t = ::boost::filesystem::path;
  using IWindowRocket_t = ::covellite::gui::IWindow;
  using Layer_t = ::covellite::gui::Layer;
  class DemoLayer1;
  class DemoLayer2;
  class TableLayer;
  class ScrollLayer;

private:
  WindowRocket_t & m_WindowRocket;
  Events_t m_Events;

public:
  explicit ExampleWindow(WindowRocket_t &);
};
