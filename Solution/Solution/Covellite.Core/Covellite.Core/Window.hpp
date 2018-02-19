
#pragma once

namespace covellite
{

namespace core
{

/**
* \ingroup gCovelliteCoreClasses
* \brief
*   ласс входит в проект \ref CovelliteCorePage \n
*  Ѕазовый класс дл€ всех классов окон.
* \details
*  - —одержит код, который должен быть доступен дл€ всех классов окон всех
*  проектов.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  03 ќкт€брь 2017    \n
*  21 ќкт€брь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class Window
{
public:
  int32_t GetStatusBarHeight(void) const;
  void Exit(void) const;
};

} // namespace core

} // namespace covellite
