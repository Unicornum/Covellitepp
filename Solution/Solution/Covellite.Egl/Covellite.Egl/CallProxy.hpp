
#pragma once
#include <alicorn\std\exception.hpp>
#include <alicorn\std\call-proxy.hpp>

namespace covellite
{

namespace egl
{

/**
* \ingroup gCovelliteEglClasses
* \brief
*  Класс входит в проект \ref CovelliteEglPage \n
*  Вспомогательный класс для работы макроса проверки вызова функций EGL.
*
* \version
*  1.0.0.0        \n
* \date
*  18 Ноябрь 2016    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016
*/
class CallProxy final :
  public ::alicorn::extension::std::CallProxy<CallProxy>
{
public:
  static bool IsSuccessfull(const EGLBoolean);
  static ::std::string GetMessageText(const EGLBoolean);
};

} // namespace egl

} // namespace covellite

#define EGL_CALL \
  CALL_PROXY(::covellite::egl::CallProxy)
