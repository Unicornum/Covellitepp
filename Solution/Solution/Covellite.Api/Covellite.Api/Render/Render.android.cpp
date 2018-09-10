
#include "stdafx.h"
#include "Render.hpp"
#include <alicorn/std/string.hpp>

using namespace covellite::api::render;

/*static*/ auto Render::GetRenders(void) -> ::std::vector<String_t>
{
  return
  {
    uT("Vulkan"),
    uT("OpenGLES")
  };
}
