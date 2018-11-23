
#include "stdafx.h"
#include "DirectX11.hpp"
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/lexical-cast.hpp>
#include "DxCheck.hpp"
#include "Component.hpp"
#include "fx/VertexInput.auto.hpp"
#include "fx/PixelInput.auto.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib" )

using namespace covellite::api::renderer;

DirectX11::DirectX11(const Renderer::Data & _Data)
{
  m_BkColor[0] = _Data.BkColor.R;
  m_BkColor[1] = _Data.BkColor.G;
  m_BkColor[2] = _Data.BkColor.B;
  m_BkColor[3] = _Data.BkColor.A;

  memset(&m_Constants, 0, sizeof(m_Constants));

  CreateDeviceAndSwapChain(_Data);

  DXGI_SWAP_CHAIN_DESC Desc = { 0 };
  DX_CHECK m_pSwapChain->GetDesc(&Desc);

  SetViewport(Desc.BufferDesc.Width, Desc.BufferDesc.Height);

  m_Creators =
  {
    { uT("Camera"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateCamera(_pData); } },
    { uT("State"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateState(_pData); } },
    { uT("Shader"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateShader(_pData); } },
    { uT("Texture"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateTexture(_pData); } },
    { uT("Buffer"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateBuffer(_pData); } },
    { uT("DrawCall"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateDrawCall(_pData); } },
    { uT("Position"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreatePosition(_pData); } },
  };
}

DirectX11::~DirectX11(void) = default;

DirectX11::String_t DirectX11::GetUsingApi(void) const /*override*/
{
  return uT("DirectX 11");
}

void DirectX11::ClearFrame(void) /*override*/
{
  m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), m_BkColor);
}

void DirectX11::PresentFrame(void) /*override*/
{
  m_pSwapChain->Present(0, 0);
}

void DirectX11::ResizeWindow(int32_t _Width, int32_t _Height) /*override*/
{
  SetViewport(_Width, _Height);
}

auto DirectX11::GetCreators(void) const -> const Creators_t & /*override*/
{
  return m_Creators;
}

void DirectX11::CreateDeviceAndSwapChain(const Renderer::Data & _Data)
{
  const D3D_FEATURE_LEVEL FeatureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0
  };

  DXGI_SWAP_CHAIN_DESC sd = { 0 };
  sd.OutputWindow = _Data.Handle;
  sd.Windowed = (_Data.IsFullScreen) ? FALSE : TRUE;
  sd.BufferCount = 2;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.SampleDesc.Count = 1;
  //sd.SampleDesc.Quality = 0;

  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  DX_CHECK D3D11CreateDeviceAndSwapChain(
    NULL,
    D3D_DRIVER_TYPE_HARDWARE,
    NULL,
    (IS_RELEASE_CONFIGURATION) ? 0 : D3D11_CREATE_DEVICE_DEBUG,
    FeatureLevels, sizeof(FeatureLevels) / sizeof(FeatureLevels[0]),
    D3D11_SDK_VERSION,
    &sd,
    &m_pSwapChain,
    &m_pDevice,
    NULL,
    &m_pImmediateContext);
}

void DirectX11::SetViewport(int _Width, int _Height)
{
  if (m_pRenderTargetView != nullptr)
  {
    m_pRenderTargetView.Reset();

    DX_CHECK m_pSwapChain->ResizeBuffers(2, _Width, _Height,
      DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
  }

  ComPtr_t<ID3D11Texture2D> pBackBuffer;
  DX_CHECK m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), 
    (void **)pBackBuffer.GetAddressOf());

  DX_CHECK m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, 
    &m_pRenderTargetView);

  m_pImmediateContext->OMSetRenderTargets(
    1, m_pRenderTargetView.GetAddressOf(), NULL);

  D3D11_VIEWPORT ViewPort = { 0 };
  ViewPort.Width = (FLOAT)_Width;
  ViewPort.Height = (FLOAT)_Height;
  ViewPort.MinDepth = 0.0f;
  ViewPort.MaxDepth = 1.0f;
  ViewPort.TopLeftX = 0;
  ViewPort.TopLeftY = 0;

  m_pImmediateContext->RSSetViewports(1, &ViewPort);
}

auto DirectX11::CreateCamera(const ComponentPtr_t &) -> Render_t
{
  return [&]()
  {
    UINT ViewportCount = 1;
    D3D11_VIEWPORT Viewport = { 0 };
    m_pImmediateContext->RSGetViewports(&ViewportCount, &Viewport);

    m_Constants.Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixOrthographicOffCenterLH(0,
      (FLOAT)Viewport.Width, (FLOAT)Viewport.Height, 0, -1, 1));
  };
}

auto DirectX11::CreateState(const ComponentPtr_t & _pComponent) const -> Render_t
{
  auto CreateBlendState = [&](void) -> Render_t
  {
    D3D11_BLEND_DESC BlendDesc = { 0 };
    BlendDesc.AlphaToCoverageEnable = FALSE;
    BlendDesc.IndependentBlendEnable = FALSE;
    BlendDesc.RenderTarget[0].BlendEnable = TRUE;
    BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ComPtr_t<ID3D11BlendState> pBlendState;
    DX_CHECK m_pDevice->CreateBlendState(&BlendDesc, &pBlendState);

    return [=]()
    {
      const FLOAT BlendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
      m_pImmediateContext->OMSetBlendState(
        pBlendState.Get(), BlendFactor, 0xFFFFFFFF);
    };
  };

  auto CreateSamplerState = [&](void) -> Render_t
  {
    D3D11_SAMPLER_DESC SamplerDesc = { 0 };
    SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    SamplerDesc.MinLOD = 0;
    SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ComPtr_t<ID3D11SamplerState> pSamplerState;
    DX_CHECK m_pDevice->CreateSamplerState(&SamplerDesc, &pSamplerState);

    return [=]()
    {
      m_pImmediateContext->PSSetSamplers(0, 1, pSamplerState.GetAddressOf());
    };
  };

  auto CreateScissorState = [&](void) -> Render_t
  {
    const Component::Scissor Data{ _pComponent };

    D3D11_RASTERIZER_DESC rasterDesc = { 0 };
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.FrontCounterClockwise = TRUE;
    rasterDesc.ScissorEnable = (Data.IsEnabled) ? TRUE : FALSE;

    ComPtr_t<ID3D11RasterizerState> pScissor;
    DX_CHECK m_pDevice->CreateRasterizerState(&rasterDesc, &pScissor);

    Render_t ScissorEnabled = [=]()
    {
      const Component::Scissor Data{ _pComponent };

      D3D11_RECT Rect;
      Rect.left = Data.Left;
      Rect.right = Data.Right;
      Rect.top = Data.Top;
      Rect.bottom = Data.Bottom;

      m_pImmediateContext->RSSetScissorRects(1, &Rect);
      m_pImmediateContext->RSSetState(pScissor.Get());
    };

    Render_t ScissorDisabled = [=]()
    {
      m_pImmediateContext->RSSetState(pScissor.Get());
    };

    return (Data.IsEnabled) ? ScissorEnabled : ScissorDisabled;
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Blend"), CreateBlendState },
    { uT("Sampler"), CreateSamplerState },
    { uT("Scissor"), CreateScissorState },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto DirectX11::CreatePosition(const ComponentPtr_t & _pComponent) -> Render_t
{
  return [&, _pComponent]()
  {
    const Component::Position Data{ _pComponent };

    m_Constants.World = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixTranslation(Data.X, Data.Y, Data.Z));
  };
}

class DirectX11::Buffer final
{
private:
  template<class T>
  class Support; // not implement!

  template<>
  class Support<ConstantBuffer>
  {
  public:
    static UINT GetFlag(void) { return D3D11_BIND_CONSTANT_BUFFER; }
  };

  template<>
  class Support<Vertex>
  {
  public:
    static UINT GetFlag(void) { return D3D11_BIND_VERTEX_BUFFER; }
  };

  template<>
  class Support<int>
  {
  public:
    static UINT GetFlag(void) { return D3D11_BIND_INDEX_BUFFER; }
  };

public:
  template<class T>
  static ComPtr_t<ID3D11Buffer> Create(ComPtr_t<ID3D11Device> _pDevice,
    const T * _pData, size_t _Count)
  {
    D3D11_BUFFER_DESC Desc = { 0 };
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.ByteWidth = static_cast<decltype(Desc.ByteWidth)>(sizeof(T) * _Count);
    Desc.BindFlags = Support<T>::GetFlag();

    D3D11_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pData;

    ComPtr_t<ID3D11Buffer> pBuffer;
    DX_CHECK _pDevice->CreateBuffer(&Desc, &InitData, &pBuffer);
    return pBuffer;
  }
};

auto DirectX11::CreateBuffer(const ComponentPtr_t & _pComponent) const->Render_t
{
  auto CreateVertexBuffer = [&](void) -> Render_t
  {
    const Component::Buffer<Vertex> Data{ _pComponent };

    auto pBuffer = Buffer::Create(m_pDevice, Data.pData, Data.Count);

    return [=]()
    {
      const UINT Stride = sizeof(Vertex);
      const UINT Offset = 0;
      m_pImmediateContext->IASetVertexBuffers(0, 1, 
        pBuffer.GetAddressOf(), &Stride, &Offset);
    };
  };

  auto CreateIndexBuffer = [&](void) -> Render_t
  {
    const Component::Buffer<int> Data{ _pComponent };

    auto pBuffer = Buffer::Create(m_pDevice, Data.pData, Data.Count);

    return [=]()
    {
      m_pImmediateContext->IASetIndexBuffer(
        pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Vertex"), CreateVertexBuffer },
    { uT("Index"), CreateIndexBuffer },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto DirectX11::CreateDrawCall(const ComponentPtr_t &) const -> Render_t
{
  ConstantBuffer oConstantBuffer;

  auto pBuffer = Buffer::Create(m_pDevice, &oConstantBuffer, 1);

  return [=]()
  {
    m_pImmediateContext->UpdateSubresource(
      pBuffer.Get(), 0, NULL, &m_Constants, 0, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1,
      pBuffer.GetAddressOf());

    ComPtr_t<ID3D11Buffer> pIndexBuffer;
    DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;
    UINT Offset = 0;
    m_pImmediateContext->IAGetIndexBuffer(&pIndexBuffer, &Format, &Offset);

    D3D11_BUFFER_DESC Desc = { 0 };
    pIndexBuffer->GetDesc(&Desc);

    m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->DrawIndexed((DWORD)(Desc.ByteWidth / sizeof(int)), 0, 0);
  };
}

auto DirectX11::CreateTexture(const ComponentPtr_t & _pComponent) const -> Render_t
{
  const Component::Texture Data{ _pComponent };

  D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
  TextureDesc.Width = Data.Width;
  TextureDesc.Height = Data.Height;
  TextureDesc.MipLevels = 1;
  TextureDesc.ArraySize = 1;
  TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  TextureDesc.Usage = D3D11_USAGE_DEFAULT;
  TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  TextureDesc.MiscFlags = 0;
  TextureDesc.SampleDesc.Count = 1;
  TextureDesc.SampleDesc.Quality = 0;

  D3D11_SUBRESOURCE_DATA Init = { 0 };
  Init.pSysMem = Data.pData;
  Init.SysMemPitch = Data.Width * 4;

  ComPtr_t<ID3D11Texture2D> pTexture;
  DX_CHECK m_pDevice->CreateTexture2D(&TextureDesc, &Init, &pTexture);

  D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
  SrvDesc.Format = TextureDesc.Format;
  SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  SrvDesc.Texture2D.MipLevels = TextureDesc.MipLevels;

  ComPtr_t<ID3D11ShaderResourceView> pShaderResourceView;
  DX_CHECK m_pDevice->CreateShaderResourceView(pTexture.Get(), &SrvDesc,
    &pShaderResourceView);

  return [=]()
  {
    m_pImmediateContext->PSSetShaderResources(0, 1, 
      pShaderResourceView.GetAddressOf());
  };
}

auto DirectX11::CreateShader(const ComponentPtr_t & _pComponent) const -> Render_t
{
  class Shader
  {
  public:
    static ComPtr_t<ID3DBlob> Compile(const ::std::vector<uint8_t> & _Data, 
      LPCSTR _pEntryPoint, LPCSTR _pTarget)
    {
      using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

      const DWORD ShaderFlags = (IS_RELEASE_CONFIGURATION) ? 0 :
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

      ::Microsoft::WRL::ComPtr<ID3DBlob> pCompiledEffect;
      ::Microsoft::WRL::ComPtr<ID3DBlob> pError;
      auto Result = D3DCompile(_Data.data(), _Data.size(), "[Covellite::Api]", 
        NULL, NULL, _pEntryPoint, _pTarget, ShaderFlags, 0,
        &pCompiledEffect, &pError);
      if (FAILED(Result))
      {
        throw STD_EXCEPTION << "Failed: " << Result <<
          " [" << (char *)pError->GetBufferPointer() << "].";
      }

      return pCompiledEffect;
    }
  };

  using namespace ::alicorn::extension::std;

  const Component::Shader Data{ _pComponent };

  auto pCompiledEffect = Shader::Compile(VertexInput + PixelInput +
    ::std::vector<uint8_t>{ Data.pData, Data.pData + Data.Count },
    Data.Entry.c_str(), Data.Version.c_str());

  auto VertexShader = [&](void) -> Render_t
  {
    // Описание структуры covellite::api::Vertex для передачи шейдеру
    const ::std::vector<D3D11_INPUT_ELEMENT_DESC> Layout =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32_UINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    auto * const pData = pCompiledEffect->GetBufferPointer();
    const auto DataSize = pCompiledEffect->GetBufferSize();

    ComPtr_t<ID3D11InputLayout> pVertexLayout;
    DX_CHECK m_pDevice->CreateInputLayout(
      Layout.data(), static_cast<UINT>(Layout.size()),
      pData, DataSize,
      &pVertexLayout);

    ComPtr_t<ID3D11VertexShader> pVertexShader;
    DX_CHECK m_pDevice->CreateVertexShader(
      pData, DataSize,
      NULL, &pVertexShader);

    return [=]()
    {
      m_pImmediateContext->IASetInputLayout(pVertexLayout.Get());
      m_pImmediateContext->VSSetShader(pVertexShader.Get(), NULL, 0);
    };
  };

  auto PixelShader = [&](void) -> Render_t
  {
    ComPtr_t<ID3D11PixelShader> pPixelShader;
    DX_CHECK m_pDevice->CreatePixelShader(
      pCompiledEffect->GetBufferPointer(), pCompiledEffect->GetBufferSize(), 
      NULL, &pPixelShader);

    return [=]()
    {
      m_pImmediateContext->PSSetShader(pPixelShader.Get(), NULL, 0);
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Vertex"), VertexShader },
    { uT("Pixel"), PixelShader },
  };

  return Creators[Data.Kind]();
}
