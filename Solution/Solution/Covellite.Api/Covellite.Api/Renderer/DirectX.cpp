
#include "stdafx.h"
#include "DirectX.hpp"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib" )

using DirectX_t = ::covellite::api::renderer::DirectX;

/*static*/ auto DirectX_t::Shader::Compile(const BinaryData_t & _Data,
  LPCSTR _pEntryPoint, LPCSTR _pTarget) -> ComPtr_t<ID3DBlob>
{
  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  const DWORD ShaderFlags = (IS_RELEASE_CONFIGURATION) ? 0 :
    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

  ::Microsoft::WRL::ComPtr<ID3DBlob> pCompiledEffect;
  ::Microsoft::WRL::ComPtr<ID3DBlob> pError;
  auto Result = D3DCompile(_Data.data(), _Data.size(),
    (::std::string("[Covellite::Api]: ") + _pEntryPoint).c_str(),
    NULL, NULL, _pEntryPoint, _pTarget, ShaderFlags, 0,
    &pCompiledEffect, &pError);
  if (FAILED(Result))
  {
    throw STD_EXCEPTION << "Failed: " << Result <<
      " [" << (char *)pError->GetBufferPointer() << "].";
  }

  return pCompiledEffect;
}
