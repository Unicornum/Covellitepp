
#include "stdafx.h"
#include "DirectX10.hpp"
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/lexical-cast.hpp>
#include "DxCheck.hpp"
#include "Component.hpp"
#include "fx/VertexInput.auto.hpp"
#include "fx/PixelInput.auto.hpp"

#include <d3d10.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dcompiler.lib" )

using namespace covellite::api::renderer;

DirectX10::DirectX10(const Renderer::Data & _Data)
{
  m_BkColor[0] = _Data.BkColor.R;
  m_BkColor[1] = _Data.BkColor.G;
  m_BkColor[2] = _Data.BkColor.B;
  m_BkColor[3] = _Data.BkColor.A;

  memset(&m_Constants, sizeof(m_Constants), 0);

  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  const UINT createDeviceFlags = (IS_RELEASE_CONFIGURATION) ? 0 :
    D3D10_CREATE_DEVICE_DEBUG;

  DXGI_SWAP_CHAIN_DESC sd = { 0 };
  sd.BufferCount = 1;
  sd.OutputWindow = _Data.Handle;
  sd.Windowed = _Data.IsFullScreen ? FALSE : TRUE;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  DX_CHECK D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, 
    NULL, createDeviceFlags, D3D10_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice);

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

DirectX10::~DirectX10(void) = default;

DirectX10::String_t DirectX10::GetUsingApi(void) const /*override*/
{
  return uT("DirectX 10");
}

void DirectX10::ClearFrame(void) /*override*/
{
  m_pDevice->ClearRenderTargetView(m_pRenderTargetView.Get(), m_BkColor);
}

void DirectX10::PresentFrame(void) /*override*/
{
  m_pSwapChain->Present(0, 0);
}

void DirectX10::ResizeWindow(int32_t _Width, int32_t _Height) /*override*/
{
  SetViewport(_Width, _Height);
}

auto DirectX10::GetCreators(void) const -> const Creators_t & /*override*/
{
  return m_Creators;
}

void DirectX10::SetViewport(int _Width, int _Height)
{
  if (m_pRenderTargetView)
  {
    // Release the existing renderer target
    m_pRenderTargetView.Reset();

    // Resize the swap chain's buffer to the given dimensions
    m_pSwapChain->ResizeBuffers(2, _Width, _Height,
      DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
  }

  // Recreate Render Target
  ComPtr_t<ID3D10Texture2D> pBackBuffer;
  DX_CHECK m_pSwapChain->GetBuffer(0,
    __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);

  DX_CHECK m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL,
    &m_pRenderTargetView);

  m_pDevice->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), NULL);

  D3D10_VIEWPORT vp;
  vp.Width = _Width;
  vp.Height = _Height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  m_pDevice->RSSetViewports(1, &vp);
}

auto DirectX10::CreateCamera(const ComponentPtr_t &) -> Render_t
{
  return [&]()
  {
    UINT ViewportCount = 1;
    D3D10_VIEWPORT Viewport = { 0 };
    m_pDevice->RSGetViewports(&ViewportCount, &Viewport);

    m_Constants.Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixOrthographicOffCenterLH(0, (float)Viewport.Width,
      (float)Viewport.Height, 0, -1, 1));
  };
}

auto DirectX10::CreateState(const ComponentPtr_t & _pComponent) const -> Render_t
{
  auto CreateBlendState = [&](const ComponentPtr_t & /*_pComponent*/) -> Render_t
  {
    D3D10_BLEND_DESC bd = { 0 };
    bd.AlphaToCoverageEnable = FALSE;
    bd.BlendEnable[0] = TRUE;
    bd.SrcBlend = D3D10_BLEND_SRC_ALPHA;
    bd.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
    bd.BlendOp = D3D10_BLEND_OP_ADD;
    bd.SrcBlendAlpha = D3D10_BLEND_ONE;
    bd.DestBlendAlpha = D3D10_BLEND_ZERO;
    bd.BlendOpAlpha = D3D10_BLEND_OP_ADD;
    bd.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

    ComPtr_t<ID3D10BlendState> pBlendState;
    DX_CHECK m_pDevice->CreateBlendState(&bd, &pBlendState);

    return [=]()
    {
      const FLOAT BlendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
      m_pDevice->OMSetBlendState(pBlendState.Get(),
        BlendFactor, 0xFFFFFFFF);
    };
  };

  auto CreateSamplerState = [&](const ComponentPtr_t & /*_pComponent*/) -> Render_t
  {
    D3D10_SAMPLER_DESC sampDesc = { 0 };
    sampDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D10_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D10_FLOAT32_MAX;

    ComPtr_t<ID3D10SamplerState> pSamplerState;
    DX_CHECK m_pDevice->CreateSamplerState(&sampDesc, &pSamplerState);

    return [=]()
    {
      m_pDevice->PSSetSamplers(0, 1, pSamplerState.GetAddressOf());
    };
  };

  auto CreateScissorState = [&](const ComponentPtr_t & /*_pComponent*/) -> Render_t
  {
    const Component::Scissor Data{ _pComponent };

    D3D10_RASTERIZER_DESC rasterDesc = { 0 };
    rasterDesc.FillMode = D3D10_FILL_SOLID;
    rasterDesc.CullMode = D3D10_CULL_NONE;
    rasterDesc.FrontCounterClockwise = TRUE;
    rasterDesc.ScissorEnable = (Data.IsEnabled) ? TRUE : FALSE;

    ComPtr_t<ID3D10RasterizerState> pScissor;
    DX_CHECK m_pDevice->CreateRasterizerState(&rasterDesc, &pScissor);

    Render_t ScissorEnabled = [=]()
    {
      const Component::Scissor Data{ _pComponent };

      D3D10_RECT rect;
      rect.left = Data.Left;
      rect.right = Data.Right;
      rect.top = Data.Top;
      rect.bottom = Data.Bottom;

      m_pDevice->RSSetScissorRects(1, &rect);
      m_pDevice->RSSetState(pScissor.Get());
    };

    Render_t ScissorDisabled = [=]()
    {
      m_pDevice->RSSetState(pScissor.Get());
    };

    return (Data.IsEnabled) ? ScissorEnabled : ScissorDisabled;
  };

  Creators_t Creators =
  {
    { uT("Blend"), CreateBlendState },
    { uT("Sampler"), CreateSamplerState },
    { uT("Scissor"), CreateScissorState },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))](_pComponent);
}

auto DirectX10::CreatePosition(const ComponentPtr_t & _pComponent) -> Render_t
{
  return [&, _pComponent]()
  {
    const Component::Position Data{ _pComponent };

    m_Constants.World = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixTranslation(Data.X, Data.Y, Data.Z));
  };
}

class DirectX10::Buffer
{
private:
  template<class T>
  class Support; // Not implement!

  template<>
  class Support<::ConstantBuffer>
  {
  public:
    static UINT GetFlag(void) { return D3D10_BIND_CONSTANT_BUFFER; }
  };

  template<>
  class Support<Vertex>
  {
  public:
    static UINT GetFlag(void) { return D3D10_BIND_VERTEX_BUFFER; }
  };

  template<>
  class Support<int>
  {
  public:
    static UINT GetFlag(void) { return D3D10_BIND_INDEX_BUFFER; }
  };

public:
  template<class T>
  static ComPtr_t<ID3D10Buffer> Create(ComPtr_t<ID3D10Device> _pDevice, 
    const T * _pData, size_t _Count)
  {
    D3D10_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = static_cast<decltype(bd.ByteWidth)>(sizeof(T) * _Count);
    bd.BindFlags = Support<::std::remove_const<T>::type>::GetFlag();

    D3D10_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pData;

    ComPtr_t<ID3D10Buffer> pBuffer;
    DX_CHECK _pDevice->CreateBuffer(&bd, &InitData, &pBuffer);
    return pBuffer;
  }
};

auto DirectX10::CreateBuffer(const ComponentPtr_t & _pComponent) const->Render_t
{
  auto CreateVertexBuffer = [&](const ComponentPtr_t & _pComponent) -> Render_t
  {
    const Component::Buffer<Vertex> Data{ _pComponent };

    auto pBuffer = Buffer::Create(m_pDevice, Data.pData, Data.Count);

    return [=]()
    {
      const UINT stride = sizeof(IGraphicApi::Vertex);
      const UINT offset = 0;
      m_pDevice->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
    };
  };

  auto CreateIndexBuffer = [&](const ComponentPtr_t & _pComponent)->Render_t
  {
    const Component::Buffer<int> Data{ _pComponent };

    auto pBuffer = Buffer::Create(m_pDevice, Data.pData, Data.Count);

    return [=]()
    {
      m_pDevice->IASetIndexBuffer(pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    };
  };

  Creators_t Creators =
  {
    { uT("Vertex"), CreateVertexBuffer },
    { uT("Index"), CreateIndexBuffer },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))](_pComponent);
}

auto DirectX10::CreateDrawCall(const ComponentPtr_t &) const -> Render_t
{
  ::ConstantBuffer oConstantBuffer;

  auto pBuffer = Buffer::Create(m_pDevice, &oConstantBuffer, 1);

  return [=]()
  {
    m_pDevice->UpdateSubresource(pBuffer.Get(), 0, NULL, &m_Constants, 0, 0);
    m_pDevice->VSSetConstantBuffers(0, 1, pBuffer.GetAddressOf());

    ComPtr_t<ID3D10Buffer> pIndexBuffer;
    DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;
    UINT Offset = 0;
    m_pDevice->IAGetIndexBuffer(&pIndexBuffer, &Format, &Offset);

    D3D10_BUFFER_DESC Desc = { 0 };
    pIndexBuffer->GetDesc(&Desc);

    m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDevice->DrawIndexed((DWORD)(Desc.ByteWidth / sizeof(int)), 0, 0);
  };
}

auto DirectX10::CreateTexture(const ComponentPtr_t & _pComponent) const -> Render_t
{
  const Component::Texture Data{ _pComponent };

  D3D10_TEXTURE2D_DESC textureDesc = { 0 };
  textureDesc.Width = Data.Width;
  textureDesc.Height = Data.Height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.Usage = D3D10_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  textureDesc.MiscFlags = 0;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;

  D3D10_SUBRESOURCE_DATA Init = { 0 };
  Init.pSysMem = Data.pData;
  Init.SysMemPitch = Data.Width * 4;

  ComPtr_t<ID3D10Texture2D> pTexture;
  DX_CHECK m_pDevice->CreateTexture2D(&textureDesc, &Init, &pTexture);

  D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc = { 0 };
  srvDesc.Format = textureDesc.Format;
  srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
  srvDesc.Texture2D.MostDetailedMip = 0;

  ComPtr_t<ID3D10ShaderResourceView> pShaderResourceView;
  DX_CHECK m_pDevice->CreateShaderResourceView(pTexture.Get(),
    &srvDesc, &pShaderResourceView);

  return [=]()
  {
    m_pDevice->PSSetShaderResources(0, 1, pShaderResourceView.GetAddressOf());
  };
}

auto DirectX10::CreateShader(const ComponentPtr_t & _pComponent) const -> Render_t
{
  class Shader
  {
  public:
    static ComPtr_t<ID3DBlob> Compile(const ::std::vector<uint8_t> & _Data,
      LPCSTR _pEntryPoint, LPCSTR _pTarget)
    {
      // 20 Ноябрь 2018 17:46 (unicornum.verum@gmail.com)
      TODO("Повтор кода из реализации DirectX11");

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
    const ::std::vector<D3D10_INPUT_ELEMENT_DESC> LayoutDesc =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR",    0, DXGI_FORMAT_R32_UINT, 0, 8, D3D10_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };

    ComPtr_t<ID3D10InputLayout> pVertexLayout;
    DX_CHECK m_pDevice->CreateInputLayout(
      LayoutDesc.data(), (UINT)LayoutDesc.size(),
      pCompiledEffect->GetBufferPointer(), pCompiledEffect->GetBufferSize(),
      &pVertexLayout);

    ComPtr_t<ID3D10VertexShader> pVertexShader;
    DX_CHECK m_pDevice->CreateVertexShader(
      pCompiledEffect->GetBufferPointer(), pCompiledEffect->GetBufferSize(), 
      &pVertexShader);

    return [=]()
    {
      m_pDevice->IASetInputLayout(pVertexLayout.Get());
      m_pDevice->VSSetShader(pVertexShader.Get());
    };
  };

  auto PixelShader = [&](void) -> Render_t
  {
    ComPtr_t<ID3D10PixelShader> pPixelShader;
    DX_CHECK m_pDevice->CreatePixelShader(pCompiledEffect->GetBufferPointer(),
      pCompiledEffect->GetBufferSize(), &pPixelShader);

    return [=]()
    {
      m_pDevice->PSSetShader(pPixelShader.Get());
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Vertex"), VertexShader },
    { uT("Pixel"), PixelShader },
  };

  return Creators[Data.Kind]();
}
