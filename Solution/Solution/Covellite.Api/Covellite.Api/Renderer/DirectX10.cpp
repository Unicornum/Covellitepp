
#include "stdafx.h"
#include "DirectX10.hpp"
#include <glm/glm.force.hpp>
#include <alicorn/std/vector.hpp>
#include <Covellite/Api/Vertex.hpp>
#include <Covellite/Api/Defines.hpp>
#include "DxCheck.hpp"
#include "DirectX.hpp"
#include "Component.hpp"
#include "Shaders/Shaders.hpp"
#include "GraphicApi.Constants.hpp"

#include <d3d10_1.h>
#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

namespace covellite
{

namespace api
{

namespace renderer
{

class DirectX10::Buffer
{
public:
  template<class T>
  class Support
  {
  public:
    static UINT GetFlag(void) noexcept { return D3D10_BIND_VERTEX_BUFFER; }
    static const UINT Index = static_cast<UINT>(-1);
  };

  template<>
  class Support<int>
  {
  public:
    static UINT GetFlag(void) noexcept { return D3D10_BIND_INDEX_BUFFER; }
  };

  template<int TIndex>
  class Constant
  {
  public:
    static UINT GetFlag(void) noexcept { return D3D10_BIND_CONSTANT_BUFFER; }
    static const UINT Index = TIndex;
  };

  template<>
  class Support<::Camera> : public Constant<COVELLITE_BUFFER_INDEX_CAMERA> {};
  template<>
  class Support<::Fog> : public Constant<COVELLITE_BUFFER_INDEX_FOG> {};
  template<>
  class Support<::Object> : public Constant<COVELLITE_BUFFER_INDEX_OBJECT> {};
  template<>
  class Support<uint8_t> : public Constant<COVELLITE_BUFFER_INDEX_USER> {};
  template<>
  class Support<::Matrices> : public Constant<COVELLITE_BUFFER_INDEX_MATRICES> {};
  template<>
  class Support<::SceneLights> : public Constant<COVELLITE_BUFFER_INDEX_LIGHTS> {};

  template<class T>
  static Render_t GetRender(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const ComPtr_t<ID3D10Buffer> & _pBuffer)
  {
    return [=](void)
    {
      constexpr UINT stride = sizeof(T);
      constexpr UINT offset = 0;
      _pDevice->IASetVertexBuffers(0, 1,
        _pBuffer.GetAddressOf(), &stride, &offset);
    };
  }

  template<class T>
  static Render_t GetRender(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const ComPtr_t<ID3D10Buffer> & _pBuffer,
    const cbBufferMap_t<T> & _cbMapper)
  {
    return [=](void)
    {
      const auto IsDirty = _cbMapper(nullptr);
      if (IsDirty)
      {
        T * pData = nullptr;
        DX_CHECK _pBuffer->Map(D3D10_MAP_WRITE_NO_OVERWRITE, 0, (void**)&pData);
        _cbMapper(pData);
        _pBuffer->Unmap();
      }

      constexpr UINT stride = sizeof(T);
      constexpr UINT offset = 0;
      _pDevice->IASetVertexBuffers(0, 1,
        _pBuffer.GetAddressOf(), &stride, &offset);
    };
  }

public:
  template<class T>
  static ComPtr_t<ID3D10Buffer> Create(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const T * _pData, size_t _Count)
  {
    D3D10_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = static_cast<decltype(bd.ByteWidth)>(sizeof(T) * _Count);
    bd.BindFlags = Support<::std::remove_const<T>::type>::GetFlag();

    D3D10_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pData;

    const D3D10_SUBRESOURCE_DATA * const pInitData = 
      (_pData == nullptr) ? nullptr : &InitData;

    ComPtr_t<ID3D10Buffer> pBuffer;
    DX_CHECK _pDevice->CreateBuffer(&bd, pInitData, &pBuffer);
    return pBuffer;
  }

  template<class T>
  static ComPtr_t<ID3D10Buffer> CreateDynamic(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const T * _pData, 
    size_t _Count)
  {
    D3D10_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D10_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    bd.ByteWidth = static_cast<decltype(bd.ByteWidth)>(sizeof(T) * _Count);
    bd.BindFlags = Support<::std::remove_const<T>::type>::GetFlag();

    D3D10_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pData;

    const D3D10_SUBRESOURCE_DATA * const pInitData =
      (_pData == nullptr) ? nullptr : &InitData;

    ComPtr_t<ID3D10Buffer> pBuffer;
    DX_CHECK _pDevice->CreateBuffer(&bd, pInitData, &pBuffer);
    return pBuffer;
  }

  template<class T>
  static ComPtr_t<ID3D10Buffer> CreateConstant(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const T * _pData, const ::std::size_t _Count = 1)
  {
    const auto pBuffer = Create(_pDevice, _pData, _Count);
    _pDevice->VSSetConstantBuffers(Support<T>::Index, 1, pBuffer.GetAddressOf());
    _pDevice->PSSetConstantBuffers(Support<T>::Index, 1, pBuffer.GetAddressOf());
    return pBuffer;
  }

  template<class T>
  static Creator_t GetCreator(const ComPtr_t<ID3D10Device> & _pDevice)
  {
    return [=](const ComponentPtr_t & _pComponent)
    {
      const Component::Buffer<T> BufferData{ *_pComponent };

      return GetRender<T>(_pDevice,
        Create(_pDevice, BufferData.Data.data(), BufferData.Data.size()));
    };
  }

  template<class T>
  static Creator_t GetCreator(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const cbBufferMap_t<T> & _cbMapper)
  {
    return [=](const ComponentPtr_t & _pComponent)
    {
      const Component::Buffer<T> BufferData{ *_pComponent };

      return GetRender<T>(_pDevice,
        CreateDynamic(_pDevice, BufferData.Data.data(), BufferData.Data.size()), _cbMapper);
    };
  }
};

template<class T>
class DirectX10::ConstantBuffer final :
  public Constants::Data<T>
{
public:
  void Update(void) const override
  {
    ComPtr_t<ID3D10Device> pDevice;
    m_pBuffer->GetDevice(&pDevice);

    pDevice->UpdateSubresource(m_pBuffer.Get(), 0, NULL, &m_Data, 0, 0);
  }

private:
  const ComPtr_t<ID3D10Buffer> m_pBuffer;

public:
  ConstantBuffer(const ComPtr_t<ID3D10Device> & _pDevice) :
    m_pBuffer{ Buffer::CreateConstant(_pDevice, &m_Data) }
  {
  }
};

DirectX10::DirectX10(const Data_t & _Data)
{
  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  constexpr UINT DeviceFlags = 
    (IS_RELEASE_CONFIGURATION) ? 0 : D3D10_CREATE_DEVICE_DEBUG;

  DXGI_SWAP_CHAIN_DESC sd = { 0 };
  sd.BufferCount = 2;
  sd.OutputWindow = ::covellite::any_cast<HWND>(_Data.Handle);
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
    NULL, DeviceFlags, D3D10_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice);

  D3D10_RASTERIZER_DESC RasterizerDesc = { 0 };
  RasterizerDesc.FillMode = D3D10_FILL_SOLID;
  RasterizerDesc.CullMode = D3D10_CULL_BACK;
  RasterizerDesc.FrontCounterClockwise = TRUE;
  RasterizerDesc.DepthClipEnable = TRUE;

  DX_CHECK m_pDevice->CreateRasterizerState(
    &RasterizerDesc, &m_pDefaultRasterizerState);

  SetViewport(static_cast<UINT>(_Data.ClientRect.Width), 
    static_cast<UINT>(_Data.ClientRect.Height));

  MakeConstants<ConstantBuffer>(m_pDevice);
}

DirectX10::~DirectX10(void) = default;

DirectX10::String_t DirectX10::GetUsingApi(void) const /*override*/
{
  return uT("DirectX 10");
}

void DirectX10::PresentFrame(void) /*override*/
{
  m_pSwapChain->Present(0, 0);

  GraphicApi::PresentFrame();
}

void DirectX10::ResizeWindow(const Rect & _ClientRect) /*override*/
{
  SetViewport(static_cast<UINT>(_ClientRect.Width), static_cast<UINT>(_ClientRect.Height));
}

auto DirectX10::CreateCamera(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using Size_t = ::std::tuple<UINT, UINT>;
  using fnViewportSize_t = ::std::function<Size_t(void)>;

  const auto pCamera = _pComponent;

  const fnViewportSize_t GetScaleViewportSize = [=](void)
  {
    DXGI_SWAP_CHAIN_DESC Desc = { 0 };
    DX_CHECK m_pSwapChain->GetDesc(&Desc);

    const float Scale = (*pCamera)[uT("scale")];
    const auto Width = static_cast<UINT>(Scale * Desc.BufferDesc.Width);
    const auto Height = static_cast<UINT>(Scale * Desc.BufferDesc.Height);

    return Size_t{ Width, Height };
  };

  const fnViewportSize_t GetWindowViewportSize = [=](void)
  {
    DXGI_SWAP_CHAIN_DESC Desc = { 0 };
    DX_CHECK m_pSwapChain->GetDesc(&Desc);

    return Size_t{ Desc.BufferDesc.Width, Desc.BufferDesc.Height };
  };

  const fnViewportSize_t GetUserViewportSize = [=](void)
  {
    const int Width = (*pCamera)[uT("width")];
    const int Height = (*pCamera)[uT("height")];

    return Size_t{ static_cast<UINT>(Width), static_cast<UINT>(Height) };
  };

  const auto IsScaleViewportSize =
    (*pCamera)[uT("scale")].IsType<float>();
  const auto IsUserViewportSize =
    (*pCamera)[uT("width")].IsType<int>() &&
    (*pCamera)[uT("height")].IsType<int>();

  const auto GetViewportSize =
    (IsScaleViewportSize) ? GetScaleViewportSize :
    (IsUserViewportSize) ? GetUserViewportSize :
    GetWindowViewportSize;

  const auto SetDefaultRenderTarget = [=](void)
  {
    m_CurrentRenderTargets = { m_pScreenRenderTargetView.Get() };
    m_pCurrentDepthStencilView = m_pScreenDepthStencilView;

    m_pDevice->OMSetRenderTargets(static_cast<UINT>(m_CurrentRenderTargets.size()),
      &m_CurrentRenderTargets[0], nullptr);

    m_pDevice->RSSetState(m_pDefaultRasterizerState.Get());

    D3D10_VIEWPORT ViewPort = { 0 };
    ViewPort.TopLeftX = 0;
    ViewPort.TopLeftY = 0;
    ::std::tie(ViewPort.Width, ViewPort.Height) = GetViewportSize();
    ViewPort.MinDepth = 0.0f;
    ViewPort.MaxDepth = 1.0f;
    m_pDevice->RSSetViewports(1, &ViewPort);
  };

  const auto DisableBlendRender = CreateBlendState(false);
  const auto DisableDepthRender = GetDepthState(false, false, false);

  const auto ServiceComponents = 
    CapturingServiceComponent::Get(_pComponent,
    {
      { uT("Position"), api::Component::Make({}) },
      { uT("Rotation"), api::Component::Make({}) },
    });

  const Render_t CameraOrthographic = [=](void)
  {
    SetDefaultRenderTarget();
    DisableBlendRender();
    DisableDepthRender();

    UINT ViewportCount = 1;
    D3D10_VIEWPORT Viewport = { 0 };
    m_pDevice->RSGetViewports(&ViewportCount, &Viewport);

    const Component::Position Pos{ *ServiceComponents[0] };

    auto & CameraMatrices = m_pConstants->Get<::Camera>();

    CameraMatrices.Projection = ::glm::transpose(::glm::ortho(
      Pos.X, Pos.X + Viewport.Width,
      Pos.Y + Viewport.Height, Pos.Y,
      1.0f, -1.0f));

    const auto View = ::glm::identity<::glm::mat4>();
    CameraMatrices.View = ::glm::transpose(View);
    CameraMatrices.ViewInverse = ::glm::transpose(::glm::inverse(View));
    m_pConstants->Update<::Camera>();

    (*_pComponent)[uT("view")] = CameraMatrices.View;
    (*_pComponent)[uT("projection")] = CameraMatrices.Projection;
  };

  const Render_t CameraPerspective = [=](void)
  {
    SetDefaultRenderTarget();
    DisableBlendRender();
    DisableDepthRender();

    UINT ViewportCount = 1;
    D3D10_VIEWPORT Viewport = { 0 };
    m_pDevice->RSGetViewports(&ViewportCount, &Viewport);

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
  };

  return (_pComponent->Kind == uT("Perspective")) ?
    CameraPerspective : CameraOrthographic;
}

class DirectX10::Texture
{
public:
  using Ptr_t = ::std::shared_ptr<Texture>;

public:
  const ComponentPtr_t      m_pDataTexture;
  const UINT                m_iDestination;
  const DXGI_FORMAT         m_Format;
  int                       m_Width = 0;
  int                       m_Height = 0;
  ComPtr_t<ID3D10Texture2D>          m_pTexture;
  ComPtr_t<ID3D10Texture2D>          m_pReadTexture;
  ComPtr_t<ID3D10RenderTargetView>   m_pRenderTargetView;
  ComPtr_t<ID3D10DepthStencilView>   m_pDepthStencilView;
  ComPtr_t<ID3D10ShaderResourceView> m_pShaderResourceView;

public:
  void MakeTarget(
    const ComPtr_t<ID3D10Device> & _pDevice, 
    const UINT _Width, 
    const UINT _Height)
  {
    using BufferMapper_t = cbBufferMap_t<const void>;

    const Component::Texture TextureData{ *m_pDataTexture, uT("albedo") };

    m_pTexture = (TextureData.Destination != uT("depth")) ?
      MakeRGBTarget(_pDevice, _Width, _Height) :
      MakeDepthTarget(_pDevice, _Width, _Height);

    m_Width = _Width;
    m_Height = _Height;

    if (TextureData.IsMapping)
    {
      m_pReadTexture = MakeRGBACopy(_pDevice, _Width, _Height);
    }
  }

  void MakeRGBASource(const ComPtr_t<ID3D10Device> & _pDevice)
  {
    const Component::Texture TextureData{ *m_pDataTexture, uT("albedo") };

    m_Width = TextureData.Width;
    m_Height = TextureData.Height;

    m_pTexture = (TextureData.IsUsingMipmapping) ?
      MakeRGBAMipmapsSource(_pDevice, TextureData) :
      MakeRGBASource(_pDevice, TextureData);
  }

  ComPtr_t<ID3D10Texture2D> MakeRGBACopy(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const UINT _Width, const UINT _Height)
  {
    m_Width = _Width;
    m_Height = _Height;

    D3D10_TEXTURE2D_DESC textureDesc = { 0 };
    textureDesc.Width = _Width;
    textureDesc.Height = _Height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = m_Format;
    textureDesc.Usage = D3D10_USAGE_STAGING;
    //textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    //textureDesc.MiscFlags = 0;
    textureDesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;

    ComPtr_t<ID3D10Texture2D> pTexture;
    DX_CHECK _pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
    return pTexture;
  }

private:
  ComPtr_t<ID3D10Texture2D> MakeRGBASource(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const Component::Texture & _TextureData)
  {
    D3D10_TEXTURE2D_DESC textureDesc = { 0 };
    textureDesc.Width = static_cast<UINT>(_TextureData.Width);
    textureDesc.Height = static_cast<UINT>(_TextureData.Height);
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = m_Format;
    textureDesc.Usage = D3D10_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    textureDesc.MiscFlags = 0;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;

    ComPtr_t<ID3D10Texture2D> pTexture;
    DX_CHECK _pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);

    _pDevice->UpdateSubresource(pTexture.Get(), 0, NULL,
      _TextureData.Data.data(), static_cast<UINT>(_TextureData.Width * 4), 0);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc = { 0 };
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;

    DX_CHECK _pDevice->CreateShaderResourceView(pTexture.Get(),
      &srvDesc, &m_pShaderResourceView);
    return pTexture;
  }

  ComPtr_t<ID3D10Texture2D> MakeRGBAMipmapsSource(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const Component::Texture & _TextureData)
  {
    D3D10_TEXTURE2D_DESC textureDesc = { 0 };
    textureDesc.Width = static_cast<UINT>(_TextureData.Width);
    textureDesc.Height = static_cast<UINT>(_TextureData.Height);
    textureDesc.MipLevels = 0; // full set of subtextures
    textureDesc.ArraySize = 1;
    textureDesc.Format = m_Format;
    textureDesc.Usage = D3D10_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
    textureDesc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;

    ComPtr_t<ID3D10Texture2D> pTexture;
    DX_CHECK _pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);

    _pDevice->UpdateSubresource(pTexture.Get(), 0, NULL,
      _TextureData.Data.data(), static_cast<UINT>(_TextureData.Width * 4), 0);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc = { 0 };
    srvDesc.Format = m_Format;
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = static_cast<UINT>(-1);

    DX_CHECK _pDevice->CreateShaderResourceView(pTexture.Get(),
      &srvDesc, &m_pShaderResourceView);

    _pDevice->GenerateMips(m_pShaderResourceView.Get());
    return pTexture;
  }

private:
  ComPtr_t<ID3D10Texture2D> MakeRGBTarget(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const UINT _Width,
    const UINT _Height)
  {
    const auto CreateTextureTargetAndSource = [=](void)
    {
      (*m_pDataTexture)[uT("width")] = (int)_Width;
      (*m_pDataTexture)[uT("height")] = (int)_Height;

      D3D10_TEXTURE2D_DESC textureDesc = { 0 };
      textureDesc.Width = _Width;
      textureDesc.Height = _Height;
      textureDesc.MipLevels = 1; //0 - full set of subtextures
      textureDesc.ArraySize = 1;
      textureDesc.Format = m_Format;
      textureDesc.Usage = D3D10_USAGE_DEFAULT;
      textureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
      textureDesc.MiscFlags = 0;
      textureDesc.SampleDesc.Count = 1;
      textureDesc.SampleDesc.Quality = 0;

      ComPtr_t<ID3D10Texture2D> pTexture;
      DX_CHECK _pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
      return pTexture;
    };

    const auto CreateShaderResourceView = [=](auto _pTexture)
    {
      D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc = { 0 };
      srvDesc.Format = m_Format;
      srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
      srvDesc.Texture2D.MipLevels = 1;

      ComPtr_t<ID3D10ShaderResourceView> pShaderResourceView;
      DX_CHECK _pDevice->CreateShaderResourceView(_pTexture.Get(),
        &srvDesc, &pShaderResourceView);
      return pShaderResourceView;
    };

    const auto CreateRenderTargetView = [=](auto _pTexture)
    {
      D3D10_RENDER_TARGET_VIEW_DESC Desc = { 0 };
      Desc.Format = m_Format;
      Desc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;

      ComPtr_t<ID3D10RenderTargetView> pRenderTargetView;
      DX_CHECK _pDevice->CreateRenderTargetView(_pTexture.Get(),
        &Desc, &pRenderTargetView);
      return pRenderTargetView;
    };

    const auto pTexture = CreateTextureTargetAndSource();
    m_pRenderTargetView = CreateRenderTargetView(pTexture);
    m_pShaderResourceView = CreateShaderResourceView(pTexture);
    return pTexture;
  }

  ComPtr_t<ID3D10Texture2D> MakeDepthTarget(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const UINT _Width,
    const UINT _Height)
  {
    const auto CreateDepthTargetAndSource = [=](void)
    {
      (*m_pDataTexture)[uT("width")] = (int)_Width;
      (*m_pDataTexture)[uT("height")] = (int)_Height;

      D3D10_TEXTURE2D_DESC textureDesc = { 0 };
      textureDesc.Width = _Width;
      textureDesc.Height = _Height;
      textureDesc.MipLevels = 1; //0 - full set of subtextures
      textureDesc.ArraySize = 1;
      textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
      textureDesc.Usage = D3D10_USAGE_DEFAULT;
      textureDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
      textureDesc.MiscFlags = 0;
      textureDesc.SampleDesc.Count = 1;
      textureDesc.SampleDesc.Quality = 0;

      ComPtr_t<ID3D10Texture2D> pTexture;
      DX_CHECK _pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
      return pTexture;
    };

    const auto CreateDeptShaderResourceView = [=](auto _pTexture)
    {
      D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc = { 0 };
      srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
      srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
      srvDesc.Texture2D.MipLevels = 1;

      ComPtr_t<ID3D10ShaderResourceView> pShaderResourceView;
      DX_CHECK _pDevice->CreateShaderResourceView(_pTexture.Get(),
        &srvDesc, &pShaderResourceView);
      return pShaderResourceView;
    };

    const auto CreateDepthStencilView = [=](auto _pTexture)
    {
      D3D10_DEPTH_STENCIL_VIEW_DESC Desc = { 0 };
      Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      Desc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;

      ComPtr_t<ID3D10DepthStencilView> pDepthStencilView;
      DX_CHECK _pDevice->CreateDepthStencilView(_pTexture.Get(),
        &Desc, &pDepthStencilView);
      return pDepthStencilView;
    };

    const auto pTexture = CreateDepthTargetAndSource();
    m_pDepthStencilView = CreateDepthStencilView(pTexture);
    m_pShaderResourceView = CreateDeptShaderResourceView(pTexture);
    return pTexture;
  }

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

auto DirectX10::CreateBkSurface(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto pBkSurface = _pComponent;

  const auto GetViewportSize = [=](void)
  {
    using Size_t = ::std::tuple<UINT, UINT>;

    D3D10_VIEWPORT ViewPort = { 0 };
    UINT NumViewports = 1u;
    m_pDevice->RSGetViewports(&NumViewports, &ViewPort);

    (*pBkSurface)[uT("width")] = static_cast<int>(ViewPort.Width);
    (*pBkSurface)[uT("height")] = static_cast<int>(ViewPort.Height);

    return Size_t{ ViewPort.Width, ViewPort.Height };
  };

  const auto [Width, Height] = GetViewportSize();

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
    const auto [Width, Height] = GetViewportSize();

    m_CurrentRenderTargets.clear();

    for (auto & pTexture : *pBkSurfaceTextures)
    {
      if (pTexture->m_Width != Width || pTexture->m_Height != Height)
      {
        pTexture->MakeTarget(m_pDevice, Width, Height);
      }

      if (pTexture->m_pRenderTargetView)
      {
        m_CurrentRenderTargets.push_back(pTexture->m_pRenderTargetView.Get());
      }

      if (pTexture->m_pDepthStencilView)
      {
        m_pCurrentDepthStencilView = pTexture->m_pDepthStencilView;
      }

      ID3D10ShaderResourceView * NullResourceView[1] = { nullptr };
      m_pDevice->PSSetShaderResources(
        pTexture->m_iDestination, 1, NullResourceView);
    }

    if (m_CurrentRenderTargets.empty())
    {
      m_CurrentRenderTargets = { nullptr };
    }

    m_pDevice->OMSetRenderTargets(
      static_cast<UINT>(m_CurrentRenderTargets.size()),
      &m_CurrentRenderTargets[0], nullptr);
  };
}

auto DirectX10::CreateState(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto CreateSamplerState = [&](void)
  {
    D3D10_SAMPLER_DESC sampDesc = { 0 };
    sampDesc.Filter = D3D10_FILTER_ANISOTROPIC;
    sampDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D10_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D10_FLOAT32_MAX;

    ComPtr_t<ID3D10SamplerState> pSamplerState;
    DX_CHECK m_pDevice->CreateSamplerState(&sampDesc, &pSamplerState);

    return [=](void)
    {
      m_pDevice->PSSetSamplers(0, 1, pSamplerState.GetAddressOf());
    };
  };

  const auto CreateScissorState = [&](void)
  {
    const Component::Scissor ScissorData{ *_pComponent };

    D3D10_RASTERIZER_DESC RasterizerDesc = { 0 };
    RasterizerDesc.FillMode = D3D10_FILL_SOLID;
    RasterizerDesc.CullMode = D3D10_CULL_BACK;
    RasterizerDesc.FrontCounterClockwise = TRUE;
    RasterizerDesc.DepthClipEnable = TRUE;
    RasterizerDesc.ScissorEnable = (ScissorData.IsEnabled) ? TRUE : FALSE;

    ComPtr_t<ID3D10RasterizerState> pScissor;
    DX_CHECK m_pDevice->CreateRasterizerState(&RasterizerDesc, &pScissor);

    const auto pScissorRect = CapturingServiceComponent::Get(_pComponent, 
      { { uT("Rect"), _pComponent } })[0];

    Render_t ScissorEnabled = [=](void)
    {
      const Component::Scissor ScissorData{ *pScissorRect };

      D3D10_RECT rect;
      rect.left = ScissorData.Left;
      rect.right = ScissorData.Right;
      rect.top = ScissorData.Top;
      rect.bottom = ScissorData.Bottom;

      m_pDevice->RSSetScissorRects(1, &rect);
      m_pDevice->RSSetState(pScissor.Get());
    };

    Render_t ScissorDisabled = [=](void)
    {
      m_pDevice->RSSetState(pScissor.Get());
    };

    return (ScissorData.IsEnabled) ? ScissorEnabled : ScissorDisabled;
  };

  const auto CreateRasterizerState = [&](void)
  {
    const Component::Rasterizer RasterizerData{ *_pComponent };

    D3D10_RASTERIZER_DESC RasterizerDesc = { 0 };
    RasterizerDesc.FillMode = D3D10_FILL_SOLID;
    RasterizerDesc.FrontCounterClockwise = TRUE;
    RasterizerDesc.DepthClipEnable = TRUE;

    // cppcheck-suppress internalAstError
    RasterizerDesc.CullMode = 
      (RasterizerData.CullMode == uT("None")) ? D3D10_CULL_NONE :
      (RasterizerData.CullMode == uT("Front")) ? D3D10_CULL_FRONT :
      (RasterizerData.CullMode == uT("Back")) ? D3D10_CULL_BACK :
      throw STD_EXCEPTION << "Unexpected cull mode:" << RasterizerData.CullMode;

    ComponentPtr_t pScissorRect;

    const auto DoDataRect = [&](const ComponentPtr_t & _pDataRect) noexcept
    {
      pScissorRect = _pDataRect;
      RasterizerDesc.ScissorEnable = TRUE;
    };

    CapturingServiceComponent::Process(_pComponent,
      {
        { uT("Rect"), DoDataRect },
      });

    ComPtr_t<ID3D10RasterizerState> pRasterizerState;
    DX_CHECK m_pDevice->CreateRasterizerState(&RasterizerDesc, &pRasterizerState);

    Render_t Result;

    if (pScissorRect == nullptr)
    {
      Result = [=](void)
      {
        m_pDevice->RSSetState(pRasterizerState.Get());
      };
    }
    else
    {
      Result = [=](void)
      {
        const Component::Scissor ScissorData{ *pScissorRect };

        D3D10_RECT rect;
        rect.left = ScissorData.Left;
        rect.right = ScissorData.Right;
        rect.top = ScissorData.Top;
        rect.bottom = ScissorData.Bottom;

        m_pDevice->RSSetScissorRects(1, &rect);
        m_pDevice->RSSetState(pRasterizerState.Get());
      };
    }

    return Result;
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
        m_pDevice->ClearRenderTargetView(
          pRenderTargetView, BackgroundColor.data());
      }
    };
  };

  const auto CreateAlphaTestState = [](void)
  {
    return nullptr;
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Blend"),      [this](void) { return CreateBlendState(true); } },
    { uT("Sampler"),    CreateSamplerState    },
    { uT("Scissor"),    CreateScissorState    },
    { uT("Rasterizer"), CreateRasterizerState },
    { uT("Depth"),      CreateDepthState      },
    { uT("Clear"),      CreateClearState      },
    { uT("AlphaTest"),  CreateAlphaTestState  },
  };

  return Creators[_pComponent->Kind]();
}

auto DirectX10::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using BufferMapper_t = cbBufferMap_t<const void>;

  const auto pDataTexture = CapturingServiceComponent::Get(_pComponent, 
    { { uT("Texture"), _pComponent } })[0];

  Texture::Ptr_t pTexture = (*pDataTexture)[uT("entity")].Default(Texture::Ptr_t{});
  if (pTexture == nullptr)
  {
    pTexture = ::std::make_shared<Texture>(pDataTexture);
    pTexture->MakeRGBASource(m_pDevice);
  }
  else
  {
    (*pDataTexture)[uT("entity")] = Texture::Ptr_t{};
  }

  if (pTexture->m_pReadTexture == nullptr)
  {
    return [=](void)
    {
      m_pDevice->PSSetShaderResources(pTexture->m_iDestination, 1,
        pTexture->m_pShaderResourceView.GetAddressOf());
    };
  }

  const BufferMapper_t cbBufferMapper =
    (*pDataTexture)[uT("mapper")].Default(BufferMapper_t{});

  return [=](void)
  {
    if (cbBufferMapper(nullptr))
    {
      const auto pTextureCopy = pTexture->m_pReadTexture;

      m_pDevice->CopyResource(pTextureCopy.Get(), pTexture->m_pTexture.Get());

      D3D10_MAPPED_TEXTURE2D mappedTex = { 0 };
      DX_CHECK pTextureCopy->Map(0, D3D10_MAP_READ, 0, &mappedTex);
      cbBufferMapper(mappedTex.pData);
      pTextureCopy->Unmap(0);
    }

    m_pDevice->PSSetShaderResources(pTexture->m_iDestination, 1,
      pTexture->m_pShaderResourceView.GetAddressOf());
  };
}

auto DirectX10::CreateShader(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using namespace ::alicorn::extension::std;

  const auto pShaderDataComponent = CapturingServiceComponent::Get(_pComponent, 
    { { uT("Shader"), _pComponent } })[0];

  const auto ShaderData = Component::Shader{ *pShaderDataComponent, ::Default };

  ::std::string Define = 
    "#define COVELLITE_SHADER_DESKTOP\r\n"
    "#define COVELLITE_SHADER_HLSL\r\n"
    "#define COVELLITE_SHADER_VERTEX\r\n";

  const auto HeaderShaderText = ::Predefined + ::Data + 
    ShaderData.GetInstanceInput(::Input);
  auto ShaderText = ShaderData.Data;
  auto Entry = ShaderData.Entry;

  if (ShaderData.Kind == uT("Pixel"))
  {
    Define =
      "#define COVELLITE_SHADER_DESKTOP\r\n"
      "#define COVELLITE_SHADER_HLSL\r\n"
      "#define COVELLITE_SHADER_PIXEL\r\n";

    if (ShaderData.ReturnType != "void")
    {
      const auto PixelMain = ShaderData.ReturnType + " psMain(Pixel _Value)" +
        ((ShaderData.ReturnType == "float4") ? " : SV_Target" : "") + "\r\n"
        "{\r\n"
        "  return " + Entry + "(_Value);\r\n"
        "}\r\n";
      Entry = "psMain";
      ShaderText += DirectX::Shader::Convert(PixelMain);
    }
  }

  const auto pCompiledShader = DirectX::Shader::Compile(
    DirectX::Shader::Convert(Define) + HeaderShaderText, ShaderText, Entry.c_str(),
    DirectX::Shader::GetVersion(ShaderData.Kind).c_str());

  const auto CreateVertexShader = 
    [&](const ::std::vector<D3D10_INPUT_ELEMENT_DESC> & _LayoutDesc)
  {
    ComPtr_t<ID3D10InputLayout> pVertexLayout;
    DX_CHECK m_pDevice->CreateInputLayout(
      _LayoutDesc.data(), static_cast<UINT>(_LayoutDesc.size()),
      pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
      &pVertexLayout);

    ComPtr_t<ID3D10VertexShader> pVertexShader;
    DX_CHECK m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),
      pCompiledShader->GetBufferSize(), &pVertexShader);

    return [=](void)
    {
      m_pDevice->IASetInputLayout(pVertexLayout.Get());
      m_pDevice->VSSetShader(pVertexShader.Get());
    };
  };

  const auto VertexShader = [&](void)
  {
    ::std::vector<D3D10_INPUT_ELEMENT_DESC> LayoutDesc =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
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
        "TEXCOORD", i + 1, Format, 1, Size, D3D10_INPUT_PER_INSTANCE_DATA, 1
      });
    }

    return CreateVertexShader(LayoutDesc);
  };

  const auto PixelShader = [&](void)
  {
    ComPtr_t<ID3D10PixelShader> pPixelShader;
    DX_CHECK m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),
      pCompiledShader->GetBufferSize(), &pPixelShader);

    return [=](void)
    {
      m_pDevice->PSSetShader(pPixelShader.Get());
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Vertex"), VertexShader },
    { uT("Pixel"),  PixelShader },
  };

  return Creators[ShaderData.Kind]();
}

auto DirectX10::CreateBuffer(const ComponentPtr_t & _pBuffer) -> Render_t /*override*/
{
  const auto pBufferData = CapturingServiceComponent::Get(_pBuffer, 
    { { uT("Buffer"), _pBuffer } })[0];

  if ((*pBufferData)[uT("content")].IsType<Buffer_t<::covellite::api::Vertex>>())
  {
    using BufferMapper_t = cbBufferMap_t<::covellite::api::Vertex>;

    const BufferMapper_t & cbBufferMapper =
      (*_pBuffer)[uT("mapper")].Default(BufferMapper_t{});

    return (cbBufferMapper == nullptr) ?
      Buffer::GetCreator<::covellite::api::Vertex>(m_pDevice)(pBufferData) :
      Buffer::GetCreator<::covellite::api::Vertex>(m_pDevice, cbBufferMapper)(pBufferData);
  }
  else if ((*_pBuffer)[uT("mapper")].IsType<const cbBufferMap_t<::Lights_t> &>())
  {
    const cbBufferMap_t<::Lights_t> cbBufferMapper =
      (*_pBuffer)[uT("mapper")].Default(cbBufferMap_t<::Lights_t>{});
    if (cbBufferMapper == nullptr)
    {
      throw STD_EXCEPTION << "Unexpected mapper nullptr: " << _pBuffer->Id;
    }

    return [=](void)
    {
      cbBufferMapper(&m_pConstants->Get<::Object>().Lights);
    };
  }
  else if ((*_pBuffer)[uT("mapper")].IsType<const cbBufferMap_t<void> &>())
  {
    using BufferMap_t = cbBufferMap_t<void>;

    const BufferMap_t cbBufferMapper = 
      (*_pBuffer)[uT("mapper")].Default(BufferMap_t{});
    if (!cbBufferMapper)
    {
      throw STD_EXCEPTION << "Unexpected empty mapper: " << _pBuffer->Id;
    }

    const ::std::size_t BufferSize = (*pBufferData)[uT("size")].Default((::std::size_t)0);
    if (BufferSize == 0)
    {
      throw STD_EXCEPTION << "Unexpected zero size: " << _pBuffer->Id;
    }

    const auto pData = 
      ::std::make_shared<BinaryData_t>(BufferSize, (uint8_t)0x00);
    const auto pBuffer = 
      Buffer::Create(m_pDevice, pData->data(), pData->size());

    return [=](void)
    {
      cbBufferMapper(pData->data());

      constexpr auto BufferIndex =
        Buffer::Support<BinaryData_t::value_type>::Index;

      // Поскольку каждый буфер индивидуален, но работает через один и тот же
      // слот, то и активировать его нужно каждый раз.
      m_pDevice->VSSetConstantBuffers(BufferIndex, 1, pBuffer.GetAddressOf());
      m_pDevice->PSSetConstantBuffers(BufferIndex, 1, pBuffer.GetAddressOf());

      m_pDevice->UpdateSubresource(pBuffer.Get(), 0, NULL, pData->data(), 0, 0);
    };
  }

  return nullptr;
}

auto DirectX10::CreateTransform(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto PreRender =
    (_pComponent->Kind == uT("Unknown")) ? CreateBasePreRenderGeometry(_pComponent) :
    (_pComponent->Kind == uT("Static")) ? CreateStaticPreRenderGeometry(_pComponent) :
    (_pComponent->Kind == uT("Billboard")) ? CreateBillboardPreRenderGeometry(_pComponent) :
      throw STD_EXCEPTION << "Unexpected transform component: " <<
      " [id=" << _pComponent->Id << ", kind: " << _pComponent->Kind << "].";

  return [=](void)
  {
    PreRender();
    m_pConstants->Update<::Object>();
  };
}

auto DirectX10::CreatePresentBuffer(const ComponentPtr_t & _pBuffer) -> Render_t /*override*/
{
  using BufferMapper_t = cbBufferMap_t<void>;

  ComponentPtr_t pIndexBufferData = _pBuffer;
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

  CapturingServiceComponent::Process(_pBuffer,
    {
      { uT("Buffer"), SaveBuffer },
    });

  const Component::Buffer<int> IndexBufferData{ *pIndexBufferData };

  const auto pIndexBuffer =
    Buffer::Create(m_pDevice, IndexBufferData.Data.data(), IndexBufferData.Data.size());
  const auto IndexCount = static_cast<UINT>(IndexBufferData.Data.size());

  if (pInstanceBufferData == nullptr)
  {
    return [=](void)
    {
      m_pDevice->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
      m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      m_pDevice->DrawIndexed(IndexCount, 0, 0);
    };
  }

  const ::std::size_t BufferSize =
    (*pInstanceBufferData)[uT("size")].Default((::std::size_t)16);
  const auto MaxInstanceCount = static_cast<UINT>(
    (::std::size_t)(*pInstanceBufferData)[uT("count")].Default((::std::size_t)1));

  if (BufferSize % 16 != 0)
  {
    throw STD_EXCEPTION << _pBuffer->Id << ": size % 16 != 0";
  }

  if (BufferSize % MaxInstanceCount != 0)
  {
    throw STD_EXCEPTION << _pBuffer->Id << ": size % count != 0";
  }

  const auto Stride = static_cast<UINT>(BufferSize / MaxInstanceCount);

  const auto pInstanceBuffer =
    Buffer::CreateDynamic<int8_t>(m_pDevice, nullptr, BufferSize);

  return [=](void)
  {
    const BufferMapper_t cbBufferMapper =
      (*pInstanceBufferData)[uT("mapper")].Default(BufferMapper_t{});
    const auto InstanceCount = ::std::min(MaxInstanceCount, static_cast<UINT>(
      (::std::size_t)(*pInstanceBufferData)[uT("count")].Default((::std::size_t)0)));

    const auto IsDirty = cbBufferMapper(nullptr);
    if (IsDirty)
    {
      void * pData = nullptr;
      DX_CHECK pInstanceBuffer->Map(D3D10_MAP_WRITE_NO_OVERWRITE, 0, &pData);
      cbBufferMapper(pData);
      pInstanceBuffer->Unmap();
    }

    constexpr UINT offset = 0;
    m_pDevice->IASetVertexBuffers(1, 1,
      pInstanceBuffer.GetAddressOf(), &Stride, &offset);
    m_pDevice->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDevice->DrawIndexedInstanced(IndexCount, InstanceCount, 0, 0, 0);
  };
}

void DirectX10::SetViewport(const UINT _Width, const UINT _Height)
{
  if (m_pScreenRenderTargetView)
  {
    // Release the existing renderer target
    m_pScreenRenderTargetView.Reset();

    // Resize the swap chain's buffer to the given dimensions
    m_pSwapChain->ResizeBuffers(2, _Width, _Height,
      DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
  }

  // Recreate Render Target
  ComPtr_t<ID3D10Texture2D> pBackBuffer;
  DX_CHECK m_pSwapChain->GetBuffer(0,
    __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);

  DX_CHECK m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL,
    &m_pScreenRenderTargetView);

  D3D10_TEXTURE2D_DESC descDepth = { 0 };
  descDepth.Width = _Width;
  descDepth.Height = _Height;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_D32_FLOAT;
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D10_USAGE_DEFAULT;
  descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;

  ComPtr_t<ID3D10Texture2D> pDepthBuffer;
  DX_CHECK m_pDevice->CreateTexture2D(&descDepth, NULL, &pDepthBuffer);

  D3D10_DEPTH_STENCIL_VIEW_DESC descDSV = { 0 };
  descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;

  DX_CHECK m_pDevice->CreateDepthStencilView(pDepthBuffer.Get(),
    &descDSV, &m_pScreenDepthStencilView);
}

auto DirectX10::CreateBlendState(bool _IsEnabled) -> Render_t
{
  ComPtr_t<ID3D10BlendState> pBlendState;

  if (_IsEnabled)
  {
    D3D10_BLEND_DESC BlendDesc = { 0 };
    BlendDesc.AlphaToCoverageEnable = FALSE;
    BlendDesc.BlendEnable[0] = TRUE;
    BlendDesc.BlendEnable[1] = TRUE;
    BlendDesc.BlendEnable[2] = TRUE;
    BlendDesc.BlendEnable[3] = TRUE;
    BlendDesc.BlendEnable[4] = TRUE;
    BlendDesc.BlendEnable[5] = TRUE;
    BlendDesc.BlendEnable[6] = TRUE;
    BlendDesc.BlendEnable[7] = TRUE;
    BlendDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
    BlendDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
    BlendDesc.BlendOp = D3D10_BLEND_OP_ADD;
    BlendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
    BlendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
    BlendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
    BlendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
    BlendDesc.RenderTargetWriteMask[1] = D3D10_COLOR_WRITE_ENABLE_ALL;
    BlendDesc.RenderTargetWriteMask[2] = D3D10_COLOR_WRITE_ENABLE_ALL;
    BlendDesc.RenderTargetWriteMask[3] = D3D10_COLOR_WRITE_ENABLE_ALL;
    BlendDesc.RenderTargetWriteMask[4] = D3D10_COLOR_WRITE_ENABLE_ALL;
    BlendDesc.RenderTargetWriteMask[5] = D3D10_COLOR_WRITE_ENABLE_ALL;
    BlendDesc.RenderTargetWriteMask[6] = D3D10_COLOR_WRITE_ENABLE_ALL;
    BlendDesc.RenderTargetWriteMask[7] = D3D10_COLOR_WRITE_ENABLE_ALL;

    DX_CHECK m_pDevice->CreateBlendState(&BlendDesc, &pBlendState);
  }

  return [=](void)
  {
    const FLOAT BlendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_pDevice->OMSetBlendState(pBlendState.Get(), BlendFactor, 0xFFFFFFFF);
  };
}

auto DirectX10::GetDepthState(
  bool _IsEnabled,
  bool _IsClear,
  bool _IsOverwrite) -> Render_t
{
  if (!_IsEnabled)
  {
    return [=](void)
    {
      m_pDevice->OMSetRenderTargets(static_cast<UINT>(m_CurrentRenderTargets.size()),
        &m_CurrentRenderTargets[0], nullptr);
    };
  }

  D3D10_DEPTH_STENCIL_DESC dsDesc = { 0 };
  dsDesc.DepthEnable = true;
  dsDesc.DepthWriteMask = _IsOverwrite ?
    D3D10_DEPTH_WRITE_MASK_ALL : D3D10_DEPTH_WRITE_MASK_ZERO;
  dsDesc.DepthFunc = D3D10_COMPARISON_GREATER;
  dsDesc.StencilEnable = false;

  ComPtr_t<ID3D10DepthStencilState> pDSState;
  DX_CHECK m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState);

  const Render_t RenderDepthEnabled = [=](void)
  {
    m_pDevice->OMSetRenderTargets(static_cast<UINT>(m_CurrentRenderTargets.size()),
      &m_CurrentRenderTargets[0], m_pCurrentDepthStencilView.Get());
    m_pDevice->OMSetDepthStencilState(pDSState.Get(), 1);
  };

  const Render_t RenderDepthClear = [=](void)
  {
    m_pDevice->OMSetRenderTargets(static_cast<UINT>(m_CurrentRenderTargets.size()),
      &m_CurrentRenderTargets[0], m_pCurrentDepthStencilView.Get());
    m_pDevice->OMSetDepthStencilState(pDSState.Get(), 1);
    m_pDevice->ClearDepthStencilView(m_pCurrentDepthStencilView.Get(),
      D3D10_CLEAR_DEPTH, 0.0f, 0);
  };

  return _IsClear ? RenderDepthClear : RenderDepthEnabled;
}

auto DirectX10::CreateBasePreRenderGeometry(const ComponentPtr_t & _pComponent) -> Render_t
{
  ::std::deque<Render_t> PreRenders;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    PreRenders.push_front([=](void)
    {
      auto & World = m_pConstants->Get<::Object>().World;

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

auto DirectX10::CreateStaticPreRenderGeometry(const ComponentPtr_t & _pComponent) -> Render_t
{
  CreateBasePreRenderGeometry(_pComponent)();
  const auto World = m_pConstants->Get<::Object>().World;

  return [=](void)
  {
    m_pConstants->Get<::Object>().World = World;
  };
}

auto DirectX10::CreateBillboardPreRenderGeometry(const ComponentPtr_t & _pComponent) -> Render_t
{
  ::std::deque<Render_t> PreRenders;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    PreRenders.push_front([=](void)
    {
      auto & World = m_pConstants->Get<::Object>().World;

      const Component::Position Position{ *_pPosition };

      World = ::glm::translate(World,
        ::glm::vec3{ Position.X, Position.Y, Position.Z });
    });
  };

  PreRenders.push_front([=](void)
  {
    m_pConstants->Get<::Object>().World =
      ::glm::transpose(m_pConstants->Get<::Object>().World);
  });

  CapturingServiceComponent::Process(_pComponent,
    { { uT("Position"), CreatePosition } });

  PreRenders.push_front([=](void)
  {
    auto & World = m_pConstants->Get<::Object>().World;

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
