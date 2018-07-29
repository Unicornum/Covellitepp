
#pragma once

namespace covellite
{

namespace app
{

/**
* \ingroup CovelliteAppGroup
* \brief
*  Класс входит в проект \ref CovelliteAppPage \n
*  Интерфейсный класс для классов окон фреймворка.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
*  2.1.0.0        \n
*  2.2.0.0        \n
* \date
*  19 Ноябрь 2016    \n
*  19 Сентябрь 2017    \n
*  24 Сентябрь 2017    \n
*  20 Июнь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2018
*/
class IWindow
{
public:
  virtual ~IWindow(void) = default;
};

} // namespace app

} // namespace covellite
