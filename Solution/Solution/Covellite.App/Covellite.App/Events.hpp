
#pragma once

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  ����� ������ � ������ \ref CovelliteAppPage \n
*  ����� ������� ������ ����������.
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
class Application_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  �������������� �������.
  */
  enum Id
  {
    Start = 0,  ///< ������������ ��� ������� ��������� (��� ����������). \n
                ///< \b �����! � ������� �� Windows (��� ������� WM_CREATE 
                ///< �������� \b ����� �������� ����), ��� ������� ������ 
                ///< ���������, ��� ���� \b ������ ���� �������, ������� ��� 
                ///< ������� ������������ ��� �������� �������� ���� � ������ 
                ///< ����������.
    Update,     ///< ������������ ��� ������������� ���������� ��������� 
                ///< ��������� (��� ����������).
    Exit,       ///< - ������������ ��� ���������� ������ ��������� 
                ///< (��� ����������).
                ///< - ����� ������� �� ������ ����� ��������� ��� �� 
                ///< ���������� (��� ����������).
  };
};

namespace { Application_t Application; }

} // namespace events

} // namespace covellite
