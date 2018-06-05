
#pragma once
#include <alicorn\std\exception.hpp>
#include <alicorn\std\check-proxy.hpp>

namespace covellite
{

namespace egl
{

/**
* \ingroup gCovelliteEglClasses
* \brief
*  ����� ������ � ������ \ref CovelliteEglPage \n
*  ��������������� ����� ��� ������ ������� �������� ������ ������� EGL.
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
class CallProxy final :
  public ::alicorn::extension::std::CheckProxy<CallProxy>
{
public:
  static bool IsSuccessfull(const EGLBoolean);
  static ::std::string GetMessageText(const EGLBoolean);
};

} // namespace egl

} // namespace covellite

#define EGL_CALL \
  CHECK_PROXY(::covellite::egl::CallProxy)
