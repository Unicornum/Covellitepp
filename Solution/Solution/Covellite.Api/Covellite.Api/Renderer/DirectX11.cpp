
#include "stdafx.h"
#include "DirectX11.hpp"
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/lexical-cast.hpp>
#include <Covellite/Api/Vertex.hpp>
#include "DxCheck.hpp"
#include "Component.hpp"
#include "fx/Data.auto.hpp"
#include "fx/Input.auto.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib" )

using namespace covellite::api::renderer;

class DirectX11::Buffer final
{
private:
  template<class T>
  class Support
  {
  public:
    static UINT GetFlag(void) { return D3D11_BIND_VERTEX_BUFFER; }
  };

  template<>
  class Support<::Matrices>
  {
  public:
    static UINT GetFlag(void) { return D3D11_BIND_CONSTANT_BUFFER; }
  };

  template<>
  class Support<::Material>
  {
  public:
    static UINT GetFlag(void) { return D3D11_BIND_CONSTANT_BUFFER; }
  };

  template<>
  class Support<::Lights>
  {
  public:
    static UINT GetFlag(void) { return D3D11_BIND_CONSTANT_BUFFER; }
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

DirectX11::DirectX11(const Renderer::Data & _Data)
{
  m_BkColor[0] = _Data.BkColor.R;
  m_BkColor[1] = _Data.BkColor.G;
  m_BkColor[2] = _Data.BkColor.B;
  m_BkColor[3] = _Data.BkColor.A;

  memset(&m_Constants, 0, sizeof(m_Constants));

  CreateDeviceAndSwapChain(_Data);

  m_Creators =
  {
    { 
      uT("Data"), [&](const ComponentPtr_t & _pComponent)
      {     
        m_PreRenderComponent.push_back(_pComponent);
        return Render_t{};
      } 
    },
    { uT("State"), [&](const ComponentPtr_t & _pComponent)
      { return CreateState(_pComponent); } },
    { uT("Light"), [&](const ComponentPtr_t & _pComponent)
      { return CreateLight(_pComponent); } },
    { uT("Material"), [&](const ComponentPtr_t & _pComponent)
      { return CreateMaterial(_pComponent); } },
    { uT("Texture"), [&](const ComponentPtr_t & _pComponent)
      { return CreateTexture(_pComponent); } },
    { uT("Shader"), [&](const ComponentPtr_t & _pComponent)
      { return CreateShader(_pComponent); } },
    { uT("Buffer"), [&](const ComponentPtr_t & _pComponent)
      { return CreateBuffer(_pComponent); } },
    { uT("Present"), [&](const ComponentPtr_t & _pComponent)
      { return CreatePresent(_pComponent); } },
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

  DXGI_SWAP_CHAIN_DESC Desc = { 0 };
  DX_CHECK m_pSwapChain->GetDesc(&Desc);

  SetViewport(Desc.BufferDesc.Width, Desc.BufferDesc.Height);
}

void DirectX11::SetViewport(int _Width, int _Height)
{
  D3D11_VIEWPORT ViewPort = { 0 };
  ViewPort.TopLeftX = 0;
  ViewPort.TopLeftY = 0;
  ViewPort.Width = (FLOAT)_Width;
  ViewPort.Height = (FLOAT)_Height;
  ViewPort.MinDepth = 0.0f;
  ViewPort.MaxDepth = 1.0f;
  m_pImmediateContext->RSSetViewports(1, &ViewPort);

  CreateRenderTargetView(_Width, _Height);
  CreateDepthStencilView(_Width, _Height);
}

void DirectX11::CreateRenderTargetView(int _Width, int _Height)
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
}

void DirectX11::CreateDepthStencilView(int _Width, int _Height)
{
  m_pDepthStencilView.Reset();

  const DXGI_FORMAT DeptBufferFormat = DXGI_FORMAT_D32_FLOAT;

  D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
  TextureDesc.Width = _Width;
  TextureDesc.Height = _Height;
  TextureDesc.MipLevels = 1;
  TextureDesc.ArraySize = 1;
  TextureDesc.Format = DeptBufferFormat;
  TextureDesc.SampleDesc.Count = 1;
  TextureDesc.SampleDesc.Quality = 0;
  TextureDesc.Usage = D3D11_USAGE_DEFAULT;
  TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

  ComPtr_t<ID3D11Texture2D> pDepthBuffer;
  DX_CHECK m_pDevice->CreateTexture2D(&TextureDesc, NULL, &pDepthBuffer);

  D3D11_DEPTH_STENCIL_DESC DeptStencilDesc = { 0 };

  // Depth test parameters
  DeptStencilDesc.DepthEnable = true;
  DeptStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

  // Stencil test parameters
  DeptStencilDesc.StencilEnable = false;

  ComPtr_t<ID3D11DepthStencilState> pDSState;
  DX_CHECK m_pDevice->CreateDepthStencilState(&DeptStencilDesc, &pDSState);

  m_pImmediateContext->OMSetDepthStencilState(pDSState.Get(), 1);

  D3D11_DEPTH_STENCIL_VIEW_DESC DeptStencilViewDesc = { 0 };
  DeptStencilViewDesc.Format = DeptBufferFormat;
  DeptStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

  DX_CHECK m_pDevice->CreateDepthStencilView(pDepthBuffer.Get(),
    &DeptStencilViewDesc, &m_pDepthStencilView);
}

auto DirectX11::CreateState(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto CreateSamplerState = [&](void)
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

    return [=](void)
    {
      m_pImmediateContext->PSSetSamplers(0, 1, pSamplerState.GetAddressOf());
    };
  };

  auto CreateScissorState = [&](void)
  {
    const Component::Scissor ScissorData{ _pComponent };

    D3D11_RASTERIZER_DESC rasterDesc = { 0 };
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.FrontCounterClockwise = TRUE;
    rasterDesc.ScissorEnable = (ScissorData.IsEnabled) ? TRUE : FALSE;

    ComPtr_t<ID3D11RasterizerState> pScissor;
    DX_CHECK m_pDevice->CreateRasterizerState(&rasterDesc, &pScissor);

    const auto pScissorRect = GetPreRenderComponent(_pComponent, uT("Rect"));

    Render_t ScissorEnabled = [=](void)
    {
      const Component::Scissor ScissorRect{ pScissorRect };

      D3D11_RECT Rect = { 0 };
      Rect.left = ScissorRect.Left;
      Rect.right = ScissorRect.Right;
      Rect.top = ScissorRect.Top;
      Rect.bottom = ScissorRect.Bottom;

      m_pImmediateContext->RSSetScissorRects(1, &Rect);
      m_pImmediateContext->RSSetState(pScissor.Get());
    };

    Render_t ScissorDisabled = [=](void)
    {
      m_pImmediateContext->RSSetState(pScissor.Get());
    };

    return (ScissorData.IsEnabled) ? ScissorEnabled : ScissorDisabled;
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Blend"), [&](void) { return CreateBlendState(true); } },
    { uT("Sampler"), CreateSamplerState },
    { uT("Scissor"), CreateScissorState },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto DirectX11::CreateLight(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto * const pLights = &m_Lights;
  auto * const pCurrentCameraId = &m_CurrentCameraId;

  auto CreateAmbient = [&](void)
  {
    return [=](void)
    {
      auto & Color = (*pLights)[*pCurrentCameraId].Ambient.Color;

      Color.ARGBAmbient = _pComponent->GetValue(uT("ambient"), 0xFF000000);
      Color.ARGBDiffuse = _pComponent->GetValue(uT("diffuse"), 0xFF000000);
      Color.ARGBSpecular = _pComponent->GetValue(uT("specular"), 0xFF000000);
    };
  };

  auto CreateDirection = [&](void)
  {
    const auto pDirection = GetPreRenderComponent(
      ::covellite::api::Component::Make({ { uT("x"), 1.0f } }), uT("Direction"));

    return [=](void)
    {
      auto & Light = (*pLights)[*pCurrentCameraId].Direction;

      Light.Color.ARGBAmbient = _pComponent->GetValue(uT("ambient"), 0xFF000000);
      Light.Color.ARGBDiffuse = _pComponent->GetValue(uT("diffuse"), 0xFF000000);
      Light.Color.ARGBSpecular = _pComponent->GetValue(uT("specular"), 0xFF000000);

      const Component::Position Direction{ pDirection };
      Light.Direction = { Direction.X, Direction.Y, Direction.Z };
    };
  };

  auto CreatePoint = [&](void)
  {
    auto pPosition = ::covellite::api::Component::Make({ });
    auto pAttenuation = ::covellite::api::Component::Make({ });

    PreRenderComponentsProcess(
      {
        { uT("Position"),
          [&](const ComponentPtr_t & _pComponent) { pPosition = _pComponent; } },
        { uT("Attenuation"),
          [&](const ComponentPtr_t & _pComponent) { pAttenuation = _pComponent; } },
      });

    return [=](void)
    {
      auto & Lights = (*pLights)[*pCurrentCameraId].Points;

      if (Lights.UsedSlotCount >= MAX_LIGHT_POINT_COUNT)
      {
        // 04 Январь 2019 19:30 (unicornum.verum@gmail.com)
        TODO("Запись в лог информации об избыточных источниках света.");
        return;
      }

      auto & Light = Lights.Lights[Lights.UsedSlotCount++];

      Light.Color.ARGBAmbient = _pComponent->GetValue(uT("ambient"), 0xFF000000);
      Light.Color.ARGBDiffuse = _pComponent->GetValue(uT("diffuse"), 0xFF000000);
      Light.Color.ARGBSpecular = _pComponent->GetValue(uT("specular"), 0xFF000000);

      const Component::Position Position{ pPosition };
      Light.Position = { Position.X, Position.Y, Position.Z };

      Light.Attenuation =
      {
        pAttenuation->GetValue(uT("const"), 1.0f),
        pAttenuation->GetValue(uT("linear"), 0.0f),
        pAttenuation->GetValue(uT("exponent"), 0.0f)
      };
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Ambient"), CreateAmbient },
    { uT("Direction"), CreateDirection },
    { uT("Point"), CreatePoint },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto DirectX11::CreateMaterial(const ComponentPtr_t & _pComponent) -> Render_t
{
  ::Material Material = { 0 };

  Material.Color.ARGBAmbient =
    _pComponent->GetValue(uT("ambient"), 0xFF000000);
  Material.Color.ARGBDiffuse =
    _pComponent->GetValue(uT("diffuse"), 0xFF000000);
  Material.Color.ARGBSpecular =
    _pComponent->GetValue(uT("specular"), 0xFF000000);
  Material.ARGBEmission =
    _pComponent->GetValue(uT("emission"), 0xFF000000);
  Material.Shininess =
    _pComponent->GetValue(uT("shininess"), 0.0f);

  auto pBuffer = Buffer::Create(m_pDevice, &Material, 1);

  return [=](void)
  {
    m_pImmediateContext->PSSetConstantBuffers(MATERIAL_BUFFER_INDEX, 1,
      pBuffer.GetAddressOf());
    //m_pImmediateContext->UpdateSubresource(
    //  pBuffer.Get(), 0, NULL, &Material, 0, 0);
  };
}

auto DirectX11::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t
{
  const Component::Texture TextureData{
    GetPreRenderComponent(_pComponent, uT("Texture")) };

  D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
  TextureDesc.Width = TextureData.Width;
  TextureDesc.Height = TextureData.Height;
  TextureDesc.MipLevels = 1;
  TextureDesc.ArraySize = 1;
  TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  TextureDesc.Usage = D3D11_USAGE_DEFAULT;
  TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  TextureDesc.MiscFlags = 0;
  TextureDesc.SampleDesc.Count = 1;
  TextureDesc.SampleDesc.Quality = 0;

  D3D11_SUBRESOURCE_DATA Init = { 0 };
  Init.pSysMem = TextureData.pData;
  Init.SysMemPitch = TextureData.Width * 4;

  ComPtr_t<ID3D11Texture2D> pTexture;
  DX_CHECK m_pDevice->CreateTexture2D(&TextureDesc, &Init, &pTexture);

  D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
  SrvDesc.Format = TextureDesc.Format;
  SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  SrvDesc.Texture2D.MipLevels = TextureDesc.MipLevels;

  ComPtr_t<ID3D11ShaderResourceView> pShaderResourceView;
  DX_CHECK m_pDevice->CreateShaderResourceView(pTexture.Get(), &SrvDesc,
    &pShaderResourceView);

  return [=](void)
  {
    m_pImmediateContext->PSSetShaderResources(0, 1,
      pShaderResourceView.GetAddressOf());
  };
}

class DirectX11::Shader
{
  using Creator_t = ::std::function<Render_t(void)>;

  template<class T>
  class Layout; // Not implement!

  template<>
  class Layout<api::Vertex::Gui>
  {
  public:
    static ::std::vector<D3D11_INPUT_ELEMENT_DESC> GetDesc(void)
    {
      return
      {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32_UINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      };
    }
  };

  template<>
  class Layout<api::Vertex::Textured>
  {
  public:
    static ::std::vector<D3D11_INPUT_ELEMENT_DESC> GetDesc(void)
    {
      return
      {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      };
    }
  };

public:
  static ComPtr_t<ID3DBlob> Compile(const ::std::vector<uint8_t> & _Data,
    LPCSTR _pEntryPoint, LPCSTR _pTarget)
  {
    // 20 Ноябрь 2018 17:46 (unicornum.verum@gmail.com)
    TODO("Повтор кода из реализации DirectX10");

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

  template<class T>
  static Creator_t GetCreator(
    const ComPtr_t<ID3D11Device> & _pDevice,
    const ComPtr_t<ID3D11DeviceContext> & _pImmediateContext,
    const ComPtr_t<ID3DBlob> & _pCompiledShader)
  {
    return [&](void)
    {
      const auto LayoutDesc = Layout<T>::GetDesc();

      const auto * const pData = _pCompiledShader->GetBufferPointer();
      const auto DataSize = _pCompiledShader->GetBufferSize();
      
      // 31 Декабрь 2018 10:55 (unicornum.verum@gmail.com)
      TODO("Получение девайса из _pImmediateContext");

      ComPtr_t<ID3D11InputLayout> pVertexLayout;
      DX_CHECK _pDevice->CreateInputLayout(
        LayoutDesc.data(), static_cast<UINT>(LayoutDesc.size()),
        pData, DataSize,
        &pVertexLayout);

      ComPtr_t<ID3D11VertexShader> pVertexShader;
      DX_CHECK _pDevice->CreateVertexShader(
        pData, DataSize,
        NULL, &pVertexShader);

      return [=](void)
      {
        _pImmediateContext->IASetInputLayout(pVertexLayout.Get());
        _pImmediateContext->VSSetShader(pVertexShader.Get(), NULL, 0);
      };
    };
  }
};

auto DirectX11::CreateShader(const ComponentPtr_t & _pComponent) -> Render_t
{
  const Component::Shader ShaderData{
    GetPreRenderComponent(_pComponent, uT("Shader.HLSL")) };

  using namespace ::alicorn::extension::std;

  const auto CompleteShaderData = ::Data + ::Input +
    ::std::vector<uint8_t>{ ShaderData.pData, ShaderData.pData + ShaderData.Count };

  const auto pCompiledShader = Shader::Compile(CompleteShaderData,
    ShaderData.Entry.c_str(), ShaderData.Version.c_str());

  const auto PixelShader = [&](void)
  {
    ComPtr_t<ID3D11PixelShader> pPixelShader;
    DX_CHECK m_pDevice->CreatePixelShader(
      pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
      NULL, &pPixelShader);

    const auto LightsRender = GetLightsRender();

    return [=](void)
    {
      m_pImmediateContext->PSSetShader(pPixelShader.Get(), NULL, 0);

      LightsRender();
    };
  };

  using Vertex_t = ::covellite::api::Vertex;

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { 
      Vertex_t::Gui::GetName(), 
      Shader::GetCreator<Vertex_t::Gui>(m_pDevice, m_pImmediateContext, pCompiledShader)
    },
    { 
      Vertex_t::Textured::GetName(), 
      Shader::GetCreator<Vertex_t::Textured>(m_pDevice, m_pImmediateContext, pCompiledShader)
    },
    { 
      uT("Pixel"), 
      PixelShader
    },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto DirectX11::CreateBuffer(const ComponentPtr_t & _pComponent) -> Render_t
{
  using Vertex_t = ::covellite::api::Vertex;

  auto CreateVertexGuiBuffer = [&](void)
  {
    const Component::Buffer<Vertex_t::Gui> VertexData{ 
      GetPreRenderComponent(_pComponent, Vertex_t::Gui::GetName()) };

    auto pBuffer = Buffer::Create(m_pDevice, VertexData.pData, VertexData.Count);

    return [=](void)
    {
      const UINT Stride = sizeof(Vertex_t::Gui);
      const UINT Offset = 0;
      m_pImmediateContext->IASetVertexBuffers(0, 1, 
        pBuffer.GetAddressOf(), &Stride, &Offset);
    };
  };

  auto CreateVertexTexturedBuffer = [&](void)
  {
    const Component::Buffer<Vertex_t::Textured> VertexData{ 
      GetPreRenderComponent(_pComponent, Vertex_t::Textured::GetName()) };

    auto pBuffer = Buffer::Create(m_pDevice, VertexData.pData, VertexData.Count);

    return [=](void)
    {
      const UINT Stride = sizeof(Vertex_t::Textured);
      const UINT Offset = 0;
      m_pImmediateContext->IASetVertexBuffers(0, 1,
        pBuffer.GetAddressOf(), &Stride, &Offset);
    };
  };

  auto CreateIndexBuffer = [&](void)
  {
    const Component::Buffer<int> IndexData{
      GetPreRenderComponent(_pComponent, uT("Index")) };

    auto pBuffer = Buffer::Create(m_pDevice, IndexData.pData, IndexData.Count);

    return [=](void)
    {
      m_pImmediateContext->IASetIndexBuffer(
        pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { 
      Vertex_t::Gui::GetName(),
      CreateVertexGuiBuffer 
    },
    {
      Vertex_t::Textured::GetName(),
      CreateVertexTexturedBuffer
    },
    { 
      uT("Index"), 
      CreateIndexBuffer 
    },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto DirectX11::CreatePresent(const ComponentPtr_t & _pComponent) -> Render_t
{
  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    {
      uT("Camera"),
      [&](void) { return CreateCamera(_pComponent); }
    },
    {
      uT("Geometry"),
      [&](void) { return CreateGeometry(_pComponent); }
    },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto DirectX11::CreateCamera(const ComponentPtr_t & _pComponent) -> Render_t
{
  // 03 Январь 2019 17:48 (unicornum.verum@gmail.com)
  TODO("Выделить в отдельный класс информацию о сцене и вынести ее в общий базовый класс");
  auto * const pMatrices = &m_Constants;
  auto * const pLights = &m_Lights;
  auto * const pCurrentCameraId = &m_CurrentCameraId;
  auto * const pCurrentLights = &m_CurrentLights;

  const auto CameraId = _pComponent->Id;

  const auto BlendDisabledRender = CreateBlendState(false);
  const auto DeptRender = GetDeptRender(_pComponent);

  const auto LightsRender = [=](void)
  {
    *pCurrentCameraId = CameraId;
    *pCurrentLights = (*pLights)[CameraId];

    ::Lights EmptyLights;
    memset(&EmptyLights, 0, sizeof(EmptyLights));

    (*pLights)[CameraId] = EmptyLights;
  };

  const Render_t CameraGui = [=](void)
  {
    BlendDisabledRender();
    DeptRender();
    LightsRender();

    UINT ViewportCount = 1;
    D3D11_VIEWPORT Viewport = { 0 };
    m_pImmediateContext->RSGetViewports(&ViewportCount, &Viewport);

    pMatrices->Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixOrthographicOffCenterLH(0,
      Viewport.Width, Viewport.Height, 0, -1, 1));

    pMatrices->View = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixIdentity());
  };

  auto pPosition = ::covellite::api::Component::Make({});
  auto pRotation = ::covellite::api::Component::Make({});

  PreRenderComponentsProcess(
    {
      { uT("Position"),
        [&](const ComponentPtr_t & _pComponent) { pPosition = _pComponent; } },
      { uT("Rotation"),
        [&](const ComponentPtr_t & _pComponent) { pRotation = _pComponent; } },
    });

  const Render_t CameraFocal = [=](void)
  {
    BlendDisabledRender();
    DeptRender();
    LightsRender();

    UINT ViewportCount = 1;
    D3D11_VIEWPORT Viewport = { 0 };
    m_pImmediateContext->RSGetViewports(&ViewportCount, &Viewport);

    const auto AngleY = (float)::alicorn::extension::cpp::math::GreedToRadian *
      _pComponent->GetValue(uT("angle"), 90.0f);
    const auto AspectRatio = Viewport.Width / Viewport.Height;

    pMatrices->Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixPerspectiveFovLH(
        AngleY, AspectRatio, 0.01f, 200.0f));

    // Точка, куда смотрит камера - задается как компонент 
    // Transform.Position.
    const Component::Position Pos{ pPosition };
    const auto Look = ::DirectX::XMVectorSet(Pos.X, Pos.Y, Pos.Z, 1.0f);

    // Расстояние от камеры до Look.
    const auto Distance =
      _pComponent->GetValue(uT("distance"), 0.0f) + 0.1f;

    // Точка, где расположена камера - вычисляется на основе Look, Distance и
    // компонента Transform.Rotation.
    const Component::Position Rot{ pRotation };

    auto Transform =
      ::DirectX::XMMatrixRotationX(Rot.X) *
      ::DirectX::XMMatrixRotationY(Rot.Y) *
      ::DirectX::XMMatrixRotationZ(Rot.Z) *
      ::DirectX::XMMatrixTranslation(Pos.X, Pos.Y, Pos.Z);
    auto Eye = ::DirectX::XMVector3TransformCoord(
      ::DirectX::XMVectorSet(Distance, 0.0f, 0.0f, 1.0f),
      Transform);

    pMatrices->View = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixLookAtLH(Eye, Look,
        ::DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)));
  };

  const auto Focal = _pComponent->GetValue(uT("focal"), uT("Disabled"));
  return (Focal == uT("Enabled")) ? CameraFocal : CameraGui;
}

auto DirectX11::CreateGeometry(const ComponentPtr_t &) -> Render_t
{
  const auto PreRendersGeometry = GetPreRendersGeometry();

  return [=](void)
  {
    for (const auto & Render : PreRendersGeometry) Render();

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

auto DirectX11::CreateBlendState(bool _IsEnabled) -> Render_t
{
  ComPtr_t<ID3D11BlendState> pBlendState;

  if (_IsEnabled)
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

    DX_CHECK m_pDevice->CreateBlendState(&BlendDesc, &pBlendState);
  }

  return [=](void)
  {
    const FLOAT BlendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_pImmediateContext->OMSetBlendState(
      pBlendState.Get(), BlendFactor, 0xFFFFFFFF);
  };
}

auto DirectX11::GetLightsRender(void) -> Render_t
{
  auto * const pCurrentLights = &m_CurrentLights;

  if (m_pLights == nullptr)
  {
    m_pLights = Buffer::Create(m_pDevice, pCurrentLights, 1);
  }

  return [=](void)
  {
    m_pImmediateContext->PSSetConstantBuffers(LIGHTS_BUFFER_INDEX, 1,
      m_pLights.GetAddressOf());
    m_pImmediateContext->UpdateSubresource(
      m_pLights.Get(), 0, NULL, pCurrentLights, 0, 0);
  };
}

auto DirectX11::GetDeptRender(const ComponentPtr_t & _pComponent) -> Render_t
{
  Render_t RenderDeptEnabled = [=](void)
  {
    m_pImmediateContext->OMSetRenderTargets(1, 
      m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(),
      D3D11_CLEAR_DEPTH, 1.0f, 0);
  };

  Render_t RenderDeptDisabled = [=](void)
  {
    m_pImmediateContext->OMSetRenderTargets(1, 
      m_pRenderTargetView.GetAddressOf(), nullptr);
  };

  const auto Dept = _pComponent->GetValue(uT("dept"), uT("Disabled"));
  return (Dept == uT("Enabled")) ? RenderDeptEnabled : RenderDeptDisabled;
}

auto DirectX11::GetPreRendersGeometry(void) -> Renders_t
{
  Renders_t Result;

  auto * const pWorldViewProjection = &m_Constants;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    Result.push_back([=](void)
    {
      const Component::Position Position{ _pPosition };

      pWorldViewProjection->World *=
        ::DirectX::XMMatrixTranslation(Position.X, Position.Y, Position.Z);
    });
  };

  auto CreateRotation = [&](const ComponentPtr_t & _pRotation)
  {
    Result.push_back([=](void)
    {
      const Component::Position Rotation{ _pRotation };

      pWorldViewProjection->World *= ::DirectX::XMMatrixRotationX(Rotation.X);
      pWorldViewProjection->World *= ::DirectX::XMMatrixRotationY(Rotation.Y);
      pWorldViewProjection->World *= ::DirectX::XMMatrixRotationZ(Rotation.Z);
    });
  };

  auto CreateScale = [&](const ComponentPtr_t & _pScale)
  {
    Result.push_back([=](void)
    {
      const Component::Position Scale{ _pScale };

      pWorldViewProjection->World *=
        ::DirectX::XMMatrixScaling(Scale.X, Scale.Y, Scale.Z);
    });
  };

  Result.push_back([=](void)
  {
    pWorldViewProjection->World = ::DirectX::XMMatrixIdentity();
  });

  PreRenderComponentsProcess(
    {
      { uT("Position"), CreatePosition },
      { uT("Rotation"), CreateRotation },
      { uT("Scale"), CreateScale },
    });

  if (m_pMatrices == nullptr)
  {
    m_pMatrices = Buffer::Create(m_pDevice, &m_Constants, 1);
  }

  Result.push_back([=](void)
  {
    pWorldViewProjection->World =
      ::DirectX::XMMatrixTranspose(pWorldViewProjection->World);
    m_pImmediateContext->VSSetConstantBuffers(MATRICES_BUFFER_INDEX, 1,
      m_pMatrices.GetAddressOf());
    m_pImmediateContext->UpdateSubresource(
      m_pMatrices.Get(), 0, NULL, pWorldViewProjection, 0, 0);
  });

  return Result;
}

auto DirectX11::GetPreRenderComponent(const ComponentPtr_t & _pDefault, 
  const String_t & _Kind) -> ComponentPtr_t
{
  auto pResult = _pDefault;

  PreRenderComponentsProcess(
    {
      { 
        _Kind,
        [&](const ComponentPtr_t & _pComponent) { pResult = _pComponent; }
      },
    });

  return pResult;
}

void DirectX11::PreRenderComponentsProcess(const PreRenders_t & _PreRenders)
{
  for (; !m_PreRenderComponent.empty(); m_PreRenderComponent.pop_front())
  {
    const auto pComponent = m_PreRenderComponent.front();
    const auto Kind = pComponent->GetValue(uT("kind"), uT("Unknown"));

    auto itComponent = _PreRenders.find(Kind);
    if (itComponent != _PreRenders.end())
    {
      itComponent->second(pComponent);
    }
    else
    {
      // 13 Декабрь 2018 14:22 (unicornum.verum@gmail.com)
      TODO("Писать в лог warning о лишнем компоненте.");
    }
  }
}
