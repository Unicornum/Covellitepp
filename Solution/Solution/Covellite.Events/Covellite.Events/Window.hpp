
#pragma once
#include <Covellite/Events/Type.hpp>

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsGroup
* \brief
*  ����� ������ � ������ \ref CovelliteEventsPage \n
*  ����� ������� ������ ���� ����������.
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
class Window_t final :
  public Type
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  �������������� �������.
  */
  enum Id
  {
    LostFocus = 0,  ///< ���������� �������� ����� ����� (��� ����������).
    GainedFocus,    ///< ���������� �������� ����� ����� (��� ����������).
    Resize,         ///< ��������� �������� ���� ���������, ��������,
                    ///< ��� �������� Android ���������� (��� ����������).
    Drawing,        ///< ������ ���������� ��� ������������� ����������� ����.
                    ///< � ����������� �� ������� ������� ��������� 
                    ///< (��. Application::Run()), ����� ���������� ����������,
                    ///< ���� ������ ����� ��������� ���������� ���������
                    ///< (��� ����������).
    StartDrawing,   ///< ������ ���������� ��������������� ����� �������� 
                    ///< Drawing (��� ����������).
    FinishDrawing,  ///< ������ ���������� ����� ����� ������� Drawing
                    ///< (��� ����������).
    Close,          ///< �������� ���� ���������� (��� ����������).
  };
};

namespace
{

Window_t Window;

} // namespace unnamed

} // namespace events

} // namespace covellite
