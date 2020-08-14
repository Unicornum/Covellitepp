
#include "stdafx.h"
#include "DirectX11.hpp"
#include <glm/glm.force.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/lexical-cast.hpp>
#include "DxCheck.hpp"
#include "DirectX.hpp"
#include <Covellite/Api/Vertex.hpp>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "Shaders/Shaders.hpp"
#include <Covellite/Api/Defines.hpp>
#include "Component.hpp"
#include "GraphicApi.Constants.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

// ************************************************************************** //

class DirectX11::Buffer final
{
public:
  template<class T>
  class Support
  {
  public:
    static constexpr UINT Flag = D3D11_BIND_VERTEX_BUFFER;
  };

  template<UINT TIndex>
  class Constant
  {
  public:
    static constexpr UINT Flag = D3D11_BIND_CONSTANT_BUFFER;
    static constexpr UINT Index = TIndex;
  };

  template<>
  class Support<::Camera> : public Constant<COVELLITE_BUFFER_INDEX_CAMERA> { };

  template<>
  class Support<::Fog> : public Constant<COVELLITE_BUFFER_INDEX_FOG> { };

  template<>
  class Support<::Object> : public Constant<COVELLITE_BUFFER_INDEX_OBJECT> { };

  template<>
  class Support<uint8_t> : public Constant<COVELLITE_BUFFER_INDEX_USER> { };

  template<>
  class Support<::Matrices> : public Constant<COVELLITE_BUFFER_INDEX_MATRICES> { };

  template<>
  class Support<::SceneLights> : public Constant<COVELLITE_BUFFER_INDEX_LIGHTS> { };

  template<>
  class Support<int>
  {
  public:
    static constexpr UINT Flag = D3D11_BIND_INDEX_BUFFER;
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
    Desc.CPUAccessFlags = static_cast<UINT>(_IsDynamic ? D3D11_CPU_ACCESS_WRITE : 0);
    Desc.ByteWidth = static_cast<decltype(Desc.ByteWidth)>(sizeof(T) * _Count);
    Desc.BindFlags = Support<T>::Flag;

    D3D11_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pData;

    const auto * const pInitData = (_pData == nullptr) ? nullptr : &InitData;

    ComPtr_t<ID3D11Buffer> pBuffer;
    DX_CHECK _pDevice->CreateBuffer(&Desc, pInitData, &pBuffer);
    return pBuffer;
  }

  template<class T>
  static ComPtr_t<ID3D11Buffer> Create(
    const ComPtr_t<ID3D11Device> & _pDevice, 
    const ComPtr_t<ID3D11DeviceContext> & _pImmediateContext,
    const T * _pData)
  {
    const auto pResult = Create(_pDevice, false, _pData, 1);

    constexpr auto Index = Support<T>::Index;
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
  m_IsResizeWindow = true;
  SetRenderTargetSize(static_cast<UINT>(_Width), static_cast<UINT>(_Height));
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

  SetRenderTargetSize(static_cast<UINT>(_Data.Width), static_cast<UINT>(_Data.Height));
}

void DirectX11::SetRenderTargetSize(const UINT _Width, const UINT _Height)
{
  CreateRenderTargetView(_Width, _Height);
  CreateDepthStencilView(_Width, _Height);
}

void DirectX11::CreateRenderTargetView(const UINT _Width, const UINT _Height)
{
  if (m_pScreenRenderTargetView != nullptr)
  {
    m_pScreenRenderTargetView.Reset();

    DX_CHECK m_pSwapChain->ResizeBuffers(2, _Width, _Height,
      DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
  }

  ComPtr_t<ID3D11Texture2D> pBackBuffer;
  DX_CHECK m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
    (void **)pBackBuffer.GetAddressOf());

  DX_CHECK m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL,
    &m_pScreenRenderTargetView);
}

void DirectX11::CreateDepthStencilView(const UINT _Width, const UINT _Height)
{
  m_pScreenDepthStencilView.Reset();

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
    &DeptStencilViewDesc, &m_pScreenDepthStencilView);
}

auto DirectX11::CreateCamera(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto CameraId = _pComponent->Id;

  const auto SetDefaultRenderTarget = [=](void)
  {
    m_CurrentRenderTargets = { m_pScreenRenderTargetView.Get() };
    m_pCurrentDepthStencilView = m_pScreenDepthStencilView;

    DXGI_SWAP_CHAIN_DESC Desc = { 0 };
    DX_CHECK m_pSwapChain->GetDesc(&Desc);

    D3D11_VIEWPORT ViewPort = { 0 };
    ViewPort.TopLeftX = 0;
    ViewPort.TopLeftY = 0;
    ViewPort.Width = static_cast<FLOAT>(Desc.BufferDesc.Width);
    ViewPort.Height = static_cast<FLOAT>(Desc.BufferDesc.Height);
    ViewPort.MinDepth = 0.0f;
    ViewPort.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &ViewPort);
  };

  const auto DisabledBlendRender = CreateBlendState(false);
  const auto DisableDepthRender = GetDepthState(false, false, false);

  const auto LightsRender = [=](void)
  {
    m_pConstants->SetCameraId(CameraId);
  };

  const auto ServiceComponents = CapturingServiceComponent::Get(_pComponent,
    {
      { uT("Position"), api::Component::Make({}) },
      { uT("Rotation"), api::Component::Make({}) },
    });

  const Render_t CameraOptographic = [=](void)
  {
    const Component::Position Pos{ *ServiceComponents[0] };

    SetDefaultRenderTarget();
    DisabledBlendRender();
    DisableDepthRender();
    LightsRender();

    UINT ViewportCount = 1;
    D3D11_VIEWPORT Viewport = { 0 };
    m_pImmediateContext->RSGetViewports(&ViewportCount, &Viewport);

    auto & CameraMatrices = m_pConstants->Get<::Camera>();

    CameraMatrices.Projection = ::glm::transpose(::glm::ortho(
      Pos.X, Pos.X + Viewport.Width,
      Pos.Y + Viewport.Height, Pos.Y,
      1.0f, -1.0f));

    const auto View = ::glm::identity<::glm::mat4>();
    CameraMatrices.View = ::glm::transpose(View);
    CameraMatrices.ViewInverse = ::glm::transpose(::glm::inverse(View));
    m_pConstants->Update<::Camera>();

    m_pConstants->Get<::Matrices>().Projection = CameraMatrices.Projection;
    m_pConstants->Get<::Matrices>().View = CameraMatrices.View;
    m_pConstants->Get<::Matrices>().ViewInverse = CameraMatrices.ViewInverse;

    (*_pComponent)[uT("view")] = CameraMatrices.View;
    (*_pComponent)[uT("projection")] = CameraMatrices.Projection;
  };

  const Render_t CameraPerspective = [=](void)
  {
    SetDefaultRenderTarget();
    DisabledBlendRender();
    DisableDepthRender();
    LightsRender();

    UINT ViewportCount = 1;
    D3D11_VIEWPORT Viewport = { 0 };
    m_pImmediateContext->RSGetViewports(&ViewportCount, &Viewport);

    auto & CameraMatrices = m_pConstants->Get<::Camera>();

    // ************************** Матрица проекции ************************** //

    const auto AngleY = (float)(*_pComponent)[uT("fov")].Default(90.0f) *
      ::alicorn::extension::cpp::math::Constant<float>::DegreeToRadian;
    const float zNear = (*_pComponent)[uT("znear")].Default(0.01f);
    const float zFar = (*_pComponent)[uT("zfar")].Default(200.0f);

    CameraMatrices.Projection = ::glm::transpose(::glm::perspectiveFovRH(AngleY,
      (float)Viewport.Width, (float)Viewport.Height, zFar, zNear));

    // **************************** Матрица вида **************************** //

    // Точка, куда смотрит камера - задается как компонент Data.Position.
    const Component::Position Look{ *ServiceComponents[0] };

    const auto GetEye = [&](void) -> ::glm::vec3
    {
      // Расстояние от камеры до Look.
      const float Distance = (*_pComponent)[uT("distance")].Default(0.0f);

      // Точка, где расположена камера - вычисляется на основе Look, Distance и
      // компонента Data.Rotation.

      const Component::Position Rot{ *ServiceComponents[1] };

      ::glm::mat4 Transform = ::glm::identity<::glm::mat4>();

      Transform = ::glm::translate(Transform,
        ::glm::vec3{ Look.X, Look.Y, Look.Z });
      Transform = ::glm::rotate(Transform,
        Rot.Z, ::glm::vec3{ 0.0f, 0.0f, 1.0f });
      Transform = ::glm::rotate(Transform,
        Rot.Y, ::glm::vec3{ 0.0f, 1.0f, 0.0f });
      Transform = ::glm::rotate(Transform,
        Rot.X, ::glm::vec3{ 1.0f, 0.0f, 0.0f });

      return Transform * ::glm::vec4{ Distance + 0.1f, 0.0f, 0.0f, 1.0f };
    };

    const auto View = ::glm::lookAtRH(
      GetEye(),
      ::glm::vec3{ Look.X, Look.Y, Look.Z },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f }); // Up

    CameraMatrices.View = ::glm::transpose(View);
    CameraMatrices.ViewInverse = ::glm::transpose(::glm::inverse(View));
    m_pConstants->Update<::Camera>();

    (*_pComponent)[uT("view")] = CameraMatrices.View;
    (*_pComponent)[uT("projection")] = CameraMatrices.Projection;

    m_pConstants->Get<::Matrices>().Projection = CameraMatrices.Projection;
    m_pConstants->Get<::Matrices>().View = CameraMatrices.View;
    m_pConstants->Get<::Matrices>().ViewInverse = CameraMatrices.ViewInverse;
  };

  return (_pComponent->Kind == uT("Perspective")) ? 
    CameraPerspective : CameraOptographic;
}

class DirectX11::Texture final
{
public:
  using Ptr_t = ::std::shared_ptr<Texture>;
  using ComponentWeakPtr_t = ::std::weak_ptr<::covellite::api::Component>;

public:
  const ComponentWeakPtr_t  m_pDataTexture;
  const String_t            m_Destination;
  const UINT                m_iDestination;
  const DXGI_FORMAT         m_Format;
  ComPtr_t<ID3D11Texture2D>          m_pTexture;
  ComPtr_t<ID3D11Texture2D>          m_pReadDataTexture;
  ComPtr_t<ID3D11RenderTargetView>   m_pRenderTargetView;
  ComPtr_t<ID3D11DepthStencilView>   m_pDepthStencilView;
  ComPtr_t<ID3D11ShaderResourceView> m_pShaderResourceView;

public:
  void MakeTarget(
    const ComPtr_t<ID3D11Device> & _pDevice,
    const UINT _Width,
    const UINT _Height)
  {
    m_pTexture = (m_Destination != uT("depth")) ?
      MakeRGBATarget(_pDevice, _Width, _Height) :
      MakeDepthTarget(_pDevice, _Width, _Height);

    const auto pDataTexture = m_pDataTexture.lock();
    if (pDataTexture)
    {
      (*pDataTexture)[uT("width")] = static_cast<int>(_Width);
      (*pDataTexture)[uT("height")] = static_cast<int>(_Height);

      if ((*pDataTexture)[uT("mapper")].IsType<const cbBufferMap_t<const void> &>())
      {
        m_pReadDataTexture = MakeRGBACopy(_pDevice, _Width, _Height);
      }
    }
  }

  void MakeSource(
    const ComPtr_t<ID3D11Device> & _pDevice,
    const ComPtr_t<ID3D11DeviceContext> & _pImmediateContext,
    const UINT _Width,
    const UINT _Height,
    const void * _pData,
    const bool _IsMipmapping)
  {
    m_pTexture = (_IsMipmapping) ?
      MakeRGBAMipSource(_pDevice, _pImmediateContext, _Width, _Height, _pData) :
      MakeRGBASource(_pDevice, _pImmediateContext, _Width, _Height, _pData);
  }

  ComPtr_t<ID3D11Texture2D> MakeRGBACopy(
    const ComPtr_t<ID3D11Device> & _pDevice,
    const UINT _Width, const UINT _Height) const
  {
    D3D11_TEXTURE2D_DESC textureDesc = { 0 };
    textureDesc.Width = _Width;
    textureDesc.Height = _Height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = m_Format;
    textureDesc.Usage = D3D11_USAGE_STAGING;
    textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    textureDesc.SampleDesc.Count = 1;

    ComPtr_t<ID3D11Texture2D> pTexture;
    DX_CHECK _pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
    return pTexture;
  }

private:
  ComPtr_t<ID3D11Texture2D> MakeRGBASource(
    const ComPtr_t<ID3D11Device> & _pDevice,
    const ComPtr_t<ID3D11DeviceContext> & _pImmediateContext,
    const UINT _Width,
    const UINT _Height,
    const void * _pData)
  {
    D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
    TextureDesc.Width = _Width;
    TextureDesc.Height = _Height;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = m_Format;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.MiscFlags = 0;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;

    ComPtr_t<ID3D11Texture2D> pTexture2D;
    DX_CHECK _pDevice->CreateTexture2D(&TextureDesc, nullptr, &pTexture2D);

    _pImmediateContext->UpdateSubresource(pTexture2D.Get(), 0, NULL,
      _pData, _Width * 4, 0);

    D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
    SrvDesc.Format = TextureDesc.Format;
    SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SrvDesc.Texture2D.MipLevels = TextureDesc.MipLevels;

    DX_CHECK _pDevice->CreateShaderResourceView(pTexture2D.Get(), &SrvDesc,
      &m_pShaderResourceView);

    return pTexture2D;
  }

  ComPtr_t<ID3D11Texture2D> MakeRGBAMipSource(
    const ComPtr_t<ID3D11Device> & _pDevice,
    const ComPtr_t<ID3D11DeviceContext> & _pImmediateContext,
    const UINT _Width,
    const UINT _Height,
    const void * _pData)
  {
    D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
    TextureDesc.Width = _Width;
    TextureDesc.Height = _Height;
    TextureDesc.MipLevels = 0;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = m_Format;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;

    ComPtr_t<ID3D11Texture2D> pTexture2D;
    DX_CHECK _pDevice->CreateTexture2D(&TextureDesc, nullptr, &pTexture2D);

    _pImmediateContext->UpdateSubresource(pTexture2D.Get(), 0, NULL,
      _pData, _Width * 4, 0);

    D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
    SrvDesc.Format = TextureDesc.Format;
    SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SrvDesc.Texture2D.MipLevels = static_cast<UINT>(-1);

    DX_CHECK _pDevice->CreateShaderResourceView(pTexture2D.Get(), &SrvDesc,
      &m_pShaderResourceView);

    _pImmediateContext->GenerateMips(m_pShaderResourceView.Get());

    return pTexture2D;
  }

private:
  ComPtr_t<ID3D11Texture2D> MakeRGBATarget(
    const ComPtr_t<ID3D11Device> & _pDevice,
    const UINT _Width, const UINT _Height)
  {
    D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
    TextureDesc.Width = _Width;
    TextureDesc.Height = _Height;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = m_Format;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.MiscFlags = 0;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;

    ComPtr_t<ID3D11Texture2D> pTexture2D;
    DX_CHECK _pDevice->CreateTexture2D(&TextureDesc, nullptr, &pTexture2D);

    D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
    SrvDesc.Format = TextureDesc.Format;
    SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SrvDesc.Texture2D.MipLevels = TextureDesc.MipLevels;

    DX_CHECK _pDevice->CreateShaderResourceView(pTexture2D.Get(), &SrvDesc,
      &m_pShaderResourceView);

    D3D11_RENDER_TARGET_VIEW_DESC TargetDesc = { 0 };
    TargetDesc.Format = TextureDesc.Format;
    TargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    ComPtr_t<ID3D11RenderTargetView> pRenderTargetView;
    DX_CHECK _pDevice->CreateRenderTargetView(pTexture2D.Get(),
      &TargetDesc, &m_pRenderTargetView);

    return pTexture2D;
  }

  ComPtr_t<ID3D11Texture2D> MakeDepthTarget(
    const ComPtr_t<ID3D11Device> & _pDevice,
    const UINT _Width, const UINT _Height)
  {
    D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
    TextureDesc.Width = _Width;
    TextureDesc.Height = _Height;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.MiscFlags = 0;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;

    ComPtr_t<ID3D11Texture2D> pTexture2D;
    DX_CHECK _pDevice->CreateTexture2D(&TextureDesc, nullptr, &pTexture2D);

    D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
    SrvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SrvDesc.Texture2D.MipLevels = TextureDesc.MipLevels;

    DX_CHECK _pDevice->CreateShaderResourceView(pTexture2D.Get(), &SrvDesc,
      &m_pShaderResourceView);

    D3D11_DEPTH_STENCIL_VIEW_DESC Desc = { 0 };
    Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    Desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    ComPtr_t<ID3D11DepthStencilView> pDepthStencilView;
    DX_CHECK _pDevice->CreateDepthStencilView(pTexture2D.Get(),
      &Desc, &m_pDepthStencilView);

    return pTexture2D;
  }

private:
  static UINT GetDestinationIndex(const ComponentPtr_t & _pData)
  {
    const int Index = (*_pData)[uT("index")].Default(-1);
    if (Index >= 0) return static_cast<UINT>(Index);

    static const ::std::vector<String_t> Destinations =
    {
      uT("albedo"),
      uT("metalness"),
      uT("roughness"),
      uT("normal"),
      uT("occlusion"),
      uT("depth"),
    };

    const String_t Destination =
      (*_pData)[uT("destination")].Default(uT("albedo"));

    const auto itDestination =
      ::std::find(Destinations.cbegin(), Destinations.cend(), Destination);
    if (itDestination == Destinations.cend())
    {
      throw STD_EXCEPTION << "Unexpected destination texture: " << Destination;
    }

    return static_cast<UINT>(
      ::std::distance(Destinations.cbegin(), itDestination));
  };

  static DXGI_FORMAT GetFormat(const ComponentPtr_t & _pData)
  {
    const int Capacity = (*_pData)[uT("capacity")].Default(8);

    return
      (Capacity == 32) ? DXGI_FORMAT_R32G32B32A32_FLOAT :
      (Capacity == 16) ? DXGI_FORMAT_R16G16B16A16_FLOAT :
      DXGI_FORMAT_R8G8B8A8_UNORM;
  }

public:
  explicit Texture(const ComponentPtr_t & _pDataTexture) :
    m_pDataTexture{ _pDataTexture },
    m_Destination{ (*_pDataTexture)[uT("destination")].Default(uT("albedo")) },
    m_iDestination{ GetDestinationIndex(_pDataTexture) },
    m_Format{ GetFormat(_pDataTexture) }
  {

  }
  Texture(const Texture &) = delete;
  Texture(Texture &&) = delete;
  Texture & operator= (const Texture &) = delete;
  Texture & operator= (Texture &&) = delete;
  ~Texture(void) = default;
};

auto DirectX11::CreateBkSurface(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using Size_t = ::std::tuple<UINT, UINT>;
  using fnBkSurfaceSize_t = ::std::function<Size_t(void)>;

  const auto pBkSurface = _pComponent;

  const fnBkSurfaceSize_t GetScaleBkSurfaceSize = [=](void)
  {
    DXGI_SWAP_CHAIN_DESC Desc = { 0 };
    DX_CHECK m_pSwapChain->GetDesc(&Desc);

    const float Scale = (*pBkSurface)[uT("scale")];

    const int Width = static_cast<int>(Scale * Desc.BufferDesc.Width);
    const int Height = static_cast<int>(Scale * Desc.BufferDesc.Height);

    (*pBkSurface)[uT("width")] = Width;
    (*pBkSurface)[uT("height")] = Height;

    return Size_t{ static_cast<UINT>(Width), static_cast<UINT>(Height) };
  };

  const fnBkSurfaceSize_t GetWindowBkSurfaceSize = [=](void)
  {
    DXGI_SWAP_CHAIN_DESC Desc = { 0 };
    DX_CHECK m_pSwapChain->GetDesc(&Desc);

    const int Width = static_cast<int>(Desc.BufferDesc.Width);
    const int Height = static_cast<int>(Desc.BufferDesc.Height);

    (*pBkSurface)[uT("width")] = Width;
    (*pBkSurface)[uT("height")] = Height;

    return Size_t{ static_cast<UINT>(Width), static_cast<UINT>(Height) };
  };

  const fnBkSurfaceSize_t GetUserBkSurfaceSize = [=](void)
  {
    DXGI_SWAP_CHAIN_DESC Desc = { 0 };
    DX_CHECK m_pSwapChain->GetDesc(&Desc); // результат не используется, но 
                                           // так проще тестировать

    const int Width = (*pBkSurface)[uT("width")];
    const int Height = (*pBkSurface)[uT("height")];

    return Size_t{ static_cast<UINT>(Width), static_cast<UINT>(Height) };
  };

  const auto IsScaleBkSurfaceSize =
    (*pBkSurface)[uT("scale")].IsType<float>();
  const auto IsUserBkSurfaceSize =
    (*pBkSurface)[uT("width")].IsType<int>() &&
    (*pBkSurface)[uT("height")].IsType<int>();

  const auto GetBkSurfaceSize =
    (IsScaleBkSurfaceSize) ? GetScaleBkSurfaceSize :
    (IsUserBkSurfaceSize) ? GetUserBkSurfaceSize : 
    GetWindowBkSurfaceSize;

  const auto [Width, Height] = GetBkSurfaceSize();

  const auto pBkSurfaceTextures =
    ::std::make_shared<::std::vector<Texture::Ptr_t>>();

  const auto DoDataTexture = [&](const ComponentPtr_t & _pDataTexture)
  {
    auto pTexture = ::std::make_shared<Texture>(_pDataTexture);
    pTexture->MakeTarget(m_pDevice, Width, Height);
    (*_pDataTexture)[uT("entity")] = pTexture;
    pBkSurfaceTextures->push_back(pTexture);
  };

  CapturingServiceComponent::Process(_pComponent,
    {
      { uT("Texture"), DoDataTexture },
    });

  return [=](void)
  {
    const auto [Width, Height] = GetBkSurfaceSize();

    if (m_IsResizeWindow)
    {
      for (auto & pTexture : *pBkSurfaceTextures)
      {
        pTexture->MakeTarget(m_pDevice, Width, Height);
      }
    }

    m_CurrentRenderTargets.clear();

    for (auto & pTexture : *pBkSurfaceTextures)
    {
      if (pTexture->m_pRenderTargetView)
      {
        m_CurrentRenderTargets.push_back(pTexture->m_pRenderTargetView.Get());
      }

      if (pTexture->m_pDepthStencilView)
      {
        m_pCurrentDepthStencilView = pTexture->m_pDepthStencilView;
      }

      static ID3D11ShaderResourceView * NullResourceView[1] = { nullptr };
      m_pImmediateContext->PSSetShaderResources(
        pTexture->m_iDestination, 1, NullResourceView);
    }

    if (m_CurrentRenderTargets.empty())
    {
      m_CurrentRenderTargets = { nullptr };
    }

    m_pImmediateContext->OMSetRenderTargets(
      static_cast<UINT>(m_CurrentRenderTargets.size()),
      &m_CurrentRenderTargets[0], nullptr);

    D3D11_VIEWPORT ViewPort = { 0 };
    ViewPort.TopLeftX = 0;
    ViewPort.TopLeftY = 0;
    ViewPort.Width = static_cast<FLOAT>(Width);
    ViewPort.Height = static_cast<FLOAT>(Height);
    ViewPort.MinDepth = 0.0f;
    ViewPort.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &ViewPort);
  };
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
    const Component::Scissor ScissorData{ *_pComponent };

    D3D11_RASTERIZER_DESC rasterDesc = { 0 };
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = TRUE;
    rasterDesc.ScissorEnable = (ScissorData.IsEnabled) ? TRUE : FALSE;

    ComPtr_t<ID3D11RasterizerState> pScissor;
    DX_CHECK m_pDevice->CreateRasterizerState(&rasterDesc, &pScissor);

    const auto pScissorRect = CapturingServiceComponent::Get(_pComponent,
      { {uT("Rect"), _pComponent} })[0];

    Render_t ScissorEnabled = [=](void)
    {
      const Component::Scissor ScissorRect{ *pScissorRect };

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
      (*_pComponent)[uT("enabled")].Default(false),
      (*_pComponent)[uT("clear")].Default(false),
      (*_pComponent)[uT("overwrite")].Default(true));
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
      ARGBtoFloat4((*_pComponent)[uT("color")].Default(0xFF000000));

    return [=](void)
    {
      for (auto * pRenderTargetView : m_CurrentRenderTargets)
      {
        m_pImmediateContext->ClearRenderTargetView(
          pRenderTargetView, BackgroundColor.data());
      }
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
  return DoCreateFog<::Fog>(_pComponent, true);
}

auto DirectX11::CreateLight(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  return DoCreateLight<::SceneLights>(_pComponent);
}

auto DirectX11::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using BufferMapper_t = cbBufferMap_t<const void>;

  const auto pTextureData = CapturingServiceComponent::Get(_pComponent, 
    { { uT("Texture"), _pComponent } })[0];

  const Component::Texture TextureData{ *pTextureData, uT("albedo") };

  Texture::Ptr_t pTexture = (*pTextureData)[uT("entity")].Default(Texture::Ptr_t{});
  if (pTexture == nullptr)
  {
    pTexture = ::std::make_shared<Texture>(pTextureData);
    pTexture->MakeSource(m_pDevice, m_pImmediateContext, 
      static_cast<UINT>(TextureData.Width), static_cast<UINT>(TextureData.Height), 
      TextureData.Data.data(), TextureData.IsUsingMipmapping);
  }
  else
  {
    (*pTextureData)[uT("entity")] = Texture::Ptr_t{};
  }

  if (pTexture->m_pReadDataTexture == nullptr)
  {
    return [=](void)
    {
      m_pImmediateContext->PSSetShaderResources(pTexture->m_iDestination, 1,
        pTexture->m_pShaderResourceView.GetAddressOf());
    };
  }

  const BufferMapper_t cbBufferMapper = 
    (*pTextureData)[uT("mapper")].Default(BufferMapper_t{});

  return [=](void)
  {
    if (cbBufferMapper(nullptr))
    {
      m_pImmediateContext->CopyResource(
        pTexture->m_pReadDataTexture.Get(), pTexture->m_pTexture.Get());

      D3D11_MAPPED_SUBRESOURCE Resource = { 0 };
      DX_CHECK m_pImmediateContext->Map(pTexture->m_pReadDataTexture.Get(), 0,
        D3D11_MAP_READ, 0, &Resource);
      cbBufferMapper(Resource.pData);
      m_pImmediateContext->Unmap(pTexture->m_pReadDataTexture.Get(), 0);
    }

    m_pImmediateContext->PSSetShaderResources(pTexture->m_iDestination, 1,
      pTexture->m_pShaderResourceView.GetAddressOf());
  };
}

auto DirectX11::CreateShader(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using namespace ::alicorn::extension::std;

  const auto pShaderDataComponent = CapturingServiceComponent::Get(_pComponent, 
    { { uT("Shader"), _pComponent } })[0];

  const auto GetShaderData = [&](void)
  {
    //try
    //{
    //  return Component::Shader{ *pShaderDataComponent, ::DefaultDeprecated };
    //}
    //catch (const ::std::exception &) {}

    return Component::Shader{ *pShaderDataComponent, ::Default };
  };

  const auto ShaderData = GetShaderData();

  ::std::string Define =
    "#define COVELLITE_SHADER_DESKTOP\r\n"
    "#define COVELLITE_SHADER_HLSL\r\n"
    "#define COVELLITE_SHADER_VERTEX\r\n";
  const auto HeaderShaderText = ::Predefined + ::Data +
    ShaderData.GetInstanceInput(::Input);
  auto ShaderText = ShaderData.Data;
  ::std::string Entry = ShaderData.Entry;

  if (ShaderData.Kind == uT("Pixel"))
  {
    Define =
      "#define COVELLITE_SHADER_DESKTOP\r\n"
      "#define COVELLITE_SHADER_HLSL\r\n"
      "#define COVELLITE_SHADER_PIXEL\r\n";

    if (ShaderData.ReturnType == "float4" || 
      ShaderData.ReturnType == "vec4")
    {
      ShaderText += DirectX::Shader::Convert(
        "float4 psMain(Pixel _Value) : SV_Target\r\n"
        "{\r\n"
        "  return " + Entry + "(_Value);\r\n"
        "}\r\n");
      Entry = "psMain";
    }
  }

  const auto pCompiledShader = DirectX::Shader::Compile(
    DirectX::Shader::Convert(Define) + HeaderShaderText, ShaderText, 
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
    ::std::vector<D3D11_INPUT_ELEMENT_DESC> LayoutDesc =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    for (UINT i = 0; i < static_cast<UINT>(ShaderData.Instance.size()); i++)
    {
      const auto Type = ShaderData.Instance[i];
      const auto Format =
        (Type == uT("f")) ? DXGI_FORMAT_R32G32B32A32_FLOAT :
        (Type == uT("i")) ? DXGI_FORMAT_R32G32B32A32_SINT :
        DXGI_FORMAT_UNKNOWN;
      const auto Size = i * 4 * static_cast<UINT>(
        (Type == uT("f")) ? sizeof(float) :
        (Type == uT("i")) ? sizeof(int) :
        0);

      LayoutDesc.push_back(
        {
          "TEXCOORD", i + 1, Format, 1, Size, D3D11_INPUT_PER_INSTANCE_DATA, 1
        });
    }

    return VertexShader(LayoutDesc);
  }

  ComPtr_t<ID3D11PixelShader> pPixelShader;
  DX_CHECK m_pDevice->CreatePixelShader(
    pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
    NULL, &pPixelShader);

  return [=](void)
  {
    m_pImmediateContext->PSSetShader(pPixelShader.Get(), NULL, 0);
    m_pConstants->Update<::SceneLights>();
  };
}

auto DirectX11::CreateBuffer(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  namespace vertex = ::covellite::api::vertex;

  const auto pBufferData = CapturingServiceComponent::Get(_pComponent, 
    { { uT("Buffer"), _pComponent } })[0];

  if ((*pBufferData)[uT("content")].IsType<::std::vector<vertex::Polygon>>())
  {
    const Component::Buffer<vertex::Polygon> VertexData{ *pBufferData };

    auto pBuffer = Buffer::Create(m_pDevice, false, 
      VertexData.Data.data(), VertexData.Data.size());

    return [=](void)
    {
      constexpr UINT Stride = sizeof(vertex::Polygon);
      constexpr UINT Offset = 0;
      m_pImmediateContext->IASetVertexBuffers(0, 1,
        pBuffer.GetAddressOf(), &Stride, &Offset);
    };
  }
  else if ((*pBufferData)[uT("content")].IsType<::std::vector<vertex::Polyhedron>>())
  {
    using BufferMapper_t = cbBufferMap_t<vertex::Polyhedron>;

    const Component::Buffer<vertex::Polyhedron> VertexData{ *pBufferData };

    const BufferMapper_t & cbBufferMapper =
      (*_pComponent)[uT("mapper")].Default(BufferMapper_t{});

    auto pBuffer = Buffer::Create(m_pDevice, (cbBufferMapper != nullptr), 
      VertexData.Data.data(), VertexData.Data.size());

    const Render_t StaticRender = [=](void)
    {
      constexpr UINT Stride = sizeof(vertex::Polyhedron);
      constexpr UINT Offset = 0;
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
        cbBufferMapper(static_cast<vertex::Polyhedron *>(Resource.pData));
        m_pImmediateContext->Unmap(pBuffer.Get(), 0);
      }

      StaticRender();
    };

    return (cbBufferMapper == nullptr) ? StaticRender : DynamicRender;
  }
  else if ((*pBufferData)[uT("content")].IsType<Buffer_t<::covellite::api::Vertex>>())
  {
    using BufferMapper_t = cbBufferMap_t<::covellite::api::Vertex>;

    const Component::Buffer<::covellite::api::Vertex> VertexData{ *pBufferData };

    const BufferMapper_t & cbBufferMapper =
      (*_pComponent)[uT("mapper")].Default(BufferMapper_t{});

    auto pBuffer = Buffer::Create(m_pDevice, (cbBufferMapper != nullptr),
      VertexData.Data.data(), VertexData.Data.size());

    const Render_t StaticRender = [=](void)
    {
      constexpr UINT Stride = sizeof(::covellite::api::Vertex);
      constexpr UINT Offset = 0;
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
        cbBufferMapper(static_cast<::covellite::api::Vertex *>(Resource.pData));
        m_pImmediateContext->Unmap(pBuffer.Get(), 0);
      }

      StaticRender();
    };

    return (cbBufferMapper == nullptr) ? StaticRender : DynamicRender;
  }
  else if ((*_pComponent)[uT("mapper")].IsType<const cbBufferMap_t<::Lights_t> &>())
  {
    using BufferMapper_t = cbBufferMap_t<::Lights_t>;

    const BufferMapper_t cbBufferMapper =
      (*_pComponent)[uT("mapper")].Default(BufferMapper_t{});
    if (!cbBufferMapper)
    {
      throw STD_EXCEPTION << "Unexpected empty mapper: " << _pComponent->Id;
    }

    return [=](void)
    {
      cbBufferMapper(&m_pConstants->Get<::Object>().Lights);
    };
  }
  else if ((*_pComponent)[uT("mapper")].IsType<const cbBufferMap_t<void> &>())
  {
    using BufferMap_t = cbBufferMap_t<void>;

    const BufferMap_t cbBufferMapper = 
      (*_pComponent)[uT("mapper")].Default(BufferMap_t{});
    if (!cbBufferMapper)
    {
      throw STD_EXCEPTION << "Unexpected empty mapper: " << _pComponent->Id;
    }

    const ::std::size_t BufferSize = 
      (*pBufferData)[uT("size")].Default((::std::size_t)0);
    if (BufferSize == 0)
    {
      throw STD_EXCEPTION << "Unexpected zero size: " << _pComponent->Id;
    }

    const auto pData = 
      ::std::make_shared<BinaryData_t>(BufferSize, (uint8_t)0x00);
    const auto pBuffer = Buffer::Create(
      m_pDevice, false, pData->data(), pData->size());
    constexpr auto BufferIndex = 
      Buffer::Support<BinaryData_t::value_type>::Index;

    return [=](void)
    {
      // Получение константного буфера по имени: https://docs.microsoft.com/en-us/windows/win32/api/d3d11shader/nf-d3d11shader-id3d11shaderreflection-getconstantbufferbyname
      cbBufferMapper(pData->data());

      // Поскольку каждый буфер индивидуален, но работает через один и тот же
      // слот, то и активировать его нужно каждый раз.
      m_pImmediateContext->VSSetConstantBuffers(
        BufferIndex, 1, pBuffer.GetAddressOf());
      m_pImmediateContext->PSSetConstantBuffers(
        BufferIndex, 1, pBuffer.GetAddressOf());

      m_pImmediateContext->UpdateSubresource(
        pBuffer.Get(), 0, NULL, pData->data(), 0, 0);
    };
  }

  const Component::Buffer<int> IndexData{ *pBufferData };

  auto pBuffer = Buffer::Create(m_pDevice, false, 
    IndexData.Data.data(), IndexData.Data.size());

  return [=](void)
  {
    m_pImmediateContext->IASetIndexBuffer(
      pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  };
}

auto DirectX11::CreateTransform(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto BuildTransformMatrix =
    (_pComponent->Kind == uT("Unknown")) ? CreateDefaultTransformRender<::Object>(_pComponent) :
    (_pComponent->Kind == uT("Static")) ? CreateStaticTransformRender<::Object>(_pComponent) :
    (_pComponent->Kind == uT("Billboard")) ? CreateBillboardTransformRender<::Camera, ::Object>(_pComponent) :
      throw STD_EXCEPTION << "Unexpected transform component: " <<
        " [id=" << _pComponent->Id << ", kind=" << _pComponent->Kind << "].";

  return [=](void)
  {
    BuildTransformMatrix();
    m_pConstants->Update<::Object>();
  };
}

auto DirectX11::CreatePresentBuffer(const ComponentPtr_t & _pComponent) ->Render_t /*override*/
{
  using BufferMapper_t = cbBufferMap_t<void>;

  ComponentPtr_t pIndexBufferData = _pComponent;
  ComponentPtr_t pInstanceBufferData = nullptr;

  const auto SaveBuffer = [&](const ComponentPtr_t & _pBufferData)
  {
    if ((*_pBufferData)[uT("content")].IsType<Buffer_t<int>>())
    {
      pIndexBufferData = _pBufferData;
    }
    else if ((*_pBufferData)[uT("mapper")].IsType<const BufferMapper_t &>())
    {
      pInstanceBufferData = _pBufferData;
    }
    else
    {
      throw STD_EXCEPTION << "Unexpected buffer data component.";
    }
  };

  CapturingServiceComponent::Process(_pComponent,
    {
      { uT("Buffer"), SaveBuffer },
    });

  const Component::Buffer<int> IndexBufferData{ *pIndexBufferData };

  const auto pIndexBuffer = Buffer::Create(m_pDevice, false, 
    IndexBufferData.Data.data(), IndexBufferData.Data.size());
  const auto IndexCount = static_cast<DWORD>(IndexBufferData.Data.size());

  if (pInstanceBufferData == nullptr)
  {
    return [=](void)
    {
      m_pImmediateContext->IASetIndexBuffer(
        pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
      m_pImmediateContext->IASetPrimitiveTopology(
        D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      m_pImmediateContext->DrawIndexed(IndexCount, 0, 0);
    };
  }

  const BufferMapper_t cbBufferMapper =
    (*pInstanceBufferData)[uT("mapper")].Default(BufferMapper_t{});

  // 23 Сентябрь 2019 22:29 (unicornum.verum@gmail.com)
  TODO("Тест значений size и count по умолчанию");
  const ::std::size_t BufferSize =
    (*pInstanceBufferData)[uT("size")].Default((::std::size_t)16);

  // 24 Сентябрь 2019 08:00 (unicornum.verum@gmail.com)
  TODO("Перенести чтение параметра внутрь рендера для возможности его изменения");
  const auto InstanceCount = static_cast<UINT>(
    (::std::size_t)(*pInstanceBufferData)[uT("count")].Default((::std::size_t)1));

  // 24 Сентябрь 2019 08:27 (unicornum.verum@gmail.com)
  TODO("Проверки выравнивания BufferSize в 16 байт и делимости без остатка BufferSize / InstanceCount");
  const auto Stride = static_cast<UINT>(
    BufferSize / InstanceCount);

  const auto pInstanceBuffer = 
    Buffer::Create<int8_t>(m_pDevice, true, nullptr, BufferSize);

  return [=](void)
  {
    const auto IsDirty = cbBufferMapper(nullptr);
    if (IsDirty)
    {
      D3D11_MAPPED_SUBRESOURCE Resource = { 0 };
      DX_CHECK m_pImmediateContext->Map(pInstanceBuffer.Get(), 0,
        D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Resource);
      cbBufferMapper(Resource.pData);
      m_pImmediateContext->Unmap(pInstanceBuffer.Get(), 0);
    }

    constexpr UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(1, 1,
      pInstanceBuffer.GetAddressOf(), &Stride, &offset);
    m_pImmediateContext->IASetIndexBuffer(
      pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_pImmediateContext->IASetPrimitiveTopology(
      D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->DrawIndexedInstanced(
      IndexCount, InstanceCount, 0, 0, 0);
  };
}

auto DirectX11::CreateGeometry(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const String_t Variety = (*_pComponent)[uT("variety")].Default(uT("Default"));

  // cppcheck-suppress internalAstError
  const auto BuildTransformMatrix =
    (Variety == uT("Default")) ? CreateDefaultTransformRender<::Matrices>(_pComponent) :
    (Variety == uT("Static")) ? CreateStaticTransformRender<::Matrices>(_pComponent) :
    (Variety == uT("Billboard")) ? CreateBillboardTransformRender<::Matrices, ::Matrices>(_pComponent) :
      throw STD_EXCEPTION << "Unknown variety: " << Variety <<
        " [id=" << _pComponent->Id << "].";

  return [=](void)
  {
    BuildTransformMatrix();
    m_pConstants->Update<::Matrices>();

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

    for (auto & RenderTarget : BlendDesc.RenderTarget)
    {
      RenderTarget.BlendEnable = TRUE;
      RenderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
      RenderTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
      RenderTarget.BlendOp = D3D11_BLEND_OP_ADD;
      RenderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
      RenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
      RenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
      RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

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
      m_pImmediateContext->OMSetRenderTargets(
        static_cast<UINT>(m_CurrentRenderTargets.size()),
        &m_CurrentRenderTargets[0], nullptr);
    };
  }

  D3D11_DEPTH_STENCIL_DESC DeptStencilDesc = { 0 };
  DeptStencilDesc.DepthEnable = true;
  DeptStencilDesc.DepthWriteMask = _IsOverwrite ? 
    D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
  DeptStencilDesc.StencilEnable = false;

  ComPtr_t<ID3D11DepthStencilState> pDSState;
  DX_CHECK m_pDevice->CreateDepthStencilState(&DeptStencilDesc, &pDSState);

  Render_t RenderDepthEnabled = [=](void)
  {
    // 12 Октябрь 2019 16:04 (unicornum.verum@gmail.com)
    TODO("Тест того, что если BkSurface не устанавливает буфер глубины, то используется буфер глубины экрана");

    m_pImmediateContext->OMSetDepthStencilState(pDSState.Get(), 1);
    m_pImmediateContext->OMSetRenderTargets(
      static_cast<UINT>(m_CurrentRenderTargets.size()),
      &m_CurrentRenderTargets[0], m_pCurrentDepthStencilView.Get());
  };

  Render_t RenderDepthClear = [=](void)
  {
    // 12 Октябрь 2019 16:04 (unicornum.verum@gmail.com)
    TODO("Тест того, что если BkSurface не устанавливает буфер глубины, то используется буфер глубины экрана");

    m_pImmediateContext->OMSetDepthStencilState(pDSState.Get(), 1);
    m_pImmediateContext->OMSetRenderTargets(
      static_cast<UINT>(m_CurrentRenderTargets.size()),
      &m_CurrentRenderTargets[0], m_pCurrentDepthStencilView.Get());
    m_pImmediateContext->ClearDepthStencilView(m_pCurrentDepthStencilView.Get(),
      D3D11_CLEAR_DEPTH, 0.0f, 0);
  };

  return _IsClear ? RenderDepthClear : RenderDepthEnabled;
}

template<class TConstantBuffer>
auto DirectX11::CreateDefaultTransformRender(
  const ComponentPtr_t & _pComponent) -> Render_t
{
  ::std::deque<Render_t> PreRenders;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    PreRenders.push_front([=](void)
    {
      auto & World = m_pConstants->Get<TConstantBuffer>().World;

      const Component::Position Position{ *_pPosition };

      World = ::glm::translate(World,
        ::glm::vec3{ Position.X, Position.Y, Position.Z });
    });
  };

  auto CreateRotation = [&](const ComponentPtr_t & _pRotation)
  {
    PreRenders.push_front([=](void)
    {
      auto & World = m_pConstants->Get<::Object>().World;

      const Component::Rotation Rotation{ *_pRotation };

      World = ::glm::rotate(World, Rotation.Z, ::glm::vec3{ 0.0f, 0.0f, 1.0f });
      World = ::glm::rotate(World, Rotation.Y, ::glm::vec3{ 0.0f, 1.0f, 0.0f });
      World = ::glm::rotate(World, Rotation.X, ::glm::vec3{ 1.0f, 0.0f, 0.0f });
    });
  };

  auto CreateScale = [&](const ComponentPtr_t & _pScale)
  {
    PreRenders.push_front([=](void)
    {
      auto & World = m_pConstants->Get<::Object>().World;

      const Component::Scale Scale{ *_pScale };

      World = ::glm::scale(World, ::glm::vec3{ Scale.X, Scale.Y, Scale.Z });
    });
  };

  PreRenders.push_front([=](void)
  {
    m_pConstants->Get<::Object>().World =
      ::glm::transpose(m_pConstants->Get<::Object>().World);
  });

  CapturingServiceComponent::Process(_pComponent,
    {
      { uT("Position"), CreatePosition },
      { uT("Rotation"), CreateRotation },
      { uT("Scale"), CreateScale },
    });

  PreRenders.push_front([=](void)
  {
    m_pConstants->Get<::Object>().World = ::glm::identity<::glm::mat4>();
  });

  return [PreRenders](void)
  {
    for (auto & Render : PreRenders) Render();
  };
}

template<class TConstantBuffer>
auto DirectX11::CreateStaticTransformRender(const ComponentPtr_t & _pComponent) -> Render_t
{
  CreateDefaultTransformRender<TConstantBuffer>(_pComponent)();
  const auto World = m_pConstants->Get<TConstantBuffer>().World;

  return [=](void)
  {
    m_pConstants->Get<TConstantBuffer>().World = World;
  };
}

template<class TCamera, class TConstantBuffer>
auto DirectX11::CreateBillboardTransformRender(const ComponentPtr_t & _pComponent) -> Render_t
{
  ::std::deque<Render_t> PreRenders;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    PreRenders.push_front([=](void)
    {
      auto & World = m_pConstants->Get<TConstantBuffer>().World;

      const Component::Position Position{ *_pPosition };

      World = ::glm::translate(World,
        ::glm::vec3{ Position.X, Position.Y, Position.Z });
    });
  };

  PreRenders.push_front([=](void)
  {
    m_pConstants->Get<TConstantBuffer>().World =
      ::glm::transpose(m_pConstants->Get<TConstantBuffer>().World);
  });

  CapturingServiceComponent::Process(_pComponent, 
    { { uT("Position"), CreatePosition } });

  PreRenders.push_front([=](void)
  {
    auto & World = m_pConstants->Get<TConstantBuffer>().World;

    // Матрица View уже траспонированная!
    World = m_pConstants->Get<::Camera>().View;

    World[0][3] = 0.0f;
    World[1][3] = 0.0f;
    World[2][3] = 0.0f;
    World[3][0] = 0.0f;
    World[3][1] = 0.0f;
    World[3][2] = 0.0f;
    World[3][3] = 1.0f;
  });

  return [PreRenders](void)
  {
    for (auto & Render : PreRenders) Render();
  };
}

} // namespace renderer

} // namespace api

} // namespace covellite
