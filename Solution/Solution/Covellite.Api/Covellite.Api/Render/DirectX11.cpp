
#include "stdafx.h"
#include "DirectX11.hpp"
#include <alicorn/platform/winapi-check.hpp>
#include "fx/Render.auto.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib" )

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
    D3D11_TEXTURE2D_DESC textureDesc = { 0 };
    textureDesc.Width = _Source.Width;
    textureDesc.Height = _Source.Height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.MiscFlags = 0;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;

    D3D11_SUBRESOURCE_DATA Init = { 0 };
    Init.pSysMem = _Source.pData;
    Init.SysMemPitch = _Source.Width* 4;

    ComPtr_t<ID3D11Device> pDevice;
    m_pImmediateContext->GetDevice(&pDevice);

    ComPtr_t<ID3D11Texture2D> pTexture;
    auto Result = pDevice->CreateTexture2D(&textureDesc, &Init, &pTexture);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { 0 };
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;

    Result = pDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_pTexture);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";
  }
};

class DirectX11::Buffer final
{
  using Render_t = ::std::function<void(const Buffer &)>;

public:
  inline void Render(void) { m_Render(*this); }

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
        const UINT stride = sizeof(IGraphicApi::Vertex);
        const UINT offset = 0;
        _Buffer.m_pImmediateContext->IASetVertexBuffers(0, 1,
          _Buffer.m_pBuffer.GetAddressOf(), &stride, &offset);
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
    D3D11_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(T) * _Count;
    bd.BindFlags = Support<T>::GetFlag();

    D3D11_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pData;

    ComPtr_t<ID3D11Device> pDevice;
    m_pImmediateContext->GetDevice(&pDevice);

    auto Result = pDevice->CreateBuffer(&bd, &InitData, &m_pBuffer);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;
  }
};

class DirectX11::Geometry final :
  public IGraphicApi::IGeometry
{
public:
  void Update(FLOAT _X, FLOAT _Y) override
  {
    m_Constants.mWorld = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixTranslation(_X, _Y, 0));

    UINT ViewportCount = 1;
    D3D11_VIEWPORT Viewport = { 0 };
    m_pImmediateContext->RSGetViewports(&ViewportCount, &Viewport);

    m_Constants.mProjection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixOrthographicOffCenterLH(0, (float)Viewport.Width,
      (float)Viewport.Height, 0, -1, 1));
  }

  void Render(void) override
  {
    m_Vertices.Render();
    m_Indices.Render();

    // 18 Август 2018 12:31 (unicornum.verum@gmail.com)
    TODO("Делегировать установку значений объекту девайса.");
    m_pImmediateContext->UpdateSubresource(
      m_pConstantBuffer.Get(), 0, NULL, &m_Constants, 0, 0);
    m_pImmediateContext->VSSetConstantBuffers(
      0, 1, m_pConstantBuffer.GetAddressOf());

    if (m_pTexture) m_pTexture->Render();
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

DXGI_SWAP_CHAIN_DESC sd = { 0 };

DirectX11::DirectX11(const Data & _Data)
{
  UINT createDeviceFlags = 0;

# ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
# endif

  sd.OutputWindow = _Data.Handle;
  sd.Windowed = (_Data.IsFullScreen) ? FALSE : TRUE;
  sd.BufferCount = 1;
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;

  HRESULT Result = S_OK;

  for (auto FeatureLevel : { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 })
  {
    Result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
      createDeviceFlags, &FeatureLevel, 1, D3D11_SDK_VERSION, &sd, 
      &m_pSwapChain, &m_pd3dDevice, NULL, &m_pImmediateContext);
    if (SUCCEEDED(Result)) break;
  }

  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  DXGI_SWAP_CHAIN_DESC Desc = { 0 };
  Result = m_pSwapChain->GetDesc(&Desc);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";

  SetViewport(Desc.BufferDesc.Width, Desc.BufferDesc.Height);
  SetupEffect();

  D3D11_BLEND_DESC bd = { 0 };
  bd.AlphaToCoverageEnable = FALSE;
  bd.IndependentBlendEnable = FALSE;
  bd.RenderTarget[0].BlendEnable = TRUE;
  bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

  Result = m_pd3dDevice->CreateBlendState(&bd, &m_pBlendEnable);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

  D3D11_SAMPLER_DESC sampDesc = { 0 };
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;      // Тип фильтрации - линейная
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;         // Задаем координаты
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

  Result = m_pd3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerState);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

  //Create scissor raster states
  D3D11_RASTERIZER_DESC rasterDesc = { 0 };
  rasterDesc.FillMode = D3D11_FILL_SOLID;
  rasterDesc.CullMode = D3D11_CULL_NONE;
  rasterDesc.FrontCounterClockwise = TRUE;

  rasterDesc.ScissorEnable = TRUE;
  Result = m_pd3dDevice->CreateRasterizerState(&rasterDesc, &m_pScissorTestEnable);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";

  rasterDesc.ScissorEnable = FALSE;
  Result = m_pd3dDevice->CreateRasterizerState(&rasterDesc, &m_pScissorTestDisable);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";
}

DirectX11::~DirectX11(void) = default;

void DirectX11::ClearWindow(void) /*override*/
{
  const FLOAT ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // RGBA
  m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), ClearColor);
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
  D3D11_RECT rect;
  rect.left = _X;
  rect.right = _X + _Width;
  rect.top = _Y;
  rect.bottom = _Y + _Height;

  m_pImmediateContext->RSSetScissorRects(1, &rect);
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

void DirectX11::SetupEffect(void)
{
  UINT shaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
  shaderFlags |= D3DCOMPILE_DEBUG;
  shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  ComPtr_t<ID3DBlob> pCompiledEffect;
  ComPtr_t<ID3DBlob> pError;
  auto Result = D3DCompile(::Render.data(), ::Render.size(), NULL, NULL, NULL, 
    "VS", "vs_4_0", shaderFlags, 0, &pCompiledEffect, &pError);
  if (FAILED(Result))
  {
    throw STD_EXCEPTION << "Failed: " << Result <<
      " [" << (char *)pError->GetBufferPointer() << "].";
  }

  Result = m_pd3dDevice->CreateVertexShader(pCompiledEffect->GetBufferPointer(),
    pCompiledEffect->GetBufferSize(), NULL, &m_pVertexShader);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

  const D3D11_INPUT_ELEMENT_DESC layout[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32_UINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  Result = m_pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
    pCompiledEffect->GetBufferPointer(), pCompiledEffect->GetBufferSize(),
    &m_pVertexLayout);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

  Result = D3DCompile(::Render.data(), ::Render.size(), NULL, NULL, NULL, 
    "PS", "ps_4_0", shaderFlags, 0, &pCompiledEffect, &pError);
  if (FAILED(Result))
  {
    throw STD_EXCEPTION << "Failed: " << Result <<
      " [" << (char *)pError->GetBufferPointer() << "].";
  }

  Result = m_pd3dDevice->CreatePixelShader(pCompiledEffect->GetBufferPointer(),
    pCompiledEffect->GetBufferSize(), NULL, &m_pPixelShader);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

  D3D11_BUFFER_DESC cbd = { 0 };
  cbd.Usage = D3D11_USAGE_DEFAULT;
  cbd.ByteWidth = sizeof(ConstantBuffer);
  cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  Result = m_pd3dDevice->CreateBuffer(&cbd, NULL, &m_pConstantBuffer);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
}

void DirectX11::SetViewport(int _Width, int _Height)
{
  if (m_pRenderTargetView != nullptr)
  {
    m_pRenderTargetView.Reset();

    // Resize the swap chain's buffer to the given dimensions
    m_pSwapChain->ResizeBuffers(2, _Width, _Height,
      DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
  }

  // Recreate Render Target
  ComPtr_t<ID3D11Texture2D> pBackBuffer;
  auto Result = m_pSwapChain->GetBuffer(0,
    __uuidof(ID3D11Texture2D), (LPVOID*)pBackBuffer.GetAddressOf());
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";

  Result = m_pd3dDevice->CreateRenderTargetView(
    pBackBuffer.Get(), NULL, &m_pRenderTargetView);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";

  m_pImmediateContext->OMSetRenderTargets(
    1, m_pRenderTargetView.GetAddressOf(), NULL);

  D3D11_VIEWPORT vp = { 0 };
  vp.Width = (FLOAT)_Width;
  vp.Height = (FLOAT)_Height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  m_pImmediateContext->RSSetViewports(1, &vp);
}
