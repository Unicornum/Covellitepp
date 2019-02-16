
#pragma once
#include <Covellite/Gui/EventListener.hpp>

namespace covellite
{

namespace gui
{

namespace mock
{

class EventListenerProxy :
  public ::alicorn::extension::testing::Proxy<EventListenerProxy>
{
  using EventListenerPtr_t = 
    ::covellite::gui::Window::EventListenerPtr_t;

public:
  MOCK_METHOD1(Make, EventListenerPtr_t(::mock::Id_t));
};

} // namespace mock

inline Window::EventListener::EventListener(const Events_t &)
{

}

inline void Window::EventListener::ProcessEvent(Event_t &) /*override*/
{

}

inline /*static*/ auto Window::EventListener::Make(const Events_t &) -> EventListenerPtr_t
{
  return mock::EventListenerProxy::GetInstance()->Make(0);
}

} // namespace gui

} // namespace covellite
