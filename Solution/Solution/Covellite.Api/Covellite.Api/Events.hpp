
#pragma once

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsGroup
* \brief
*  ����� ������ � ������ \ref CovelliteAppPage \n
*  ����� ������� ��������� ���� ���������.
*  
* \version
*  1.0.0.0        \n
* \date
*  21 ���� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class Drawing_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  �������������� �������.
  */
  enum Id
  {
    Do, ///< ������������ ��� ������������� ��������� ���� ��������� 
        ///< (��� ����������).
  };
};

namespace { Drawing_t Drawing; }

} // namespace events

} // namespace covellite
