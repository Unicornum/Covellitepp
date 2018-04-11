
#pragma once

////////////////////////////////////////////////////////////////////////////////
// 11 Апрель 2018 12:22 (unicornum.verum@gmail.com)
TODO("Вернуть ссылку, когда будет дополнен ANativeActivity");
//#include <alicorn\android.mock.hpp>

class jobject_impl {};
using jobject = jobject_impl *;

class jclass_impl {};
using jclass = jclass_impl *;

class JNIEnv {};
class JavaVM {};

class AAssetManager {};

struct ANativeActivity
{
  const char * externalDataPath = nullptr;
  AAssetManager * assetManager = nullptr;
  JavaVM * vm = nullptr;
};

#include <alicorn\platform\environment.mock.hpp>
////////////////////////////////////////////////////////////////////////////////

struct AInputEvent {};
struct ANativeWindow {};
struct AConfiguration {};

struct android_app
{
  int32_t(*onInputEvent)(android_app *, AInputEvent *);
  void(*onAppCmd)(android_app *, int32_t);
  void * userData = nullptr;
  ANativeActivity * activity = nullptr;
  ANativeWindow * window = nullptr;
  AConfiguration * config = nullptr;

  int destroyRequested = 0;
  size_t StepToDestroyRequest = 0;
};

struct android_poll_source
{
  void(*process)(android_app *, android_poll_source *);
  ::std::vector<int32_t> m_Commands;
  size_t m_Index = 0;
};

#include "Events.hpp"
#include "Event.hpp"
#include "Log.hpp"