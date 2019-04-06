
#pragma once
#include <Covellite/Covellite.hpp>

namespace events
{

class Demo_t
{
public:
  enum Id
  {
    Start = 0,
    Auto,
    Manual,
    Exit,
    MoveForward,
    MoveBackward,
    MoveLeftForward,
    MoveLeftBackward,
    MoveRightForward,
    MoveRightBackward,
    TurnLeft,
    TurnRight,
  };
};

namespace { Demo_t Demo; }

} // namespace events

namespace layers
{

/**
* \ingroup ExampleGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� GUI ������ ����������������� ������.
*  
* \version
*  1.0.0.0        \n
* \date
*  04 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class Demo final :
  public ::covellite::gui::Layer 
{
  using Updater_t = ::std::function<void(void)>;

public:
  void Subscribe(const EventHandlerPtr_t &) override {}

private:
  void EmployFontSize(void);
  void ActivateProcessMainMenuEvents(void);
  void ActivateProcessGameMenuEvents(void);
  void ActivateProcessUserActionEvents(void);
  Updater_t GetProcessLoading(void);
  Updater_t GetProcessCalculateFps(void);

private:
  Updater_t m_Updater = [](void) {};
  int m_PercentLoading = 0;

public:
  explicit Demo(IWindowGui_t &);
};

} // namespace layers
