
#pragma once
#include <windows.h>
#include "OpenGLCommon.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ���������� ������� ������������ API ��� Windows/OpenGL.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  16 ������� 2017    \n
*  26 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017 - 2018
*/
class OpenGL final :
  public OpenGLCommon
{
public:
  // ��������� IRender:
  void Present(void) override;

protected:
  void DoOrtho(int32_t, int32_t) override;

private:
  HWND        m_hWnd;
  HDC         m_hDeviceContex;
  HGLRC       m_hRenderContex;

public:
  explicit OpenGL(const Data &);
  ~OpenGL(void);
};

FACTORY_REGISTER_STRING_NAME(OpenGL);

} // namespace renderer

} // namespace api

} // namespace covellite
