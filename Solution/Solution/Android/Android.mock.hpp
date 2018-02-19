
#pragma once
#include <alicorn\android.mock.hpp>

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