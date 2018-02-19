
#pragma once
#include <alicorn\platform\environment.hpp>

namespace covellite
{

namespace jni
{

/**
* \ingroup gCovelliteJniClasses
* \brief
*  Класс входит в проект \ref CovelliteJniPage \n
*  Класс вспомогательных функций.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
* \date
*  07 Декабрь 2016    \n
*  17 Декабрь 2016    \n
*  09 Февраль 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/
class Environment final :
  public alicorn::system::platform::Environment
{
public:
  int32_t GetStatusBarHeight(void);
  void ShowSoftKeyboard(void);
};

} // namespace jni

} // namespace covellite
