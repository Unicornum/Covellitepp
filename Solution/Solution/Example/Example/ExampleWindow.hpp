
#pragma once
#include <Covellite\Covellite.hpp>

/**
* \ingroup gExampleClasses
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс GUI окна программы.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  12 Декабрь 2016    \n
*  11 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/
class ExampleWindow final :
  public covellite::core::IWindow,
  public covellite::core::Window
{
  using WindowRocketPtr_t = ::std::shared_ptr<covellite::rocket::Window>;
  using Path_t = ::boost::filesystem::path;
  using Layer_t = covellite::rocket::Layer;
  class DemoLayer1;
  class DemoLayer2;
  class TableLayer;
  class ScrollLayer;

public:
  void Subscribe(const EventHandlerPtr_t &) override;

private:
  WindowRocketPtr_t m_pWindowRocket;

public:
  explicit ExampleWindow(const WindowRocketPtr_t &);
};
