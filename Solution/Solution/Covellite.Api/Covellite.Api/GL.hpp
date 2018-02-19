
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

#endif
#endif
