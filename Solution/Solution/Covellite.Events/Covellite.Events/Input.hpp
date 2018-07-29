
#pragma once
#include <Covellite/Events/Type.hpp>
#include <Covellite/Events/Params.hpp>

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsGroup
* \brief
*  ����� ������ � ������ \ref CovelliteEventsPage \n
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
class Input_t final :
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
    Motion = 0,   ///< ������ ���������� ��� �������� ���� �� ����/������ �� 
                  ///< ������ (�������� - params::Motion).
    Touch,        ///< ������ ���������� ��� ��� ������� ����� ������ ����/
                  ///< ������� ������ ������ (��� ����������).
    Release,      ///< ������ ���������� ��� ���������� ����� ������ ����/
                  ///< ������ ������ �� ������ (��� ����������).
    KeyPressed,   ///< ������ ���������� ��� ������� ������ �� ����������;
                  ///< ������������ ��� �������� Unicode ���� ������� �������
                  ///< ������ ��������� ����� ����� (�������� - 
                  ///< params::KeyPressed).
    KeyDown,      ///< ������ ���������� ��� ������� ������ �� ����������;
                  ///< ������������ ��� ��������� ������������ ���� ������
                  ///< (�������� - int32_t).
    KeyUp,        ///< ������ ���������� ��� ���������� ������ �� ����������;
                  ///< ������������ ��� ��������� ������������ ���� ������
                  ///< (�������� - int32_t).
    Back,         ///< ������ �������� � ����������� ����. ������������ ��� 
                  ///< ������� Alt + Left � Windows � Back (������ ������) �
                  ///< Android (��� ����������).
  };
};

namespace
{

Input_t Input;

} // namespace unnamed

namespace params
{

/**
* \ingroup CovelliteEventsParamsGroup
* \brief
*  ����� ������ � ������ \ref CovelliteEventsPage \n
*  ���������, ������������ ������� �������� ������ �� ������.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  23 �������� 2017    \n
*  24 ������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class Motion final
{
public:
  int32_t X;      ///< X ���������� ������� � ��������.
  int32_t Y;      ///< Y ���������� ������� � ��������.
};

/**
* \ingroup CovelliteEventsParamsGroup
* \brief
*  ����� ������ � ������ \ref CovelliteEventsPage \n
*  ���������, ������������ ������� ������� ������ ����������.
*
* \version
*  1.0.0.0        \n
* \date
*  23 �������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class KeyPressed final
{
public:
  int32_t UnicodeKeyCode;
};

} // namespace params

} // namespace events

} // namespace covellite
