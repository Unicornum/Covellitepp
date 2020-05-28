
#include "stdafx.h"
#include <Covellite/Gui/EventListener.hpp>
#include <alicorn/std/exception.hpp>
#include <Covellite/Events/Events.hpp>
#include <Covellite/Events.hpp>
#include <Covellite/Gui/Events.hpp>

using namespace covellite::gui;

  Window::EventListener::EventListener(const Events_t & _Events) :
  m_Events(_Events)
{
}

void Window::EventListener::ProcessEvent(Event_t & _Event) /*override*/
{
  try
  {
    auto * pTargetElement = _Event.GetTargetElement();
    if (pTargetElement == nullptr)
    {
      throw STD_EXCEPTION << "Target element null pointer.";
    }

    auto * pDocument = pTargetElement->GetOwnerDocument();
    if (pDocument == nullptr)
    {
      throw STD_EXCEPTION << "Owner document null pointer.";
    }

    const auto Id = 
      events::Gui{ CovelliteGuiStringToUtf8(_Event.GetType()) }
      .DocumentId(CovelliteGuiStringToUtf8(pDocument->GetId()))
      .ElementId(CovelliteGuiStringToUtf8(pTargetElement->GetId()));

    m_Events[Id]();
  }
  catch (const ::std::exception & _Ex)
  {
    m_Events[events::Error.Exception](_Ex.what());
  }
}

/*static*/ auto Window::EventListener::Make(const Events_t & _Events) -> EventListenerPtr_t
{
  return ::std::make_shared<EventListener>(_Events);
}
