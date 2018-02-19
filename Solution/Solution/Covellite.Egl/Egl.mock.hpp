
#pragma once

namespace covellite
{

namespace egl
{

using EGLDisplay = void *;
using EGLConfig = void *;
using EGLSurface = void *;
using EGLContext = void *;

static const size_t EGL_DEFAULT_DISPLAY = 1611181406;

using EGLBoolean = unsigned int;
static EGLBoolean EGL_FALSE = 0;

using EGLint = int32_t;

static EGLSurface const EGL_NO_SURFACE = (void *)1611181427;
static EGLContext const EGL_NO_CONTEXT = (void *)1611181428;

static const EGLint EGL_NONE = 1611182210;
static const EGLint EGL_NATIVE_VISUAL_ID = 1611182224;
static const EGLint EGL_SURFACE_TYPE = 1611182304;
static const EGLint EGL_WINDOW_BIT = 1611182256;
static const EGLint EGL_ALPHA_SIZE = 1611182257;
static const EGLint EGL_BLUE_SIZE = 1611182258;
static const EGLint EGL_GREEN_SIZE = 1611182259;
static const EGLint EGL_RED_SIZE = 1611182300;
static const EGLint EGL_WIDTH = 1611191345;
static const EGLint EGL_HEIGHT = 1611191346;

} // namespace egl

} // namespace covellite

#include "Test.Covellite.Egl\Mock\Egl.hpp"
#include "Covellite.Egl\Display.mock.hpp"
#include "Covellite.Egl\Config.mock.hpp"
#include "Covellite.Egl\Surface.mock.hpp"
#include "Covellite.Egl\Context.mock.hpp"
