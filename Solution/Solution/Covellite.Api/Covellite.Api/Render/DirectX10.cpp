
#include "stdafx.h"
#include "DirectX10.hpp"
#include <alicorn/platform/winapi-check.hpp>
#include "fx/Render.auto.hpp"

#include <d3d10.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dcompiler.lib" )

using namespace covellite::api::render;

class D3D10Texture final :
  public IGraphicApi::ITexture
{
public:
  void Render(void) override
  {
    m_pDevice->PSSetShaderResources(0, 1, &m_pTexture);
  }

private:
  ID3D10Device * m_pDevice;
  ID3D10ShaderResourceView * m_pTexture = nullptr;

public:
  D3D10Texture(ID3D10Device * _pDevice, const Data & _Source) :
    m_pDevice(_pDevice)
  {
    D3D10_TEXTURE2D_DESC textureDesc = { 0 };
    textureDesc.Width = _Source.Width;
    textureDesc.Height = _Source.Height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Usage = D3D10_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    textureDesc.MiscFlags = 0;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;

    D3D10_SUBRESOURCE_DATA Init = { 0 };
    Init.pSysMem = _Source.pData;
    Init.SysMemPitch = _Source.Width * 4;

    ID3D10Texture2D * pTexture = nullptr;
    auto Result = m_pDevice->CreateTexture2D(&textureDesc, &Init, &pTexture);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc = { 0 };
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;

    Result = m_pDevice->CreateShaderResourceView(pTexture, &srvDesc, &m_pTexture);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

    pTexture->Release();
  }
  ~D3D10Texture(void)
  {
    m_pTexture->Release();
  }
};

class D3D10Buffer final
{
  using Render_t = ::std::function<void(const D3D10Buffer &)>;

public:
  inline void Render(void) { m_Render(*this); }

private:
  template<class T>
  class Support; // not implement!

  template<>
  class Support<IGraphicApi::Vertex>
  {
  public:
    inline static UINT GetFlag(void) { return D3D10_BIND_VERTEX_BUFFER; }
    static Render_t GetRender(void)
    {
      return [](const D3D10Buffer & _Buffer)
      {
        const UINT stride = sizeof(IGraphicApi::Vertex);
        const UINT offset = 0;
        _Buffer.m_pDevice->IASetVertexBuffers(0, 1,
          &_Buffer.m_pBuffer, &stride, &offset);
      };
    }
  };

  template<>
  class Support<int>
  {
  public:
    inline static UINT GetFlag(void) { return D3D10_BIND_INDEX_BUFFER; }
    static Render_t GetRender(void)
    {
      return [](const D3D10Buffer & _Buffer)
      {
        _Buffer.m_pDevice->IASetIndexBuffer(_Buffer.m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
      };
    }
  };

private:
  ID3D10Device * const m_pDevice;
  const Render_t m_Render;
  ID3D10Buffer * m_pBuffer;

public:
  template<class T>
  D3D10Buffer(ID3D10Device * _pDevice, const T * _pData, UINT _Count) :
    m_pDevice(_pDevice),
    m_Render(Support<T>::GetRender())
  {
    D3D10_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(T) * _Count;
    bd.BindFlags = Support<T>::GetFlag();

    D3D10_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pData;

    auto Result = m_pDevice->CreateBuffer(&bd, &InitData, &m_pBuffer);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;
  }
  ~D3D10Buffer(void)
  {
    m_pBuffer->Release();
  }
};

class D3D10Geometry final :
  public IGraphicApi::IGeometry
{
public:
  void Update(FLOAT _X, FLOAT _Y) override
  {
    m_Constants.mWorld = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixTranslation(_X, _Y, 0));

    UINT ViewportCount = 1;
    D3D10_VIEWPORT Viewport = { 0 };
    m_pDevice->RSGetViewports(&ViewportCount, &Viewport);

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
    m_pDevice->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_Constants, 0, 0);
    m_pDevice->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    if (m_pTexture) m_pTexture->Render();
    m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDevice->DrawIndexed(m_PrimitivesCount * 3, 0, 0);
  }

private:
  ID3D10Device * m_pDevice;
  ID3D10Buffer * m_pConstantBuffer;
  ConstantBuffer m_Constants;
  D3D10Buffer m_Vertices;
  D3D10Buffer m_Indices;
  const DWORD m_PrimitivesCount;
  IGraphicApi::ITexture * m_pTexture;

public:
  D3D10Geometry(
    ID3D10Device * _pDevice,
    ID3D10Buffer * _pConstantBuffer,
    const IGeometry::Data & _SourceData) :
    m_pDevice(_pDevice),
    m_pConstantBuffer(_pConstantBuffer),
    m_Vertices(m_pDevice, _SourceData.pVertices, _SourceData.VerticesCount),
    m_Indices(m_pDevice, _SourceData.pIndices, _SourceData.IndicesCount),
    m_PrimitivesCount((DWORD)_SourceData.IndicesCount / 3),
    m_pTexture(_SourceData.pTexture)
  {
    m_Constants.IsTextureDisabled = (m_pTexture == nullptr) ?
      ::DirectX::XMFLOAT2{ 0.0f, 0.0f } :
      ::DirectX::XMFLOAT2{ 1.0f, 1.0f };
  }
};

DirectX10::DirectX10(const Data & _Data)
{
  UINT createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

  //Setup swap chain
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

  //Create device and swapchain
  auto Result = D3D10CreateDeviceAndSwapChain(NULL, 
    D3D10_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, D3D10_SDK_VERSION, 
    &sd, &m_pSwapChain, &m_pDevice);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";

  DXGI_SWAP_CHAIN_DESC Desc = { 0 };
  Result = m_pSwapChain->GetDesc(&Desc);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";

  SetViewport(Desc.BufferDesc.Width, Desc.BufferDesc.Height);

  SetupEffect();

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

  Result = m_pDevice->CreateBlendState(&bd, &m_pBlendEnable);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

  D3D10_SAMPLER_DESC sampDesc = { 0 };
  sampDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;      // Тип фильтрации - линейная
  sampDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;         // Задаем координаты
  sampDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D10_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D10_FLOAT32_MAX;

  Result = m_pDevice->CreateSamplerState(&sampDesc, &m_pSamplerState);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

  D3D10_RASTERIZER_DESC rasterDesc = { 0 };
  rasterDesc.FillMode = D3D10_FILL_SOLID;
  rasterDesc.CullMode = D3D10_CULL_NONE;
  rasterDesc.FrontCounterClockwise = TRUE;

  rasterDesc.ScissorEnable = TRUE;
  Result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pScissorTestEnable);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";

  rasterDesc.ScissorEnable = FALSE;
  Result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pScissorTestDisable);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";
}

DirectX10::~DirectX10(void) noexcept
{
  m_pDevice->ClearState();

  if (m_pBlendEnable != nullptr)
  {
    m_pBlendEnable->Release();
    m_pBlendEnable = nullptr;
  }

  if (m_pPixelShader != nullptr)
  {
    m_pPixelShader->Release();
    m_pPixelShader = nullptr;
  }

  if (m_pVertexShader != nullptr)
  {
    m_pVertexShader->Release();
    m_pVertexShader = nullptr;
  }

  if (m_pVertexLayout != nullptr)
  {
    m_pVertexLayout->Release();
    m_pVertexLayout = nullptr;
  }

  if (m_pScissorTestEnable != nullptr)
  {
    m_pScissorTestEnable->Release();
    m_pScissorTestEnable = nullptr;
  }

  if (m_pScissorTestDisable != nullptr)
  {
    m_pScissorTestDisable->Release();
    m_pScissorTestDisable = nullptr;
  }

  if (m_pConstantBuffer != nullptr)
  {
    m_pConstantBuffer->Release();
    m_pConstantBuffer = nullptr;
  }

  if (m_pSamplerState != nullptr)
  {
    m_pSamplerState->Release();
    m_pSamplerState = nullptr;
  }

  if (m_pRenderTargetView != nullptr)
  {
    m_pRenderTargetView->Release();
    m_pRenderTargetView = nullptr;
  }

  if (m_pSwapChain != nullptr)
  {
    m_pSwapChain->Release();
    m_pSwapChain = nullptr;
  }

  if (m_pDevice != nullptr)
  {
    m_pDevice->Release();
    m_pDevice = nullptr;
  }
}

void DirectX10::ClearWindow(void) /*override*/
{
  // 25 Август 2018 19:43 (unicornum.verum@gmail.com)
  TODO("Цвет взять из переданных в конструкторе параметров.");

  float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
  m_pDevice->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
}

void DirectX10::Present(void) /*override*/
{
  m_pSwapChain->Present(0, 0);
}

void DirectX10::ResizeWindow(int32_t _Width, int32_t _Height) /*override*/
{
  SetViewport(_Width, _Height);
}

DirectX10::String_t DirectX10::GetUsingApi(void) const /*override*/
{
  return uT("DirectX 10");
}

DirectX10::ITexture * DirectX10::Create(const ITexture::Data & _SourceData) /*override*/
{
  return new D3D10Texture(m_pDevice, _SourceData);
}

void DirectX10::Destroy(ITexture * _pTexture) /*override*/
{
  delete _pTexture;
}

DirectX10::IGeometry * DirectX10::Create(const IGeometry::Data & _SourceData) /*override*/
{
  return new D3D10Geometry(m_pDevice, m_pConstantBuffer, _SourceData);
}

void DirectX10::Destroy(IGeometry * _pGeometry) /*override*/
{
  delete _pGeometry;
}

void DirectX10::EnableScissorRegion(int _X, int _Y, int _Width, int _Height) /*override*/
{
  D3D10_RECT rect;
  rect.left = _X;
  rect.right = _X + _Width;
  rect.top = _Y;
  rect.bottom = _Y + _Height;

  m_pDevice->RSSetState(m_pScissorTestEnable);
  m_pDevice->RSSetScissorRects(1, &rect);
}

void DirectX10::DisableScissorRegion(void) /*override*/
{
  m_pDevice->RSSetState(m_pScissorTestDisable);
}

void DirectX10::Render(void) /*override*/
{
  m_pDevice->IASetInputLayout(m_pVertexLayout);
  m_pDevice->VSSetShader(m_pVertexShader);
  m_pDevice->PSSetShader(m_pPixelShader);
  m_pDevice->PSSetSamplers(0, 1, &m_pSamplerState);

  const FLOAT BlendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  m_pDevice->OMSetBlendState(m_pBlendEnable,
    BlendFactor, 0xFFFFFFFF);
}

void DirectX10::SetupEffect(void)
{
  DWORD ShaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program. - BMD
  ShaderFlags |= D3DCOMPILE_DEBUG;
  ShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

#endif

  //Create our effect from Memory
  ID3DBlob * pCompiledEffect = NULL;
  ID3DBlob * pError = NULL;
  auto Result = D3DCompile(::Render.data(), ::Render.size(), "[Covellite::Api]",
    NULL, NULL, "VS", "vs_4_0", ShaderFlags, 0, &pCompiledEffect, &pError);
  if (FAILED(Result))
  {
    throw STD_EXCEPTION << "Failed: " << Result <<
      " [" << (char *)pError->GetBufferPointer() << "].";
  }

  Result = m_pDevice->CreateVertexShader(pCompiledEffect->GetBufferPointer(),
    pCompiledEffect->GetBufferSize(), &m_pVertexShader);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

  // Описание структуры covellite::api::Vertex для передачи шейдеру
  const D3D10_INPUT_ELEMENT_DESC layout[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32_UINT, 0, 8, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
  };

  Result = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
    pCompiledEffect->GetBufferPointer(), pCompiledEffect->GetBufferSize(),
    &m_pVertexLayout);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

  pCompiledEffect->Release();

  Result = D3DCompile(::Render.data(), ::Render.size(), "[Covellite::Api]",
    NULL, NULL, "PS", "ps_4_0", ShaderFlags, 0, &pCompiledEffect, &pError);
  if (FAILED(Result))
  {
    throw STD_EXCEPTION << "Failed: " << Result <<
      " [" << (char *)pError->GetBufferPointer() << "].";
  }

  Result = m_pDevice->CreatePixelShader(pCompiledEffect->GetBufferPointer(),
    pCompiledEffect->GetBufferSize(), &m_pPixelShader);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed: " << Result;

  pCompiledEffect->Release();

  D3D10_BUFFER_DESC cbd = { 0 };
  cbd.Usage = D3D10_USAGE_DEFAULT;
  cbd.ByteWidth = sizeof(ConstantBuffer);
  cbd.BindFlags = D3D10_BIND_CONSTANT_BUFFER;

  Result = m_pDevice->CreateBuffer(&cbd, NULL, &m_pConstantBuffer);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
}

void DirectX10::SetViewport(int _Width, int _Height)
{
  if (m_pRenderTargetView)
  {
    // Release the existing render target
    m_pRenderTargetView->Release();
    m_pRenderTargetView = NULL;

    // Resize the swap chain's buffer to the given dimensions
    m_pSwapChain->ResizeBuffers(2, _Width, _Height, 
      DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
  }

  // Recreate Render Target
  ID3D10Texture2D * pBackBuffer;
  auto Result = m_pSwapChain->GetBuffer(0, 
    __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";

  Result = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, 
    &m_pRenderTargetView);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed.";

  pBackBuffer->Release();

  m_pDevice->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

  D3D10_VIEWPORT vp;
  vp.Width = _Width;
  vp.Height = _Height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  m_pDevice->RSSetViewports(1, &vp);
}
