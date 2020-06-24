
#pragma once
#include <alicorn/platform/windows.hpp>
#include "OpenGLCommonShader.hpp"

class OpenGL_test;

namespace covellite
{

namespace api
{

namespace renderer
{

/**
* \ingroup CovelliteApiRendererGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ���������� ������� ������������ API ��� Windows/OpenGL.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
*  3.0.0.0        \n
* \date
*  16 ������� 2017    \n
*  26 ������ 2018    \n
*  09 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017 - 2019
*/
class OpenGL final :
  public OpenGLCommonShader
{
public:
  // ��������� IGraphicApi:
  void PresentFrame(void) override;

private:
  HWND  m_hWnd;
  HDC   m_hDeviceContex;
  HGLRC m_hRenderContex;

public:
  explicit OpenGL(const Data_t &);
  OpenGL(const OpenGL &) = delete;
  OpenGL(OpenGL &&) = delete;
  OpenGL & operator= (const OpenGL &) = delete;
  OpenGL & operator= (OpenGL &&) = delete;
  ~OpenGL(void) noexcept;

private:
  friend OpenGL_test;
};

FACTORY_REGISTER_STRING_NAME(OpenGL);

} // namespace renderer

} // namespace api

} // namespace covellite
