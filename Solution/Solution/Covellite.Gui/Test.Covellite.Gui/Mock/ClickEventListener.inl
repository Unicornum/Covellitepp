
#pragma once
#include <Covellite/Gui/ClickEventListener.hpp>

namespace covellite
{

namespace gui
{

namespace mock
{

class ClickEventListenerProxy :
  public ::alicorn::extension::testing::Proxy<ClickEventListenerProxy>
{
  using ClickEventListenerPtr_t = 
    ::covellite::gui::Window::ClickEventListenerPtr_t;

public:
  MOCK_METHOD1(Make, ClickEventListenerPtr_t(::mock::Id_t));
};

} // namespace mock

inline Window::ClickEventListener::ClickEventListener(const Events_t &)
{

}

inline void Window::ClickEventListener::ProcessEvent(Event_t &) /*override*/
{

}

inline /*static*/ auto Window::ClickEventListener::Make(const Events_t &) -> ClickEventListenerPtr_t
{
  return mock::ClickEventListenerProxy::GetInstance()->Make(0);
}

} // namespace gui

} // namespace covellite
