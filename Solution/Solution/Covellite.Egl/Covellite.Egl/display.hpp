
#pragma once

namespace covellite
{

namespace egl
{

class Surface;
class Context;

/**
* \ingroup gCovelliteEglClasses
* \brief
*  ����� ������ � ������ \ref CovelliteEglPage \n
*  �����-������ ��� EGLDisplay.
* \details
*  - ������������ ��� ���������������� ����������� ����������� ����������
*  �������.
*
* \version
*  1.0.0.0        \n
* \date
*  18 ������ 2016    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2016
*/
class Display final
{
private:
  EGLDisplay m_Display;

public:
  Display(void);
  ~Display(void) noexcept;

private:
  friend class Config;
  friend class Surface;
  friend class Context;
};

} // namespace egl

} // namespace covellite
