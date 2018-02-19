
#pragma once

/**
* \ingroup gCovelliteCoreClasses
* \brief
*   ласс входит в проект \ref CovelliteCorePage \n
*   ласс дл€ передачи параметров функции-обработчику событий операционной 
*  системы.
*
* \version
*  1.0.0.0        \n
* \date
*  21 ќкт€брь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/

namespace covellite
{

namespace core
{

#if BOOST_OS_WINDOWS

class Message
{
public:
  UINT Command;
  WPARAM wParam;
  LPARAM lParam;
};

#elif BOOST_OS_ANDROID

class Message
{
public:
  int32_t Command;
  AInputEvent * pInputEvent;
};

#else

class Message 
{ 
};

#endif

} // namespace core

} // namespace covellite
