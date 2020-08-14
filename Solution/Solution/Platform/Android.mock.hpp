
#pragma once
#include <alicorn\android.mock.hpp>

struct AInputEvent {};
struct AConfiguration {};
struct ANativeWindow {};

struct android_app
{
  int32_t(*onInputEvent)(android_app *, AInputEvent *) = nullptr;
  void(*onAppCmd)(android_app *, int32_t) = nullptr;
  void * userData = nullptr;
  ANativeActivity * activity = nullptr;
  ANativeWindow * window = nullptr;
  AConfiguration * config = nullptr;

  int destroyRequested = 0;
  size_t StepToDestroyRequest = 0;
};

struct android_poll_source
{
  void(*process)(android_app *, android_poll_source *) = nullptr;
  ::std::vector<int32_t> m_Commands;
  size_t m_Index = 0;
};

#include "Events.hpp"
#include "Event.hpp"
#include "Log.hpp"
