
#include "stdafx.h"
#include <Covellite\Core\ClickEventListener.hpp>
#include <alicorn\std\exception.hpp>
#include <Covellite\Core\Params.hpp>
#include <Covellite\Core\Event.hpp>

namespace covellite
{

namespace core
{

EventHandler::ClickEventListener::ClickEventListener(
  OnRocketEvent_t & _OnRocket, OnEvent_t<Event::Framework> & _OnFramework) :
  OnRocket(_OnRocket),
  OnFramework(_OnFramework)
{
}

void EventHandler::ClickEventListener::ProcessEvent(
  Rocket::Core::Event & _Event) /*override*/
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

    const ::std::string ElementId = pTargetElement->GetId().CString();

    const String_t UnknownType = "unknown";

    params::Click ClickInfo
    {
      pDocument->GetId().CString(),
      pTargetElement->GetTagName().CString(),
      pTargetElement->GetAttribute("type", UnknownType).CString()
    };

    OnRocket[Event::Click][ElementId](ClickInfo);
  }
  catch (const ::std::exception & _Ex)
  {
    OnFramework[Event::Error](params::Error{ _Ex.what() });
  }
}

} // namespace core

} // namespace covellite
