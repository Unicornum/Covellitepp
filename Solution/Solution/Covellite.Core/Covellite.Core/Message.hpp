
#pragma once

/**
* \ingroup gCovelliteCoreClasses
* \brief
*  ����� ������ � ������ \ref CovelliteCorePage \n
*  ����� ��� �������� ���������� �������-����������� ������� ������������ 
*  �������.
*
* \version
*  1.0.0.0        \n
* \date
*  21 ������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
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
