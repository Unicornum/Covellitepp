
#include "stdafx.h"
#include "Renderer.hpp"
#include <alicorn/std/string.hpp>

using namespace covellite::api::renderer;

/*static*/ auto Renderer::GetRenderers(void) -> ::std::vector<String_t>
{
  return
  {
    uT("DirectX12"),
    uT("DirectX11"),
    uT("Vulkan"),
    uT("OpenGL")
  };
}
