
#include "stdafx.h"
#include "DirectX11.hpp"
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/lexical-cast.hpp>
#include <Covellite/Api/Vertex.hpp>
#include "DxCheck.hpp"
#include "DirectX.hpp"
#include "Component.hpp"

#include <d3d11.h>
#include <directxmath.h>
#pragma comment(lib, "d3d11.lib")

#include "fx/Hlsl.hpp"

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
    inline static UINT GetFlag(void) { return D3D11_BIND_CONSTANT_BUFFER; }
    inline static void SetConstantBuffer(
      const ComPtr_t<ID3D11DeviceContext> & _pImmediateContext,
      const ComPtr_t<ID3D11Buffer> & _pBuffer)
    {
      _pImmediateContext->VSSetConstantBuffers(
        MATRICES_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
      _pImmediateContext->PSSetConstantBuffers(
        MATRICES_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
    }
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
    inline static UINT GetFlag(void) { return D3D11_BIND_CONSTANT_BUFFER; }
    inline static void SetConstantBuffer(
      const ComPtr_t<ID3D11DeviceContext> & _pImmediateContext,
      const ComPtr_t<ID3D11Buffer> & _pBuffer)
    {
      _pImmediateContext->VSSetConstantBuffers(
        LIGHTS_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
      _pImmediateContext->PSSetConstantBuffers(
        LIGHTS_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
    }
  };

  template<>
  class Support<int>
  {
  public:
    static UINT GetFlag(void) { return D3D11_BIND_INDEX_BUFFER; }
  };

public:
  template<class T>
  static ComPtr_t<ID3D11Buffer> Create(const ComPtr_t<ID3D11Device> & _pDevice,
    const T * _pData, size_t _Count)
  {
    D3D11_BUFFER_DESC Desc = { 0 };
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.ByteWidth = static_cast<decltype(Desc.ByteWidth)>(sizeof(T) * _Count);
    Desc.BindFlags = Support<T>::GetFlag();

    //Desc.BindFlags = ::std::is_same<int, T>::value ?
    //  D3D11_BIND_INDEX_BUFFER :
    //  (::std::is_base_of<::covellite::api::Vertex::Common<T>, T>::value ?
    //    D3D11_BIND_VERTEX_BUFFER :
    //    D3D11_BIND_CONSTANT_BUFFER);

    D3D11_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pData;

    ComPtr_t<ID3D11Buffer> pBuffer;
    DX_CHECK _pDevice->CreateBuffer(&Desc, &InitData, &pBuffer);
    return pBuffer;
  }

  template<class T>
  static ComPtr_t<ID3D11Buffer> Create(
    const ComPtr_t<ID3D11Device> & _pDevice, 
    const ComPtr_t<ID3D11DeviceContext> & _pImmediateContext,
    const T * _pData)
  {
    const auto pResult = Create(_pDevice, _pData, 1);
    Support<T>::SetConstantBuffer(_pImmediateContext, pResult);
    return pResult;
  }
};

class DirectX11::Data final
{
  using CameraId_t = String_t;

private:
  template<class T>
  class ConstantBuffer final
  {
  public:
    T Data;

  public:
    void Update(void)
    {
      m_pImmediateContext->UpdateSubresource(
        m_pBuffer.Get(), 0, NULL, &Data, 0, 0);
    }

  private:
    const ComPtr_t<ID3D11DeviceContext> m_pImmediateContext;
    const ComPtr_t<ID3D11Buffer> m_pBuffer;

  public:
    ConstantBuffer(const ComPtr_t<ID3D11Device> & _pDevice,
      const ComPtr_t<ID3D11DeviceContext> & _pImmediateContext) :
      m_pImmediateContext{ _pImmediateContext },
      m_pBuffer{ Buffer::Create(_pDevice, _pImmediateContext, &Data) }
    {
      memset(&Data, 0, sizeof(Data));
    }
  };

public:
  template<class T>
  T & Get(void) = delete;

  template<class T>
  void Update(void) = delete;

  template<>
  inline ::Matrices & Get(void) { return m_Matrices.Data; }

  template<>
  inline void Update<::Matrices>(void)
  {
    Get<::Matrices>().World =
      ::DirectX::XMMatrixTranspose(Get<::Matrices>().World);

    m_Matrices.Update();
  }

  void SetCameraId(const CameraId_t & _CameraId)
  {
    m_CurrentCameraId = _CameraId;
    m_CurrentLights.Data = Get<::Lights>();

    memset(&Get<::Lights>(), 0, sizeof(::Lights));
  }

  template<>
  inline ::Lights & Get(void) { return m_Lights[m_CurrentCameraId]; }

  template<>
  inline void Update<::Lights>(void) { m_CurrentLights.Update(); }

private:
  ConstantBuffer<::Matrices>        m_Matrices;
  ConstantBuffer<::Lights>          m_CurrentLights;
  ::std::map<CameraId_t, ::Lights>  m_Lights;
  CameraId_t                        m_CurrentCameraId;

public:
  Data(const ComPtr_t<ID3D11Device> & _pDevice,
    const ComPtr_t<ID3D11DeviceContext> & _pImmediateContext) :
    m_Matrices{ _pDevice, _pImmediateContext },
    m_CurrentLights{ _pDevice, _pImmediateContext }
  {
  }
};

DirectX11::DirectX11(const Renderer::Data & _Data) :
  m_BkColor{ _Data.BkColor.R, _Data.BkColor.G, _Data.BkColor.B, _Data.BkColor.A }
{
  CreateDeviceAndSwapChain(_Data);

  m_pData = ::std::make_shared<Data>(m_pDevice, m_pImmediateContext);

  m_Creators =
  {
    { 
      uT("Data"), [&](const ComponentPtr_t & _pComponent)
      {     
        m_ServiceComponents.Add(_pComponent);
        return Render_t{};
      } 
    },
    { uT("Camera"), [&](const ComponentPtr_t & _pComponent)
      { return CreateCamera(_pComponent); } },
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
  m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), 
    m_BkColor.data());
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

auto DirectX11::CreateCamera(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CameraId = _pComponent->Id;

  const auto DisabledBlendRender = CreateBlendState(false);
  const auto DisableDepthRender = GetDepthState(false, false);

  const auto LightsRender = [=](void)
  {
    m_pData->SetCameraId(CameraId);
  };

  const auto ServiceComponents = m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({}) },
      { uT("Rotation"), api::Component::Make({}) },
    });

  const Render_t CameraGui = [=](void)
  {
    const Component::Position Pos{ ServiceComponents[0] };

    DisabledBlendRender();
    DisableDepthRender();
    LightsRender();

    UINT ViewportCount = 1;
    D3D11_VIEWPORT Viewport = { 0 };
    m_pImmediateContext->RSGetViewports(&ViewportCount, &Viewport);

    m_pData->Get<::Matrices>().Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixOrthographicOffCenterLH(
        Pos.X, Pos.X + Viewport.Width,
        Pos.Y + Viewport.Height, Pos.Y,
        1.0f, -1.0f));

    m_pData->Get<::Matrices>().View = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixIdentity());
  };

  const Render_t CameraFocal = [=](void)
  {
    DisabledBlendRender();
    DisableDepthRender();
    LightsRender();

    UINT ViewportCount = 1;
    D3D11_VIEWPORT Viewport = { 0 };
    m_pImmediateContext->RSGetViewports(&ViewportCount, &Viewport);

    const auto AngleY = (float)::alicorn::extension::cpp::math::GreedToRadian *
      _pComponent->GetValue(uT("fov"), 90.0f);
    const auto AspectRatio = Viewport.Width / Viewport.Height;

    m_pData->Get<::Matrices>().Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixPerspectiveFovRH(
        AngleY, AspectRatio, 0.01f, 200.0f));

    // Точка, куда смотрит камера - задается как компонент 
    // Transform.Position.
    const Component::Position Pos{ ServiceComponents[0] };
    const auto Look = ::DirectX::XMVectorSet(Pos.X, Pos.Y, Pos.Z, 1.0f);

    // Расстояние от камеры до Look.
    const auto Distance =
      _pComponent->GetValue(uT("distance"), 0.0f) + 0.1f;

    // Точка, где расположена камера - вычисляется на основе Look, Distance и
    // компонента Transform.Rotation.
    const Component::Position Rot{ ServiceComponents[1] };

    auto Transform =
      ::DirectX::XMMatrixRotationX(Rot.X) *
      ::DirectX::XMMatrixRotationY(Rot.Y) *
      ::DirectX::XMMatrixRotationZ(Rot.Z) *
      ::DirectX::XMMatrixTranslation(Pos.X, Pos.Y, Pos.Z);
    auto Eye = ::DirectX::XMVector3TransformCoord(
      ::DirectX::XMVectorSet(Distance, 0.0f, 0.0f, 1.0f),
      Transform);

    m_pData->Get<::Matrices>().View = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixLookAtRH(Eye, Look,
        ::DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)));
  };

  const auto Focal = _pComponent->GetValue(uT("focal"), uT("Disabled"));
  return (_pComponent->Kind == uT("Perspective") || Focal == uT("Enabled")) ? 
    CameraFocal : CameraGui;
}

auto DirectX11::CreateState(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CreateSamplerState = [&](void)
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

  const auto CreateScissorState = [&](void)
  {
    const Component::Scissor ScissorData{ _pComponent };

    D3D11_RASTERIZER_DESC rasterDesc = { 0 };
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = TRUE;
    rasterDesc.ScissorEnable = (ScissorData.IsEnabled) ? TRUE : FALSE;

    ComPtr_t<ID3D11RasterizerState> pScissor;
    DX_CHECK m_pDevice->CreateRasterizerState(&rasterDesc, &pScissor);

    const auto pScissorRect = 
      m_ServiceComponents.Get({ {uT("Rect"), _pComponent} })[0];

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

  const auto CreateDepthState = [&](void)
  {
    return GetDepthState(_pComponent->GetValue(uT("enabled"), false),
      _pComponent->GetValue(uT("clear"), false));
  };

  const auto CreateClearState = [&](void)
  {
    const auto ARGBtoFloat4 = [](const uint32_t _HexColor)
    {
      return ::std::vector<FLOAT>
      {
        ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
        ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
        ((_HexColor & 0x000000FF) >> 0) / 255.0f,
        ((_HexColor & 0xFF000000) >> 24) / 255.0f,
      };
    };
    const auto BackgroundColor =
      ARGBtoFloat4(_pComponent->GetValue(uT("color"), 0xFF000000));

    return [=](void)
    {
      m_pImmediateContext->ClearRenderTargetView(
        m_pRenderTargetView.Get(), BackgroundColor.data());
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Blend"), [&](void) { return CreateBlendState(true); } },
    { uT("Sampler"), CreateSamplerState },
    { uT("Scissor"), CreateScissorState },
    { uT("Depth"),   CreateDepthState },
    { uT("Clear"),   CreateClearState   },
  };

  return Creators[_pComponent->Kind]();
}

auto DirectX11::CreateLight(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto GetColor = [=](void)
  {
    return _pComponent->GetValue(uT("color"), 0xFF000000);
  };

  const auto ServiceComponents = m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({ }) },
      { uT("Direction"), api::Component::Make({ { uT("x"), 1.0f } }) },
      { uT("Attenuation"), api::Component::Make({ }) },
    });

  auto CreateAmbient = [&](void)
  {
    return [=](void)
    {
      auto & Light = m_pData->Get<::Lights>().Ambient;

      Light.IsValid = 1;
      Light.ARGBColor = GetColor();
    };
  };

  auto CreateDirection = [&](void)
  {
    const auto pDirection = ServiceComponents[1];

    return [=](void)
    {
      auto & Light = m_pData->Get<::Lights>().Direction;

      Light.IsValid = 1;
      Light.ARGBColor = GetColor();

      const Component::Position Direction{ pDirection };
      Light.Direction = { Direction.X, Direction.Y, Direction.Z, 0.0f };
    };
  };

  auto CreatePoint = [&](void)
  {
    auto pPosition = ServiceComponents[0];
    auto pAttenuation = ServiceComponents[2];

    return [=](void)
    {
      const Component::Position Position{ pPosition };

      auto & Lights = m_pData->Get<::Lights>().Points;

      if (Lights.UsedSlotCount >= MAX_LIGHT_POINT_COUNT)
      {
        // 04 Январь 2019 19:30 (unicornum.verum@gmail.com)
        TODO("Запись в лог информации об избыточных источниках света.");
        return;
      }

      auto & Light = Lights.Lights[Lights.UsedSlotCount++];

      Light.ARGBColor = GetColor();
      Light.Position = { Position.X, Position.Y, Position.Z, 1.0f };
      Light.Attenuation =
      {
        pAttenuation->GetValue(uT("const"), 1.0f),
        pAttenuation->GetValue(uT("linear"), 0.0f),
        pAttenuation->GetValue(uT("exponent"), 0.0f),
        0.0f
      };
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Ambient"), CreateAmbient },
    { uT("Direction"), CreateDirection },
    { uT("Point"), CreatePoint },
  };

  return Creators[_pComponent->Kind]();
}

auto DirectX11::CreateMaterial(const ComponentPtr_t & _pComponent) -> Render_t
{
  ::Material Material = { 0 };
  Material.ARGBAmbient  = _pComponent->GetValue(uT("ambient"), 0xFF000000);
  Material.ARGBDiffuse  = _pComponent->GetValue(uT("diffuse"), 0xFF000000);
  Material.ARGBSpecular = _pComponent->GetValue(uT("specular"), 0xFF000000);
  Material.ARGBEmission = _pComponent->GetValue(uT("emission"), 0xFF000000);
  Material.Shininess    = _pComponent->GetValue(uT("shininess"), 0.0f);

  auto pBuffer = Buffer::Create(m_pDevice, &Material, 1);

  return [=](void)
  {
    m_pImmediateContext->VSSetConstantBuffers(
      MATERIAL_BUFFER_INDEX, 1, pBuffer.GetAddressOf());
    m_pImmediateContext->PSSetConstantBuffers(
      MATERIAL_BUFFER_INDEX, 1, pBuffer.GetAddressOf());
  };
}

auto DirectX11::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t
{
  const Component::Texture TextureData{
    m_ServiceComponents.Get({ { uT("Texture"), _pComponent } })[0] };

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

auto DirectX11::CreateShader(const ComponentPtr_t & _pComponent) -> Render_t
{
  using namespace ::alicorn::extension::std;

  const auto pShaderData =
    m_ServiceComponents.Get({ { uT("Shader.HLSL"), _pComponent } })[0];

  const auto DefaultShaderData = ::Vertex + ::Pixel;

  const Component::Shader ShaderData{ pShaderData, DefaultShaderData };

  const auto CompleteShaderData = ::Data + ::Input +
    ::std::vector<uint8_t>{ ShaderData.pData, ShaderData.pData + ShaderData.Count };

  const auto pCompiledShader = DirectX::Shader::Compile(CompleteShaderData,
    ShaderData.Entry.c_str(), ShaderData.Version.c_str());

  const auto VertexShader = 
    [&](const ::std::vector<D3D11_INPUT_ELEMENT_DESC> & _LayoutDesc)
  {
    const auto * const pData = pCompiledShader->GetBufferPointer();
    const auto DataSize = pCompiledShader->GetBufferSize();

    ComPtr_t<ID3D11InputLayout> pVertexLayout;
    DX_CHECK m_pDevice->CreateInputLayout(
      _LayoutDesc.data(), static_cast<UINT>(_LayoutDesc.size()),
      pData, DataSize,
      &pVertexLayout);

    ComPtr_t<ID3D11VertexShader> pVertexShader;
    DX_CHECK m_pDevice->CreateVertexShader(
      pData, DataSize,
      NULL, &pVertexShader);

    return [=](void)
    {
      m_pImmediateContext->IASetInputLayout(pVertexLayout.Get());
      m_pImmediateContext->VSSetShader(pVertexShader.Get(), NULL, 0);
    };
  };

  if (ShaderData.Kind == uT("Polygon"))
  {
    return VertexShader(
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR",    0, DXGI_FORMAT_R32_UINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    });
  }
  else if (ShaderData.Kind == uT("Polyhedron"))
  {
    return VertexShader(
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    });
  }

  ComPtr_t<ID3D11PixelShader> pPixelShader;
  DX_CHECK m_pDevice->CreatePixelShader(
    pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
    NULL, &pPixelShader);

  return [=](void)
  {
    m_pImmediateContext->PSSetShader(pPixelShader.Get(), NULL, 0);
    m_pData->Update<::Lights>();
  };
}

auto DirectX11::CreateBuffer(const ComponentPtr_t & _pComponent) -> Render_t
{
  using Vertex_t = ::covellite::api::Vertex;

  const auto pBufferData =
    m_ServiceComponents.Get({ { uT("Buffer"), _pComponent } })[0];

  if (pBufferData->IsType<const Vertex_t::Polygon *>(uT("data")))
  {
    const Component::Buffer<Vertex_t::Polygon> VertexData{ pBufferData };

    auto pBuffer = Buffer::Create(m_pDevice, VertexData.pData, VertexData.Count);

    return [=](void)
    {
      const UINT Stride = sizeof(Vertex_t::Polygon);
      const UINT Offset = 0;
      m_pImmediateContext->IASetVertexBuffers(0, 1,
        pBuffer.GetAddressOf(), &Stride, &Offset);
    };
  }
  else if (pBufferData->IsType<const Vertex_t::Polyhedron *>(uT("data")))
  {
    const Component::Buffer<Vertex_t::Polyhedron> VertexData{ pBufferData };

    auto pBuffer = Buffer::Create(m_pDevice, VertexData.pData, VertexData.Count);

    return [=](void)
    {
      const UINT Stride = sizeof(Vertex_t::Polyhedron);
      const UINT Offset = 0;
      m_pImmediateContext->IASetVertexBuffers(0, 1,
        pBuffer.GetAddressOf(), &Stride, &Offset);
    };
  }

  const Component::Buffer<int> IndexData{ pBufferData };

  auto pBuffer = Buffer::Create(m_pDevice, IndexData.pData, IndexData.Count);

  return [=](void)
  {
    m_pImmediateContext->IASetIndexBuffer(
      pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  };
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

  return Creators[_pComponent->Kind]();
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

auto DirectX11::GetDepthState(bool _IsEnabled, bool _IsClear) -> Render_t
{
  Render_t RenderDepthDisabled = [=](void)
  {
    m_pImmediateContext->OMSetRenderTargets(1,
      m_pRenderTargetView.GetAddressOf(), nullptr);
  };

  Render_t RenderDepthEnabled = [=](void)
  {
    m_pImmediateContext->OMSetRenderTargets(1, 
      m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
  };

  Render_t RenderDepthClear = [=](void)
  {
    RenderDepthEnabled();
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(),
      D3D11_CLEAR_DEPTH, 1.0f, 0);
  };

  return _IsEnabled ? 
    (_IsClear ? RenderDepthClear : RenderDepthEnabled) : RenderDepthDisabled;
}

auto DirectX11::GetPreRendersGeometry(void) -> Renders_t
{
  Renders_t Result;

  Result.push_back([=](void)
  {
    m_pData->Get<::Matrices>().World = ::DirectX::XMMatrixIdentity();
  });

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    Result.push_back([=](void)
    {
      const Component::Position Position{ _pPosition };

      m_pData->Get<::Matrices>().World *=
        ::DirectX::XMMatrixTranslation(Position.X, Position.Y, Position.Z);
    });
  };

  auto CreateRotation = [&](const ComponentPtr_t & _pRotation)
  {
    Result.push_back([=](void)
    {
      const Component::Position Rotation{ _pRotation };

      m_pData->Get<::Matrices>().World *= ::DirectX::XMMatrixRotationX(Rotation.X);
      m_pData->Get<::Matrices>().World *= ::DirectX::XMMatrixRotationY(Rotation.Y);
      m_pData->Get<::Matrices>().World *= ::DirectX::XMMatrixRotationZ(Rotation.Z);
    });
  };

  auto CreateScale = [&](const ComponentPtr_t & _pScale)
  {
    Result.push_back([=](void)
    {
      const Component::Position Scale{ _pScale };

      m_pData->Get<::Matrices>().World *=
        ::DirectX::XMMatrixScaling(Scale.X, Scale.Y, Scale.Z);
    });
  };

  m_ServiceComponents.Process(
    {
      { uT("Position"), CreatePosition },
      { uT("Rotation"), CreateRotation },
      { uT("Scale"), CreateScale },
    });

  Result.push_back([=](void)
  {
    m_pData->Update<::Matrices>();
  });

  return Result;
}
