
#pragma once

#if BOOST_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#define GL_CLAMP_TO_EDGE 0x812F

#elif BOOST_OS_ANDROID
#ifndef __USING_GTEST

#include <egl\egl.h>
#include <gles\gl.h>
#include <gles\glext.h>

inline void glOrtho(GLfloat _Left, GLfloat _Right, GLfloat _Bottom,
  GLfloat _Top, GLfloat _zNear, GLfloat _zFar)
{
  glOrthof(_Left, _Right, _Bottom, _Top, _zNear, _zFar);
}

inline void glFrustum(GLfloat _Left, GLfloat _Right, GLfloat _Bottom,
  GLfloat _Top, GLfloat _zNear, GLfloat _zFar)
{
  glFrustumf(_Left, _Right, _Bottom, _Top, _zNear, _zFar);
}

inline void glClearDepth(GLfloat _Depth)
{
  glClearDepthf(_Depth);
}

inline void glDepthRange(GLclampf _zNear, GLclampf _zFar)
{
  glDepthRangef(_zNear, _zFar);
}

inline void glFogi(GLenum _Name, GLint _Value)
{
  glFogx(_Name, _Value);
}

#endif // __USING_GTEST
#endif // BOOST_OS_...
