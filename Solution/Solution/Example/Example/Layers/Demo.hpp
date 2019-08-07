
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
    Animation,
    Exit,

    None,
    MoveForward,
    MoveBackward,
    MoveLeftForward,
    MoveLeftBackward,
    MoveRightForward,
    MoveRightBackward,
    TurnLeft,
    TurnRight,
    TurnUp,
    TurnDown,
  };
};

namespace { Demo_t Demo; }

} // namespace events

namespace layers
{

/**
* \ingroup ExampleGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс GUI панели демонстрационного режима.
*  
* \version
*  1.0.0.0        \n
* \date
*  04 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Demo final :
  public ::covellite::gui::Layer 
{
  using Updater_t = ::std::function<void(void)>;

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
