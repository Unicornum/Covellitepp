
#include "stdafx.h"
#include "DirectX11.hpp"
#include <directxmath.h>
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/lexical-cast.hpp>
#include "DxCheck.hpp"
#include "DirectX.hpp"

#pragma warning(push)
#pragma warning(disable: 4996)
#include <Covellite/Api/Vertex.hpp>
#pragma warning(pop)

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "Shaders/Shaders.hpp"
#include <Covellite/Api/Defines.hpp>
#include "Component.hpp"
#include "GraphicApi.Constants.hpp"

using namespace covellite::api::renderer;

// ************************************************************************** //

class DirectX11::Buffer final
{
private:
  template<class T>
  class Support
  {
  public:
    static UINT GetFlag(void) { return D3D11_BIND_VERTEX_BUFFER; }
  };

  template<UINT TIndex>
  class Constant
  {
  public:
    inline static UINT GetFlag(void) { return D3D11_BIND_CONSTANT_BUFFER; }
    inline static UINT GetIndex(void) { return TIndex; }
  };

  template<>
  class Support<::Camera> : public Constant<CAMERA_BUFFER_INDEX> { };

  template<>
  class Support<::Matrices> : public Constant<MATRICES_BUFFER_INDEX> { };

  template<>
  class Support<::Lights> : public Constant<LIGHTS_BUFFER_INDEX> { };

  template<>
  class Support<::Fog> : public Constant<FOG_BUFFER_INDEX> { };

  template<>
  class Support<int>
  {
  public:
    static UINT GetFlag(void) { return D3D11_BIND_INDEX_BUFFER; }
  };

public:
  template<class T>
  static ComPtr_t<ID3D11Buffer> Create(
    const ComPtr_t<ID3D11Device> & _pDevice,
    const bool _IsDynamic,
    const T * _pData, 
    size_t _Count)
  {
    D3D11_BUFFER_DESC Desc = { 0 };
    Desc.Usage = _IsDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    Desc.CPUAccessFlags = _IsDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
    Desc.ByteWidth = static_cast<decltype(Desc.ByteWidth)>(sizeof(T) * _Count);
    Desc.BindFlags = Support<T>::GetFlag();

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
    const auto pResult = Create(_pDevice, false, _pData, 1);

    const auto Index = Support<T>::GetIndex();

    _pImmediateContext->VSSetConstantBuffers(Index, 1, pResult.GetAddressOf());
    _pImmediateContext->PSSetConstantBuffers(Index, 1, pResult.GetAddressOf());

    return pResult;
  }
};

template<class T>
class DirectX11::ConstantBuffer final :
  public Constants::Data<T>
{
public:
  void Update(void) const override
  {
    m_pImmediateContext->UpdateSubresource(
      m_pBuffer.Get(), 0, NULL, &m_Data, 0, 0);
  }

private:
  const ComPtr_t<ID3D11DeviceContext> m_pImmediateContext;
  const ComPtr_t<ID3D11Buffer> m_pBuffer;

public:
  ConstantBuffer(
    const ComPtr_t<ID3D11Device> & _pDevice,
    const ComPtr_t<ID3D11DeviceContext> & _pImmediateContext) :
    m_pImmediateContext{ _pImmediateContext },
    m_pBuffer{ Buffer::Create(_pDevice, _pImmediateContext, &m_Data) }
  {
  }
};

// ************************************************************************** //

DirectX11::DirectX11(const Data_t & _Data)
{
  CreateDeviceAndSwapChain(_Data);

  MakeConstants<ConstantBuffer>(m_pDevice, m_pImmediateContext);
}

DirectX11::~DirectX11(void) = default;

DirectX11::String_t DirectX11::GetUsingApi(void) const /*override*/
{
  return uT("DirectX 11");
}

void DirectX11::PresentFrame(void) /*override*/
{
  m_pSwapChain->Present(0, 0);

  GraphicApi::PresentFrame();
}

void DirectX11::ResizeWindow(int32_t _Width, int32_t _Height) /*override*/
{
  SetViewport(_Width, _Height);
}

void DirectX11::CreateDeviceAndSwapChain(const Data_t & _Data)
{
  const D3D_FEATURE_LEVEL FeatureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0
  };

  DXGI_SWAP_CHAIN_DESC sd = { 0 };
  sd.OutputWindow = ::covellite::any_cast<HWND>(_Data.Handle);
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

  D3D11_DEPTH_STENCIL_VIEW_DESC DeptStencilViewDesc = { 0 };
  DeptStencilViewDesc.Format = DeptBufferFormat;
  DeptStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

  DX_CHECK m_pDevice->CreateDepthStencilView(pDepthBuffer.Get(),
    &DeptStencilViewDesc, &m_pDepthStencilView);
}

auto DirectX11::CreateCamera(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto CameraId = _pComponent->Id;

  const auto DisabledBlendRender = CreateBlendState(false);
  const auto DisableDepthRender = GetDepthState(false, false, false);

  const auto LightsRender = [=](void)
  {
    m_pConstants->SetCameraId(CameraId);
  };

  const auto ServiceComponents = m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({}) },
      { uT("Rotation"), api::Component::Make({}) },
    });

  const Render_t CameraOptographic = [=](void)
  {
    const Component::Position Pos{ ServiceComponents[0] };

    DisabledBlendRender();
    DisableDepthRender();
    LightsRender();

    UINT ViewportCount = 1;
    D3D11_VIEWPORT Viewport = { 0 };
    m_pImmediateContext->RSGetViewports(&ViewportCount, &Viewport);

    m_pConstants->Get<::Matrices>().Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixOrthographicOffCenterLH(
        Pos.X, Pos.X + Viewport.Width,
        Pos.Y + Viewport.Height, Pos.Y,
        1.0f, -1.0f));

    m_pConstants->Get<::Matrices>().View = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixIdentity());

    ::DirectX::XMVECTOR Determinant;
    m_pConstants->Get<::Matrices>().ViewInverse = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixInverse(&Determinant, ::DirectX::XMMatrixIdentity()));
  };

  const Render_t CameraPerspective = [=](void)
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

    m_pConstants->Get<::Matrices>().Projection = ::DirectX::XMMatrixTranspose(
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

    const auto View = ::DirectX::XMMatrixLookAtRH(Eye, Look,
      ::DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));

    m_pConstants->Get<::Matrices>().View = ::DirectX::XMMatrixTranspose(View);

    ::DirectX::XMVECTOR Determinant;
    m_pConstants->Get<::Matrices>().ViewInverse = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixInverse(&Determinant, View));
  };

  return (_pComponent->Kind == uT("Perspective")) ? 
    CameraPerspective : CameraOptographic;
}

auto DirectX11::CreateState(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
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
    return GetDepthState(
      _pComponent->GetValue(uT("enabled"), false),
      _pComponent->GetValue(uT("clear"), false),
      _pComponent->GetValue(uT("overwrite"), true));
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

  const auto CreateAlphaTestState = [&](void)
  {
    return nullptr;
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Blend"), [&](void) { return CreateBlendState(true); } },
    { uT("Sampler"),    CreateSamplerState   },
    { uT("Scissor"),    CreateScissorState   },
    { uT("Depth"),      CreateDepthState     },
    { uT("Clear"),      CreateClearState     },
    { uT("AlphaTest"),  CreateAlphaTestState },
  };

  return Creators[_pComponent->Kind]();
}

auto DirectX11::CreateFog(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  return DoCreateFog<::Fog>(_pComponent);
}

auto DirectX11::CreateMaterial(const ComponentPtr_t &) -> Render_t /*override*/
{
  return nullptr;
}

auto DirectX11::CreateLight(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  return DoCreateLight<::Lights>(_pComponent);
}

auto DirectX11::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto GetDestinationIndex = [&](const String_t & _Destination)
  {
    static const ::std::vector<String_t> Destinations =
    {
      uT("albedo"),
      uT("metalness"),
      uT("roughness"),
      uT("normal"),
      uT("occlusion"),
    };

    const auto itDestination = 
      ::std::find(Destinations.cbegin(), Destinations.cend(), _Destination);
    if (itDestination == Destinations.cend())
    {
      throw STD_EXCEPTION << "Unexpected destination texture: " << _Destination
        << " [id=" << _pComponent->Id << "].";
    }

    return static_cast<UINT>(
      ::std::distance(Destinations.cbegin(), itDestination));
  };

  const auto pTextureData =
    m_ServiceComponents.Get({ { uT("Texture"), _pComponent } })[0];

  const auto iDestination = GetDestinationIndex(
    pTextureData->GetValue(uT("destination"), uT("albedo")));

  const Component::Texture TextureData{ pTextureData };

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
    m_pImmediateContext->PSSetShaderResources(iDestination, 1,
      pShaderResourceView.GetAddressOf());
  };
}

auto DirectX11::CreateShader(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using namespace ::alicorn::extension::std;

  const auto pShaderDataComponent =
    m_ServiceComponents.Get({ { uT("Shader"), _pComponent } })[0];

  const auto GetShaderData = [&](void)
  {
    try
    {
      return Component::Shader{ pShaderDataComponent, ::DefaultDeprecated };
    }
    catch (const ::std::exception &) {}

    return Component::Shader{ pShaderDataComponent, ::Default };
  };

  const auto ShaderData = GetShaderData();

  ::std::string Define =
    "#define COVELLITE_SHADER_DESKTOP\r\n"
    "#define COVELLITE_SHADER_HLSL\r\n"
    "#define COVELLITE_SHADER_VERTEX\r\n";
  auto ShaderText = ::Predefined + ::Data + ::Input + ::std::vector<uint8_t>{
    ShaderData.pData, ShaderData.pData + ShaderData.Count };
  ::std::string Entry = ShaderData.Entry;

  if (ShaderData.Kind == uT("Pixel"))
  {
    Define =
      "#define COVELLITE_SHADER_DESKTOP\r\n"
      "#define COVELLITE_SHADER_HLSL\r\n"
      "#define COVELLITE_SHADER_PIXEL\r\n";

    const auto PixelMain =
      "float4 psMain(Pixel _Value) : SV_Target\r\n"
      "{\r\n"
      "  return " + Entry + "(_Value);\r\n"
      "}\r\n";

    ShaderText += DirectX::Shader::Convert(PixelMain);
    Entry = "psMain";
  }

  const auto pCompiledShader = 
    DirectX::Shader::Compile(DirectX::Shader::Convert(Define) + ShaderText,
      Entry.c_str(), DirectX::Shader::GetVersion(ShaderData.Kind).c_str());

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
  else if (ShaderData.Kind == uT("Vertex"))
  {
    return VertexShader(
      {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      });
  }

  ComPtr_t<ID3D11PixelShader> pPixelShader;
  DX_CHECK m_pDevice->CreatePixelShader(
    pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
    NULL, &pPixelShader);

  return [=](void)
  {
    m_pImmediateContext->PSSetShader(pPixelShader.Get(), NULL, 0);
    m_pConstants->Update<::Lights>();
    m_pConstants->Update<::Fog>();
  };
}

#pragma warning(push)
#pragma warning(disable: 4996)

auto DirectX11::CreateBuffer(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  namespace vertex = ::covellite::api::vertex;

  const auto pBufferData =
    m_ServiceComponents.Get({ { uT("Buffer"), _pComponent } })[0];

  if (pBufferData->IsType<const vertex::Polygon *>(uT("data")))
  {
    const Component::Buffer<vertex::Polygon> VertexData{ pBufferData };

    auto pBuffer = 
      Buffer::Create(m_pDevice, false, VertexData.pData, VertexData.Count);

    return [=](void)
    {
      static const UINT Stride = sizeof(vertex::Polygon);
      static const UINT Offset = 0;
      m_pImmediateContext->IASetVertexBuffers(0, 1,
        pBuffer.GetAddressOf(), &Stride, &Offset);
    };
  }
  else if (pBufferData->IsType<const vertex::Polyhedron *>(uT("data")))
  {
    using BufferMapper_t = cbBufferMap_t<vertex::Polyhedron>;

    const Component::Buffer<vertex::Polyhedron> VertexData{ pBufferData };

    const auto & cbBufferMapper =
      _pComponent->GetValue<const BufferMapper_t &>(uT("mapper"), nullptr);

    auto pBuffer = Buffer::Create(m_pDevice, (cbBufferMapper != nullptr), 
      VertexData.pData, VertexData.Count);

    const Render_t StaticRender = [=](void)
    {
      static const UINT Stride = sizeof(vertex::Polyhedron);
      static const UINT Offset = 0;
      m_pImmediateContext->IASetVertexBuffers(0, 1,
        pBuffer.GetAddressOf(), &Stride, &Offset);
    };

    const Render_t DynamicRender = [=](void)
    {
      const auto IsDirty = cbBufferMapper(nullptr);
      if (IsDirty)
      {
        D3D11_MAPPED_SUBRESOURCE Resource = { 0 };
        DX_CHECK m_pImmediateContext->Map(pBuffer.Get(), 0,
          D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Resource);
        cbBufferMapper(reinterpret_cast<vertex::Polyhedron *>(Resource.pData));
        m_pImmediateContext->Unmap(pBuffer.Get(), 0);
      }

      StaticRender();
    };

    return (cbBufferMapper == nullptr) ? StaticRender : DynamicRender;
  }
  else if (pBufferData->IsType<const ::covellite::api::Vertex *>(uT("data")))
  {
    using BufferMapper_t = cbBufferMap_t<::covellite::api::Vertex>;

    const Component::Buffer<::covellite::api::Vertex> VertexData{ pBufferData };

    const auto & cbBufferMapper =
      _pComponent->GetValue<const BufferMapper_t &>(uT("mapper"), nullptr);

    auto pBuffer = Buffer::Create(m_pDevice, (cbBufferMapper != nullptr),
      VertexData.pData, VertexData.Count);

    const Render_t StaticRender = [=](void)
    {
      static const UINT Stride = sizeof(::covellite::api::Vertex);
      static const UINT Offset = 0;
      m_pImmediateContext->IASetVertexBuffers(0, 1,
        pBuffer.GetAddressOf(), &Stride, &Offset);
    };

    const Render_t DynamicRender = [=](void)
    {
      const auto IsDirty = cbBufferMapper(nullptr);
      if (IsDirty)
      {
        D3D11_MAPPED_SUBRESOURCE Resource = { 0 };
        DX_CHECK m_pImmediateContext->Map(pBuffer.Get(), 0,
          D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Resource);
        cbBufferMapper(reinterpret_cast<::covellite::api::Vertex *>(Resource.pData));
        m_pImmediateContext->Unmap(pBuffer.Get(), 0);
      }

      StaticRender();
    };

    return (cbBufferMapper == nullptr) ? StaticRender : DynamicRender;
  }

  const Component::Buffer<int> IndexData{ pBufferData };

  auto pBuffer = 
    Buffer::Create(m_pDevice, false, IndexData.pData, IndexData.Count);

  return [=](void)
  {
    m_pImmediateContext->IASetIndexBuffer(
      pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  };
}

#pragma warning(pop)

auto DirectX11::CreateGeometry(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto GetPreRenderStaticGeometry = [&](void) -> Render_t
  {
    GetPreRenderGeometry(true)();
    const auto World = m_pConstants->Get<::Matrices>().World;

    return [=](void)
    {
      m_pConstants->Get<::Matrices>().World = World;
      m_pConstants->Update<::Matrices>();
    };
  };

  const auto Variety = _pComponent->GetValue(uT("variety"), uT("Default"));

  const auto PreRenderGeometry = 
    (Variety == uT("Default")) ? GetPreRenderGeometry(false) :
    (Variety == uT("Static")) ? GetPreRenderStaticGeometry() :
    (Variety == uT("Billboard")) ? GetPreRenderBillboardGeometry() :
      throw STD_EXCEPTION << "Unknown variety: " << Variety <<
        " [id=" << _pComponent->Id << "].";

  return [=](void)
  {
    PreRenderGeometry();

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

auto DirectX11::GetDepthState(
  const bool _IsEnabled, 
  const bool _IsClear, 
  const bool _IsOverwrite) -> Render_t
{
  if (!_IsEnabled)
  {
    return [=](void)
    {
      m_pImmediateContext->OMSetRenderTargets(1,
        m_pRenderTargetView.GetAddressOf(), nullptr);
    };
  }

  D3D11_DEPTH_STENCIL_DESC DeptStencilDesc = { 0 };
  DeptStencilDesc.DepthEnable = true;
  DeptStencilDesc.DepthWriteMask = _IsOverwrite ? 
    D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
  DeptStencilDesc.StencilEnable = false;

  ComPtr_t<ID3D11DepthStencilState> pDSState;
  DX_CHECK m_pDevice->CreateDepthStencilState(&DeptStencilDesc, &pDSState);

  Render_t RenderDepthEnabled = [=](void)
  {
    m_pImmediateContext->OMSetDepthStencilState(pDSState.Get(), 1);
    m_pImmediateContext->OMSetRenderTargets(1, 
      m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
  };

  Render_t RenderDepthClear = [=](void)
  {
    m_pImmediateContext->OMSetDepthStencilState(pDSState.Get(), 1);
    m_pImmediateContext->OMSetRenderTargets(1,
      m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(),
      D3D11_CLEAR_DEPTH, 1.0f, 0);
  };

  return _IsClear ? RenderDepthClear : RenderDepthEnabled;
}

auto DirectX11::GetPreRenderGeometry(const bool _IsStatic) -> Render_t
{
  Renders_t Result;

  Result.push_back([=](void)
  {
    m_pConstants->Get<::Matrices>().World = ::DirectX::XMMatrixIdentity();
  });

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    Result.push_back([=](void)
    {
      const Component::Position Position{ _pPosition };

      m_pConstants->Get<::Matrices>().World *=
        ::DirectX::XMMatrixTranslation(Position.X, Position.Y, Position.Z);
    });
  };

  auto CreateRotation = [&](const ComponentPtr_t & _pRotation)
  {
    Result.push_back([=](void)
    {
      const Component::Rotation Rotation{ _pRotation };

      m_pConstants->Get<::Matrices>().World *= ::DirectX::XMMatrixRotationX(Rotation.X);
      m_pConstants->Get<::Matrices>().World *= ::DirectX::XMMatrixRotationY(Rotation.Y);
      m_pConstants->Get<::Matrices>().World *= ::DirectX::XMMatrixRotationZ(Rotation.Z);
    });
  };

  auto CreateScale = [&](const ComponentPtr_t & _pScale)
  {
    Result.push_back([=](void)
    {
      const Component::Scale Scale{ _pScale };

      m_pConstants->Get<::Matrices>().World *=
        ::DirectX::XMMatrixScaling(Scale.X, Scale.Y, Scale.Z);
    });
  };

  m_ServiceComponents.Process(
    {
      { uT("Position"), CreatePosition },
      { uT("Rotation"), CreateRotation },
      { uT("Scale"), CreateScale },
    });

  Result.push_back([this](void)
  {
    m_pConstants->Get<::Matrices>().World =
      ::DirectX::XMMatrixTranspose(m_pConstants->Get<::Matrices>().World);
  });

  if (!_IsStatic)
  {
    Result.push_back([this](void)
    {
      m_pConstants->Update<::Matrices>();
    });
  }

  return [Result](void)
  {
    for (const auto & Render : Result) Render();
  };
}

auto DirectX11::GetPreRenderBillboardGeometry(void) -> Render_t
{
  Renders_t Result;

  Result.push_back([=](void)
  {
    ::DirectX::XMFLOAT4X4 Matrix;
    // Матрица View уже траспонированная!
    XMStoreFloat4x4(&Matrix, m_pConstants->Get<::Matrices>().View);

    Matrix._14 = 0.0f;
    Matrix._24 = 0.0f;
    Matrix._34 = 0.0f;

    // Уже!
    Matrix._41 = 0.0f;
    Matrix._42 = 0.0f;
    Matrix._43 = 0.0f;
    Matrix._44 = 1.0f;

    m_pConstants->Get<::Matrices>().World = XMLoadFloat4x4(&Matrix);
  });

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    Result.push_back([=](void)
    {
      const Component::Position Position{ _pPosition };

      m_pConstants->Get<::Matrices>().World *=
        ::DirectX::XMMatrixTranslation(Position.X, Position.Y, Position.Z);
    });
  };

  m_ServiceComponents.Process(
    {
      { uT("Position"), CreatePosition },
    });

  Result.push_back([this](void)
  {
    m_pConstants->Get<::Matrices>().World =
      ::DirectX::XMMatrixTranspose(m_pConstants->Get<::Matrices>().World);
    m_pConstants->Update<::Matrices>();
  });

  return [Result](void)
  {
    for (const auto & Render : Result) Render();
  };
}
