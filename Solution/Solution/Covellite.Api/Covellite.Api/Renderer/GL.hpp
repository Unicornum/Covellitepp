
#pragma once

#ifndef __USING_GTEST

#if BOOST_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "GLExt.hpp"

#elif BOOST_OS_ANDROID

#include <egl\egl.h>
#include <gles\gl.h>
#include <gles\glext.h>

inline void glOrtho(GLfloat _Left, GLfloat _Right, GLfloat _Bottom,
  GLfloat _Top, GLfloat _zNear, GLfloat _zFar)
{
  glOrthof(_Left, _Right, _Bottom, _Top, _zNear, _zFar);
}

inline void glFogi(GLenum _Name, GLint _Value)
{
  glFogx(_Name, _Value);
}

inline void glClearDepth(GLfloat depth)
{
  glClearDepthf(depth);
}

#if __ANDROID_API__ >= 18

#include <gles3\gl3.h>
#include <gles3\gl3ext.h>

#endif // __ANDROID_API__
#endif // BOOST_OS_...

#endif // __USING_GTEST
