
#pragma once
#include <Covellite/Rocket/Window.hpp>

namespace covellite
{

namespace rocket
{

/**
* \ingroup CovelliteRocketGroup
* \brief
*  ����� ������ � \ref CovelliteRocketPage \n
*  ��������������� ����� ��� �������� ������� libRocket ������� ����/�����.
*
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  17 ������� 2016    \n
*  26 ���� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2016 - 2018
*/
class Window::ClickEventListener final :
  public Rocket::Core::EventListener
{
  using Event_t = Rocket::Core::Event;

public:
  // ��������� EventListener:
  void ProcessEvent(Event_t &) override;

public:
  static ClickEventListenerPtr_t Make(const Events_t &);

private:
  Events_t m_Events;

public:
  explicit ClickEventListener(const Events_t &);
};

} // namespace rocket

} // namespace covellite
