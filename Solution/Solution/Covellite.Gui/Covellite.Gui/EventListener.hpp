
#pragma once
#include <Covellite/Gui/Window.hpp>

namespace covellite
{

namespace gui
{

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Класс входит в \ref CovelliteGuiPage \n
*  Вспомогательный класс для передачи событий libRocket классам окон/слоев.
*
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  17 Декабрь 2016    \n
*  26 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2018
*/
class Window::EventListener final :
  public Rocket::Core::EventListener
{
  using Event_t = Rocket::Core::Event;

public:
  // Интерфейс EventListener:
  void ProcessEvent(Event_t &) override;

public:
  static EventListenerPtr_t Make(const Events_t &);

private:
  Events_t m_Events;

public:
  explicit EventListener(const Events_t &);
};

} // namespace gui

} // namespace covellite
