
#pragma once

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  ����� ������ � ������ \ref CovelliteOsPage \n
*  ����� ������� ���� ������������ �������.
*  
* \version
*  1.0.0.0        \n
* \date
*  06 ���� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class Window_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  �������������� �������.
  */
  enum Id
  {
    Resize = 0,  ///< ��������� �������� ���� (��� ����������).
  };
};

namespace { Window_t Window; }

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  ����� ������ � ������ \ref CovelliteOsPage \n
*  ����� ������� �������.
*
* \version
*  1.0.0.0        \n
* \date
*  06 ���� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class Cursor_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  �������������� �������.
  */
  enum Id
  {
    Motion = 0, ///< ����������� ������� (�������� - Position, ���������� ����� 
                ///< ���������� �������).
    Touch,      ///< ������� �������; � Windows - ������� ����� ������ ����,
                ///< � Android - ������� ������ (��� ����������).
    Release,    ///< ���������� ������� (��� ����������).
  };

public:
  /**
  * \ingroup CovelliteEventsParamsGroup
  * \brief
  *  ����� ������ � ������ \ref CovelliteOsPage \n
  *  ����� ��������� ��� �������� ������� ��������� �������.
  */
  class Position final
  {
  public:
    const int32_t X;
    const int32_t Y;

  public:
    Position(int32_t _X, int32_t _Y) : X(_X), Y(_Y) {}
  };
};

namespace { Cursor_t Cursor; }

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  ����� ������ � ������ \ref CovelliteOsPage \n
*  ����� ������� ������ ����������.
*
* \version
*  1.0.0.0        \n
* \date
*  07 ���� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class Key_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  �������������� �������.
  */
  enum Id
  {
    Pressed = 0,  ///< ������� ���������� ������� ���������� (�������� - 
                  ///< Key_t::Code, ���������� UNICODE ��� ������� �������).
    Down,         ///< ������� ������� ���������� (�������� - Key_t::Code, 
                  ///< ���������� ASCII ��� ������� �������).
    Up,           ///< ���������� ������� ���������� (�������� - Key_t::Code, 
                  ///< ���������� ASCII ��� ������� �������).
    Menu,         ///< ������� ������ ���� ���������; ALT + ������ � Windows,
                  ///< ����� ������ ������ � Android,  (��� ����������).
    Back,         ///< ������� ������ �������� � ����������� ����; 
                  ///< ALT + <- � Windows, ������ ������ ������ � Android
                  ///< (��� ����������).
  };

public:
  /**
  * \ingroup CovelliteEventsParamsGroup
  * \brief
  *  ����� ������ � ������ \ref CovelliteOsPage \n
  *  ����� ��������� ��� �������� ������� ��������� �������.
  */
  using Code = int32_t;

public:
  // 32 - ��� ���, ������� ���������� java ��� ��� ������� ������ �� 
  // ���������� (������������ ��-�� ����, ��� � �������� ���� ���������� 
  // �������� ��� ������� ������� �� ��������� ASCII ���������).
  static const int32_t APP_CMD_KEY_PRESSED = 32;
};

namespace { Key_t Key; }

} // namespace events

} // namespace covellite
