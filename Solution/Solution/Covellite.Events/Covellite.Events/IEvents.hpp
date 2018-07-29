
#pragma once

namespace covellite
{

namespace events
{

class Events;

/**
* \ingroup CovelliteEventsGroup
* \brief
*  ����� ������ � ������ \ref CovelliteEventsPage \n
*  ������������ ����� ��� �������, ������� ������ ������������ ������� 
*  ����������.
* \details
*  - ������������ ��� ������������ ������������ ��������, ������� ����� 
*  �������������� ��� �������� ������� ������� ���������� ����� �� �������� 
*  �������.
*  
* \version
*  1.0.0.0        \n
* \date
*  17 ���� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class IEvents
{
protected:
  using Events_t = ::covellite::events::Events;

public:
  virtual operator Events_t (void) const = 0;

public:
  virtual ~IEvents(void) = default;
};

} // namespace events

} // namespace covellite
