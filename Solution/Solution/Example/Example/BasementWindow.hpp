
#pragma once
#include <Covellite/Covellite.hpp>
#include <memory>

namespace basement { class IBasement; }

/**
* \ingroup ExampleGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Промежуточный класс окна отрисовки 3D сцены.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
*  2.1.0.0        \n
* \date
*  04 Ноябрь 2017    \n
*  11 Март 2019    \n
*  24 Июнь 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017 - 2020
*/
class BasementWindow final :
  public ::covellite::app::IWindow
{
  using Events_t = ::covellite::events::Events;
  using WindowApi_t = ::covellite::api::IWindow;
  using WindowExpanse_t = ::covellite::expanse::IWindow;
  using BasementPtr_t = ::std::shared_ptr<::basement::IBasement>;

private:
  Events_t      m_Events;
  BasementPtr_t m_pBasement;

public:
  BasementWindow(const WindowApi_t &, WindowExpanse_t &);
};
