
#pragma once

namespace covellite
{

namespace core
{

/**
* \ingroup gCovelliteCoreClasses
* \brief
*  ����� ������ � ������ \ref CovelliteCorePage \n
*  ����� ������������� ��������������� ������� ����������.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.1.1.0        \n
*  1.1.2.0        \n
* \date
*  18 �������� 2017    \n
*  24 �������� 2017    \n
*  13 ������� 2017    \n
*  22 ������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class Event final
{
public:
  // 32 - ��� ���, ������� ���������� java ��� ��� ������� ������ �� 
  // ���������� (������������ ��-�� ����, ��� � �������� ���� ���������� 
  // �������� ��� ������� ������� �� ��������� ASCII ���������).
  static const int32_t APP_CMD_KEY_PRESSED = 32;

public:
  /// ������ ��������� �������.
  enum System
  {
    Stop = 0,    ///< ���������� ������ ���������� (��� ����������).
    Pause,       ///< ����� � ������ ���������� (��� ����������).
    Resume,      ///< ������������� ������ ���������� (��� ����������).
    Destroy,     ///< ���������� ������ ��������� (��� ����������).

    LastSystem
  };

  /// ������ ������� �������.
  enum Window
  {
    Create          ///< �������� ���� ����������. \b �����! � ������� �� 
                    ///< Windows (��� ������� WM_CREATE �������� ����� �������� 
                    ///< ����), ��� ������� ������ ���������, ��� ���� ������ 
                    ///< ���� �������, ������� ��� ������� ������������ ��� 
                    ///< �������� �������� ���� � ������ ���������� (��� 
                    ///< ����������).
      = LastSystem,
    Close,          ///< �������� ���� ���������� (��� ����������).
    LostFocus,      ///< ���������� �������� ����� ����� (��� ����������).
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

    LastWindow
  };

  /// ������ ������� ����� ������������.
  enum Input
  {
    Motion        ///< ������ ���������� ��� �������� ���� �� ����/������ �� 
                  ///< ������ (�������� - params::Motion).
      = LastWindow,
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
                  ///< (�������� - params::KeyCode).
    KeyUp,        ///< ������ ���������� ��� ���������� ������ �� ����������;
                  ///< ������������ ��� ��������� ������������ ���� ������
                  ///< (�������� - params::KeyCode).
    Back,         ///< ������ �������� � ����������� ����. ������������ ��� 
                  ///< ������� Alt + Left � Windows � Back (������ ������) �
                  ///< Android (��� ����������).

    LastInput
  };

  /// ������ ������������� ������� ����������.
  enum Framework
  {
    Error       ///< ���������� ��� ������ ����������� ������ �������� 
                ///< (�������� - params::Error).
      = LastInput,

    LastFramework
  };

  /// ������ ������� libRocket.
  enum Rocket
  {
    Click       ///< ���� �� �������� ���������� (�������� - params::Click).
      = LastFramework,  
  };
};

} // namespace core

} // namespace covellite
