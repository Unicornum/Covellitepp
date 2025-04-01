
#pragma once
#include <string>
#include <vector>
#include <glm/glm.force.hpp>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "../../Covellite.Api/Renderer/Shaders/Shaders.hpp"

#pragma warning(push)
#pragma warning(disable: 4100)
#pragma warning(disable: 6001)
#pragma warning(disable: 6011)
#pragma warning(disable: 6101)
#pragma warning(disable: 6387)
#pragma warning(disable: 6388)
#pragma warning(disable: 26495)
#pragma warning(disable: 28196)
#pragma warning(disable: 28251)

namespace DirectX
{

template<class T>
inline bool operator== (const T & _Left, const T & _Right)
{
  return (memcmp(&_Left, &_Right, sizeof(T)) == 0);
}

} // namespace DirectX

inline bool operator== (
  const DXGI_SWAP_CHAIN_DESC & _Left,
  const DXGI_SWAP_CHAIN_DESC & _Right)
{
  return DirectX::operator== (_Left, _Right);
}

inline bool operator== (
  const D3D11_RENDER_TARGET_VIEW_DESC & _Left,
  const D3D11_RENDER_TARGET_VIEW_DESC & _Right)
{
  return DirectX::operator== (_Left, _Right);
}

inline bool operator== (
  const D3D11_SAMPLER_DESC & _Left,
  const D3D11_SAMPLER_DESC & _Right)
{
  return DirectX::operator== (_Left, _Right);
}

inline bool operator== (
  const D3D11_BLEND_DESC & _Left,
  const D3D11_BLEND_DESC & _Right)
{
  return DirectX::operator== (_Left, _Right);
}

inline bool operator== (
  const D3D11_RASTERIZER_DESC & _Left,
  const D3D11_RASTERIZER_DESC & _Right)
{
  return DirectX::operator== (_Left, _Right);
}

inline bool operator== (
  const D3D11_SHADER_RESOURCE_VIEW_DESC & _Left,
  const D3D11_SHADER_RESOURCE_VIEW_DESC & _Right)
{
  return DirectX::operator== (_Left, _Right);
}

inline bool operator== (
  const D3D11_TEXTURE2D_DESC & _Left,
  const D3D11_TEXTURE2D_DESC & _Right)
{
  return DirectX::operator== (_Left, _Right);
}

inline bool operator== (
  const D3D11_DEPTH_STENCIL_DESC & _Left,
  const D3D11_DEPTH_STENCIL_DESC & _Right)
{
  return DirectX::operator== (_Left, _Right);
}

inline bool operator== (
  const D3D11_DEPTH_STENCIL_VIEW_DESC & _Left,
  const D3D11_DEPTH_STENCIL_VIEW_DESC & _Right)
{
  return DirectX::operator== (_Left, _Right);
}

inline bool operator== (
  const D3D11_SUBRESOURCE_DATA & _Left,
  const D3D11_SUBRESOURCE_DATA & _Right)
{
  if (_Left.SysMemPitch != _Right.SysMemPitch) return false;
  if (_Left.SysMemSlicePitch != _Right.SysMemSlicePitch) return false;

  if (_Left.pSysMem == nullptr || _Right.pSysMem == nullptr)
  {
    return (_Left.pSysMem == _Right.pSysMem);
  }

  const auto LeftDataSize = *reinterpret_cast<const size_t *>(_Left.pSysMem);
  const auto RightDataSize = *reinterpret_cast<const size_t *>(_Right.pSysMem);
  if (LeftDataSize != RightDataSize) return false;

  return (memcmp(_Left.pSysMem, _Right.pSysMem, LeftDataSize) == 0);
}

inline bool operator== (
  const D3D11_BUFFER_DESC & _Left,
  const D3D11_BUFFER_DESC & _Right)
{
  return DirectX::operator== (_Left, _Right);
}

inline bool operator== (
  const D3D11_INPUT_ELEMENT_DESC & _Left,
  const D3D11_INPUT_ELEMENT_DESC & _Right)
{
  if (::std::string(_Left.SemanticName) != _Right.SemanticName) return false;
  if (_Left.SemanticIndex != _Right.SemanticIndex) return false;
  if (_Left.Format != _Right.Format) return false;
  if (_Left.InputSlot != _Right.InputSlot) return false;
  if (_Left.AlignedByteOffset != _Right.AlignedByteOffset) return false;
  if (_Left.InputSlotClass != _Right.InputSlotClass) return false;
  if (_Left.InstanceDataStepRate != _Right.InstanceDataStepRate) return false;

  return true;
}

inline bool operator== (
  const ::Camera & _Left,
  const ::Camera & _Right)
{
  if (!(_Left.Projection == _Right.Projection)) return false;
  if (!(_Left.View == _Right.View)) return false;
  if (!(_Left.ViewInverse == _Right.ViewInverse)) return false;
  return true;
}

inline bool operator== (
  const ::Object & _Left,
  const ::Object & _Right)
{
  if (!(_Left.World == _Right.World)) return false;
  return true;
}

namespace mock
{

namespace DirectX11
{

class Buffer :
  public ID3D11Buffer
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Outptr_  ID3D11Device **ppDevice) { }

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) {
    return E_FAIL;
  }

public:
  virtual void STDMETHODCALLTYPE GetType(
    /* [annotation] */
    _Out_  D3D11_RESOURCE_DIMENSION *pResourceDimension) { }

  virtual void STDMETHODCALLTYPE SetEvictionPriority(
    /* [annotation] */
    _In_  UINT EvictionPriority) {}

  virtual UINT STDMETHODCALLTYPE GetEvictionPriority(void) { return 0; }

public:
  MOCK_METHOD0(GetDesc, D3D11_BUFFER_DESC(void));

  virtual void STDMETHODCALLTYPE GetDesc(
    /* [annotation] */
    _Out_  D3D11_BUFFER_DESC *pDesc)
  {
    *pDesc = GetDesc();
  }
};

class CameraBuffer : public Buffer { };
class FogBuffer : public Buffer { };
class ObjectBuffer : public Buffer { };
class MatricesBuffer : public Buffer { };
class LightsBuffer : public Buffer { };
class Uint8Buffer : public Buffer { };

class Device :
  public ID3D11Device
{
public:
  MOCK_METHOD1(GetResult, HRESULT(::std::string));

public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }
  MOCK_METHOD0(Release, ULONG(void));

public:
  MOCK_METHOD2(CreateBuffer, ID3D11Buffer *(D3D11_BUFFER_DESC, D3D11_SUBRESOURCE_DATA));
  MOCK_METHOD1(CreateBuffer, ID3D11Buffer *(D3D11_BUFFER_DESC));

  virtual HRESULT STDMETHODCALLTYPE CreateBuffer(
    /* [annotation] */
    _In_  const D3D11_BUFFER_DESC * pDesc,
    /* [annotation] */
    _In_opt_  const D3D11_SUBRESOURCE_DATA * _pInitialData,
    /* [annotation] */
    _Out_opt_  ID3D11Buffer ** ppBuffer) 
  {
    *ppBuffer = (_pInitialData == nullptr) ? 
      CreateBuffer(*pDesc) : CreateBuffer(*pDesc, *_pInitialData);
    return GetResult("CreateBuffer");
  }

  virtual HRESULT STDMETHODCALLTYPE CreateTexture1D(
    /* [annotation] */
    _In_  const D3D11_TEXTURE1D_DESC *pDesc,
    /* [annotation] */
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *pInitialData,
    /* [annotation] */
    _Out_opt_  ID3D11Texture1D **ppTexture1D) {
    return E_FAIL;
  }

  MOCK_METHOD2(CreateTexture2D, ID3D11Texture2D *(D3D11_TEXTURE2D_DESC, D3D11_SUBRESOURCE_DATA));
  MOCK_METHOD2(CreateTexture2DSize, void(UINT, UINT));

  virtual HRESULT STDMETHODCALLTYPE CreateTexture2D(
    /* [annotation] */
    _In_  const D3D11_TEXTURE2D_DESC *pDesc,
    /* [annotation] */
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *pInitialData,
    /* [annotation] */
    _Out_opt_  ID3D11Texture2D **ppTexture2D) 
  {
    static const D3D11_SUBRESOURCE_DATA ZeroData = { 0 };

    *ppTexture2D = CreateTexture2D(*pDesc, 
      (pInitialData == nullptr) ? ZeroData : *pInitialData);
    CreateTexture2DSize(pDesc->Width, pDesc->Height);
    return GetResult("CreateTexture2D");
  }

  virtual HRESULT STDMETHODCALLTYPE CreateTexture3D(
    /* [annotation] */
    _In_  const D3D11_TEXTURE3D_DESC *pDesc,
    /* [annotation] */
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA *pInitialData,
    /* [annotation] */
    _Out_opt_  ID3D11Texture3D **ppTexture3D) {
    return E_FAIL;
  }

  MOCK_METHOD2(CreateShaderResourceView, ID3D11ShaderResourceView *(
    ID3D11Resource *, D3D11_SHADER_RESOURCE_VIEW_DESC));

  virtual HRESULT STDMETHODCALLTYPE CreateShaderResourceView(
    /* [annotation] */
    _In_  ID3D11Resource *pResource,
    /* [annotation] */
    _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc,
    /* [annotation] */
    _Out_opt_  ID3D11ShaderResourceView **ppSRView) 
  {
    *ppSRView = CreateShaderResourceView(pResource, *pDesc);
    return GetResult("CreateShaderResourceView");
  }

  virtual HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView(
    /* [annotation] */
    _In_  ID3D11Resource *pResource,
    /* [annotation] */
    _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc,
    /* [annotation] */
    _Out_opt_  ID3D11UnorderedAccessView **ppUAView) {
    return E_FAIL;
  }

  MOCK_METHOD2(CreateRenderTargetView, 
    ID3D11RenderTargetView *(ID3D11Resource *, D3D11_RENDER_TARGET_VIEW_DESC));

  virtual HRESULT STDMETHODCALLTYPE CreateRenderTargetView(
    ID3D11Resource * pResource,
    const D3D11_RENDER_TARGET_VIEW_DESC * pDesc,
    ID3D11RenderTargetView ** ppRTView) 
  {
    static const D3D11_RENDER_TARGET_VIEW_DESC ZeroData = { 0 };

    *ppRTView = CreateRenderTargetView(pResource,
      (pDesc == nullptr) ? ZeroData : *pDesc);

    return GetResult("CreateRenderTargetView");
  }

  MOCK_METHOD2(CreateDepthStencilView, ID3D11DepthStencilView *(ID3D11Resource *, D3D11_DEPTH_STENCIL_VIEW_DESC));

  virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilView(
    /* [annotation] */
    _In_  ID3D11Resource *pResource,
    /* [annotation] */
    _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc,
    /* [annotation] */
    _Out_opt_  ID3D11DepthStencilView **ppDepthStencilView) 
  {
    *ppDepthStencilView = CreateDepthStencilView(pResource, *pDesc);
    return GetResult("CreateDepthStencilView");
  }

  using InputElements_t = ::std::vector<D3D11_INPUT_ELEMENT_DESC>;

  MOCK_METHOD3(CreateInputLayout, ID3D11InputLayout *(InputElements_t,
    const void *, SIZE_T));

  virtual HRESULT STDMETHODCALLTYPE CreateInputLayout(
    /* [annotation] */
    _In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,
    /* [annotation] */
    _In_range_(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)  UINT NumElements,
    /* [annotation] */
    _In_reads_(BytecodeLength)  const void *pShaderBytecodeWithInputSignature,
    /* [annotation] */
    _In_  SIZE_T BytecodeLength,
    /* [annotation] */
    _Out_opt_  ID3D11InputLayout **ppInputLayout) 
  {
    InputElements_t InputElements{ pInputElementDescs, 
      pInputElementDescs + NumElements };

    *ppInputLayout = CreateInputLayout(InputElements, 
      pShaderBytecodeWithInputSignature, BytecodeLength);

    return GetResult("CreateInputLayout");
  }

  MOCK_METHOD3(CreateVertexShader, ID3D11VertexShader *(const void *, SIZE_T,
    ID3D11ClassLinkage *));

  virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(
    /* [annotation] */
    _In_reads_(BytecodeLength)  const void *pShaderBytecode,
    /* [annotation] */
    _In_  SIZE_T BytecodeLength,
    /* [annotation] */
    _In_opt_  ID3D11ClassLinkage *pClassLinkage,
    /* [annotation] */
    _Out_opt_  ID3D11VertexShader **ppVertexShader) 
  {
    *ppVertexShader = CreateVertexShader(pShaderBytecode, 
      BytecodeLength, pClassLinkage);

    return GetResult("CreateVertexShader");
  }

  virtual HRESULT STDMETHODCALLTYPE CreateGeometryShader(
    /* [annotation] */
    _In_reads_(BytecodeLength)  const void *pShaderBytecode,
    /* [annotation] */
    _In_  SIZE_T BytecodeLength,
    /* [annotation] */
    _In_opt_  ID3D11ClassLinkage *pClassLinkage,
    /* [annotation] */
    _Out_opt_  ID3D11GeometryShader **ppGeometryShader) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(
    /* [annotation] */
    _In_reads_(BytecodeLength)  const void *pShaderBytecode,
    /* [annotation] */
    _In_  SIZE_T BytecodeLength,
    /* [annotation] */
    _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
    /* [annotation] */
    _In_range_(0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT)  UINT NumEntries,
    /* [annotation] */
    _In_reads_opt_(NumStrides)  const UINT *pBufferStrides,
    /* [annotation] */
    _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumStrides,
    /* [annotation] */
    _In_  UINT RasterizedStream,
    /* [annotation] */
    _In_opt_  ID3D11ClassLinkage *pClassLinkage,
    /* [annotation] */
    _Out_opt_  ID3D11GeometryShader **ppGeometryShader) {
    return E_FAIL;
  }

  MOCK_METHOD3(CreatePixelShader, ID3D11PixelShader *(const void *, SIZE_T,
    ID3D11ClassLinkage *));

  virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(
    /* [annotation] */
    _In_reads_(BytecodeLength)  const void *pShaderBytecode,
    /* [annotation] */
    _In_  SIZE_T BytecodeLength,
    /* [annotation] */
    _In_opt_  ID3D11ClassLinkage *pClassLinkage,
    /* [annotation] */
    _Out_opt_  ID3D11PixelShader **ppPixelShader) 
  {
    *ppPixelShader = CreatePixelShader(pShaderBytecode,
      BytecodeLength, pClassLinkage);

    return GetResult("CreatePixelShader");
  }

  virtual HRESULT STDMETHODCALLTYPE CreateHullShader(
    /* [annotation] */
    _In_reads_(BytecodeLength)  const void *pShaderBytecode,
    /* [annotation] */
    _In_  SIZE_T BytecodeLength,
    /* [annotation] */
    _In_opt_  ID3D11ClassLinkage *pClassLinkage,
    /* [annotation] */
    _Out_opt_  ID3D11HullShader **ppHullShader) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE CreateDomainShader(
    /* [annotation] */
    _In_reads_(BytecodeLength)  const void *pShaderBytecode,
    /* [annotation] */
    _In_  SIZE_T BytecodeLength,
    /* [annotation] */
    _In_opt_  ID3D11ClassLinkage *pClassLinkage,
    /* [annotation] */
    _Out_opt_  ID3D11DomainShader **ppDomainShader) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE CreateComputeShader(
    /* [annotation] */
    _In_reads_(BytecodeLength)  const void *pShaderBytecode,
    /* [annotation] */
    _In_  SIZE_T BytecodeLength,
    /* [annotation] */
    _In_opt_  ID3D11ClassLinkage *pClassLinkage,
    /* [annotation] */
    _Out_opt_  ID3D11ComputeShader **ppComputeShader) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE CreateClassLinkage(
    /* [annotation] */
    _Out_  ID3D11ClassLinkage **ppLinkage) {
    return E_FAIL;
  }

  MOCK_METHOD1(CreateBlendState, ID3D11BlendState *(D3D11_BLEND_DESC));

  virtual HRESULT STDMETHODCALLTYPE CreateBlendState(
    /* [annotation] */
    _In_  const D3D11_BLEND_DESC *pBlendStateDesc,
    /* [annotation] */
    _Out_opt_  ID3D11BlendState **ppBlendState) 
  {
    *ppBlendState = CreateBlendState(*pBlendStateDesc);
    return GetResult("CreateBlendState");
  }

  MOCK_METHOD1(CreateDepthStencilState, ID3D11DepthStencilState *(D3D11_DEPTH_STENCIL_DESC));

  virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilState(
    /* [annotation] */
    _In_  const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc,
    /* [annotation] */
    _Out_opt_  ID3D11DepthStencilState **ppDepthStencilState) 
  {
    *ppDepthStencilState = CreateDepthStencilState(*pDepthStencilDesc);
    return GetResult("CreateDepthStencilState");
  }

  MOCK_METHOD1(CreateRasterizerState, ID3D11RasterizerState *(D3D11_RASTERIZER_DESC));

  virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState(
    /* [annotation] */
    _In_  const D3D11_RASTERIZER_DESC * pRasterizerDesc,
    /* [annotation] */
    _Out_opt_  ID3D11RasterizerState ** ppRasterizerState) 
  {
    *ppRasterizerState = CreateRasterizerState(*pRasterizerDesc);
    return GetResult("CreateRasterizerState");
  }

  MOCK_METHOD1(CreateSamplerState, ID3D11SamplerState *(D3D11_SAMPLER_DESC));

  virtual HRESULT STDMETHODCALLTYPE CreateSamplerState(
    /* [annotation] */
    _In_  const D3D11_SAMPLER_DESC *pSamplerDesc,
    /* [annotation] */
    _Out_opt_  ID3D11SamplerState **ppSamplerState) 
  {
    *ppSamplerState = CreateSamplerState(*pSamplerDesc);
    return GetResult("CreateSamplerState");
  }

  virtual HRESULT STDMETHODCALLTYPE CreateQuery(
    /* [annotation] */
    _In_  const D3D11_QUERY_DESC *pQueryDesc,
    /* [annotation] */
    _Out_opt_  ID3D11Query **ppQuery) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE CreatePredicate(
    /* [annotation] */
    _In_  const D3D11_QUERY_DESC *pPredicateDesc,
    /* [annotation] */
    _Out_opt_  ID3D11Predicate **ppPredicate) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE CreateCounter(
    /* [annotation] */
    _In_  const D3D11_COUNTER_DESC *pCounterDesc,
    /* [annotation] */
    _Out_opt_  ID3D11Counter **ppCounter) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE CreateDeferredContext(
    UINT ContextFlags,
    /* [annotation] */
    _Out_opt_  ID3D11DeviceContext **ppDeferredContext) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE OpenSharedResource(
    /* [annotation] */
    _In_  HANDLE hResource,
    /* [annotation] */
    _In_  REFIID ReturnedInterface,
    /* [annotation] */
    _Out_opt_  void **ppResource) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE CheckFormatSupport(
    /* [annotation] */
    _In_  DXGI_FORMAT Format,
    /* [annotation] */
    _Out_  UINT *pFormatSupport) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(
    /* [annotation] */
    _In_  DXGI_FORMAT Format,
    /* [annotation] */
    _In_  UINT SampleCount,
    /* [annotation] */
    _Out_  UINT *pNumQualityLevels) {
    return E_FAIL;
  }

  virtual void STDMETHODCALLTYPE CheckCounterInfo(
    /* [annotation] */
    _Out_  D3D11_COUNTER_INFO *pCounterInfo) { }

  virtual HRESULT STDMETHODCALLTYPE CheckCounter(
    /* [annotation] */
    _In_  const D3D11_COUNTER_DESC *pDesc,
    /* [annotation] */
    _Out_  D3D11_COUNTER_TYPE *pType,
    /* [annotation] */
    _Out_  UINT *pActiveCounters,
    /* [annotation] */
    _Out_writes_opt_(*pNameLength)  LPSTR szName,
    /* [annotation] */
    _Inout_opt_  UINT *pNameLength,
    /* [annotation] */
    _Out_writes_opt_(*pUnitsLength)  LPSTR szUnits,
    /* [annotation] */
    _Inout_opt_  UINT *pUnitsLength,
    /* [annotation] */
    _Out_writes_opt_(*pDescriptionLength)  LPSTR szDescription,
    /* [annotation] */
    _Inout_opt_  UINT *pDescriptionLength) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport(
    D3D11_FEATURE Feature,
    /* [annotation] */
    _Out_writes_bytes_(FeatureSupportDataSize)  void *pFeatureSupportData,
    UINT FeatureSupportDataSize) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) {
    return E_FAIL;
  }

  virtual D3D_FEATURE_LEVEL STDMETHODCALLTYPE GetFeatureLevel(void) { throw ::std::exception{}; }

  virtual UINT STDMETHODCALLTYPE GetCreationFlags(void) { return 0; }

  virtual HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason(void) { return E_FAIL; }

  virtual void STDMETHODCALLTYPE GetImmediateContext(
    /* [annotation] */
    _Out_  ID3D11DeviceContext **ppImmediateContext) { }

  virtual HRESULT STDMETHODCALLTYPE SetExceptionMode(
    UINT RaiseFlags) {
    return E_FAIL;
  }

  virtual UINT STDMETHODCALLTYPE GetExceptionMode(void) { return 0; }
};

class DeviceContext :
  public ID3D11DeviceContext
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }
  MOCK_METHOD0(Release, ULONG(void));

public:
  MOCK_METHOD0(GetDevice, ID3D11Device *(void));

  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Out_  ID3D11Device **ppDevice) 
  {
    *ppDevice = GetDevice();
  }

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) {
    return E_FAIL;
  }

public:
  MOCK_METHOD3(VSSetConstantBuffers, void(UINT, UINT, ID3D11Buffer *));

  virtual void STDMETHODCALLTYPE VSSetConstantBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers) 
  {
    VSSetConstantBuffers(StartSlot, NumBuffers, *ppConstantBuffers);
  }

  MOCK_METHOD2(PSSetShaderResources, void(UINT, ::std::vector<ID3D11ShaderResourceView *>));

  virtual void STDMETHODCALLTYPE PSSetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews) 
  {
    ::std::vector<ID3D11ShaderResourceView *> ShaderResourceViews{
      ppShaderResourceViews, ppShaderResourceViews + NumViews };
    PSSetShaderResources(StartSlot, ShaderResourceViews);
  }

  MOCK_METHOD3(PSSetShader, void(ID3D11PixelShader *, 
    ID3D11ClassInstance *const *, UINT));
  MOCK_METHOD3(PSSetSamplers, void(UINT, UINT, ID3D11SamplerState *));

  virtual void STDMETHODCALLTYPE PSSetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers) 
  {
    PSSetSamplers(StartSlot, NumSamplers, *ppSamplers);
  }

  MOCK_METHOD3(VSSetShader, void(ID3D11VertexShader *, 
    ID3D11ClassInstance *const *, UINT));

  MOCK_METHOD3(DrawIndexed, void (
    /* [annotation] */
    _In_  UINT /*IndexCount*/,
    /* [annotation] */
    _In_  UINT /*StartIndexLocation*/,
    /* [annotation] */
    _In_  INT /*BaseVertexLocation*/));

  virtual void STDMETHODCALLTYPE Draw(
    /* [annotation] */
    _In_  UINT VertexCount,
    /* [annotation] */
    _In_  UINT StartVertexLocation) {}

  MOCK_METHOD2(Mapped, D3D11_MAPPED_SUBRESOURCE (ID3D11Resource *, D3D11_MAP));
  MOCK_METHOD2(Map, HRESULT(UINT, UINT));

  virtual HRESULT STDMETHODCALLTYPE Map(
    /* [annotation] */
    _In_  ID3D11Resource *pResource,
    /* [annotation] */
    _In_  UINT Subresource,
    /* [annotation] */
    _In_  D3D11_MAP MapType,
    /* [annotation] */
    _In_  UINT MapFlags,
    /* [annotation] */
    _Out_  D3D11_MAPPED_SUBRESOURCE *pMappedResource) 
  {
    *pMappedResource = Mapped(pResource, MapType);
    return Map(Subresource, MapFlags);
  }

  MOCK_METHOD2(Unmap, void (
    /* [annotation] */
    _In_  ID3D11Resource *,
    /* [annotation] */
    _In_  UINT));

  MOCK_METHOD3(PSSetConstantBuffers, void(UINT, UINT, ID3D11Buffer *));

  virtual void STDMETHODCALLTYPE PSSetConstantBuffers(
    /* [annotation] */
    UINT StartSlot,
    /* [annotation] */
    UINT NumBuffers,
    /* [annotation] */
    ID3D11Buffer *const *ppConstantBuffers) 
  {
    PSSetConstantBuffers(StartSlot, NumBuffers, *ppConstantBuffers);
  }

  MOCK_METHOD1(IASetInputLayout, void(ID3D11InputLayout *));
  MOCK_METHOD5(IASetVertexBuffers, void(UINT, UINT, ID3D11Buffer *, UINT, UINT));

  virtual void STDMETHODCALLTYPE IASetVertexBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppVertexBuffers,
    /* [annotation] */
    _In_reads_opt_(NumBuffers)  const UINT *pStrides,
    /* [annotation] */
    _In_reads_opt_(NumBuffers)  const UINT *pOffsets) 
  {
    IASetVertexBuffers(StartSlot, NumBuffers, *ppVertexBuffers, *pStrides, *pOffsets);
  }

  MOCK_METHOD3(IASetIndexBuffer,  void (
    /* [annotation] */
    _In_opt_  ID3D11Buffer */*pIndexBuffer*/,
    /* [annotation] */
    _In_  DXGI_FORMAT /*Format*/,
    /* [annotation] */
    _In_  UINT /*Offset*/));

  MOCK_METHOD5(DrawIndexedInstanced, void(UINT, UINT, UINT, INT, UINT));

  //virtual void STDMETHODCALLTYPE DrawIndexedInstanced(
  //  /* [annotation] */
  //  _In_  UINT IndexCountPerInstance,
  //  /* [annotation] */
  //  _In_  UINT InstanceCount,
  //  /* [annotation] */
  //  _In_  UINT StartIndexLocation,
  //  /* [annotation] */
  //  _In_  INT BaseVertexLocation,
  //  /* [annotation] */
  //  _In_  UINT StartInstanceLocation) {}

  virtual void STDMETHODCALLTYPE DrawInstanced(
    /* [annotation] */
    _In_  UINT VertexCountPerInstance,
    /* [annotation] */
    _In_  UINT InstanceCount,
    /* [annotation] */
    _In_  UINT StartVertexLocation,
    /* [annotation] */
    _In_  UINT StartInstanceLocation) {}

  virtual void STDMETHODCALLTYPE GSSetConstantBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers) {}

  virtual void STDMETHODCALLTYPE GSSetShader(
    /* [annotation] */
    _In_opt_  ID3D11GeometryShader *pShader,
    /* [annotation] */
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
    UINT NumClassInstances) {}

  MOCK_METHOD1(IASetPrimitiveTopology, void(D3D11_PRIMITIVE_TOPOLOGY));

  virtual void STDMETHODCALLTYPE VSSetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews) {}

  virtual void STDMETHODCALLTYPE VSSetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers) {}

  virtual void STDMETHODCALLTYPE Begin(
    /* [annotation] */
    _In_  ID3D11Asynchronous *pAsync) {}

  virtual void STDMETHODCALLTYPE End(
    /* [annotation] */
    _In_  ID3D11Asynchronous *pAsync) {}

  virtual HRESULT STDMETHODCALLTYPE GetData(
    /* [annotation] */
    _In_  ID3D11Asynchronous *pAsync,
    /* [annotation] */
    _Out_writes_bytes_opt_(DataSize)  void *pData,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_  UINT GetDataFlags) {
    return E_FAIL;
  }

  virtual void STDMETHODCALLTYPE SetPredication(
    /* [annotation] */
    _In_opt_  ID3D11Predicate *pPredicate,
    /* [annotation] */
    _In_  BOOL PredicateValue) {}

  virtual void STDMETHODCALLTYPE GSSetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews) {}

  virtual void STDMETHODCALLTYPE GSSetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers) {}

  MOCK_METHOD2(OMSetRenderTargets, void(::std::vector<ID3D11RenderTargetView *>,
    ID3D11DepthStencilView *));

  virtual void STDMETHODCALLTYPE OMSetRenderTargets(
    /* [annotation] */
    _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
    /* [annotation] */
    _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const *ppRenderTargetViews,
    /* [annotation] */
    _In_opt_  ID3D11DepthStencilView *pDepthStencilView) 
  {
    ::std::vector<ID3D11RenderTargetView *> RenderTargetViews(
      ppRenderTargetViews, ppRenderTargetViews + NumViews);
    OMSetRenderTargets(RenderTargetViews, pDepthStencilView);
  }

  virtual void STDMETHODCALLTYPE OMSetRenderTargetsAndUnorderedAccessViews(
    /* [annotation] */
    _In_  UINT NumRTVs,
    /* [annotation] */
    _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const *ppRenderTargetViews,
    /* [annotation] */
    _In_opt_  ID3D11DepthStencilView *pDepthStencilView,
    /* [annotation] */
    _In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1)  UINT UAVStartSlot,
    /* [annotation] */
    _In_  UINT NumUAVs,
    /* [annotation] */
    _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
    /* [annotation] */
    _In_reads_opt_(NumUAVs)  const UINT *pUAVInitialCounts) {}

  MOCK_METHOD3(OMSetBlendState, void (ID3D11BlendState *,
    ::std::vector<FLOAT>, UINT));

  virtual void STDMETHODCALLTYPE OMSetBlendState(
    /* [annotation] */
    _In_opt_  ID3D11BlendState *pBlendState,
    /* [annotation] */
    _In_opt_  const FLOAT BlendFactor[4],
    /* [annotation] */
    _In_  UINT SampleMask) 
  {
    ::std::vector<FLOAT> Factor{ BlendFactor, BlendFactor + 4 };

    OMSetBlendState(pBlendState, Factor, SampleMask);
  }

  MOCK_METHOD2(OMSetDepthStencilState, void(ID3D11DepthStencilState *, UINT));

  virtual void STDMETHODCALLTYPE SOSetTargets(
    /* [annotation] */
    _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumBuffers,
    /* [annotation] */
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppSOTargets,
    /* [annotation] */
    _In_reads_opt_(NumBuffers)  const UINT *pOffsets) {}

  virtual void STDMETHODCALLTYPE DrawAuto(void) { }

  virtual void STDMETHODCALLTYPE DrawIndexedInstancedIndirect(
    /* [annotation] */
    _In_  ID3D11Buffer *pBufferForArgs,
    /* [annotation] */
    _In_  UINT AlignedByteOffsetForArgs) {}

  virtual void STDMETHODCALLTYPE DrawInstancedIndirect(
    /* [annotation] */
    _In_  ID3D11Buffer *pBufferForArgs,
    /* [annotation] */
    _In_  UINT AlignedByteOffsetForArgs) {}

  virtual void STDMETHODCALLTYPE Dispatch(
    /* [annotation] */
    _In_  UINT ThreadGroupCountX,
    /* [annotation] */
    _In_  UINT ThreadGroupCountY,
    /* [annotation] */
    _In_  UINT ThreadGroupCountZ) {}

  virtual void STDMETHODCALLTYPE DispatchIndirect(
    /* [annotation] */
    _In_  ID3D11Buffer *pBufferForArgs,
    /* [annotation] */
    _In_  UINT AlignedByteOffsetForArgs) {}

  MOCK_METHOD1(RSSetState, void (ID3D11RasterizerState *));
  MOCK_METHOD1(RSSetViewports, void(::std::vector<D3D11_VIEWPORT>));

  virtual void STDMETHODCALLTYPE RSSetViewports(
    /* [annotation] */
    _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
    /* [annotation] */
    _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT *pViewports) 
  {
    ::std::vector<D3D11_VIEWPORT> Viewports;

    for (UINT i = 0; i < NumViewports; i++)
    {
      Viewports.push_back(pViewports[i]);
    }

    RSSetViewports(Viewports);
  }

  MOCK_METHOD1(RSSetScissorRects, void(::std::vector<D3D11_RECT>));

  virtual void STDMETHODCALLTYPE RSSetScissorRects(
    /* [annotation] */
    _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects,
    /* [annotation] */
    _In_reads_opt_(NumRects)  const D3D11_RECT *pRects) 
  {
    ::std::vector<D3D11_RECT> Viewports;

    for (UINT i = 0; i < NumRects; i++)
    {
      Viewports.push_back(pRects[i]);
    }

    RSSetScissorRects(Viewports);
  }

  virtual void STDMETHODCALLTYPE CopySubresourceRegion(
    /* [annotation] */
    _In_  ID3D11Resource *pDstResource,
    /* [annotation] */
    _In_  UINT DstSubresource,
    /* [annotation] */
    _In_  UINT DstX,
    /* [annotation] */
    _In_  UINT DstY,
    /* [annotation] */
    _In_  UINT DstZ,
    /* [annotation] */
    _In_  ID3D11Resource *pSrcResource,
    /* [annotation] */
    _In_  UINT SrcSubresource,
    /* [annotation] */
    _In_opt_  const D3D11_BOX *pSrcBox) {}

  MOCK_METHOD2(CopyResource, void (ID3D11Resource *, ID3D11Resource *));

  MOCK_METHOD6(UpdateSubresource, void(ID3D11Resource *, UINT,
    const D3D11_BOX *, ::Camera, UINT, UINT));
  MOCK_METHOD6(UpdateSubresource, void(ID3D11Resource *, UINT,
    const D3D11_BOX *, ::Object, UINT, UINT));
  MOCK_METHOD6(UpdateSubresource, void(ID3D11Resource *, UINT,
    const D3D11_BOX *, ::std::vector<uint8_t>, UINT, UINT));
  MOCK_METHOD7(UpdateSubresource, void(ID3D11Resource *, UINT,
    const D3D11_BOX *, const void *, UINT, UINT, int));

  virtual void UpdateSubresource(
    /* [annotation] */
    _In_  ID3D11Resource * pDstResource,
    /* [annotation] */
    _In_  UINT DstSubresource,
    /* [annotation] */
    _In_opt_  const D3D11_BOX * pDstBox,
    /* [annotation] */
    _In_  const void * pSrcData,
    /* [annotation] */
    _In_  UINT SrcRowPitch,
    /* [annotation] */
    _In_  UINT SrcDepthPitch)
  {
    if (dynamic_cast<ID3D11Texture2D *>(pDstResource) != nullptr)
    {
      ::std::vector<uint8_t> Uint8Data;

      if (pSrcData != nullptr)
      {
        auto * pData = reinterpret_cast<const uint8_t *>(pSrcData);
        while (true)
        {
          Uint8Data.push_back(*pData);
          if (*pData == 0x00) break;

          pData++;
        }
      }

      UpdateSubresource(pDstResource, DstSubresource, pDstBox,
        Uint8Data, SrcRowPitch, SrcDepthPitch);
    }
    else if (dynamic_cast<CameraBuffer *>(pDstResource) != nullptr)
    {
      UpdateSubresource(pDstResource, DstSubresource, pDstBox,
        *reinterpret_cast<const ::Camera *>(pSrcData),
        SrcRowPitch, SrcDepthPitch);
    }
    else if (dynamic_cast<ObjectBuffer *>(pDstResource) != nullptr)
    {
      UpdateSubresource(pDstResource, DstSubresource, pDstBox,
        *reinterpret_cast<const ::Object *>(pSrcData),
        SrcRowPitch, SrcDepthPitch);
    }
    else if (dynamic_cast<Uint8Buffer *>(pDstResource) != nullptr)
    {
      ::std::vector<uint8_t> Uint8Data;

      auto * pData = reinterpret_cast<const uint8_t *>(pSrcData);
      while (*pData != 0x00)
      {
        Uint8Data.push_back(*pData);
        pData++;
      }

      UpdateSubresource(pDstResource, DstSubresource, pDstBox,
        Uint8Data, SrcRowPitch, SrcDepthPitch);
    }
    else
    {
      UpdateSubresource(pDstResource, DstSubresource, pDstBox,
        *reinterpret_cast<const ::Object *>(pSrcData),
        SrcRowPitch, SrcDepthPitch);
    }
  }

  virtual void STDMETHODCALLTYPE CopyStructureCount(
    /* [annotation] */
    _In_  ID3D11Buffer *pDstBuffer,
    /* [annotation] */
    _In_  UINT DstAlignedByteOffset,
    /* [annotation] */
    _In_  ID3D11UnorderedAccessView *pSrcView) {}

  using Color_t = ::std::vector<FLOAT>;

  MOCK_METHOD2(ClearRenderTargetView, void(ID3D11RenderTargetView *, Color_t));

  virtual void STDMETHODCALLTYPE ClearRenderTargetView(
    /* [annotation] */
    _In_  ID3D11RenderTargetView *pRenderTargetView,
    /* [annotation] */
    _In_  const FLOAT ColorRGBA[4]) 
  {
    ClearRenderTargetView(pRenderTargetView, 
      Color_t{ ColorRGBA[0], ColorRGBA[1], ColorRGBA[2], ColorRGBA[3] });
  }

  virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewUint(
    /* [annotation] */
    _In_  ID3D11UnorderedAccessView *pUnorderedAccessView,
    /* [annotation] */
    _In_  const UINT Values[4]) {}

  virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat(
    /* [annotation] */
    _In_  ID3D11UnorderedAccessView *pUnorderedAccessView,
    /* [annotation] */
    _In_  const FLOAT Values[4]) {}

  MOCK_METHOD4(ClearDepthStencilView, void(
    /* [annotation] */
    _In_  ID3D11DepthStencilView *,
    /* [annotation] */
    _In_  UINT,
    /* [annotation] */
    _In_  FLOAT,
    /* [annotation] */
    _In_  UINT8));

  MOCK_METHOD1(GenerateMips, void (ID3D11ShaderResourceView *));

  virtual void STDMETHODCALLTYPE SetResourceMinLOD(
    /* [annotation] */
    _In_  ID3D11Resource *pResource,
    FLOAT MinLOD) {}

  virtual FLOAT STDMETHODCALLTYPE GetResourceMinLOD(
    /* [annotation] */
    _In_  ID3D11Resource *pResource) {
    return 0.0f;
  }

  virtual void STDMETHODCALLTYPE ResolveSubresource(
    /* [annotation] */
    _In_  ID3D11Resource *pDstResource,
    /* [annotation] */
    _In_  UINT DstSubresource,
    /* [annotation] */
    _In_  ID3D11Resource *pSrcResource,
    /* [annotation] */
    _In_  UINT SrcSubresource,
    /* [annotation] */
    _In_  DXGI_FORMAT Format) {}

  virtual void STDMETHODCALLTYPE ExecuteCommandList(
    /* [annotation] */
    _In_  ID3D11CommandList *pCommandList,
    BOOL RestoreContextState) {

  }

  virtual void STDMETHODCALLTYPE HSSetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews) {

  }

  virtual void STDMETHODCALLTYPE HSSetShader(
    /* [annotation] */
    _In_opt_  ID3D11HullShader *pHullShader,
    /* [annotation] */
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
    UINT NumClassInstances) {

  }

  virtual void STDMETHODCALLTYPE HSSetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers) {

  }

  virtual void STDMETHODCALLTYPE HSSetConstantBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers) {

  }

  virtual void STDMETHODCALLTYPE DSSetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews) {

  }

  virtual void STDMETHODCALLTYPE DSSetShader(
    /* [annotation] */
    _In_opt_  ID3D11DomainShader *pDomainShader,
    /* [annotation] */
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
    UINT NumClassInstances) {

  }

  virtual void STDMETHODCALLTYPE DSSetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers) {

  }

  virtual void STDMETHODCALLTYPE DSSetConstantBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers) {

  }

  virtual void STDMETHODCALLTYPE CSSetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews) {

  }

  virtual void STDMETHODCALLTYPE CSSetUnorderedAccessViews(
    /* [annotation] */
    _In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_1_UAV_SLOT_COUNT - StartSlot)  UINT NumUAVs,
    /* [annotation] */
    _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
    /* [annotation] */
    _In_reads_opt_(NumUAVs)  const UINT *pUAVInitialCounts) {

  }

  virtual void STDMETHODCALLTYPE CSSetShader(
    /* [annotation] */
    _In_opt_  ID3D11ComputeShader *pComputeShader,
    /* [annotation] */
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
    UINT NumClassInstances) {

  }

  virtual void STDMETHODCALLTYPE CSSetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers) {

  }

  virtual void STDMETHODCALLTYPE CSSetConstantBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers) {

  }

  virtual void STDMETHODCALLTYPE VSGetConstantBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers) {

  }

  virtual void STDMETHODCALLTYPE PSGetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews) {

  }

  virtual void STDMETHODCALLTYPE PSGetShader(
    /* [annotation] */
    _Out_  ID3D11PixelShader **ppPixelShader,
    /* [annotation] */
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
    /* [annotation] */
    _Inout_opt_  UINT *pNumClassInstances) {

  }

  virtual void STDMETHODCALLTYPE PSGetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers) {

  }

  virtual void STDMETHODCALLTYPE VSGetShader(
    /* [annotation] */
    _Out_  ID3D11VertexShader **ppVertexShader,
    /* [annotation] */
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
    /* [annotation] */
    _Inout_opt_  UINT *pNumClassInstances) {

  }

  virtual void STDMETHODCALLTYPE PSGetConstantBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers) {

  }

  virtual void STDMETHODCALLTYPE IAGetInputLayout(
    /* [annotation] */
    _Out_  ID3D11InputLayout **ppInputLayout) {

  }

  virtual void STDMETHODCALLTYPE IAGetVertexBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppVertexBuffers,
    /* [annotation] */
    _Out_writes_opt_(NumBuffers)  UINT *pStrides,
    /* [annotation] */
    _Out_writes_opt_(NumBuffers)  UINT *pOffsets) {

  }

  MOCK_METHOD2(IAGetIndexBuffer, ID3D11Buffer *(DXGI_FORMAT, UINT));

  virtual void STDMETHODCALLTYPE IAGetIndexBuffer(
    /* [annotation] */
    _Out_opt_  ID3D11Buffer **pIndexBuffer,
    /* [annotation] */
    _Out_opt_  DXGI_FORMAT *Format,
    /* [annotation] */
    _Out_opt_  UINT *Offset) 
  {
    *pIndexBuffer = IAGetIndexBuffer(*Format, *Offset);
  }

  virtual void STDMETHODCALLTYPE GSGetConstantBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers) {

  }

  virtual void STDMETHODCALLTYPE GSGetShader(
    /* [annotation] */
    _Out_  ID3D11GeometryShader **ppGeometryShader,
    /* [annotation] */
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
    /* [annotation] */
    _Inout_opt_  UINT *pNumClassInstances) {

  }

  virtual void STDMETHODCALLTYPE IAGetPrimitiveTopology(
    /* [annotation] */
    _Out_  D3D11_PRIMITIVE_TOPOLOGY *pTopology) {

  }

  virtual void STDMETHODCALLTYPE VSGetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews) {

  }

  virtual void STDMETHODCALLTYPE VSGetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers) {

  }

  virtual void STDMETHODCALLTYPE GetPredication(
    /* [annotation] */
    _Out_opt_  ID3D11Predicate **ppPredicate,
    /* [annotation] */
    _Out_opt_  BOOL *pPredicateValue) {

  }

  virtual void STDMETHODCALLTYPE GSGetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews) {

  }

  virtual void STDMETHODCALLTYPE GSGetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers) {

  }

  virtual void STDMETHODCALLTYPE OMGetRenderTargets(
    /* [annotation] */
    _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
    /* [annotation] */
    _Out_writes_opt_(NumViews)  ID3D11RenderTargetView **ppRenderTargetViews,
    /* [annotation] */
    _Out_opt_  ID3D11DepthStencilView **ppDepthStencilView) {

  }

  virtual void STDMETHODCALLTYPE OMGetRenderTargetsAndUnorderedAccessViews(
    /* [annotation] */
    _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumRTVs,
    /* [annotation] */
    _Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView **ppRenderTargetViews,
    /* [annotation] */
    _Out_opt_  ID3D11DepthStencilView **ppDepthStencilView,
    /* [annotation] */
    _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1)  UINT UAVStartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot)  UINT NumUAVs,
    /* [annotation] */
    _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **ppUnorderedAccessViews) {

  }

  virtual void STDMETHODCALLTYPE OMGetBlendState(
    /* [annotation] */
    _Out_opt_  ID3D11BlendState **ppBlendState,
    /* [annotation] */
    _Out_opt_  FLOAT BlendFactor[4],
    /* [annotation] */
    _Out_opt_  UINT *pSampleMask) {

  }

  virtual void STDMETHODCALLTYPE OMGetDepthStencilState(
    /* [annotation] */
    _Out_opt_  ID3D11DepthStencilState **ppDepthStencilState,
    /* [annotation] */
    _Out_opt_  UINT *pStencilRef) {

  }

  virtual void STDMETHODCALLTYPE SOGetTargets(
    /* [annotation] */
    _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumBuffers,
    /* [annotation] */
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppSOTargets) {

  }

  virtual void STDMETHODCALLTYPE RSGetState(
    /* [annotation] */
    _Out_  ID3D11RasterizerState **ppRasterizerState) {

  }

  MOCK_METHOD1(RSGetViewports, D3D11_VIEWPORT(UINT));

  virtual void STDMETHODCALLTYPE RSGetViewports(
    /* [annotation] */
    _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *pNumViewports,
    /* [annotation] */
    _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT *pViewports) 
  {
    *pViewports = RSGetViewports(*pNumViewports);
  }

  virtual void STDMETHODCALLTYPE RSGetScissorRects(
    /* [annotation] */
    _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *pNumRects,
    /* [annotation] */
    _Out_writes_opt_(*pNumRects)  D3D11_RECT *pRects) {

  }

  virtual void STDMETHODCALLTYPE HSGetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews) {

  }

  virtual void STDMETHODCALLTYPE HSGetShader(
    /* [annotation] */
    _Out_  ID3D11HullShader **ppHullShader,
    /* [annotation] */
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
    /* [annotation] */
    _Inout_opt_  UINT *pNumClassInstances) {

  }

  virtual void STDMETHODCALLTYPE HSGetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers) {

  }

  virtual void STDMETHODCALLTYPE HSGetConstantBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers) {

  }

  virtual void STDMETHODCALLTYPE DSGetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews) {

  }

  virtual void STDMETHODCALLTYPE DSGetShader(
    /* [annotation] */
    _Out_  ID3D11DomainShader **ppDomainShader,
    /* [annotation] */
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
    /* [annotation] */
    _Inout_opt_  UINT *pNumClassInstances) {

  }

  virtual void STDMETHODCALLTYPE DSGetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers) {

  }

  virtual void STDMETHODCALLTYPE DSGetConstantBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers) {

  }

  virtual void STDMETHODCALLTYPE CSGetShaderResources(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
    /* [annotation] */
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews) {

  }

  virtual void STDMETHODCALLTYPE CSGetUnorderedAccessViews(
    /* [annotation] */
    _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot)  UINT NumUAVs,
    /* [annotation] */
    _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **ppUnorderedAccessViews) {

  }

  virtual void STDMETHODCALLTYPE CSGetShader(
    /* [annotation] */
    _Out_  ID3D11ComputeShader **ppComputeShader,
    /* [annotation] */
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
    /* [annotation] */
    _Inout_opt_  UINT *pNumClassInstances) {

  }

  virtual void STDMETHODCALLTYPE CSGetSamplers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
    /* [annotation] */
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers) {

  }

  virtual void STDMETHODCALLTYPE CSGetConstantBuffers(
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
    /* [annotation] */
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
    /* [annotation] */
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers) {

  }

  virtual void STDMETHODCALLTYPE ClearState(void) {  }

  virtual void STDMETHODCALLTYPE Flush(void) {  }

  virtual D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE GetType(void) { 
    throw ::std::exception{};
  }

  virtual UINT STDMETHODCALLTYPE GetContextFlags(void) { return 0; }

  virtual HRESULT STDMETHODCALLTYPE FinishCommandList(
    BOOL RestoreDeferredContextState,
    /* [annotation] */
    _Out_opt_  ID3D11CommandList **ppCommandList) {
    return E_FAIL;
  }
};

class Texture2D :
  public ID3D11Texture2D
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Outptr_  ID3D11Device **ppDevice) {}

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) { return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) { return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) { return E_FAIL; }

public:
  virtual void STDMETHODCALLTYPE GetType(
    /* [annotation] */
    _Out_  D3D11_RESOURCE_DIMENSION *pResourceDimension) {}

  virtual void STDMETHODCALLTYPE SetEvictionPriority(
    /* [annotation] */
    _In_  UINT EvictionPriority) {}

  virtual UINT STDMETHODCALLTYPE GetEvictionPriority(void) { return 0; }

public:
  virtual void STDMETHODCALLTYPE GetDesc(
    /* [annotation] */
    _Out_  D3D11_TEXTURE2D_DESC *pDesc) {}
};

class RenderTargetView :
  public ID3D11RenderTargetView
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
      /* [in] */ REFIID riid,
      /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
      return E_FAIL;
    }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Out_  ID3D11Device **ppDevice) {}

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) {
    return E_FAIL;
  }

public:
  virtual void STDMETHODCALLTYPE GetResource(
    /* [annotation] */
    _Out_  ID3D11Resource **ppResource) {}

public:
  virtual void STDMETHODCALLTYPE GetDesc(
    /* [annotation] */
    _Out_  D3D11_RENDER_TARGET_VIEW_DESC *pDesc) {}

};

class RasterizerState :
  public ID3D11RasterizerState
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) { return E_FAIL; }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Out_  ID3D11Device **ppDevice) { }

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) { return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) { return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) { return E_FAIL; }

public:
    virtual void STDMETHODCALLTYPE GetDesc(
      /* [annotation] */
      _Out_  D3D11_RASTERIZER_DESC *pDesc) { }
};

class ShaderResourceView :
  public ID3D11ShaderResourceView
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Outptr_  ID3D11Device **ppDevice) {}

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) { return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) { return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) { return E_FAIL; }

public:
  virtual void STDMETHODCALLTYPE GetResource(
    /* [annotation] */
    _Outptr_  ID3D11Resource **ppResource) {}

public:
  virtual void STDMETHODCALLTYPE GetDesc(
    /* [annotation] */
    _Out_  D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc) {}
};

class Blob :
  public ID3DBlob
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) { return E_FAIL; }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));

public:
  MOCK_METHOD0(GetBufferPointer, LPVOID(void));
  MOCK_METHOD0(GetBufferSize, SIZE_T(void));
};

class InputLayout :
  public ID3D11InputLayout
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Outptr_  ID3D11Device **ppDevice) {}

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) {
    return E_FAIL;
  }
};

class VertexShader :
  public ID3D11VertexShader
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Outptr_  ID3D11Device **ppDevice) {}

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) {
    return E_FAIL;
  }
};

class PixelShader :
  public ID3D11PixelShader
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Outptr_  ID3D11Device **ppDevice) {}

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) {
    return E_FAIL;
  }
};

class SamplerState :
  public ID3D11SamplerState
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Outptr_  ID3D11Device **ppDevice) {}

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) {
    return E_FAIL;
  }

public:
  virtual void STDMETHODCALLTYPE GetDesc(
    /* [annotation] */
    _Out_  D3D11_SAMPLER_DESC *pDesc) {}
};

class BlendState :
  public ID3D11BlendState
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Outptr_  ID3D11Device **ppDevice) {}

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) {
    return E_FAIL;
  }

public:
  virtual void STDMETHODCALLTYPE GetDesc(
    /* [annotation] */
    _Out_  D3D11_BLEND_DESC *pDesc) {}
};

class DepthStencilState :
  public ID3D11DepthStencilState
{
public:
    virtual void STDMETHODCALLTYPE GetDesc(
      /* [annotation] */
      _Out_  D3D11_DEPTH_STENCIL_DESC *pDesc) {}

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Outptr_  ID3D11Device **ppDevice) {}

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) { return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) { return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) { return E_FAIL; }

public:
  BEGIN_INTERFACE
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
      /* [in] */ REFIID riid,
      /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
   return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));
};

class DepthStencilView :
  public ID3D11DepthStencilView
{
public:
  virtual void STDMETHODCALLTYPE GetDesc(
    /* [annotation] */
    _Out_  D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc) {}

public:
  virtual void STDMETHODCALLTYPE GetResource(
    /* [annotation] */
    _Outptr_  ID3D11Resource **ppResource) {}

public:
  virtual void STDMETHODCALLTYPE GetDevice(
    /* [annotation] */
    _Outptr_  ID3D11Device **ppDevice) {}

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _Inout_  UINT *pDataSize,
    /* [annotation] */
    _Out_writes_bytes_opt_(*pDataSize)  void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_  UINT DataSize,
    /* [annotation] */
    _In_reads_bytes_opt_(DataSize)  const void *pData) {
    return E_FAIL;
  }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation] */
    _In_  REFGUID guid,
    /* [annotation] */
    _In_opt_  const IUnknown *pData) {
    return E_FAIL;
  }

public:
  BEGIN_INTERFACE
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
      /* [in] */ REFIID riid,
      /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }

  MOCK_METHOD0(Release, ULONG(void));
};

class DeviceDesc
{
public:
  IDXGIAdapter * pAdapter;
  D3D_DRIVER_TYPE DriverType;
  HMODULE Software;
  UINT Flags;
  ::std::vector<D3D_FEATURE_LEVEL> FeatureLevels;
  UINT SDKVersion;
  DXGI_SWAP_CHAIN_DESC SwapChainDesc;
  D3D_FEATURE_LEVEL * pFeatureLevel;

public:
  bool operator== (const DeviceDesc & _Right) const
  {
    if (pAdapter != _Right.pAdapter) return false;
    if (DriverType != _Right.DriverType) return false;
    if (Software != _Right.Software) return false;
    if (Flags != _Right.Flags) return false;
    if (FeatureLevels != _Right.FeatureLevels) return false;
    if (SDKVersion != _Right.SDKVersion) return false;
    if (!(SwapChainDesc == _Right.SwapChainDesc)) return false;
    if (pFeatureLevel != _Right.pFeatureLevel) return false;

    return true;
  }
};

class CompileDesc
{
public:
  ::std::vector<uint8_t> SrcData;
  ::std::string SourceName;
  CONST D3D_SHADER_MACRO * pDefines = nullptr;
  ID3DInclude * pInclude = nullptr;
  ::std::string Entrypoint;
  ::std::string Target;
  UINT Flags1 = 0;
  UINT Flags2 = 0;

public:
  bool operator== (const CompileDesc & _Right) const
  {
    if (SrcData != _Right.SrcData) return false;
    if (SourceName != _Right.SourceName) return false;
    if (pDefines != _Right.pDefines) return false;
    if (pInclude != _Right.pInclude) return false;
    if (Entrypoint != _Right.Entrypoint) return false;
    if (Target != _Right.Target) return false;
    if (Flags1 != _Right.Flags1) return false;
    if (Flags2 != _Right.Flags2) return false;

    return true;
  }
};

class Proxy :
  public ::alicorn::extension::testing::Proxy<Proxy>
{
  using FeatureLevels_t = ::std::vector<D3D_FEATURE_LEVEL>;

public:
  MOCK_METHOD1(CreateDeviceAndSwapChain, HRESULT(DeviceDesc));
  MOCK_METHOD0(CreateDevice, ID3D11Device *(void));
  MOCK_METHOD0(CreateDeviceContext, ID3D11DeviceContext *(void));
  MOCK_METHOD0(CreateSwapChain, IDXGISwapChain *(void));

public:
  MOCK_METHOD1(Compile, HRESULT(CompileDesc));
  MOCK_METHOD0(CompileGetCode, ID3DBlob *(void));
  MOCK_METHOD0(CompileGetErrorMsg, ID3DBlob *(void));
};

} // namespace DirectX11

namespace DXGI
{

class SwapChain :
  public IDXGISwapChain
{
public:
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
    return E_FAIL;
  }

  virtual ULONG STDMETHODCALLTYPE AddRef(void) { return 0; }
  MOCK_METHOD0(Release, ULONG(void));

public:
  virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    /* [annotation][in] */
    _In_  REFGUID Name,
    /* [in] */ UINT DataSize,
    /* [annotation][in] */
    _In_reads_bytes_(DataSize)  const void *pData){ return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    /* [annotation][in] */
    _In_  REFGUID Name,
    /* [annotation][in] */
    _In_  const IUnknown *pUnknown){ return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    /* [annotation][in] */
    _In_  REFGUID Name,
    /* [annotation][out][in] */
    _Inout_  UINT *pDataSize,
    /* [annotation][out] */
    _Out_writes_bytes_(*pDataSize)  void *pData){ return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE GetParent(
    /* [annotation][in] */
    _In_  REFIID riid,
    /* [annotation][retval][out] */
    _Out_  void **ppParent){ return E_FAIL; }

public:
  virtual HRESULT STDMETHODCALLTYPE GetDevice(
    /* [annotation][in] */
    _In_  REFIID riid,
    /* [annotation][retval][out] */
    _Out_  void **ppDevice){ return E_FAIL; }

public:
  MOCK_METHOD2(Present, HRESULT(UINT, UINT));

  MOCK_METHOD1(GetResult, HRESULT(::std::string));

  MOCK_METHOD2(GetBuffer, void *(UINT, REFIID));

  virtual HRESULT STDMETHODCALLTYPE GetBuffer(
    UINT Buffer, REFIID riid, void ** ppSurface)
  {
    *ppSurface = GetBuffer(Buffer, riid);
    return GetResult("GetBuffer");
  }

  MOCK_METHOD2(SetFullscreenState, HRESULT(BOOL, IDXGIOutput *));

  virtual HRESULT STDMETHODCALLTYPE GetFullscreenState(
    /* [annotation][out] */
    _Out_opt_  BOOL *pFullscreen,
    /* [annotation][out] */
    _Out_opt_  IDXGIOutput **ppTarget){ return E_FAIL; }

  MOCK_METHOD0(GetDesc, DXGI_SWAP_CHAIN_DESC(void));

  virtual HRESULT STDMETHODCALLTYPE GetDesc(
    DXGI_SWAP_CHAIN_DESC * pDesc)
  { 
    *pDesc = GetDesc();
    return GetResult("GetDesc");
  }

  MOCK_METHOD5(ResizeBuffers, HRESULT(UINT, UINT, UINT, DXGI_FORMAT, UINT));

  virtual HRESULT STDMETHODCALLTYPE ResizeTarget(
    /* [annotation][in] */
    _In_  const DXGI_MODE_DESC *pNewTargetParameters){ return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE GetContainingOutput(
    /* [annotation][out] */
    _Out_  IDXGIOutput **ppOutput){ return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE GetFrameStatistics(
    /* [annotation][out] */
    _Out_  DXGI_FRAME_STATISTICS *pStats){ return E_FAIL; }

  virtual HRESULT STDMETHODCALLTYPE GetLastPresentCount(
    /* [annotation][out] */
    _Out_  UINT *pLastPresentCount){ return E_FAIL; }
};

} // namespace DXGI

} // namespace mock

inline HRESULT D3D11CreateDeviceAndSwapChain(
  IDXGIAdapter * _pAdapter,
  D3D_DRIVER_TYPE _DriverType,
  HMODULE _Software,
  UINT _Flags,
  CONST D3D_FEATURE_LEVEL * _pFeatureLevels, UINT _FeatureLevels,
  UINT _SDKVersion,
  CONST DXGI_SWAP_CHAIN_DESC * _pSwapChainDesc,
  IDXGISwapChain ** _ppSwapChain,
  ID3D11Device ** _ppDevice,
  D3D_FEATURE_LEVEL * _pFeatureLevel,
  ID3D11DeviceContext ** _ppImmediateContext)
{
  *_ppDevice = ::mock::DirectX11::Proxy::GetInstance()->CreateDevice();
  *_ppSwapChain = ::mock::DirectX11::Proxy::GetInstance()->CreateSwapChain();
  *_ppImmediateContext = ::mock::DirectX11::Proxy::GetInstance()->CreateDeviceContext();

  ::mock::DirectX11::DeviceDesc Desc = { 0 };
  Desc.pAdapter = _pAdapter;
  Desc.DriverType = _DriverType;
  Desc.Software = _Software;
  Desc.Flags = _Flags;
  Desc.SDKVersion = _SDKVersion;
  Desc.SwapChainDesc = *_pSwapChainDesc;
  Desc.pFeatureLevel = _pFeatureLevel;

  for (UINT i = 0; i < _FeatureLevels; i++)
  {
    Desc.FeatureLevels.push_back(_pFeatureLevels[i]);
  }

  return ::mock::DirectX11::Proxy::GetInstance()->CreateDeviceAndSwapChain(Desc);
}

inline HRESULT D3DCompile(
  LPCVOID _pSrcData,
  SIZE_T _SrcDataSize,
  LPCSTR _pSourceName,
  CONST D3D_SHADER_MACRO * _pDefines,
  ID3DInclude * _pInclude,
  LPCSTR _pEntrypoint,
  LPCSTR _pTarget,
  UINT _Flags1,
  UINT _Flags2,
  ID3DBlob ** _ppCode,
  ID3DBlob ** _ppErrorMsgs)
{
  *_ppCode = ::mock::DirectX11::Proxy::GetInstance()->CompileGetCode();
  *_ppErrorMsgs = ::mock::DirectX11::Proxy::GetInstance()->CompileGetErrorMsg();

  ::mock::DirectX11::CompileDesc Desc;
  Desc.SrcData = ::std::vector<uint8_t>{
    (uint8_t *)_pSrcData , (uint8_t *)_pSrcData + _SrcDataSize };
  Desc.SourceName = _pSourceName;
  Desc.pDefines = _pDefines;
  Desc.pInclude = _pInclude;
  Desc.Entrypoint = _pEntrypoint;
  Desc.Target = _pTarget;
  Desc.Flags1 = _Flags1;
  Desc.Flags2 = _Flags2;

  return ::mock::DirectX11::Proxy::GetInstance()->Compile(Desc);
}

#pragma warning(pop)
