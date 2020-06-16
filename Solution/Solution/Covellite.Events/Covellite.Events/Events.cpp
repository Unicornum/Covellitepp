
#include "stdafx.h"
#include <Covellite/Events/Events.hpp>
#include <Covellite/Events/Signals.hpp>

using namespace covellite::events;

Events::Events(void) :
  m_pSignals(::std::make_shared<Signals>())
{

}

Events::Events(const Events & _Events) noexcept :
  m_pSignals(_Events.m_pSignals)
{

}

Events::~Events(void)
{
  for (auto & Connection : m_Connections)
  {
    Connection.disconnect();
  }
}
