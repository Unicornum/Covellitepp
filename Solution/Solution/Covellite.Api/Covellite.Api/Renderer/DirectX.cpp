
#include "stdafx.h"
#include "DirectX.hpp"
#include <alicorn/std/vector.hpp>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib" )

using Shader_t = ::covellite::api::renderer::DirectX::Shader;

/*static*/ auto Shader_t::Compile(const BinaryData_t & _Header,
  const BinaryData_t & _Body, LPCSTR _pEntryPoint, LPCSTR _pTarget) -> ComPtr_t<ID3DBlob>
{
  using namespace ::alicorn::extension::std;
  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  const auto FullShaderBody = _Header + _Body;

  const DWORD ShaderFlags = (IS_RELEASE_CONFIGURATION) ? 0 :
    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

  ::Microsoft::WRL::ComPtr<ID3DBlob> pCompiledEffect;
  ::Microsoft::WRL::ComPtr<ID3DBlob> pError;
  auto Result = D3DCompile(FullShaderBody.data(), FullShaderBody.size(),
    (::std::string("[Covellite::Api]: ") + _pEntryPoint).c_str(),
    NULL, NULL, _pEntryPoint, _pTarget, ShaderFlags, 0,
    &pCompiledEffect, &pError);
  if (FAILED(Result))
  {
    throw STD_EXCEPTION << "Failed: " << Result <<
      " [header line: " << GetHeaderLines(_Header) << ", "
      << (char *)pError->GetBufferPointer() << "].";
  }

  return pCompiledEffect;
}

/*static*/ ::std::string Shader_t::GetVersion(const String_t & _Kind)
{
  return (_Kind == uT("Pixel")) ? "ps_4_0" : "vs_4_0";
}

/*static*/ auto Shader_t::Convert(const ::std::string & _Source) -> BinaryData_t
{
  const auto * pBegin = reinterpret_cast<const uint8_t *>(_Source.data());
  const auto * pEnd = pBegin + _Source.size();

  return BinaryData_t{ pBegin, pEnd };
}

/*static*/ ::std::size_t Shader_t::GetHeaderLines(const BinaryData_t & _Header)
{
  ::std::size_t Result = 0;

  auto itPosition = _Header.cbegin();
  while (true)
  {
    itPosition = ::std::find(itPosition, _Header.cend(), '\n');
    if (itPosition == _Header.cend()) break;

    ++itPosition;
    Result++;
  }

  return Result;
}
