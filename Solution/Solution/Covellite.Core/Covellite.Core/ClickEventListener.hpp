
#pragma once
#include <Covellite\Core\EventHandler.hpp>

namespace covellite
{

namespace core
{

/**
* \ingroup CovelliteCoreGroup
* \brief
*  ����� ������ � \ref CovelliteCorePage \n
*  ��������������� ����� ��� �������� ������� libRocket ������� ����/�����.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
*  1.3.0.0        \n
* \date
*  17 ������� 2016    \n
*  28 ������� 2016    \n
*  11 ������ 2017    \n
*  11 ������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2016 - 2017
*/
class EventHandler::ClickEventListener final:
  public Rocket::Core::EventListener
{
public:
  // ��������� EventListener
  void ProcessEvent(Rocket::Core::Event &) override;

private:
  OnRocketEvent_t &             OnRocket;
  OnEvent_t<Event::Framework> & OnFramework;

public:
  ClickEventListener(OnRocketEvent_t &, OnEvent_t<Event::Framework> &);
};

} // namespace core

} // namespace covellite
