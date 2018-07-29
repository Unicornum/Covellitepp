
#include "stdafx.h"
#include <Covellite/Rocket/ClickEventListener.hpp>
#include <alicorn/std/exception.hpp>
#include <Covellite/Events/Events.hpp>
#include <Covellite/Events.hpp>
#include <Covellite/Rocket/Events.hpp>

using namespace covellite::rocket;

  Window::ClickEventListener::ClickEventListener(const Events_t & _Events) :
  m_Events(_Events)
{
}

void Window::ClickEventListener::ProcessEvent(Event_t & _Event) /*override*/
{
  using String_t = Rocket::Core::String;

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

    const ::std::string DocumentId = pDocument->GetId().CString();
    const ::std::string ElementId = pTargetElement->GetId().CString();

    auto Id = events::Click.DocumentId(DocumentId).ElementId(ElementId);

    const String_t UnknownType = "unknown";

    events::Click_t::Info ClickInfo
    {
      pTargetElement->GetTagName().CString(),
      pTargetElement->GetAttribute("type", UnknownType).CString()
    };

    m_Events[Id](ClickInfo);
  }
  catch (const ::std::exception & _Ex)
  {
    m_Events[events::Error.Exception](_Ex.what());
  }
}

/*static*/ auto Window::ClickEventListener::Make(const Events_t & _Events) -> ClickEventListenerPtr_t
{
  return ::std::make_shared<ClickEventListener>(_Events);
}
