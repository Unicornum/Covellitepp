
#pragma once

namespace covellite
{

namespace core
{

/**
* \ingroup gCovelliteCoreClasses
* \brief
*  ����� ������ � ������ \ref CovelliteCorePage \n
*  ������� ����� ��� ���� ������� ����.
* \details
*  - �������� ���, ������� ������ ���� �������� ��� ���� ������� ���� ����
*  ��������.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  03 ������� 2017    \n
*  21 ������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class Window
{
public:
  int32_t GetStatusBarHeight(void) const;
  void Exit(void) const;
};

} // namespace core

} // namespace covellite
