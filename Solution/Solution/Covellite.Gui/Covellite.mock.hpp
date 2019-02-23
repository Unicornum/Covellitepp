
#pragma once

struct android_app
{
  int destroyRequested = 0;
  size_t StepToDestroyRequest = 0;
};

#include <alicorn\android.mock.hpp>
#include <Rocket\Rocket.mock.hpp>
#include "Core.mock.hpp"
#include "App.mock.hpp"
#include "Os.mock.hpp"
#include "Api.mock.hpp"
#include "Gui.mock.hpp"
#include "Rocket.mock.hpp"