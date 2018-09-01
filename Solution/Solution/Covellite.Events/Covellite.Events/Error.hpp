
#pragma once
#include <string>

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  ����� ������ � ������ \ref CovelliteEventsPage \n
*  ����� ������� ������ �� ����� ������ ����������.
*
* \version
*  1.0.0.0        \n
* \date
*  16 ���� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class Error_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  �������������� �������.
  */
  enum Id
  {
    Exception = 0,  ///< ��� ������ ����������� ������� ���� �����������
                    ///< ���������� (�������� - Error_t::Description).
  };

public:
  /**
  * \ingroup CovelliteEventsParamsGroup
  * \brief
  *  ����� ������ � ������ \ref CovelliteEventsPage \n
  *  ����� ��������� ��� �������� ������� �������� ������.
  */
  using Description = ::std::string;
};

namespace { Error_t Error; }

} // namespace events

} // namespace covellite
