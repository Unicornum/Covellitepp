
#include "stdafx.h"
#include "CallProxy.hpp"

using namespace covellite::egl;

// cppcheck-suppress unusedFunction
/*static*/ bool CallProxy::IsSuccessfull(const EGLBoolean _Result)
{
  return (_Result != EGL_FALSE);
}

// cppcheck-suppress unusedFunction
/*static*/ ::std::string CallProxy::GetMessageText(const EGLBoolean)
{
  ::std::stringstream ErrorDescription;
  ErrorDescription << "EGL error: " << eglGetError();
  return ErrorDescription.str();
}
