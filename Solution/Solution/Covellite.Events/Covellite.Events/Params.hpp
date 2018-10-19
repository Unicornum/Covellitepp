
#pragma once
#include <Covellite/Predefined.hpp>

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsParamsGroup
* \brief
*  ����� ������ � ������ \ref CovelliteEventsPage \n
*  ��������������� ����� ��� �������� ���������� ������������ �������.
* \details
*  - ����� ��� ����, ����� � �������-������������ ������� � �������� ���������
*  ����� ���� ��������� ��������������� ���� �� �������
*  \ref CovelliteEventsParamsGroup.
*
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  23 �������� 2017    \n
*  29 ���� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017 - 2018
*/
class Params final
{
public:
  template<class T>
  inline operator const T & (void) const 
  { 
    return ::covellite::any_cast<const T &>(m_Params);
  }

private:
  const Any_t m_Params;

public:
  template<class T>
  inline Params(const T & _Value) : m_Params(_Value) {}
};

} // namespace events

} // namespace covellite
