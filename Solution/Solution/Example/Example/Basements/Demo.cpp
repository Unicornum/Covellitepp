
#include "stdafx.h"
#include "Demo.hpp"
#include <alicorn/logger.hpp>
#include "Demo/Controller.hpp"

using namespace basement;

Demo::Demo(const Events_t & _Events, const RendersPtr_t & _pRenders) :
  m_View(_pRenders),
  m_Model(_Events)
{
  controller::Controller{ (model::IDbComponents &)m_Model, m_View };
  controller::Controller{ (model::IGameWorld &)m_Model, m_View };
}

Demo::~Demo(void)
{
  LOGGER(Info) << "Basements Demo destoyed.";
}

void Demo::Notify(int, const ::boost::any &) /*override*/
{

}

void Demo::Render(void) /*override*/
{
  m_View.Render();
}
