
#include "stdafx.h"
#include "DirectX11.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "DxCheck.hpp"
#include "ConstantBuffer.hpp"
#include "fx/Render.auto.hpp"

using namespace covellite::api::render;

class DirectX11::Texture final :
  public IGraphicApi::ITexture
{
public:
  void Render(void) override
  {
    m_pImmediateContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
  }

private:
  ComPtr_t<ID3D11DeviceContext>      m_pImmediateContext;
  ComPtr_t<ID3D11ShaderResourceView> m_pTexture;

public:
  Texture(DirectX11 * _pParent, const Data & _Source) :
    m_pImmediateContext(_pParent->m_pImmediateContext)
  {
    ComPtr_t<ID3D11Device> pDevice;
    m_pImmediateContext->GetDevice(&pDevice);

    D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
    TextureDesc.Width = _Source.Width;
    TextureDesc.Height = _Source.Height;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.MiscFlags = 0;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;

    D3D11_SUBRESOURCE_DATA Init = { 0 };
    Init.pSysMem = _Source.pData;
    Init.SysMemPitch = _Source.Width * 4;

    ComPtr_t<ID3D11Texture2D> pTexture;
    DX_CHECK pDevice->CreateTexture2D(&TextureDesc, &Init, &pTexture);

    D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
    SrvDesc.Format = TextureDesc.Format;
    SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SrvDesc.Texture2D.MipLevels = TextureDesc.MipLevels;

    DX_CHECK pDevice->CreateShaderResourceView(pTexture.Get(), &SrvDesc,
      &m_pTexture);
  }
};

class DirectX11::Buffer final
{
  using Render_t = ::std::function<void(const Buffer &)>;

public:
  inline void Render(void) 
  { 
    m_Render(*this); 
  }

private:
  template<class T>
  class Support; // not implement!

  template<>
  class Support<IGraphicApi::Vertex>
  {
  public:
    inline static UINT GetFlag(void) { return D3D11_BIND_VERTEX_BUFFER; }
    static Render_t GetRender(void)
    {
      return [](const Buffer & _Buffer)
      {
        const UINT Stride = sizeof(IGraphicApi::Vertex);
        const UINT Offset = 0;
        _Buffer.m_pImmediateContext->IASetVertexBuffers(0, 1,
          _Buffer.m_pBuffer.GetAddressOf(), &Stride, &Offset);
      };
    }
  };

  template<>
  class Support<int>
  {
  public:
    inline static UINT GetFlag(void) { return D3D11_BIND_INDEX_BUFFER; }
    static Render_t GetRender(void)
    {
      return [](const Buffer & _Buffer)
      {
        _Buffer.m_pImmediateContext->IASetIndexBuffer(
          _Buffer.m_pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
      };
    }
  };

private:
  ComPtr_t<ID3D11DeviceContext> m_pImmediateContext;
  const Render_t m_Render;
  ComPtr_t<ID3D11Buffer> m_pBuffer;

public:
  template<class T>
  Buffer(DirectX11 * _pParent, const T * _pData, UINT _Count) :
    m_pImmediateContext(_pParent->m_pImmediateContext),
    m_Render(Support<T>::GetRender())
  {
    ComPtr_t<ID3D11Device> pDevice;
    m_pImmediateContext->GetDevice(&pDevice);

    D3D11_BUFFER_DESC Desc = { 0 };
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.ByteWidth = sizeof(T) * _Count;
    Desc.BindFlags = Support<T>::GetFlag();

    D3D11_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pData;

    DX_CHECK pDevice->CreateBuffer(&Desc, &InitData, &m_pBuffer);
  }
};

class DirectX11::Geometry final :
  public IGraphicApi::IGeometry
{
public:
  void Update(FLOAT _X, FLOAT _Y) override
  {
    UINT ViewportCount = 1;
    D3D11_VIEWPORT Viewport = { 0 };
    m_pImmediateContext->RSGetViewports(&ViewportCount, &Viewport);

    m_Constants.Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixOrthographicOffCenterLH(0, 
      (float)Viewport.Width, (float)Viewport.Height, 0, -1, 1));

    m_Constants.World = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixTranslation(_X, _Y, 0));
  }

  void Render(void) override
  {
    m_Vertices.Render();
    m_Indices.Render();

    m_pImmediateContext->UpdateSubresource(
      m_pConstantBuffer.Get(), 0, NULL, &m_Constants, 0, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1, 
      m_pConstantBuffer.GetAddressOf());

    if (m_pTexture != nullptr) m_pTexture->Render();
    m_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->DrawIndexed(m_PrimitivesCount * 3, 0, 0);
  }

private:
  ComPtr_t<ID3D11DeviceContext> m_pImmediateContext;
  ComPtr_t<ID3D11Buffer>        m_pConstantBuffer;
  ConstantBuffer m_Constants;
  Buffer m_Vertices;
  Buffer m_Indices;
  const DWORD m_PrimitivesCount;
  IGraphicApi::ITexture * m_pTexture;

public:
  Geometry(DirectX11 * _pParent, const IGeometry::Data & _SourceData) :
    m_pImmediateContext(_pParent->m_pImmediateContext),
    m_pConstantBuffer(_pParent->m_pConstantBuffer),
    m_Vertices(_pParent, _SourceData.pVertices, _SourceData.VerticesCount),
    m_Indices(_pParent, _SourceData.pIndices, _SourceData.IndicesCount),
    m_PrimitivesCount((DWORD)_SourceData.IndicesCount / 3),
    m_pTexture(_SourceData.pTexture)
  {
    m_Constants.IsTextureDisabled = (m_pTexture == nullptr) ?
      ::DirectX::XMFLOAT2{ 0.0f, 0.0f } :
      ::DirectX::XMFLOAT2{ 1.0f, 1.0f };
  }
};

DirectX11::DirectX11(const Data & _Data)
{
  m_BkColor[0] = _Data.BkColor.R;
  m_BkColor[1] = _Data.BkColor.G;
  m_BkColor[2] = _Data.BkColor.B;
  m_BkColor[3] = _Data.BkColor.A;

  CreateDeviceAndSwapChain(_Data);

  DXGI_SWAP_CHAIN_DESC Desc = { 0 };
  DX_CHECK m_pSwapChain->GetDesc(&Desc);

  SetViewport(Desc.BufferDesc.Width, Desc.BufferDesc.Height);
  SetupEffect();

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  DX_CHECK m_pd3dDevice->CreateBuffer(&ConstantBufferDesc, NULL,
    &m_pConstantBuffer);

  D3D11_RASTERIZER_DESC rasterDesc = { 0 };
  rasterDesc.FillMode = D3D11_FILL_SOLID;
  rasterDesc.CullMode = D3D11_CULL_NONE;
  rasterDesc.FrontCounterClockwise = TRUE;

  rasterDesc.ScissorEnable = TRUE;
  DX_CHECK m_pd3dDevice->CreateRasterizerState(&rasterDesc, &m_pScissorTestEnable);

  rasterDesc.ScissorEnable = FALSE;
  DX_CHECK m_pd3dDevice->CreateRasterizerState(&rasterDesc, &m_pScissorTestDisable);

  D3D11_SAMPLER_DESC SamplerDesc = { 0 };
  SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  SamplerDesc.MinLOD = 0;
  SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  DX_CHECK m_pd3dDevice->CreateSamplerState(&SamplerDesc, &m_pSamplerState);

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

  DX_CHECK m_pd3dDevice->CreateBlendState(&BlendDesc, &m_pBlendEnable);
}

DirectX11::~DirectX11(void) = default;

void DirectX11::ClearWindow(void) /*override*/
{
  m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), m_BkColor);
}

void DirectX11::Present(void) /*override*/
{
  m_pSwapChain->Present(0, 0);
}

void DirectX11::ResizeWindow(int32_t _Width, int32_t _Height) /*override*/
{
  SetViewport(_Width, _Height);
}

DirectX11::String_t DirectX11::GetUsingApi(void) const /*override*/
{
  return uT("DirectX 11");
}

DirectX11::ITexture * DirectX11::Create(const ITexture::Data & _SourceData) /*override*/
{
  return new Texture(this, _SourceData);
}

void DirectX11::Destroy(ITexture * _pTexture) /*override*/
{
  delete _pTexture;
}

DirectX11::IGeometry * DirectX11::Create(const IGeometry::Data & _SourceData) /*override*/
{
  return new Geometry(this, _SourceData);
}

void DirectX11::Destroy(IGeometry * _pGeometry) /*override*/
{
  delete _pGeometry;
}

void DirectX11::EnableScissorRegion(int _X, int _Y, int _Width, int _Height) /*override*/
{
  D3D11_RECT Rect;
  Rect.left = _X;
  Rect.top = _Y;
  Rect.right = _X + _Width;
  Rect.bottom = _Y + _Height;

  m_pImmediateContext->RSSetScissorRects(1, &Rect);
  m_pImmediateContext->RSSetState(m_pScissorTestEnable.Get());
}

void DirectX11::DisableScissorRegion(void) /*override*/
{
  m_pImmediateContext->RSSetState(m_pScissorTestDisable.Get());
}

void DirectX11::Render(void) /*override*/
{
  m_pImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
  m_pImmediateContext->VSSetShader(m_pVertexShader.Get(), NULL, 0);
  m_pImmediateContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
  m_pImmediateContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());

  const FLOAT BlendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  m_pImmediateContext->OMSetBlendState(
    m_pBlendEnable.Get(), BlendFactor, 0xFFFFFFFF);
}

void DirectX11::CreateDeviceAndSwapChain(const Data & _Data)
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
    &m_pd3dDevice,
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

  DX_CHECK m_pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, 
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

void DirectX11::SetupEffect(void)
{
  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  const UINT Flags = (IS_RELEASE_CONFIGURATION) ? 0 :
    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

  ComPtr_t<ID3DBlob> pCompiledEffect;
  ComPtr_t<ID3DBlob> pError;
  auto Result = D3DCompile(::Render.data(), ::Render.size(), "", NULL, NULL, 
    "VS", "vs_4_0", Flags, 0, &pCompiledEffect, &pError);
  if (FAILED(Result))
  {
    throw STD_EXCEPTION << "Failed: " << Result
      << " [" << (const char *)pError->GetBufferPointer() << "].";
  }

  void * const pVertexShaderData = pCompiledEffect->GetBufferPointer();
  const auto VertexShaderDataSize = pCompiledEffect->GetBufferSize();

  DX_CHECK m_pd3dDevice->CreateVertexShader(
    pVertexShaderData, VertexShaderDataSize, NULL, &m_pVertexShader);

  const ::std::vector<D3D11_INPUT_ELEMENT_DESC> Layout =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32_UINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  DX_CHECK m_pd3dDevice->CreateInputLayout(Layout.data(), 
    static_cast<UINT>(Layout.size()),
    pVertexShaderData, VertexShaderDataSize, &m_pVertexLayout);

  Result = D3DCompile(::Render.data(), ::Render.size(), "", NULL, NULL, 
    "PS", "ps_4_0", Flags, 0, &pCompiledEffect, &pError);
  if (FAILED(Result))
  {
    throw STD_EXCEPTION << "Failed: " << Result <<
      " [" << (const char *)pError->GetBufferPointer() << "].";
  }

  DX_CHECK m_pd3dDevice->CreatePixelShader(
    pCompiledEffect->GetBufferPointer(), pCompiledEffect->GetBufferSize(), 
    NULL, &m_pPixelShader);
}
