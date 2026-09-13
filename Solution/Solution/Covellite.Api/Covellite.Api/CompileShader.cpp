
#include "stdafx.h"
#include "CompileShader.hpp"
#include "Renderer/DirectX11.hpp"

namespace covellite::api
{

/*static*/ void CompileShader::AsHLSL(const ComponentPtr_t & _pComponent)
{
  renderer::DirectX11::CompileShader(_pComponent);
}

/*static*/ void CompileShader::AsGLSL(const ComponentPtr_t & _pComponent)
{
  throw STD_EXCEPTION << "Not implemented yet.";
}

} // namespace covellite::api
