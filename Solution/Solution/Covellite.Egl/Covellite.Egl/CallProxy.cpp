
#include "stdafx.h"
#include "CallProxy.hpp"

using namespace covellite::egl;

/*static*/ bool CallProxy::IsSuccessfull(const EGLBoolean _Result)
{
  return (_Result != EGL_FALSE);
}

/*static*/ ::std::string CallProxy::GetMessageText(const EGLBoolean)
{
  ::std::stringstream ErrorDescription;
  ErrorDescription << "EGL error: " << eglGetError();
  return ErrorDescription.str();
}
