
#include "stdafx.h"
#include <alicorn/std/vector.hpp>
#include <alicorn/std/string/encoding.hpp>
#include "../Mock/DirectX11.hpp"

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса DirectX11.
*/

#define GraphicApi GraphicApi_DirectX11

// Расположение класса DirectX11
#include "../../Covellite.Api/Renderer/DirectX11.cpp"
#include "../../Covellite.Api/Renderer/DirectX.cpp"
#include "../../Covellite.Api/Renderer/GraphicApi.cpp"
#include "../../Covellite.Api/Renderer/GraphicApi.Constants.hpp"
#include "../../Covellite.Api/Renderer/GraphicApi.CapturingServiceComponent.cpp"

#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Vertex.hpp>

// Общий тестовый класс класса DirectX11
class DirectX11_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::DirectX11;
  using ITested_t = ::covellite::api::renderer::IGraphicApi;
  using Component_t = ::covellite::api::Component;
  using String_t = ::alicorn::extension::std::String;
  using Render_t = ::std::function<void(void)>;
  using Time_t = ::std::chrono::microseconds;
  using RenderTargetViews_t = ::std::vector<ID3D11RenderTargetView *>;
  using Object_t = ::std::vector<Component_t::ComponentPtr_t>;
  using TextureWeakPtr_t = ::std::weak_ptr<Tested_t::Texture>;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<HRESULT>::Set(S_OK);
    ::testing::DefaultValue<ID3D11Device *>::Set(&m_DefaultDevice);
    ::testing::DefaultValue<ID3D11DeviceContext *>::Set(&m_DefaultDeviceContext);
    ::testing::DefaultValue<IDXGISwapChain *>::Set(&m_DefaultSwapChain);
    ::testing::DefaultValue<DXGI_SWAP_CHAIN_DESC>::Set(m_DefaultDesc);
    ::testing::DefaultValue<ID3D11Buffer *>::Set(&m_DefaultBuffer);
    ::testing::DefaultValue<D3D11_BUFFER_DESC>::Set(m_DefaultBufferDesc);
    ::testing::DefaultValue<ID3DBlob *>::Set(&m_DefaultBlob);

    static const D3D11_VIEWPORT DefaultViewport = { 0, 0, 1, 2, 3, 4 };
    ::testing::DefaultValue<D3D11_VIEWPORT>::Set(DefaultViewport);

    static ::mock::DirectX11::Texture2D DefaultTexture2D;
    ::testing::DefaultValue<ID3D11Texture2D *>::Set(&DefaultTexture2D);

    static ::mock::DirectX11::DepthStencilState DefaultDepthStencilState;
    ::testing::DefaultValue<ID3D11DepthStencilState *>::Set(&DefaultDepthStencilState);

    static ::mock::DirectX11::DepthStencilView DefaultDepthStencilView;
    ::testing::DefaultValue<ID3D11DepthStencilView *>::Set(&DefaultDepthStencilView);

    static ::mock::DirectX11::RenderTargetView DefaultRenderTargetView;
    ::testing::DefaultValue<ID3D11RenderTargetView *>::Set(&DefaultRenderTargetView);

    ::testing::DefaultValue<Time_t>::Set(Time_t{});
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<HRESULT>::Clear();
    ::testing::DefaultValue<ID3D11Device *>::Clear();
    ::testing::DefaultValue<ID3D11DeviceContext *>::Clear();
    ::testing::DefaultValue<IDXGISwapChain *>::Clear();
    ::testing::DefaultValue<DXGI_SWAP_CHAIN_DESC>::Clear();
    ::testing::DefaultValue<ID3D11Buffer *>::Clear();
    ::testing::DefaultValue<D3D11_BUFFER_DESC>::Clear();
    ::testing::DefaultValue<ID3DBlob *>::Clear();
    ::testing::DefaultValue<D3D11_VIEWPORT>::Clear();
    ::testing::DefaultValue<ID3D11Texture2D *>::Clear();
    ::testing::DefaultValue<ID3D11DepthStencilState *>::Clear();
    ::testing::DefaultValue<ID3D11DepthStencilView *>::Clear();
    ::testing::DefaultValue<ID3D11RenderTargetView *>::Clear();
    ::testing::DefaultValue<Time_t>::Clear();
  }

protected:
  class Data :
    public ::covellite::api::renderer::SettingsData
  {
  public:
    Data(void)
    {
      Handle = (HWND)0;
      ClientRect.Top = 0;
      ClientRect.Width = 0;
      ClientRect.Height = 0;
      IsFullScreen = false;
    }
  };

  using Data_t = Data;

private:
  ::mock::DirectX11::Device m_DefaultDevice;
  ::mock::DirectX11::DeviceContext m_DefaultDeviceContext;
  ::mock::DXGI::SwapChain m_DefaultSwapChain;
  DXGI_SWAP_CHAIN_DESC m_DefaultDesc = { 0 };
  ::mock::DirectX11::Buffer m_DefaultBuffer;
  D3D11_BUFFER_DESC m_DefaultBufferDesc = { 0 };
  ::mock::DirectX11::Blob m_DefaultBlob;

protected:
  ::glm::vec4 ARGBtoFloat4(uint32_t _HexColor)
  {
    return ::glm::vec4
    {
      ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
      ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
      ((_HexColor & 0x000000FF) >> 0) / 255.0f,
      ((_HexColor & 0xFF000000) >> 24) / 255.0f
    };
  }

  static ::Camera & GetCameraMartix(Tested_t & _Example)
  {
    return _Example.m_pConstants->Get<::Camera>();
  }

  template<class T>
  void IntroduceBufferSize(::std::vector<T> & _Data)
  {
    (*reinterpret_cast<size_t *>(_Data.data())) = _Data.size() * sizeof(T);
  }

  void TestTextureCall(
    const Component_t::ComponentPtr_t &,
    const ::std::vector<uint8_t> &,
    const int, const int,
    const ::std::size_t,
    const DXGI_FORMAT);

  void TestTextureMipmappingCall(
    const Component_t::ComponentPtr_t &,
    const ::std::vector<uint8_t> &,
    const int, const int,
    const ::std::size_t,
    const DXGI_FORMAT);

  void TestBkSurfaceColorCall(
    const Component_t::ComponentPtr_t &,
    const int,
    const DXGI_FORMAT,
    const bool,
    const int, const int,
    const int, const int);

  void TestBkSurfaceDepthCall(
    const Component_t::ComponentPtr_t &,
    const int, const int,
    const int, const int);

  void TestBkSurfaceResizeCall(
    const Component_t::ComponentPtr_t &,
    const bool,
    const int, const int,
    const int, const int,
    const int, const int);
};

// Образец макроса для подстановки в класс DirectX11 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(DirectX11_test, Test_Function);

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateDeviceAndSwapChain_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceAndSwapChain(_))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateDeviceAndSwapChain_Windowed)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  Data_t oData;
  oData.Handle = (HWND)1809051128;
  oData.IsFullScreen = false;

  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  ::mock::DirectX11::DeviceDesc Desc = { 0 };
  Desc.DriverType = D3D_DRIVER_TYPE_HARDWARE;
  Desc.Flags = (IS_RELEASE_CONFIGURATION) ? 0 : D3D11_CREATE_DEVICE_DEBUG;
  Desc.FeatureLevels = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
  Desc.SDKVersion = D3D11_SDK_VERSION;
  Desc.SwapChainDesc.OutputWindow = ::covellite::any_cast<HWND>(oData.Handle);
  Desc.SwapChainDesc.Windowed = TRUE;
  Desc.SwapChainDesc.BufferCount = 2;
  Desc.SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  Desc.SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  Desc.SwapChainDesc.SampleDesc.Count = 1;
  Desc.SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceAndSwapChain(Desc))
    .Times(1);

  Tested_t oExample{ oData };
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateDeviceAndSwapChain_FullScreen)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  Data_t oData;
  oData.Handle = (HWND)1809051304;
  oData.IsFullScreen = true;

  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  ::mock::DirectX11::DeviceDesc Desc = { 0 };
  Desc.DriverType = D3D_DRIVER_TYPE_HARDWARE;
  Desc.Flags = (IS_RELEASE_CONFIGURATION) ? 0 : D3D11_CREATE_DEVICE_DEBUG;
  Desc.FeatureLevels = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
  Desc.SDKVersion = D3D11_SDK_VERSION;
  Desc.SwapChainDesc.OutputWindow = ::covellite::any_cast<HWND>(oData.Handle);
  Desc.SwapChainDesc.Windowed = FALSE;
  Desc.SwapChainDesc.BufferCount = 2;
  Desc.SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  Desc.SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  Desc.SwapChainDesc.SampleDesc.Count = 1;
  Desc.SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceAndSwapChain(Desc))
    .Times(1);

  Tested_t oExample{ oData };
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateDeviceAndSwapChain_Release)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  EXPECT_CALL(DirectXProxy, CreateDeviceAndSwapChain(_))
    .Times(1);

  {
    Tested_t oExample{ Data_t{} };

    EXPECT_CALL(Device, Release())
      .Times(1);

    EXPECT_CALL(DeviceContext, Release())
      .Times(AtLeast(1));

    EXPECT_CALL(SwapChain, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_RenderTargetView_SwapChain_GetBuffer_Fail)
{
  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  EXPECT_CALL(SwapChain, GetResult(Eq("GetBuffer")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_CALL(DeviceContext, Release())
    .Times(1);

  EXPECT_CALL(SwapChain, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_RenderTargetView_CreateRenderTargetView_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;
  ::mock::DirectX11::Texture2D Texture2D;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  EXPECT_CALL(SwapChain, GetBuffer(0, __uuidof(ID3D11Texture2D)))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateRenderTargetView")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_CALL(DeviceContext, Release())
    .Times(1);

  EXPECT_CALL(SwapChain, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_RenderTargetView)
{
  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;
  ::mock::DirectX11::Texture2D Texture2D;
  ::mock::DirectX11::RenderTargetView RenderTargetView;

  const D3D11_RENDER_TARGET_VIEW_DESC ZeroData = { 0 };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CreateDeviceAndSwapChain(_))
    .Times(1);

  EXPECT_CALL(SwapChain, ResizeBuffers(_, _, _, _, _))
    .Times(0);

  EXPECT_CALL(DeviceContext, RSSetViewports(_))
    .Times(0);

  EXPECT_CALL(SwapChain, GetBuffer(0, __uuidof(ID3D11Texture2D)))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, CreateRenderTargetView(&Texture2D, ZeroData))
    .Times(1)
    .WillOnce(Return(&RenderTargetView));

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  {
    Tested_t oExample{ Data_t{} };

    EXPECT_CALL(RenderTargetView, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_DeptBuffer_CreateTexture2D_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  EXPECT_CALL(Device, GetResult(_))
    .Times(1);

  EXPECT_CALL(Device, CreateTexture2D(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateTexture2D")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_CALL(DeviceContext, Release())
    .Times(1);

  EXPECT_CALL(SwapChain, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_DeptBuffer_CreateDepthStencilView_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;
  ::mock::DirectX11::Texture2D Texture2D;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  EXPECT_CALL(Device, GetResult(_))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateTexture2D(_, _))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, CreateDepthStencilView(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateDepthStencilView")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_CALL(DeviceContext, Release())
    .Times(1);

  EXPECT_CALL(SwapChain, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_DeptBuffer)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;
  ::mock::DirectX11::Texture2D Texture2D;
  ::mock::DirectX11::DepthStencilView DepthStencilView;

  const UINT Width = 1901021400;
  const UINT Height = 1901021401;

  D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
  TextureDesc.Width = Width;
  TextureDesc.Height = Height;
  TextureDesc.MipLevels = 1;
  TextureDesc.ArraySize = 1;
  TextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
  TextureDesc.SampleDesc.Count = 1;
  TextureDesc.SampleDesc.Quality = 0;
  TextureDesc.Usage = D3D11_USAGE_DEFAULT;
  TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

  const D3D11_SUBRESOURCE_DATA ZeroData = { 0 };

  D3D11_DEPTH_STENCIL_VIEW_DESC DeptStencilViewDesc = { 0 };
  DeptStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
  DeptStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  {
    InSequence Dummy;

    EXPECT_CALL(Device, CreateTexture2D(TextureDesc, ZeroData))
      .Times(1)
      .WillOnce(Return(&Texture2D));

    EXPECT_CALL(Device, CreateDepthStencilView(&Texture2D, DeptStencilViewDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilView));

    EXPECT_CALL(Texture2D, Release())
      .Times(1);
  }

  Data_t Data;
  Data.ClientRect.Width = Width;
  Data.ClientRect.Height = Height;

  Tested_t oExample{ Data };

  EXPECT_CALL(DepthStencilView, Release())
    .Times(1);

  EXPECT_CALL(SwapChain, Release())
    .Times(1);

  EXPECT_CALL(DeviceContext, Release())
    .Times(AtLeast(1));

  EXPECT_CALL(Device, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Create_ConstantBuffer_Fail)
{
  const auto TestCall = [](const int _BufferIndex)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(Device, GetResult(_))
      .Times(AtLeast(1));

    for (int i = 0; i < _BufferIndex - 1; i++)
    {
      EXPECT_CALL(Device, CreateBuffer(_, _))
        .Times(1);

      EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
        .Times(1)
        .WillOnce(Return(S_OK));
    }

    EXPECT_CALL(Device, CreateBuffer(_, _))
      .Times(1);

    EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_CALL(Device, Release())
      .Times(1);

    EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
  };

  TestCall(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_DisableFullscreenOnExit)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DXGI::SwapChain SwapChain;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  {
    Tested_t oExample{ Data_t{} };

    EXPECT_CALL(SwapChain, SetFullscreenState(FALSE, nullptr))
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_GetUsingApi)

{
  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  const auto Result = IExample.GetUsingApi();
  EXPECT_EQ(uT("DirectX 11"), Result);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_PresentFrame)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DXGI::SwapChain SwapChain;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  Tested_t oExample{ Data_t{} };
  ITested_t & IExample = oExample;

  EXPECT_CALL(SwapChain, Present(0, 0))
    .Times(1);

  IExample.PresentFrame();
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_ResizeWindow_SwapChain_ResizeBuffers_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DXGI::SwapChain SwapChain;
  ::mock::DirectX11::RenderTargetView RenderTargetView;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1)
    .WillOnce(Return(&RenderTargetView));

  Tested_t oExample{ Data_t{} };
  ITested_t & IExample = oExample;

  EXPECT_CALL(SwapChain, ResizeBuffers(_, _, _, _, _))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(IExample.ResizeWindow(::covellite::Rect{ 0, 0, 0, 0 }), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_ResizeWindow_SwapChain_GetBuffer_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DXGI::SwapChain SwapChain;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  Tested_t oExample{ Data_t{} };
  ITested_t & IExample = oExample;

  EXPECT_CALL(SwapChain, GetResult(Eq("GetBuffer")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(IExample.ResizeWindow(::covellite::Rect{ 0, 0, 0, 0 }), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_ResizeWindow_SwapChain_CreateRenderTargetView_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DXGI::SwapChain SwapChain;
  ::mock::DirectX11::Texture2D Texture2D;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  Tested_t oExample{ Data_t{} };
  ITested_t & IExample = oExample;

  InSequence Dummy;

  EXPECT_CALL(SwapChain, GetBuffer(_, _))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateRenderTargetView")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  EXPECT_THROW(IExample.ResizeWindow(::covellite::Rect{ 0, 0, 0, 0 }), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_ResizeWindow_CreateTexture2D_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  Tested_t oExample{ Data_t{} };
  ITested_t & IExample = oExample;

  InSequence Dummy;

  EXPECT_CALL(Device, GetResult(_))
    .Times(1);

  EXPECT_CALL(Device, CreateTexture2D(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateTexture2D")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_THROW(IExample.ResizeWindow(::covellite::Rect{ 0, 0, 0, 0 }), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_ResizeWindow_CreateDepthStencilView_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::Texture2D Texture2D;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  Tested_t oExample{ Data_t{} };
  ITested_t & IExample = oExample;

  InSequence Dummy;

  EXPECT_CALL(Device, GetResult(_))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateTexture2D(_, _))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, GetResult(_))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateDepthStencilView(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateDepthStencilView")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_THROW(IExample.ResizeWindow(::covellite::Rect{ 0, 0, 0, 0 }), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_ResizeWindow_RenderTargetView)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;
  ::mock::DirectX11::Texture2D Texture2D;
  ::mock::DirectX11::RenderTargetView RenderTargetViewBegin;
  ::mock::DirectX11::RenderTargetView RenderTargetView;
  const D3D11_RENDER_TARGET_VIEW_DESC ZeroData = { 0 };

  const auto Width = 1809061141;
  const auto Height = 1809061142;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1)
    .WillOnce(Return(&RenderTargetViewBegin));

  {
    Tested_t oExample{ Data_t{} };

    {
      ITested_t & IExample = oExample;

      InSequence Dummy;

      EXPECT_CALL(DeviceContext, RSSetViewports(_))
        .Times(0);

      EXPECT_CALL(RenderTargetViewBegin, Release())
        .Times(1);

      EXPECT_CALL(SwapChain, ResizeBuffers(2, Width, Height,
        DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH))
        .Times(1);

      EXPECT_CALL(SwapChain, GetBuffer(0, __uuidof(ID3D11Texture2D)))
        .Times(1)
        .WillOnce(Return(&Texture2D));

      EXPECT_CALL(Device, CreateRenderTargetView(&Texture2D, ZeroData))
        .Times(1)
        .WillOnce(Return(&RenderTargetView));

      EXPECT_CALL(Texture2D, Release())
        .Times(1);

      IExample.ResizeWindow(::covellite::Rect{ 0, 0, Width, Height });
    }

    EXPECT_CALL(RenderTargetView, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_ResizeWindow_DeptBuffer)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;
  ::mock::DirectX11::Texture2D Texture2D;
  ::mock::DirectX11::DepthStencilView BeginDepthStencilView;
  ::mock::DirectX11::DepthStencilView DepthStencilView;

  const UINT Width = 1901021427;
  const UINT Height = 1901021428;

  D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
  TextureDesc.Width = Width;
  TextureDesc.Height = Height;
  TextureDesc.MipLevels = 1;
  TextureDesc.ArraySize = 1;
  TextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
  TextureDesc.SampleDesc.Count = 1;
  TextureDesc.SampleDesc.Quality = 0;
  TextureDesc.Usage = D3D11_USAGE_DEFAULT;
  TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

  const D3D11_SUBRESOURCE_DATA ZeroData = { 0 };

  D3D11_DEPTH_STENCIL_VIEW_DESC DeptStencilViewDesc = { 0 };
  DeptStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
  DeptStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

  DXGI_SWAP_CHAIN_DESC Desc = { 0 };
  Desc.BufferDesc.Width = Width;
  Desc.BufferDesc.Height = Height;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateDepthStencilView(_, _))
    .Times(1)
    .WillOnce(Return(&BeginDepthStencilView));

  Tested_t oExample{ Data_t{} };

  {
    ITested_t & IExample = oExample;

    InSequence Dummy;

    EXPECT_CALL(BeginDepthStencilView, Release())
      .Times(1);

    EXPECT_CALL(Device, CreateTexture2D(TextureDesc, ZeroData))
      .Times(1)
      .WillOnce(Return(&Texture2D));

    EXPECT_CALL(Device, CreateDepthStencilView(&Texture2D, DeptStencilViewDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilView));

    EXPECT_CALL(Texture2D, Release())
      .Times(1);

    IExample.ResizeWindow(::covellite::Rect{ 0, 0, Width, Height });
  }

  EXPECT_CALL(DepthStencilView, Release())
    .Times(1);

  EXPECT_CALL(DeviceContext, Release())
    .Times(AtLeast(1));

  EXPECT_CALL(Device, Release())
    .Times(1);
}

void DirectX11_test::TestBkSurfaceColorCall(
  const Component_t::ComponentPtr_t & _pBkSurface,
  const int _Capacity,
  const DXGI_FORMAT _Format,
  const bool _IsUseMapper,
  const int _Width, const int _Height,
  const int _ExpectedWidth, const int _ExpectedHeight)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  const D3D11_SUBRESOURCE_DATA ZeroData = { 0 };

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  ::std::vector<::mock::DirectX11::Texture2D>
    Texture2D(Destinations.size());
  ::std::vector<::mock::DirectX11::Texture2D>
    CopyTexture2D(Destinations.size());
  ::std::vector<::mock::DirectX11::ShaderResourceView>
    ShaderResourceViews(Destinations.size());
  ::std::vector<::mock::DirectX11::RenderTargetView>
    RenderTargetView(Destinations.size());

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallCreateTexture = [&](
    const UINT _Width, const UINT _Height,
    const ::std::size_t _Slot)
  {
    D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
    TextureDesc.Width = _Width;
    TextureDesc.Height = _Height;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = _Format;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.MiscFlags = 0;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;

    D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
    SrvDesc.Format = _Format;
    SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SrvDesc.Texture2D.MipLevels = 1;

    D3D11_RENDER_TARGET_VIEW_DESC TargetDesc = { 0 };
    TargetDesc.Format = _Format;
    TargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    EXPECT_CALL(Device, CreateTexture2D(TextureDesc, ZeroData))
      .Times(1)
      .WillOnce(Return(&(Texture2D[_Slot])));

    EXPECT_CALL(Device, CreateShaderResourceView(&(Texture2D[_Slot]), SrvDesc))
      .Times(1)
      .WillOnce(Return(&(ShaderResourceViews[_Slot])));

    EXPECT_CALL(Device, CreateRenderTargetView(&(Texture2D[_Slot]), TargetDesc))
      .Times(1)
      .WillOnce(Return(&(RenderTargetView[_Slot])));

    if (_IsUseMapper)
    {
      D3D11_TEXTURE2D_DESC CopyTextureDesc = { 0 };
      CopyTextureDesc.Width = _Width;
      CopyTextureDesc.Height = _Height;
      CopyTextureDesc.MipLevels = 1;
      CopyTextureDesc.ArraySize = 1;
      CopyTextureDesc.Format = _Format;
      CopyTextureDesc.Usage = D3D11_USAGE_STAGING;
      CopyTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
      CopyTextureDesc.SampleDesc.Count = 1;

      EXPECT_CALL(Device, CreateTexture2D(CopyTextureDesc, ZeroData))
        .Times(1)
        .WillOnce(Return(&(CopyTexture2D[_Slot])));
    }
  };

  Object_t TextureComponents;
  ::std::vector<ID3D11RenderTargetView *> RenderTargetViewPtrs;

  InSequence Dummy;

  D3D11_VIEWPORT Viewport = { 0 };
  Viewport.Width = _Width;
  Viewport.Height = _Height;

  EXPECT_CALL(DeviceContext, RSGetViewports(1))
    .Times(1)
    .WillOnce(Return(Viewport));

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destinations[i] },
      });

    if (_Capacity >= 0) (*pData)[uT("capacity")] = _Capacity;
    if (_IsUseMapper) (*pData)[uT("mapper")] = BufferMapper_t{};

    TextureComponents.push_back(pData);
    TestCallCreateTexture(_ExpectedWidth, _ExpectedHeight, i);

    RenderTargetViewPtrs.push_back(&(RenderTargetView[i]));
  }

  (*_pBkSurface)[uT("service")] = TextureComponents;

  auto Render = itCreator->second(_pBkSurface);
  ASSERT_NE(nullptr, Render);

  EXPECT_EQ(_ExpectedWidth, (int)(*_pBkSurface)[uT("width")]);
  EXPECT_EQ(_ExpectedHeight, (int)(*_pBkSurface)[uT("height")]);

  {
    EXPECT_CALL(DeviceContext, RSGetViewports(1))
      .Times(1)
      .WillOnce(Return(Viewport));

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      const TextureWeakPtr_t pWeakPtrTexture =
        (*TextureComponents[i])[uT("entity")].Default(TextureWeakPtr_t{});
      const auto pTexture = pWeakPtrTexture.lock();
      ASSERT_NE(nullptr, pTexture);
      EXPECT_EQ(TextureComponents[i], pTexture->m_pDataTexture);
      EXPECT_EQ(_ExpectedWidth, (int)(*TextureComponents[i])[uT("width")].Default(0));
      EXPECT_EQ(_ExpectedHeight, (int)(*TextureComponents[i])[uT("height")].Default(0));

      EXPECT_CALL(DeviceContext, PSSetShaderResources(Eq(i),
        ::std::vector<ID3D11ShaderResourceView *>{ nullptr }))
        .Times(1);
    }

    EXPECT_CALL(DeviceContext, OMSetRenderTargets(RenderTargetViewPtrs, nullptr))
      .Times(1);

    EXPECT_CALL(DeviceContext, RSSetViewports(_))
      .Times(0);

    Render();
  }

  {
    // Второй вызов для проверки того, что список RenderTarget'ов очищается
    // и заполняется заново.

    EXPECT_CALL(DeviceContext, RSGetViewports(1))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(DeviceContext, PSSetShaderResources(_, _))
      .Times(Destinations.size());

    EXPECT_CALL(DeviceContext, OMSetRenderTargets(RenderTargetViewPtrs, nullptr))
      .Times(1);

    EXPECT_CALL(DeviceContext, RSSetViewports(_))
      .Times(0);

    Render();
  }

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    EXPECT_CALL(ShaderResourceViews[i], Release())
      .Times(1);

    EXPECT_CALL(RenderTargetView[i], Release())
      .Times(1);

    EXPECT_CALL(CopyTexture2D[i], Release())
      .Times(_IsUseMapper ? 1 : 0);

    EXPECT_CALL(Texture2D[i], Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_BkSurface_Color_UnknownFormat)
{
  TestBkSurfaceColorCall(Component_t::Make({ }),
    -1, DXGI_FORMAT_R8G8B8A8_UNORM, false,
    1024, 2048, 1024, 2048);
  TestBkSurfaceColorCall(Component_t::Make({ }),
    -1, DXGI_FORMAT_R8G8B8A8_UNORM, true,
    2048, 1024, 2048, 1024);

  TestBkSurfaceColorCall(Component_t::Make({ }),
    2008111758, DXGI_FORMAT_R8G8B8A8_UNORM, false,
    2048, 4096, 2048, 4096);
  TestBkSurfaceColorCall(Component_t::Make({ }),
    2008111818, DXGI_FORMAT_R8G8B8A8_UNORM, true,
    4096, 2048, 4096, 2048);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_BkSurface_Color_R8G8B8A8)
{
  TestBkSurfaceColorCall(Component_t::Make({ }),
    8, DXGI_FORMAT_R8G8B8A8_UNORM, false,
    2048, 1024, 2048, 1024);
  TestBkSurfaceColorCall(Component_t::Make({ }),
    8, DXGI_FORMAT_R8G8B8A8_UNORM, true,
    1024, 2048, 1024, 2048);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_BkSurface_Color_R16G16B16A16)
{
  TestBkSurfaceColorCall(Component_t::Make({ }),
    16, DXGI_FORMAT_R16G16B16A16_FLOAT, false,
    1024, 4096, 1024, 4096);
  TestBkSurfaceColorCall(Component_t::Make({ }),
    16, DXGI_FORMAT_R16G16B16A16_FLOAT, true,
    4096, 512, 4096, 512);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_BkSurface_Color_R32G32B32A32)
{
  TestBkSurfaceColorCall(Component_t::Make({ }),
    32, DXGI_FORMAT_R32G32B32A32_FLOAT, false,
    512, 1024, 512, 1024);
  TestBkSurfaceColorCall(Component_t::Make({ }),
    32, DXGI_FORMAT_R32G32B32A32_FLOAT, true,
    1024, 512, 1024, 512);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_BkSurface_Color_Fail)
{
  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  for (auto & Destination : Destinations)
  {
    const Object_t Service = 
    {
      Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destination },
      })
    };

    const auto pBkSurface = Component_t::Make(
      {
        { uT("service"), Service }
      });

    EXPECT_CALL(Device, GetResult(Eq("CreateTexture2D")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(pBkSurface), ::std::exception);

    EXPECT_CALL(Device, GetResult(Eq("CreateTexture2D")))
      .Times(1)
      .WillOnce(Return(S_OK));

    EXPECT_CALL(Device, GetResult(Eq("CreateShaderResourceView")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(pBkSurface), ::std::exception);

    EXPECT_CALL(Device, GetResult(Eq("CreateTexture2D")))
      .Times(1)
      .WillOnce(Return(S_OK));

    EXPECT_CALL(Device, GetResult(Eq("CreateShaderResourceView")))
      .Times(1)
      .WillOnce(Return(S_OK));

    EXPECT_CALL(Device, GetResult(Eq("CreateRenderTargetView")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(pBkSurface), ::std::exception);
  }
}

void DirectX11_test::TestBkSurfaceDepthCall(
  const Component_t::ComponentPtr_t & _pBkSurface,
  const int _Width, const int _Height,
  const int _ExpectedWidth, const int _ExpectedHeight)
{
  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  const D3D11_SUBRESOURCE_DATA ZeroData = { 0 };

  ::mock::DirectX11::Texture2D Texture2D;
  ::mock::DirectX11::ShaderResourceView ShaderResourceView;
  ::mock::DirectX11::DepthStencilView DepthStencilView;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallCreateTexture = [&](const UINT _Width, const UINT _Height)
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

    D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
    SrvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SrvDesc.Texture2D.MipLevels = 1;

    D3D11_DEPTH_STENCIL_VIEW_DESC DepthDesc = { 0 };
    DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DepthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    EXPECT_CALL(Device, CreateTexture2D(TextureDesc, ZeroData))
      .Times(1)
      .WillOnce(Return(&Texture2D));

    EXPECT_CALL(Device, CreateShaderResourceView(&Texture2D, SrvDesc))
      .Times(1)
      .WillOnce(Return(&ShaderResourceView));

    EXPECT_CALL(Device, CreateDepthStencilView(&Texture2D, DepthDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilView));
  };

  InSequence Dummy;

  D3D11_VIEWPORT Viewport = { 0 };
  Viewport.Width = _Width;
  Viewport.Height = _Height;

  EXPECT_CALL(DeviceContext, RSGetViewports(1))
    .Times(1)
    .WillOnce(Return(Viewport));

  const auto pTextureComponent = Component_t::Make(
    {
      { uT("kind"), uT("Texture")},
      { uT("destination"), uT("depth") },
    });

  TestCallCreateTexture(_ExpectedWidth, _ExpectedHeight);

  (*_pBkSurface)[uT("service")] = Object_t{ pTextureComponent };

  auto Render = itCreator->second(_pBkSurface);
  ASSERT_NE(nullptr, Render);

  EXPECT_EQ(_ExpectedWidth, (int)(*_pBkSurface)[uT("width")]);
  EXPECT_EQ(_ExpectedHeight, (int)(*_pBkSurface)[uT("height")]);

  EXPECT_CALL(DeviceContext, RSGetViewports(1))
    .Times(1)
    .WillOnce(Return(Viewport));

  {
    const TextureWeakPtr_t pWeakPtrTexture = 
      (*pTextureComponent)[uT("entity")].Default(TextureWeakPtr_t{});
    const auto pTexture = pWeakPtrTexture.lock();
    ASSERT_NE(nullptr, pTexture);
    EXPECT_EQ(pTextureComponent, pTexture->m_pDataTexture);
    EXPECT_EQ(_ExpectedWidth, (int)(*pTextureComponent)[uT("width")].Default(0));
    EXPECT_EQ(_ExpectedHeight, (int)(*pTextureComponent)[uT("height")].Default(0));

    EXPECT_CALL(DeviceContext, PSSetShaderResources(5,
      ::std::vector<ID3D11ShaderResourceView *>{ nullptr }))
      .Times(1);
  }

  EXPECT_CALL(DeviceContext, OMSetRenderTargets(
    ::std::vector<ID3D11RenderTargetView *>{ nullptr }, nullptr))
    .Times(1);

  EXPECT_CALL(DeviceContext, RSSetViewports(_))
    .Times(0);

  Render();

  {
    EXPECT_CALL(ShaderResourceView, Release())
      .Times(1);

    EXPECT_CALL(DepthStencilView, Release())
      .Times(1);

    EXPECT_CALL(Texture2D, Release())
      .Times(1);

    EXPECT_CALL(DepthStencilView, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_BkSurface_Depth)
{
  TestBkSurfaceDepthCall(Component_t::Make({ }),
    1024, 4096, 1024, 4096);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_BkSurface_Depth_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  const ::std::vector<String_t> Destinations =
  {
    uT("depth"),
  };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  for (auto & Destination : Destinations)
  {
    const Object_t Service =
    {
      Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destination },
      })
    };

    const auto pBkSurface = Component_t::Make(
      {
        { uT("service"), Service }
      });

    EXPECT_CALL(Device, GetResult(Eq("CreateTexture2D")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(pBkSurface), ::std::exception);

    EXPECT_CALL(Device, GetResult(Eq("CreateTexture2D")))
      .Times(1)
      .WillOnce(Return(S_OK));

    EXPECT_CALL(Device, GetResult(Eq("CreateShaderResourceView")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(pBkSurface), ::std::exception);

    EXPECT_CALL(Device, GetResult(Eq("CreateTexture2D")))
      .Times(1)
      .WillOnce(Return(S_OK));

    EXPECT_CALL(Device, GetResult(Eq("CreateShaderResourceView")))
      .Times(1)
      .WillOnce(Return(S_OK));

    EXPECT_CALL(Device, GetResult(Eq("CreateDepthStencilView")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(pBkSurface), ::std::exception);
  }
}

void DirectX11_test::TestBkSurfaceResizeCall(
  const Component_t::ComponentPtr_t & _pBkSurface,
  const bool _IsUseMapper,
  const int _Width, const int _Height,
  const int _ExpectedWidth, const int _ExpectedHeight,
  const int _ExpectedWidth2, const int _ExpectedHeight2)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
    uT("depth"),
  };

  ::std::vector<::mock::DirectX11::Texture2D>
    SourceTexture2D(Destinations.size() * 2);
  ::std::vector<::mock::DirectX11::Texture2D>
    ResizedTexture2D(Destinations.size() * 2);

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  Tested_t oExample{ Data_t{} };
  ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  Object_t Service;

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    const auto pTexture = Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destinations[i] },
      });

    if (_IsUseMapper) (*pTexture)[uT("mapper")] = BufferMapper_t{};

    Service.push_back(pTexture);
  }

  InSequence Dummy;

  {
    D3D11_VIEWPORT Viewport = { 0 };
    Viewport.Width = _Width;
    Viewport.Height = _Height;

    EXPECT_CALL(DeviceContext, RSGetViewports(1))
      .Times(1)
      .WillOnce(Return(Viewport));

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      if (_IsUseMapper)
      {
        EXPECT_CALL(Device, CreateTexture2D(_, _))
          .Times(1)
          .WillOnce(Return(&(SourceTexture2D[i * 2])));

        EXPECT_CALL(Device,
          CreateTexture2DSize(_ExpectedWidth, _ExpectedHeight))
          .Times(1);

        EXPECT_CALL(Device, CreateTexture2D(_, _))
          .Times(1)
          .WillOnce(Return(&(SourceTexture2D[i * 2 + 1])));

        EXPECT_CALL(Device,
          CreateTexture2DSize(_ExpectedWidth, _ExpectedHeight))
          .Times(1);
      }
      else
      {
        EXPECT_CALL(Device, CreateTexture2D(_, _))
          .Times(1)
          .WillOnce(Return(&(SourceTexture2D[i])));

        EXPECT_CALL(Device,
          CreateTexture2DSize(_ExpectedWidth, _ExpectedHeight))
          .Times(1);
      }
    }
  }

  (*_pBkSurface)[uT("service")] = Service;

  auto Render = itCreator->second(_pBkSurface);
  ASSERT_NE(nullptr, Render);

  EXPECT_EQ(_ExpectedWidth, (int)(*_pBkSurface)[uT("width")]);
  EXPECT_EQ(_ExpectedHeight, (int)(*_pBkSurface)[uT("height")]);

  {
    D3D11_VIEWPORT Viewport = { 0 };
    Viewport.Width = _Width;
    Viewport.Height = _Height;

    EXPECT_CALL(DeviceContext, RSGetViewports(1))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(Device, CreateTexture2DSize(_, _))
      .Times(0);

    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    EXPECT_CALL(DeviceContext, RSSetViewports(_))
      .Times(0);

    Render();

    EXPECT_EQ(_ExpectedWidth, (int)(*_pBkSurface)[uT("width")]);
    EXPECT_EQ(_ExpectedHeight, (int)(*_pBkSurface)[uT("height")]);
  }

  EXPECT_CALL(Device, CreateTexture2D(_, _))
    .Times(1);

  EXPECT_CALL(Device, CreateTexture2DSize(_, _))
    .Times(1);

  IExample.ResizeWindow(::covellite::Rect{ 0, 0, 0, 0 });

  {
    D3D11_VIEWPORT Viewport = { 0 };
    Viewport.Width = _Width * 2;
    Viewport.Height = _Height * 2;

    EXPECT_CALL(DeviceContext, RSGetViewports(1))
      .Times(1)
      .WillOnce(Return(Viewport));

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      if (_IsUseMapper)
      {
        EXPECT_CALL(Device, CreateTexture2D(_, _))
          .Times(1)
          .WillOnce(Return(&(ResizedTexture2D[i * 2])));

        EXPECT_CALL(Device,
          CreateTexture2DSize(_ExpectedWidth2, _ExpectedHeight2))
          .Times(1);

        EXPECT_CALL(SourceTexture2D[i * 2], Release())
          .Times(1);

        EXPECT_CALL(Device, CreateTexture2D(_, _))
          .Times(1)
          .WillOnce(Return(&(ResizedTexture2D[i * 2 + 1])));

        EXPECT_CALL(Device,
          CreateTexture2DSize(_ExpectedWidth2, _ExpectedHeight2))
          .Times(1);

        EXPECT_CALL(SourceTexture2D[i * 2 + 1], Release())
          .Times(1);
      }
      else
      {
        EXPECT_CALL(Device, CreateTexture2D(_, _))
          .Times(1)
          .WillOnce(Return(&(ResizedTexture2D[i])));

        EXPECT_CALL(Device,
          CreateTexture2DSize(_ExpectedWidth2, _ExpectedHeight2))
          .Times(1);

        EXPECT_CALL(SourceTexture2D[i], Release())
          .Times(1);
      }
    }

    // 11 Октябрь 2019 20:17 (unicornum.verum@gmail.com)
    TODO("Нужна проверка того, что используются новые созданные объекты");

    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    EXPECT_CALL(DeviceContext, RSSetViewports(_))
      .Times(0);

    Render();

    EXPECT_EQ(_ExpectedWidth2, (int)(*_pBkSurface)[uT("width")]);
    EXPECT_EQ(_ExpectedHeight2, (int)(*_pBkSurface)[uT("height")]);

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      EXPECT_EQ(_ExpectedWidth2, (int)(*Service[i])[uT("width")].Default(0));
      EXPECT_EQ(_ExpectedHeight2, (int)(*Service[i])[uT("height")].Default(0));
    }
  }

  IExample.PresentFrame();

  {
    D3D11_VIEWPORT Viewport = { 0 };
    Viewport.Width = _Width * 2;
    Viewport.Height = _Height * 2;

    EXPECT_CALL(DeviceContext, RSGetViewports(1))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(Device, CreateTexture2DSize(_, _))
      .Times(0);

    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    EXPECT_CALL(DeviceContext, RSSetViewports(_))
      .Times(0);

    Render();

    EXPECT_EQ(_ExpectedWidth2, (int)(*_pBkSurface)[uT("width")]);
    EXPECT_EQ(_ExpectedHeight2, (int)(*_pBkSurface)[uT("height")]);
  }
};

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_BkSurface_ResizeWindow)
{
  TestBkSurfaceResizeCall(Component_t::Make({ }),
    false,
    1024, 2048, 
    1024, 2048,
    2048, 4096);

  TestBkSurfaceResizeCall(Component_t::Make({ }),
    true,
    2048, 1024, 
    2048, 1024,
    4096, 2048);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Blend_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  const auto Type = uT("State");
  const auto pComponent = Component_t::Make(
    { 
      { uT("kind"), uT("Blend") }, 
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateBlendState(_))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateBlendState")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Blend)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::BlendState BlendState;

  D3D11_BLEND_DESC BlendDesc = { 0 };
  BlendDesc.AlphaToCoverageEnable = FALSE;
  BlendDesc.IndependentBlendEnable = FALSE;

  for (int i = 0; i < 8; i++)
  {
    BlendDesc.RenderTarget[i].BlendEnable = TRUE;
    BlendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BlendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  }

  const ::std::vector<FLOAT> Factor = { 0.0f, 0.0f, 0.0f, 0.0f };

  const auto Type = uT("State");
  const auto pComponent = Component_t::Make(
    { 
      { uT("kind"), uT("Blend") }, 
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateBlendState(BlendDesc))
    .Times(1)
    .WillOnce(Return(&BlendState));

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(DeviceContext, OMSetBlendState(&BlendState, Factor, 0xFFFFFFFF))
    .Times(1);

  Render();

  EXPECT_CALL(BlendState, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Sampler_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  const auto Type = uT("State");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Sampler") },
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateSamplerState(_))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateSamplerState")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Sampler)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::SamplerState SamplerState;

  D3D11_SAMPLER_DESC SamplerDesc = { 0 };
  SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  SamplerDesc.MinLOD = 0;
  SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  const auto Type = uT("State");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Sampler") },
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateSamplerState(SamplerDesc))
    .Times(1)
    .WillOnce(Return(&SamplerState));

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(DeviceContext, PSSetSamplers(0, 1, &SamplerState))
    .Times(1);

  Render();

  EXPECT_CALL(SamplerState, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Scissor_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  const auto Type = uT("State");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Scissor") },
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateRasterizerState(_))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateRasterizerState")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Scissor_Enabled)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RasterizerState Enable;

  D3D11_RASTERIZER_DESC Desc = { 0 };
  Desc.FillMode = D3D11_FILL_SOLID;
  Desc.CullMode = D3D11_CULL_BACK;
  Desc.FrontCounterClockwise = TRUE;
  Desc.ScissorEnable = TRUE;

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Scissor") },
      { uT("enabled"), true },
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateRasterizerState(Desc))
    .Times(1)
    .WillOnce(Return(&Enable));

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  const auto TestCallRender = [&](int _X, int _Y, int _Width, int _Height)
  {
    (*pComponent)[uT("left")] = _X;
    (*pComponent)[uT("top")] = _Y;
    (*pComponent)[uT("right")] = _X + _Width;
    (*pComponent)[uT("bottom")] = _Y + _Height;

    const ::std::vector<D3D11_RECT> Rects =
    {
      { _X, _Y, _X + _Width, _Y + _Height },
    };

    EXPECT_CALL(DeviceContext, RSSetScissorRects(Rects))
      .Times(1);

    EXPECT_CALL(DeviceContext, RSSetState(&Enable))
      .Times(1);

    Render();
  };

  TestCallRender(9061257, 9061258, 1809061259, 1809061300);
  TestCallRender(9061259, 9061260, 1809061260, 1809061301);

  EXPECT_CALL(Enable, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Scissor_Enabled_FromData)
{
  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RasterizerState Enable;

  D3D11_RASTERIZER_DESC Desc = { 0 };
  Desc.FillMode = D3D11_FILL_SOLID;
  Desc.CullMode = D3D11_CULL_BACK;
  Desc.FrontCounterClockwise = TRUE;
  Desc.ScissorEnable = TRUE;

  const auto pData = Component_t::Make(
  {
    { uT("kind"), uT("Rect") },
  });

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Scissor") },
      { uT("enabled"), true },
      { uT("service"), Object_t{ pData } }
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateRasterizerState(Desc))
    .Times(1)
    .WillOnce(Return(&Enable));

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  const auto TestCallRender = [&](int _X, int _Y, int _Width, int _Height)
  {
    (*pData)[uT("left")] = _X;
    (*pData)[uT("top")] = _Y;
    (*pData)[uT("right")] = _X + _Width;
    (*pData)[uT("bottom")] = _Y + _Height;

    const ::std::vector<D3D11_RECT> Rects =
    {
      { _X, _Y, _X + _Width, _Y + _Height },
    };

    EXPECT_CALL(DeviceContext, RSSetScissorRects(Rects))
      .Times(1);

    EXPECT_CALL(DeviceContext, RSSetState(&Enable))
      .Times(1);

    Render();
  };

  TestCallRender(9061257, 9061258, 1809061259, 1809061300);
  TestCallRender(9061259, 9061260, 1809061260, 1809061301);

  EXPECT_CALL(Enable, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Scissor_Disabled)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RasterizerState Disable;

  D3D11_RASTERIZER_DESC Desc = { 0 };
  Desc.FillMode = D3D11_FILL_SOLID;
  Desc.CullMode = D3D11_CULL_BACK;
  Desc.FrontCounterClockwise = TRUE;
  Desc.ScissorEnable = FALSE;

  const auto Type = uT("State");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Scissor") },
      { uT("enabled"), false },
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateRasterizerState(Desc))
    .Times(1)
    .WillOnce(Return(&Disable));

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(DeviceContext, RSSetState(&Disable))
    .Times(1);

  Render();

  EXPECT_CALL(Disable, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Disabled_ScreenTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1)
    .WillOnce(Return(&RenderTargetView));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera,
    const Component_t::ComponentPtr_t & _pState)
  {
    auto CameraRender = itCameraCreator->second(_pCamera);
    ASSERT_NE(nullptr, CameraRender);

    EXPECT_CALL(Device, CreateDepthStencilState(_))
      .Times(0);

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    CameraRender();

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(RenderTargetViews_t{ &RenderTargetView }, nullptr))
      .Times(1);

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(_, _))
      .Times(0);

    Render();
  };

  for (const auto & CameraType : { uT("Ortographic"), uT("Perspective") })
  {
    TestCallRender(
      Component_t::Make(
        {
          { uT("kind"), CameraType }
        }),
      Component_t::Make(
        {
          { uT("kind"), uT("Depth") }
        }));

    TestCallRender(
      Component_t::Make(
        {
          { uT("kind"), CameraType }
        }),
      Component_t::Make(
        {
          { uT("kind"), uT("Depth") },
          { uT("enabled"), false }
        }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Disabled_BkSurfaceTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  ::std::vector<::mock::DirectX11::RenderTargetView>
    RenderTargetView(Destinations.size());

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itBkSurfaceCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itBkSurfaceCreator);

  ::std::vector<ID3D11RenderTargetView *> RenderTargetViewPtrs;

  InSequence Dummy;

  Object_t Service;

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    Service.push_back(Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destinations[i] },
      }));

    EXPECT_CALL(Device, CreateRenderTargetView(_, _))
      .Times(1)
      .WillOnce(Return(&(RenderTargetView[i])));

    RenderTargetViewPtrs.push_back(&(RenderTargetView[i]));
  }

  auto BkSurfaceRender = itBkSurfaceCreator->second(Component_t::Make(
    {
      { uT("service"), Service }
    }));
  ASSERT_NE(nullptr, BkSurfaceRender);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    BkSurfaceRender();

    EXPECT_CALL(Device, CreateDepthStencilState(_))
      .Times(0);

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, 
      OMSetRenderTargets(RenderTargetViewPtrs, nullptr))
      .Times(1);

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(_, _))
      .Times(0);

    Render();
  };

  TestCallRender(
    Component_t::Make(
      {
        { uT("kind"), uT("Depth") }
      }));

  TestCallRender(
    Component_t::Make(
      {
        { uT("kind"), uT("Depth") },
        { uT("enabled"), false }
      }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Dept_CreateDepthStencilState_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::DepthStencilState DepthStencilState;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    EXPECT_CALL(Device, CreateDepthStencilState(_))
      .Times(1);

    EXPECT_CALL(Device, GetResult(Eq("CreateDepthStencilState")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(_pState), ::std::exception);
  };

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), false },
      { uT("overwrite"), false }
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), true },
      { uT("overwrite"), false }
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), false },
      { uT("overwrite"), true }
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), true },
      { uT("overwrite"), true }
    }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_NoClear_Overwrite_ScreenTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;
  ::mock::DirectX11::DepthStencilView DepthStencilView;
  ::mock::DirectX11::DepthStencilState DepthStencilState;

  D3D11_DEPTH_STENCIL_DESC DeptStencilDesc = { 0 };
  DeptStencilDesc.DepthEnable = true;
  DeptStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
  DeptStencilDesc.StencilEnable = false;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1)
    .WillOnce(Return(&RenderTargetView));

  EXPECT_CALL(Device, CreateDepthStencilView(_, _))
    .Times(1)
    .WillOnce(Return(&DepthStencilView));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera,
    const Component_t::ComponentPtr_t & _pState)
  {
    auto CameraRender = itCameraCreator->second(_pCamera);
    ASSERT_NE(nullptr, CameraRender);

    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    CameraRender();

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(RenderTargetViews_t{ &RenderTargetView }, &DepthStencilView))
      .Times(1);

    EXPECT_CALL(DeviceContext, ClearDepthStencilView(_, _, _, _))
      .Times(0);

    Render();

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));
  };

  for (const auto & CameraType : { uT("Ortographic"), uT("Perspective") })
  {
    TestCallRender(
      Component_t::Make(
      {
        { uT("kind"), CameraType }
      }), 
      Component_t::Make(
      {
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
      }));

    TestCallRender(
      Component_t::Make(
      {
        { uT("kind"), CameraType }
      }), 
      Component_t::Make(
      {
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("overwrite"), true }
      }));

    TestCallRender(
      Component_t::Make(
      {
        { uT("kind"), CameraType }
      }), 
      Component_t::Make(
      {
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("clear"), false },
      }));

    TestCallRender(
      Component_t::Make(
      {
        { uT("kind"), CameraType }
      }), 
      Component_t::Make(
      {
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("clear"), false },
        { uT("overwrite"), true }
      }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_NoClear_Overwrite_BkSurfaceTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::DepthStencilState DepthStencilState;

  D3D11_DEPTH_STENCIL_DESC DeptStencilDesc = { 0 };
  DeptStencilDesc.DepthEnable = true;
  DeptStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
  DeptStencilDesc.StencilEnable = false;

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  ::std::vector<::mock::DirectX11::RenderTargetView>
    RenderTargetView(Destinations.size());
  ::mock::DirectX11::DepthStencilView DepthStencilView;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itBkSurfaceCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itBkSurfaceCreator);

  ::std::vector<ID3D11RenderTargetView *> RenderTargetViewPtrs;

  InSequence Dummy;

  Object_t Service;

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    Service.push_back(Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destinations[i] },
      }));

    EXPECT_CALL(Device, CreateRenderTargetView(_, _))
      .Times(1)
      .WillOnce(Return(&(RenderTargetView[i])));

    RenderTargetViewPtrs.push_back(&(RenderTargetView[i]));
  }

  Service.push_back(Component_t::Make(
    {
      { uT("kind"), uT("Texture")},
      { uT("destination"), uT("depth") },
    }));

  EXPECT_CALL(Device, CreateDepthStencilView(_, _))
    .Times(1)
    .WillOnce(Return(&DepthStencilView));

  auto BkSurfaceRender = itBkSurfaceCreator->second(Component_t::Make(
    {
      { uT("service"), Service }
    }));
  ASSERT_NE(nullptr, BkSurfaceRender);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    BkSurfaceRender();

    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(RenderTargetViewPtrs, &DepthStencilView))
      .Times(1);

    EXPECT_CALL(DeviceContext, ClearDepthStencilView(_, _, _, _))
      .Times(0);

    Render();

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));
  };

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("overwrite"), true }
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), false },
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), false },
      { uT("overwrite"), true }
    }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_Clear_Overwrite_ScreenTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;
  ::mock::DirectX11::DepthStencilView DepthStencilView;
  ::mock::DirectX11::DepthStencilState DepthStencilState;

  D3D11_DEPTH_STENCIL_DESC DeptStencilDesc = { 0 };
  DeptStencilDesc.DepthEnable = true;
  DeptStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
  DeptStencilDesc.StencilEnable = false;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1)
    .WillOnce(Return(&RenderTargetView));

  EXPECT_CALL(Device, CreateDepthStencilView(_, _))
    .Times(1)
    .WillOnce(Return(&DepthStencilView));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera,
    const Component_t::ComponentPtr_t & _pState)
  {
    auto CameraRender = itCameraCreator->second(_pCamera);
    ASSERT_NE(nullptr, CameraRender);

    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    CameraRender();

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(RenderTargetViews_t{ &RenderTargetView }, &DepthStencilView))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      ClearDepthStencilView(&DepthStencilView, D3D11_CLEAR_DEPTH, 0.0f, 0))
      .Times(1);

    Render();

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));
  };

  for (const auto & CameraType : { uT("Ortographic"), uT("Perspective") })
  {
    TestCallRender(
      Component_t::Make(
      {
        { uT("kind"), CameraType }
      }), 
      Component_t::Make(
      {
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("clear"), true },
      }));

    TestCallRender(
      Component_t::Make(
      {
        { uT("kind"), CameraType }
      }), 
      Component_t::Make(
      {
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("clear"), true },
        { uT("overwrite"), true }
      }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_Clear_Overwrite_BkSurfaceTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::DepthStencilState DepthStencilState;

  D3D11_DEPTH_STENCIL_DESC DeptStencilDesc = { 0 };
  DeptStencilDesc.DepthEnable = true;
  DeptStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
  DeptStencilDesc.StencilEnable = false;

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  ::std::vector<::mock::DirectX11::RenderTargetView>
    RenderTargetView(Destinations.size());
  ::mock::DirectX11::DepthStencilView DepthStencilView;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itBkSurfaceCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itBkSurfaceCreator);

  ::std::vector<ID3D11RenderTargetView *> RenderTargetViewPtrs;

  InSequence Dummy;

  Object_t Service;

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    Service.push_back(Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destinations[i] },
      }));

    EXPECT_CALL(Device, CreateRenderTargetView(_, _))
      .Times(1)
      .WillOnce(Return(&(RenderTargetView[i])));

    RenderTargetViewPtrs.push_back(&(RenderTargetView[i]));
  }

  Service.push_back(Component_t::Make(
    {
      { uT("kind"), uT("Texture")},
      { uT("destination"), uT("depth") },
    }));

  EXPECT_CALL(Device, CreateDepthStencilView(_, _))
    .Times(1)
    .WillOnce(Return(&DepthStencilView));

  auto BkSurfaceRender = itBkSurfaceCreator->second(Component_t::Make(
    {
      { uT("service"), Service }
    }));
  ASSERT_NE(nullptr, BkSurfaceRender);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    BkSurfaceRender();

    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(RenderTargetViewPtrs, &DepthStencilView))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      ClearDepthStencilView(&DepthStencilView, D3D11_CLEAR_DEPTH, 0.0f, 0))
      .Times(1);

    Render();

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));
  };

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), true },
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), true },
      { uT("overwrite"), true }
    }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_NoClear_NoOverwrite_ScreenTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;
  ::mock::DirectX11::DepthStencilView DepthStencilView;
  ::mock::DirectX11::DepthStencilState DepthStencilState;

  D3D11_DEPTH_STENCIL_DESC DeptStencilDesc = { 0 };
  DeptStencilDesc.DepthEnable = true;
  DeptStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
  DeptStencilDesc.StencilEnable = false;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1)
    .WillOnce(Return(&RenderTargetView));

  EXPECT_CALL(Device, CreateDepthStencilView(_, _))
    .Times(1)
    .WillOnce(Return(&DepthStencilView));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera,
    const Component_t::ComponentPtr_t & _pState)
  {
    auto CameraRender = itCameraCreator->second(_pCamera);
    ASSERT_NE(nullptr, CameraRender);

    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    CameraRender();

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(RenderTargetViews_t{ &RenderTargetView }, &DepthStencilView))
      .Times(1);

    EXPECT_CALL(DeviceContext, ClearDepthStencilView(_, _, _, _))
      .Times(0);

    Render();

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));
  };

  for (const auto & CameraType : { uT("Ortographic"), uT("Perspective") })
  {
    TestCallRender(
      Component_t::Make(
      {
        { uT("kind"), CameraType }
      }), 
      Component_t::Make(
      {
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("overwrite"), false }
      }));

    TestCallRender(
      Component_t::Make(
      {
        { uT("kind"), CameraType }
      }), 
      Component_t::Make(
      {
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("clear"), false },
        { uT("overwrite"), false }
      }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_NoClear_NoOverwrite_BkSurfaceTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::DepthStencilState DepthStencilState;

  D3D11_DEPTH_STENCIL_DESC DeptStencilDesc = { 0 };
  DeptStencilDesc.DepthEnable = true;
  DeptStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
  DeptStencilDesc.StencilEnable = false;

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  ::std::vector<::mock::DirectX11::RenderTargetView>
    RenderTargetView(Destinations.size());
  ::mock::DirectX11::DepthStencilView DepthStencilView;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itBkSurfaceCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itBkSurfaceCreator);

  ::std::vector<ID3D11RenderTargetView *> RenderTargetViewPtrs;

  InSequence Dummy;

  Object_t Service;

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    Service.push_back(Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destinations[i] },
      }));

    EXPECT_CALL(Device, CreateRenderTargetView(_, _))
      .Times(1)
      .WillOnce(Return(&(RenderTargetView[i])));

    RenderTargetViewPtrs.push_back(&(RenderTargetView[i]));
  }

  Service.push_back(Component_t::Make(
    {
      { uT("kind"), uT("Texture")},
      { uT("destination"), uT("depth") },
    }));

  EXPECT_CALL(Device, CreateDepthStencilView(_, _))
    .Times(1)
    .WillOnce(Return(&DepthStencilView));

  auto BkSurfaceRender = itBkSurfaceCreator->second(Component_t::Make(
    {
      { uT("service"), Service }
    }));
  ASSERT_NE(nullptr, BkSurfaceRender);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    BkSurfaceRender();

    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(RenderTargetViewPtrs, &DepthStencilView))
      .Times(1);

    EXPECT_CALL(DeviceContext, ClearDepthStencilView(_, _, _, _))
      .Times(0);

    Render();

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));
  };

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("overwrite"), false }
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), false },
      { uT("overwrite"), false }
    }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_Clear_NoOverwrite_ScreenTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;
  ::mock::DirectX11::DepthStencilView DepthStencilView;
  ::mock::DirectX11::DepthStencilState DepthStencilState;

  D3D11_DEPTH_STENCIL_DESC DeptStencilDesc = { 0 };
  DeptStencilDesc.DepthEnable = true;
  DeptStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
  DeptStencilDesc.StencilEnable = false;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1)
    .WillOnce(Return(&RenderTargetView));

  EXPECT_CALL(Device, CreateDepthStencilView(_, _))
    .Times(1)
    .WillOnce(Return(&DepthStencilView));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera,
    const Component_t::ComponentPtr_t & _pState)
  {
    auto CameraRender = itCameraCreator->second(_pCamera);
    ASSERT_NE(nullptr, CameraRender);

    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    CameraRender();

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(RenderTargetViews_t{ &RenderTargetView }, &DepthStencilView))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      ClearDepthStencilView(&DepthStencilView, D3D11_CLEAR_DEPTH, 0.0f, 0))
      .Times(1);

    Render();

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));
  };

  for (const auto & CameraType : { uT("Ortographic"), uT("Perspective") })
  {
    TestCallRender(
      Component_t::Make(
      {
        { uT("kind"), CameraType }
      }), 
      Component_t::Make(
      {
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("clear"), true },
        { uT("overwrite"), false }
      }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_Clear_NoOverwrite_BkSurfaceTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::DepthStencilState DepthStencilState;

  D3D11_DEPTH_STENCIL_DESC DeptStencilDesc = { 0 };
  DeptStencilDesc.DepthEnable = true;
  DeptStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
  DeptStencilDesc.StencilEnable = false;

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  ::std::vector<::mock::DirectX11::RenderTargetView>
    RenderTargetView(Destinations.size());
  ::mock::DirectX11::DepthStencilView DepthStencilView;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itBkSurfaceCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itBkSurfaceCreator);

  ::std::vector<ID3D11RenderTargetView *> RenderTargetViewPtrs;

  InSequence Dummy;

  Object_t Service;

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    Service.push_back(Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destinations[i] },
      }));

    EXPECT_CALL(Device, CreateRenderTargetView(_, _))
      .Times(1)
      .WillOnce(Return(&(RenderTargetView[i])));

    RenderTargetViewPtrs.push_back(&(RenderTargetView[i]));
  }

  Service.push_back(Component_t::Make(
    {
      { uT("kind"), uT("Texture")},
      { uT("destination"), uT("depth") },
    }));

  EXPECT_CALL(Device, CreateDepthStencilView(_, _))
    .Times(1)
    .WillOnce(Return(&DepthStencilView));

  auto BkSurfaceRender = itBkSurfaceCreator->second(Component_t::Make(
    {
      { uT("service"), Service }
    }));
  ASSERT_NE(nullptr, BkSurfaceRender);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    EXPECT_CALL(DeviceContext, OMSetRenderTargets(_, _))
      .Times(1);

    BkSurfaceRender();

    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(RenderTargetViewPtrs, &DepthStencilView))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      ClearDepthStencilView(&DepthStencilView, D3D11_CLEAR_DEPTH, 0.0f, 0))
      .Times(1);

    Render();

    EXPECT_CALL(DepthStencilState, Release())
      .Times(AtLeast(1));
  };

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), true },
      { uT("overwrite"), false }
    }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Clear_ScreenTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1)
    .WillOnce(Return(&RenderTargetView));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera,
    const Component_t::ComponentPtr_t & _pState,
    const ::std::vector<FLOAT> & _ExpectedColor)
  {
    auto CameraRender = itCameraCreator->second(_pCamera);
    ASSERT_NE(nullptr, CameraRender);

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    CameraRender();

    EXPECT_CALL(DeviceContext, 
      ClearRenderTargetView(&RenderTargetView, _ExpectedColor))
      .Times(1);

    Render();
  };

  {
    const ::std::vector<FLOAT> DefaultColor =
    {
      0.0f,
      0.0f,
      0.0f,
      1.0f,
    };

    TestCallRender(
      Component_t::Make(
      {
        { uT("kind"), uT("Ortographic") },
      }),
      Component_t::Make(
      {
        { uT("kind"), uT("Clear") },
      }), 
      DefaultColor);
  }

  {
    const ::std::vector<FLOAT> Color =
    {
      0.86274509803921568627450980392157f, // DC
      0.72941176470588235294117647058824f, // BA
      0.5960784313725490196078431372549f, // 98
      0.9960784313725490196078431372549f, // FE
    };

    TestCallRender(
      Component_t::Make(
      {
        { uT("kind"), uT("Perspective") },
      }),
      Component_t::Make(
      {
        { uT("kind"), uT("Clear") },
        { uT("color"), 0xFEDCBA98 },
      }), 
      Color);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Clear_BkSurfaceTarget)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1)
    .WillOnce(Return(&RenderTargetView));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itBkSurfaceCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itBkSurfaceCreator);

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  ::std::vector<::mock::DirectX11::RenderTargetView>
    RenderTargetViews(Destinations.size());

  InSequence Dummy;

  Object_t Service;

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    Service.push_back(Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destinations[i] },
      }));

    EXPECT_CALL(Device, CreateRenderTargetView(_, _))
      .Times(1)
      .WillOnce(Return(&(RenderTargetViews[i])));
  }

  auto BkSurfaceRender = itBkSurfaceCreator->second(Component_t::Make(
    {
      { uT("service"), Service }
    }));
  ASSERT_NE(nullptr, BkSurfaceRender);

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pComponent,
    const ::std::vector<FLOAT> & _ExpectedColor)
  {
    auto Render = itCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    BkSurfaceRender();

    EXPECT_CALL(DeviceContext,
      ClearRenderTargetView(&RenderTargetView, _ExpectedColor))
      .Times(0);

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      EXPECT_CALL(DeviceContext,
        ClearRenderTargetView(&(RenderTargetViews[i]), _ExpectedColor))
        .Times(1);
    }

    Render();
  };

  {
    const ::std::vector<FLOAT> DefaultColor =
    {
      0.0f,
      0.0f,
      0.0f,
      1.0f,
    };

    TestCallRender(Component_t::Make(
      {
        { uT("kind"), uT("Clear") },
      }), DefaultColor);
  }

  {
    const ::std::vector<FLOAT> Color =
    {
      0.86274509803921568627450980392157f, // DC
      0.72941176470588235294117647058824f, // BA
      0.5960784313725490196078431372549f, // 98
      0.9960784313725490196078431372549f, // FE
    };

    TestCallRender(Component_t::Make(
      {
        { uT("kind"), uT("Clear") },
        { uT("color"), 0xFEDCBA98 },
      }), Color);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_CreateTexture2D_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  const auto pComponent = Component_t::Make({});

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateTexture2D(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateTexture2D")))
    .Times(AtLeast(1))
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_CreateShaderResourceView_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::Texture2D Texture2D;

  const auto pComponent = Component_t::Make({});

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateTexture2D(_, _))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, GetResult(Eq("CreateTexture2D")))
    .Times(1);

  EXPECT_CALL(Device, CreateShaderResourceView(_, _))
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_CALL(Device, GetResult(Eq("CreateShaderResourceView")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_UnknownDestination)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pTexture)
  {
    InSequence Dummy;

    EXPECT_CALL(Device, CreateTexture2D(_, _))
      .Times(0);

    EXPECT_CALL(Device, CreateShaderResourceView(_, _))
      .Times(0);

    EXPECT_THROW(itCreator->second(_pTexture), ::std::exception);
  };

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("destination"), uT("destination1907251219") },
      });

    TestCall(pComponent);
  }

  {
    const Object_t Data = { Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), uT("destination1907251220") },
      })};

    TestCall(Component_t::Make(
      {
        { uT("service"), Data }
      }));
  }
}

void DirectX11_test::TestTextureCall(
  const Component_t::ComponentPtr_t & _pTexture,
  const ::std::vector<uint8_t> & _BinaryData,
  const int _Width, const int _Height,
  const ::std::size_t _Slot,
  const DXGI_FORMAT _Format)
{
  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  ::mock::DirectX11::Texture2D Texture2D;
  ::mock::DirectX11::ShaderResourceView ShaderResourceView;

  D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
  TextureDesc.Width = _Width;
  TextureDesc.Height = _Height;
  TextureDesc.MipLevels = 1;
  TextureDesc.ArraySize = 1;
  TextureDesc.Format = _Format;
  TextureDesc.Usage = D3D11_USAGE_DEFAULT;
  TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  TextureDesc.MiscFlags = 0;
  TextureDesc.SampleDesc.Count = 1;
  TextureDesc.SampleDesc.Quality = 0;

  D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
  SrvDesc.Format = _Format;
  SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  SrvDesc.Texture2D.MipLevels = 1;

  InSequence Dummy;

  EXPECT_CALL(Device, CreateTexture2D(TextureDesc, D3D11_SUBRESOURCE_DATA{ 0 }))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(DeviceContext, UpdateSubresource(&Texture2D, 0, nullptr,
    _BinaryData, _Width * 4, 0))
    .Times(1);

  EXPECT_CALL(Device, CreateShaderResourceView(&Texture2D, SrvDesc))
    .Times(1)
    .WillOnce(Return(&ShaderResourceView));

  EXPECT_CALL(DeviceContext, GenerateMips(_))
    .Times(0);

  auto Render = itCreator->second(_pTexture);
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(DeviceContext, CopyResource(_, _))
    .Times(0);

  EXPECT_CALL(DeviceContext, PSSetShaderResources(Eq(_Slot),
    ::std::vector<ID3D11ShaderResourceView *>{ &ShaderResourceView }))
    .Times(1);

  Render();

  EXPECT_CALL(ShaderResourceView, Release())
    .Times(1);

  EXPECT_CALL(Texture2D, Release())
    .Times(1);
};

void DirectX11_test::TestTextureMipmappingCall(
  const Component_t::ComponentPtr_t & _pTexture,
  const ::std::vector<uint8_t> & _BinaryData,
  const int _Width, const int _Height,
  const ::std::size_t _Slot,
  const DXGI_FORMAT _Format)
{
  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  ::mock::DirectX11::Texture2D Texture2D;
  ::mock::DirectX11::ShaderResourceView ShaderResourceView;

  D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
  TextureDesc.Width = _Width;
  TextureDesc.Height = _Height;
  TextureDesc.MipLevels = 0;
  TextureDesc.ArraySize = 1;
  TextureDesc.Format = _Format;
  TextureDesc.Usage = D3D11_USAGE_DEFAULT;
  TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
  TextureDesc.SampleDesc.Count = 1;
  TextureDesc.SampleDesc.Quality = 0;

  D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
  SrvDesc.Format = _Format;
  SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  SrvDesc.Texture2D.MipLevels = static_cast<UINT>(-1);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateTexture2D(TextureDesc, D3D11_SUBRESOURCE_DATA{ 0 }))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(DeviceContext, UpdateSubresource(&Texture2D, 0, nullptr,
    _BinaryData, _Width * 4, 0))
    .Times(1);

  EXPECT_CALL(Device, CreateShaderResourceView(&Texture2D, SrvDesc))
    .Times(1)
    .WillOnce(Return(&ShaderResourceView));

  EXPECT_CALL(DeviceContext, GenerateMips(&ShaderResourceView))
    .Times(1);

  auto Render = itCreator->second(_pTexture);
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(DeviceContext, CopyResource(_, _))
    .Times(0);

  EXPECT_CALL(DeviceContext, PSSetShaderResources(Eq(_Slot),
    ::std::vector<ID3D11ShaderResourceView *>{ &ShaderResourceView }))
    .Times(1);

  Render();

  EXPECT_CALL(ShaderResourceView, Release())
    .Times(1);

  EXPECT_CALL(Texture2D, Release())
    .Times(1);
};

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_Default)
{
  const ::std::vector<uint8_t> BinaryData =
    { 0x19, 0x07, 0x25, 0x11, 0x51, 0x00 };
  constexpr int Width = 7251149;
  constexpr int Height = 1907251150;

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("content"), BinaryData },
      });

    TestTextureCall(pComponent,
      BinaryData, Width, Height, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
  }

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("content"), BinaryData },
        { uT("mipmapping"), false },
      });

    TestTextureCall(pComponent,
      BinaryData, Width, Height, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
  }

  {
    const Object_t TextureData = { Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("content"), BinaryData },
      }) };

    TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
      BinaryData, Width, Height, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
  }

  {
    const Object_t TextureData = { Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("content"), BinaryData },
        { uT("mipmapping"), false },
      }) };

    TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
      BinaryData, Width, Height, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_Index)
{
  const ::std::vector<uint8_t> BinaryData =
    { 0x19, 0x07, 0x25, 0x12, 0x06, 0x20, 0x08, 0x09, 0x10, 0x48, 0x00 };
  constexpr int Width = 8091050;
  constexpr int Height = 2008091049;

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
    uT("depth"),
  };

  for (int Index = 0; Index < 8; Index++)
  {
    {
      const auto pComponent = Component_t::Make(
        {
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("index"), Index },
        });

      TestTextureCall(pComponent, 
        BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("mipmapping"), false },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("index"), Index },
        });

      TestTextureCall(pComponent, 
        BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    {
      const Object_t TextureData = { Component_t::Make(
          {
          { uT("kind"), uT("Texture")},
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("index"), Index },
        }) };

      TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
        BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    {
      const Object_t TextureData = { Component_t::Make(
          {
          { uT("kind"), uT("Texture")},
          { uT("mipmapping"), false },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("index"), Index },
        }) };

      TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
        BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      {
        const auto pComponent = Component_t::Make(
          {
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("destination"), Destinations.at(i) },
          });

        TestTextureCall(pComponent, 
          BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
      }

      {
        const auto pComponent = Component_t::Make(
          {
            { uT("mipmapping"), false },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("destination"), Destinations.at(i) },
          });

        TestTextureCall(pComponent, 
          BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
      }

      {
        const Object_t TextureData = { Component_t::Make(
            {
            { uT("kind"), uT("Texture")},
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("destination"), Destinations.at(i) },
          }) };

        TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
          BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
      }

      {
        const Object_t TextureData = { Component_t::Make(
            {
            { uT("kind"), uT("Texture")},
            { uT("mipmapping"), false },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("destination"), Destinations.at(i) },
          }) };

        TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
          BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
      }
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_Index_Capacity)
{
  const auto TestCall = [=](const int _Capacity, const DXGI_FORMAT _Format)
  {
    const ::std::vector<uint8_t> BinaryData =
      { 0x19, 0x07, 0x25, 0x12, 0x06, 0x20, 0x08, 0x09, 0x10, 0x48, 0x00 };
    constexpr int Width = 8091050;
    constexpr int Height = 2008091049;

    const ::std::vector<String_t> Destinations =
    {
      uT("albedo"),
      uT("metalness"),
      uT("roughness"),
      uT("normal"),
      uT("occlusion"),
      uT("depth"),
    };

    for (int Index = 0; Index < 8; Index++)
    {
      {
        const auto pComponent = Component_t::Make(
          {
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("capacity"), _Capacity },
          });

        TestTextureCall(pComponent,
          BinaryData, Width, Height, Index, _Format);
      }

      {
        const auto pComponent = Component_t::Make(
          {
            { uT("mipmapping"), false },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("capacity"), _Capacity },
          });

        TestTextureCall(pComponent,
          BinaryData, Width, Height, Index, _Format);
      }

      {
        const Object_t TextureData = { Component_t::Make(
            {
            { uT("kind"), uT("Texture")},
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("capacity"), _Capacity },
          }) };

        TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
          BinaryData, Width, Height, Index, _Format);
      }

      {
        const Object_t TextureData = { Component_t::Make(
            {
            { uT("kind"), uT("Texture")},
            { uT("mipmapping"), false },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("capacity"), _Capacity },
          }) };

        TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
          BinaryData, Width, Height, Index, _Format);
      }

      for (::std::size_t i = 0; i < Destinations.size(); i++)
      {
        {
          const auto pComponent = Component_t::Make(
            {
              { uT("width"), Width },
              { uT("height"), Height },
              { uT("content"), BinaryData },
              { uT("index"), Index },
              { uT("capacity"), _Capacity },
              { uT("destination"), Destinations.at(i) },
            });

          TestTextureCall(pComponent,
            BinaryData, Width, Height, Index, _Format);
        }

        {
          const auto pComponent = Component_t::Make(
            {
              { uT("mipmapping"), false },
              { uT("width"), Width },
              { uT("height"), Height },
              { uT("content"), BinaryData },
              { uT("index"), Index },
              { uT("capacity"), _Capacity },
              { uT("destination"), Destinations.at(i) },
            });

          TestTextureCall(pComponent,
            BinaryData, Width, Height, Index, _Format);
        }

        {
          const Object_t TextureData = { Component_t::Make(
              {
              { uT("kind"), uT("Texture")},
              { uT("width"), Width },
              { uT("height"), Height },
              { uT("content"), BinaryData },
              { uT("index"), Index },
              { uT("capacity"), _Capacity },
              { uT("destination"), Destinations.at(i) },
            }) };

          TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
            BinaryData, Width, Height, Index, _Format);
        }

        {
          const Object_t TextureData = { Component_t::Make(
              {
              { uT("kind"), uT("Texture")},
              { uT("mipmapping"), false },
              { uT("width"), Width },
              { uT("height"), Height },
              { uT("content"), BinaryData },
              { uT("index"), Index },
              { uT("capacity"), _Capacity },
              { uT("destination"), Destinations.at(i) },
            }) };

          TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
            BinaryData, Width, Height, Index, _Format);
        }
      }
    }
  };

  TestCall(8, DXGI_FORMAT_R8G8B8A8_UNORM);
  TestCall(2008091114, DXGI_FORMAT_R8G8B8A8_UNORM);
  TestCall(16, DXGI_FORMAT_R16G16B16A16_FLOAT);
  TestCall(32, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_Destination)
{
  const ::std::vector<uint8_t> BinaryData =
    { 0x19, 0x07, 0x25, 0x12, 0x06, 0x00 };
  const int Width = 7251202;
  const int Height = 1907251203;

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
    uT("depth"),
  };

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    {
      const auto pComponent = Component_t::Make(
        {
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("destination"), Destinations.at(i) },
        });

      TestTextureCall(pComponent, 
        BinaryData, Width, Height, i, DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("mipmapping"), false },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("destination"), Destinations.at(i) },
        });

      TestTextureCall(pComponent,
        BinaryData, Width, Height, i, DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    {
      const Object_t TextureData = { Component_t::Make(
          {
          { uT("kind"), uT("Texture")},
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("destination"), Destinations.at(i) },
        }) };

      TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
        BinaryData, Width, Height, i, DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    {
      const Object_t TextureData = { Component_t::Make(
          {
          { uT("kind"), uT("Texture")},
          { uT("mipmapping"), false },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("destination"), Destinations.at(i) },
        }) };

      TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
        BinaryData, Width, Height, i, DXGI_FORMAT_R8G8B8A8_UNORM);
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_Destination_Capacity)
{
  const auto TestCall = [=](const int _Capacity, const DXGI_FORMAT _Format)
  {
    const ::std::vector<uint8_t> BinaryData =
    { 0x19, 0x07, 0x25, 0x12, 0x06, 0x00 };
    const int Width = 7251202;
    const int Height = 1907251203;

    const ::std::vector<String_t> Destinations =
    {
      uT("albedo"),
      uT("metalness"),
      uT("roughness"),
      uT("normal"),
      uT("occlusion"),
      uT("depth"),
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      {
        const auto pComponent = Component_t::Make(
          {
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("destination"), Destinations.at(i) },
            { uT("capacity"), _Capacity },
          });

        TestTextureCall(pComponent,
          BinaryData, Width, Height, i, _Format);
      }

      {
        const auto pComponent = Component_t::Make(
          {
            { uT("mipmapping"), false },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("destination"), Destinations.at(i) },
            { uT("capacity"), _Capacity },
          });

        TestTextureCall(pComponent,
          BinaryData, Width, Height, i, _Format);
      }

      {
        const Object_t TextureData = { Component_t::Make(
            {
            { uT("kind"), uT("Texture")},
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("destination"), Destinations.at(i) },
            { uT("capacity"), _Capacity },
          }) };

        TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
          BinaryData, Width, Height, i, _Format);
      }

      {
        const Object_t TextureData = { Component_t::Make(
            {
            { uT("kind"), uT("Texture")},
            { uT("mipmapping"), false },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("destination"), Destinations.at(i) },
            { uT("capacity"), _Capacity },
          }) };

        TestTextureCall(Component_t::Make({ { uT("service"), TextureData } }),
          BinaryData, Width, Height, i, _Format);
      }
    }
  };

  TestCall(8, DXGI_FORMAT_R8G8B8A8_UNORM);
  TestCall(2008091128, DXGI_FORMAT_R8G8B8A8_UNORM);
  TestCall(16, DXGI_FORMAT_R16G16B16A16_FLOAT);
  TestCall(32, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_Mipmapping_Default)
{
  const ::std::vector<uint8_t> BinaryData =
    { 0x19, 0x07, 0x25, 0x11, 0x51, 0x00 };
  const int Width = 7251149;
  const int Height = 1907251150;

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("width"), Width },
        { uT("mipmapping"), true },
        { uT("height"), Height },
        { uT("content"), BinaryData },
      });

    TestTextureMipmappingCall(pComponent,
      BinaryData, Width, Height, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
  }

  {
    const Object_t TextureData = { Component_t::Make(
    {
        { uT("kind"), uT("Texture")},
        { uT("mipmapping"), true },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("content"), BinaryData },
      }) };

    TestTextureMipmappingCall(Component_t::Make({ { uT("service"), TextureData } }),
      BinaryData, Width, Height, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_Mipmapping_Index)
{
  const ::std::vector<uint8_t> BinaryData =
    { 0x19, 0x07, 0x25, 0x12, 0x06, 0x00 };
  const int Width = 7251202;
  const int Height = 1907251203;

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
    uT("depth"),
  };

  for (int Index = 0; Index < 8; Index++)
  {
    {
      const auto pComponent = Component_t::Make(
        {
          { uT("width"), Width },
          { uT("mipmapping"), true },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("index"), Index },
        });

      TestTextureMipmappingCall(pComponent,
        BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    {
      const Object_t TextureData = { Component_t::Make(
        {
          { uT("kind"), uT("Texture")},
          { uT("mipmapping"), true },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("index"), Index },
        }) };

      TestTextureMipmappingCall(Component_t::Make({ { uT("service"), TextureData } }),
        BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      {
        const auto pComponent = Component_t::Make(
          {
            { uT("width"), Width },
            { uT("mipmapping"), true },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("destination"), Destinations.at(i) },
          });

        TestTextureMipmappingCall(pComponent,
          BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
      }

      {
        const Object_t TextureData = { Component_t::Make(
          {
            { uT("kind"), uT("Texture")},
            { uT("mipmapping"), true },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("destination"), Destinations.at(i) },
          }) };

        TestTextureMipmappingCall(Component_t::Make({ { uT("service"), TextureData } }),
          BinaryData, Width, Height, Index, DXGI_FORMAT_R8G8B8A8_UNORM);
      }
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_Mipmapping_Index_Capacity)
{
  const auto TestCall = [=](const int _Capacity, const DXGI_FORMAT _Format)
  {
    const ::std::vector<uint8_t> BinaryData =
    { 0x19, 0x07, 0x25, 0x12, 0x06, 0x00 };
    const int Width = 7251202;
    const int Height = 1907251203;

    const ::std::vector<String_t> Destinations =
    {
      uT("albedo"),
      uT("metalness"),
      uT("roughness"),
      uT("normal"),
      uT("occlusion"),
      uT("depth"),
    };

    for (int Index = 0; Index < 8; Index++)
    {
      {
        const auto pComponent = Component_t::Make(
          {
            { uT("width"), Width },
            { uT("mipmapping"), true },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("capacity"), _Capacity },
          });

        TestTextureMipmappingCall(pComponent,
          BinaryData, Width, Height, Index, _Format);
      }

      {
        const Object_t TextureData = { Component_t::Make(
          {
            { uT("kind"), uT("Texture")},
            { uT("mipmapping"), true },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("index"), Index },
            { uT("capacity"), _Capacity },
          }) };

        TestTextureMipmappingCall(Component_t::Make({ { uT("service"), TextureData } }),
          BinaryData, Width, Height, Index, _Format);
      }

      for (::std::size_t i = 0; i < Destinations.size(); i++)
      {
        {
          const auto pComponent = Component_t::Make(
            {
              { uT("width"), Width },
              { uT("mipmapping"), true },
              { uT("height"), Height },
              { uT("content"), BinaryData },
              { uT("index"), Index },
              { uT("capacity"), _Capacity },
              { uT("destination"), Destinations.at(i) },
            });

          TestTextureMipmappingCall(pComponent,
            BinaryData, Width, Height, Index, _Format);
        }

        {
          const Object_t TextureData = { Component_t::Make(
            {
              { uT("kind"), uT("Texture")},
              { uT("mipmapping"), true },
              { uT("width"), Width },
              { uT("height"), Height },
              { uT("content"), BinaryData },
              { uT("index"), Index },
              { uT("capacity"), _Capacity },
              { uT("destination"), Destinations.at(i) },
            }) };

          TestTextureMipmappingCall(Component_t::Make({ { uT("service"), TextureData } }),
            BinaryData, Width, Height, Index, _Format);
        }
      }
    }
  };

  TestCall(8, DXGI_FORMAT_R8G8B8A8_UNORM);
  TestCall(2008091542, DXGI_FORMAT_R8G8B8A8_UNORM);
  TestCall(16, DXGI_FORMAT_R16G16B16A16_FLOAT);
  TestCall(32, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_Mipmapping_Destination)
{
  const ::std::vector<uint8_t> BinaryData =
    { 0x19, 0x07, 0x25, 0x12, 0x06, 0x00 };
  const int Width = 7251202;
  const int Height = 1907251203;

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
    uT("depth"),
  };

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    {
      const auto pComponent = Component_t::Make(
        {
          { uT("width"), Width },
          { uT("mipmapping"), true },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("destination"), Destinations.at(i) },
        });

      TestTextureMipmappingCall(pComponent, 
        BinaryData, Width, Height, i, DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    {
      const Object_t TextureData = { Component_t::Make(
        {
          { uT("kind"), uT("Texture")},
          { uT("mipmapping"), true },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("content"), BinaryData },
          { uT("destination"), Destinations.at(i) },
        }) };

      TestTextureMipmappingCall(Component_t::Make({ { uT("service"), TextureData } }),
        BinaryData, Width, Height, i, DXGI_FORMAT_R8G8B8A8_UNORM);
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_Mipmapping_Destination_Capacity)
{
  const auto TestCall = [=](const int _Capacity, const DXGI_FORMAT _Format)
  {
    const ::std::vector<uint8_t> BinaryData =
    { 0x19, 0x07, 0x25, 0x12, 0x06, 0x00 };
    const int Width = 7251202;
    const int Height = 1907251203;

    const ::std::vector<String_t> Destinations =
    {
      uT("albedo"),
      uT("metalness"),
      uT("roughness"),
      uT("normal"),
      uT("occlusion"),
      uT("depth"),
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      {
        const auto pComponent = Component_t::Make(
          {
            { uT("width"), Width },
            { uT("mipmapping"), true },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("destination"), Destinations.at(i) },
            { uT("capacity"), _Capacity },
          });

        TestTextureMipmappingCall(pComponent,
          BinaryData, Width, Height, i, _Format);
      }

      {
        const Object_t TextureData = { Component_t::Make(
          {
            { uT("kind"), uT("Texture")},
            { uT("mipmapping"), true },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("content"), BinaryData },
            { uT("capacity"), _Capacity },
            { uT("destination"), Destinations.at(i) },
          }) };

        TestTextureMipmappingCall(Component_t::Make({ { uT("service"), TextureData } }),
          BinaryData, Width, Height, i, _Format);
      }
    }
  };

  TestCall(8, DXGI_FORMAT_R8G8B8A8_UNORM);
  TestCall(2008091544, DXGI_FORMAT_R8G8B8A8_UNORM);
  TestCall(16, DXGI_FORMAT_R16G16B16A16_FLOAT);
  TestCall(32, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_UsingExistsEntity_Default)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pData,
    const ::std::size_t _Slot)
  {
    ::mock::DirectX11::ShaderResourceView ShaderResourceView;

    const auto pTextureComponent = Component_t::Make(
      { 
        { uT("service"), Object_t{ _pData } }
      });

    const auto pTexture = ::std::make_shared<Tested_t::Texture>(_pData);

    {
      (*_pData)[uT("entity")] = TextureWeakPtr_t{ pTexture };

      EXPECT_CALL(Device, CreateShaderResourceView(_, _))
        .Times(1)
        .WillOnce(Return(&ShaderResourceView));

      pTexture->MakeTarget(&Device, 0, 0);

      EXPECT_CALL(ShaderResourceView, Release())
        .Times(0);
    }

    EXPECT_CALL(Device, CreateShaderResourceView(_, _))
      .Times(0);

    auto Render = itCreator->second(pTextureComponent);
    ASSERT_NE(nullptr, Render);

    const TextureWeakPtr_t pTextureWeakPtr =
      (*_pData)[uT("entity")].Default(TextureWeakPtr_t{});

    EXPECT_EQ(nullptr, pTextureWeakPtr.lock());

    EXPECT_CALL(DeviceContext, CopyResource(_, _))
      .Times(0);

    EXPECT_CALL(DeviceContext, PSSetShaderResources(Eq(_Slot),
      ::std::vector<ID3D11ShaderResourceView *>{ &ShaderResourceView }))
      .Times(1);

    Render();

    EXPECT_CALL(ShaderResourceView, Release())
      .Times(1);
  };

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
      });

    TestCall(pData, 0);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_UsingExistsEntity_Index)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pData,
    const ::std::size_t _Slot)
  {
    ::mock::DirectX11::ShaderResourceView ShaderResourceView;

    const auto pTextureComponent = Component_t::Make(
      {
        { uT("service"), Object_t{ _pData } }
      });

    const auto pTexture = ::std::make_shared<Tested_t::Texture>(_pData);

    {
      (*_pData)[uT("entity")] = TextureWeakPtr_t{ pTexture };

      EXPECT_CALL(Device, CreateShaderResourceView(_, _))
        .Times(1)
        .WillOnce(Return(&ShaderResourceView));

      pTexture->MakeTarget(&Device, 0, 0);

      EXPECT_CALL(ShaderResourceView, Release())
        .Times(0);
    }

    EXPECT_CALL(Device, CreateShaderResourceView(_, _))
      .Times(0);

    auto Render = itCreator->second(pTextureComponent);
    ASSERT_NE(nullptr, Render);

    const TextureWeakPtr_t pTextureWeakPtr =
      (*_pData)[uT("entity")].Default(TextureWeakPtr_t{});

    EXPECT_EQ(nullptr, pTextureWeakPtr.lock());

    EXPECT_CALL(DeviceContext, CopyResource(_, _))
      .Times(0);

    EXPECT_CALL(DeviceContext, PSSetShaderResources(Eq(_Slot),
      ::std::vector<ID3D11ShaderResourceView *>{ &ShaderResourceView }))
      .Times(1);

    Render();

    EXPECT_CALL(ShaderResourceView, Release())
      .Times(1);
  };

  for (int Index = 0; Index < 8; Index++)
  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("index"), Index },
      });

    TestCall(pData, Index);

    const ::std::vector<String_t> Destinations =
    {
      uT("albedo"),
      uT("metalness"),
      uT("roughness"),
      uT("normal"),
      uT("occlusion"),
      uT("depth"),
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      const auto pData2 = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("index"), Index },
          { uT("destination"), Destinations.at(i) },
        });

      TestCall(pData2, Index);
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_UsingExistsEntity_Destination)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pData,
    const ::std::size_t _Slot)
  {
    ::mock::DirectX11::ShaderResourceView ShaderResourceView;

    const auto pTextureComponent = Component_t::Make(
      {
        { uT("service"), Object_t{ _pData } }
      });

    const auto pTexture = ::std::make_shared<Tested_t::Texture>(_pData);

    {
      (*_pData)[uT("entity")] = TextureWeakPtr_t{ pTexture };

      EXPECT_CALL(Device, CreateShaderResourceView(_, _))
        .Times(1)
        .WillOnce(Return(&ShaderResourceView));

      pTexture->MakeTarget(&Device, 0, 0);

      EXPECT_CALL(ShaderResourceView, Release())
        .Times(0);
    }

    EXPECT_CALL(Device, CreateShaderResourceView(_, _))
      .Times(0);

    auto Render = itCreator->second(pTextureComponent);
    ASSERT_NE(nullptr, Render);

    const TextureWeakPtr_t pTextureWeakPtr =
      (*_pData)[uT("entity")].Default(TextureWeakPtr_t{});

    EXPECT_EQ(nullptr, pTextureWeakPtr.lock());

    EXPECT_CALL(DeviceContext, CopyResource(_, _))
      .Times(0);

    EXPECT_CALL(DeviceContext, PSSetShaderResources(Eq(_Slot),
      ::std::vector<ID3D11ShaderResourceView *>{ &ShaderResourceView }))
      .Times(1);

    Render();

    EXPECT_CALL(ShaderResourceView, Release())
      .Times(1);
  };

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
    uT("depth"),
  };

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destinations.at(i) },
      });

    TestCall(pData, i);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_ReadData_Index)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  class MapperProxy :
    public ::alicorn::extension::testing::Proxy<MapperProxy>
  {
  public:
    MOCK_METHOD1(Map, bool(const void *));
  };

  MapperProxy oMapperProxy;
  MapperProxy::GetInstance() = &oMapperProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;

  const BufferMapper_t Mapper = [&](const void * _pData)
  {
    return oMapperProxy.Map(_pData);
  };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  ::mock::DirectX11::Texture2D Texture2D;
  ::mock::DirectX11::Texture2D CopyTexture2D;
  ::mock::DirectX11::ShaderResourceView ShaderResourceView;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pData,
    const Tested_t::Texture::Ptr_t & _pTexture,
    const ::std::size_t _Slot)
  {
    InSequence Dummy;

    EXPECT_CALL(Device, CreateTexture2D(_, _))
      .Times(1)
      .WillOnce(Return(&Texture2D));

    EXPECT_CALL(Device, CreateShaderResourceView(&Texture2D, _))
      .Times(1)
      .WillOnce(Return(&ShaderResourceView));

    EXPECT_CALL(Device, CreateTexture2D(_, _))
      .Times(1)
      .WillOnce(Return(&CopyTexture2D));

    _pTexture->MakeTarget(&Device, 0, 0);

    EXPECT_CALL(Device, CreateTexture2D(_, _))
      .Times(0);

    auto Render = itCreator->second(Component_t::Make(
      { 
        { uT("service"), Object_t{ _pData } }
      }));
    ASSERT_NE(nullptr, Render);

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(DeviceContext, CopyResource(_, _))
      .Times(0);

    EXPECT_CALL(DeviceContext, PSSetShaderResources(Eq(_Slot),
      ::std::vector<ID3D11ShaderResourceView *>{ &ShaderResourceView }))
      .Times(1);

    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(true));

    EXPECT_CALL(DeviceContext, CopyResource(&CopyTexture2D, &Texture2D))
      .Times(1);

    EXPECT_CALL(DeviceContext, Mapped(&CopyTexture2D, D3D11_MAP_READ))
      .Times(1)
      .WillOnce(Return(D3D11_MAPPED_SUBRESOURCE{ 0 }));

    EXPECT_CALL(DeviceContext, Map(0, 0))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_CALL(oMapperProxy, Map(_))
      .Times(0);

    EXPECT_CALL(DeviceContext, Unmap(_, _))
      .Times(0);

    EXPECT_CALL(DeviceContext, PSSetShaderResources(_, _))
      .Times(0);

    EXPECT_THROW(Render(), ::std::exception);

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(true));

    EXPECT_CALL(DeviceContext, CopyResource(&CopyTexture2D, &Texture2D))
      .Times(1);

    D3D11_MAPPED_SUBRESOURCE Resource = { 0 };
    Resource.pData = (void *)1910221919;

    EXPECT_CALL(DeviceContext, Mapped(&CopyTexture2D, D3D11_MAP_READ))
      .Times(1)
      .WillOnce(Return(Resource));

    EXPECT_CALL(DeviceContext, Map(0, 0))
      .Times(1)
      .WillOnce(Return(S_OK));

    EXPECT_CALL(oMapperProxy, Map(Resource.pData))
      .Times(1);

    EXPECT_CALL(DeviceContext, Unmap(&CopyTexture2D, 0))
      .Times(1);

    EXPECT_CALL(DeviceContext, PSSetShaderResources(Eq(_Slot),
      ::std::vector<ID3D11ShaderResourceView *>{ &ShaderResourceView }))
      .Times(1);

    Render();

    ////////////////////////////////////////////////////////////////////////////

    //EXPECT_CALL(ShaderResourceView, Release())
    //  .Times(1);

    //EXPECT_CALL(CopyTexture2D, Release())
    //  .Times(1);

    //EXPECT_CALL(Texture2D, Release())
    //  .Times(1);
  };

  for (int Index = 0; Index < 8; Index++)
  {
    ::std::vector<Tested_t::Texture::Ptr_t> Textures;

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("index"), Index },
          { uT("mapper"), Mapper },
        });

      const auto pTexture = ::std::make_shared<Tested_t::Texture>(pData);
      Textures.push_back(pTexture);
      (*pData)[uT("entity")] = TextureWeakPtr_t{ pTexture };

      TestCall(pData, pTexture, Index);
    }

    const ::std::vector<String_t> Destinations =
    {
      uT("albedo"),
      uT("metalness"),
      uT("roughness"),
      uT("normal"),
      uT("occlusion"),
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("index"), Index },
          { uT("destination"), Destinations.at(i) },
          { uT("mapper"), Mapper },
        });

      const auto pTexture = ::std::make_shared<Tested_t::Texture>(pData);
      Textures.push_back(pTexture);
      (*pData)[uT("entity")] = TextureWeakPtr_t{ pTexture };

      TestCall(pData, pTexture, Index);
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_ReadData_Destination)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  class MapperProxy :
    public ::alicorn::extension::testing::Proxy<MapperProxy>
  {
  public:
    MOCK_METHOD1(Map, bool(const void *));
  };

  MapperProxy oMapperProxy;
  MapperProxy::GetInstance() = &oMapperProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;

  const BufferMapper_t Mapper = [&](const void * _pData)
  {
    return oMapperProxy.Map(_pData);
  };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  ::mock::DirectX11::Texture2D Texture2D;
  ::mock::DirectX11::Texture2D CopyTexture2D;
  ::mock::DirectX11::ShaderResourceView ShaderResourceView;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pData,
    const Tested_t::Texture::Ptr_t & _pTexture,
    const ::std::size_t _Slot)
  {
    InSequence Dummy;

    EXPECT_CALL(Device, CreateTexture2D(_, _))
      .Times(1)
      .WillOnce(Return(&Texture2D));

    EXPECT_CALL(Device, CreateShaderResourceView(&Texture2D, _))
      .Times(1)
      .WillOnce(Return(&ShaderResourceView));

    EXPECT_CALL(Device, CreateTexture2D(_, _))
      .Times(1)
      .WillOnce(Return(&CopyTexture2D));

    _pTexture->MakeTarget(&Device, 0, 0);

    EXPECT_CALL(Device, CreateTexture2D(_, _))
      .Times(0);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("service"), Object_t{ _pData } }
      }));
    ASSERT_NE(nullptr, Render);

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(DeviceContext, CopyResource(_, _))
      .Times(0);

    EXPECT_CALL(DeviceContext, PSSetShaderResources(Eq(_Slot),
      ::std::vector<ID3D11ShaderResourceView *>{ &ShaderResourceView }))
      .Times(1);

    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(true));

    EXPECT_CALL(DeviceContext, CopyResource(&CopyTexture2D, &Texture2D))
      .Times(1);

    EXPECT_CALL(DeviceContext, Mapped(&CopyTexture2D, D3D11_MAP_READ))
      .Times(1)
      .WillOnce(Return(D3D11_MAPPED_SUBRESOURCE{ 0 }));

    EXPECT_CALL(DeviceContext, Map(0, 0))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_CALL(oMapperProxy, Map(_))
      .Times(0);

    EXPECT_CALL(DeviceContext, Unmap(_, _))
      .Times(0);

    EXPECT_CALL(DeviceContext, PSSetShaderResources(_, _))
      .Times(0);

    EXPECT_THROW(Render(), ::std::exception);

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(true));

    EXPECT_CALL(DeviceContext, CopyResource(&CopyTexture2D, &Texture2D))
      .Times(1);

    D3D11_MAPPED_SUBRESOURCE Resource = { 0 };
    Resource.pData = (void *)1910221919;

    EXPECT_CALL(DeviceContext, Mapped(&CopyTexture2D, D3D11_MAP_READ))
      .Times(1)
      .WillOnce(Return(Resource));

    EXPECT_CALL(DeviceContext, Map(0, 0))
      .Times(1)
      .WillOnce(Return(S_OK));

    EXPECT_CALL(oMapperProxy, Map(Resource.pData))
      .Times(1);

    EXPECT_CALL(DeviceContext, Unmap(&CopyTexture2D, 0))
      .Times(1);

    EXPECT_CALL(DeviceContext, PSSetShaderResources(Eq(_Slot),
      ::std::vector<ID3D11ShaderResourceView *>{ &ShaderResourceView }))
      .Times(1);

    Render();

    ////////////////////////////////////////////////////////////////////////////

    //EXPECT_CALL(ShaderResourceView, Release())
    //  .Times(1);

    //EXPECT_CALL(CopyTexture2D, Release())
    //  .Times(1);

    //EXPECT_CALL(Texture2D, Release())
    //  .Times(1);
  };

  const ::std::vector<String_t> Destinations =
  {
    uT("albedo"),
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  ::std::vector<Tested_t::Texture::Ptr_t> Textures;

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), Destinations.at(i) },
        { uT("mapper"), Mapper },
      });

    const auto pTexture = ::std::make_shared<Tested_t::Texture>(pData);
    Textures.push_back(pTexture);
    (*pData)[uT("entity")] = TextureWeakPtr_t{ pTexture };

    TestCall(pData, pTexture, i);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_NotExistsEntryPoint)
{
  const ::std::string ShaderData
  {
    "Pixel vs(??? _Value)\r\n"
  };

  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  const auto pShader = Component_t::Make(
    {
      { uT("data"), (const uint8_t *)ShaderData.data() },
      { uT("count"), ShaderData.size() },
      { uT("entry"), ::std::string{ "unknown" } }
    });

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, Compile(_))
    .Times(0);

  EXPECT_STDEXCEPTION(itCreator->second(pShader),
    ".+\\.hpp\\([0-9]+\\): Entry point not found: unknown");
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_Compile_Fail)
{
  const ::std::string ShaderData
  {
    "Pixel vs(??? _Value)\r\n"
  };

  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Blob ErrorMessage;
  char * Error = "Error1809081119";

  const auto pShader = Component_t::Make(
    {
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
      { uT("entry"), ::std::string{ "vs" } }
    });

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CompileGetErrorMsg())
    .Times(1)
    .WillOnce(Return(&ErrorMessage));

  EXPECT_CALL(DirectXProxy, Compile(_))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(ErrorMessage, GetBufferPointer())
    .Times(1)
    .WillOnce(Return(Error));

  EXPECT_CALL(ErrorMessage, Release())
    .Times(1)
    .WillOnce(Return(0));

  EXPECT_STDEXCEPTION(itCreator->second(pShader),
    (".+\\.cpp\\([0-9]+\\): Failed: -2147467259 \\[header line: 172, " +
      ::std::string{ Error } +"\\]\\.").c_str());
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_CreateInputLayout_Fail)
{
  const ::std::string ShaderData =
    "Pixel vs1(Polygon _Value)\r\n"
    "Pixel vs2(Polyhedron _Value)\r\n"
    "Pixel vs3(Vertex _Value)\r\n";

  const auto TestCallRender = [&](const ::std::string & _Entry)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::Blob CompiledEffect;

    const auto pShader = Component_t::Make(
      { 
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
        { uT("entry"), _Entry }
      });

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Shader"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    InSequence Dummy;

    EXPECT_CALL(DirectXProxy, CompileGetCode())
      .Times(1)
      .WillOnce(Return(&CompiledEffect));

    EXPECT_CALL(Device, CreateInputLayout(_, _, _))
      .Times(1);

    EXPECT_CALL(Device, GetResult(Eq("CreateInputLayout")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_CALL(CompiledEffect, Release())
      .Times(1);

    EXPECT_THROW(itCreator->second(pShader), ::std::exception);
  };

  TestCallRender("vs1");
  TestCallRender("vs2");
  TestCallRender("vs3");
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_CreateVertexShader_Fail)
{
  const ::std::string ShaderData =
    "Pixel vs1(Polygon _Value)\r\n"
    "Pixel vs2(Polyhedron _Value)\r\n"
    "Pixel vs3(Vertex _Value)\r\n";

  const auto TestCallRender = [&](const ::std::string & _Entry)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::Blob CompiledEffect;

    const auto pShader = Component_t::Make(
      {
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
        { uT("entry"), _Entry }
      });

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Shader"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    InSequence Dummy;

    EXPECT_CALL(DirectXProxy, CompileGetCode())
      .Times(1)
      .WillOnce(Return(&CompiledEffect));

    EXPECT_CALL(Device, GetResult(_))
      .Times(AtLeast(1));

    EXPECT_CALL(Device, CreateVertexShader(_, _, _))
      .Times(1);

    EXPECT_CALL(Device, GetResult(Eq("CreateVertexShader")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_CALL(CompiledEffect, Release())
      .Times(1);

    EXPECT_THROW(itCreator->second(pShader), ::std::exception);
  };

  TestCallRender("vs1");
  TestCallRender("vs2");
  TestCallRender("vs3");
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_CreatePixelShader_Fail)
{
  const ::std::string ShaderData
  {
    "float4 ps1(??? _Value) : SV_Target\r\n"
    "float4 ps2(Unknown _Value) : SV_Target\r\n"
    "float4 ps3(Pixel _Value) : SV_Target\r\n"
  };

  const auto TestCallRender = [&](const ::std::string & _Entry)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::Blob CompiledEffect;

    const auto pShader = Component_t::Make(
      {
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
        { uT("entry"), _Entry }
      });

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Shader"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    InSequence Dummy;

    EXPECT_CALL(DirectXProxy, CompileGetCode())
      .Times(1)
      .WillOnce(Return(&CompiledEffect));

    EXPECT_CALL(Device, CreatePixelShader(_, _, _))
      .Times(1);

    EXPECT_CALL(Device, GetResult(Eq("CreatePixelShader")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_CALL(CompiledEffect, Release())
      .Times(1);

    EXPECT_THROW(itCreator->second(pShader), ::std::exception);
  };

  TestCallRender("ps1");
  TestCallRender("ps2");
  TestCallRender("ps3");
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_DefaultData)
{
  const ::std::string VertexShaderPrefix =
    "#define COVELLITE_SHADER_DESKTOP\r\n"
    "#define COVELLITE_SHADER_HLSL\r\n"
    "#define COVELLITE_SHADER_VERTEX\r\n";

  const ::std::string PixelShaderPrefix =
    "#define COVELLITE_SHADER_DESKTOP\r\n"
    "#define COVELLITE_SHADER_HLSL\r\n"
    "#define COVELLITE_SHADER_PIXEL\r\n";

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pShader,
    const ::std::string & _Entry,
    const ::std::string & _Version,
    const ::std::string & _Prefix,
    const ::std::string & _Postfix)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Blob CompiledEffect;

    using namespace ::alicorn::extension::std;
    using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;
    using Shader_t = ::covellite::api::renderer::DirectX::Shader;

    ::mock::DirectX11::CompileDesc Desc;
    Desc.SrcData = Shader_t::Convert(_Prefix) + ::Predefined + ::Data + 
      ::Input + ::Default + Shader_t::Convert(_Postfix);
    Desc.SourceName = "[Covellite::Api]: " + _Entry;
    Desc.pDefines = nullptr;
    Desc.pInclude = nullptr;
    Desc.Entrypoint = _Entry;
    Desc.Target = _Version;
    Desc.Flags1 = (IS_RELEASE_CONFIGURATION) ? 0 :
      D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    Desc.Flags2 = 0;

    using namespace ::testing;

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Shader"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    {
      InSequence Dummy;

      EXPECT_CALL(DirectXProxy, CompileGetCode())
        .Times(1)
        .WillOnce(Return(&CompiledEffect));

      EXPECT_CALL(DirectXProxy, Compile(Desc))
        .Times(1);

      EXPECT_CALL(CompiledEffect, Release())
        .Times(1);
    }

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);
  };

  for (const auto & Entry : { uT("vsFlat"), uT("vsVolume") })
  {
    using namespace ::alicorn::extension::std;

    const auto strEntry = string_cast<::std::string, Encoding::Ascii128>(Entry);

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("entry"), Entry },
        });

      TestCallRender(pComponent, strEntry, "vs_4_0",
        VertexShaderPrefix, "");
    }

    {
      const Object_t Data = { Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("entry"), Entry },
        }) };

      const auto pComponent = Component_t::Make({ { uT("service"), Data } });

      TestCallRender(pComponent, strEntry, "vs_4_0",
        VertexShaderPrefix, "");
    }
  }

  for (const auto & Entry : { uT("psColored"), uT("psTextured") })
  {
    using namespace ::alicorn::extension::std;

    const auto strEntry = string_cast<::std::string, Encoding::Ascii128>(Entry);

    const ::std::string PixelShaderPostfix =
      "float4 psMain(Pixel _Value) : SV_Target\r\n"
      "{\r\n"
      "  return " + strEntry + "(_Value);\r\n"
      "}\r\n";

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("entry"), Entry },
        });

      TestCallRender(pComponent, "psMain", "ps_4_0",
        PixelShaderPrefix, PixelShaderPostfix);
    }

    {
      const Object_t Data = { Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("entry"), Entry },
        }) };

      const auto pComponent = Component_t::Make({ { uT("service"), Data } });

      TestCallRender(pComponent, "psMain", "ps_4_0",
        PixelShaderPrefix, PixelShaderPostfix);
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_Vertex)
{
  const ::std::string VertexShaderPrefix =
    "#define COVELLITE_SHADER_DESKTOP\r\n"
    "#define COVELLITE_SHADER_HLSL\r\n"
    "#define COVELLITE_SHADER_VERTEX\r\n";

  const ::std::string ShaderData =
    "Pixel vs1(Polygon _Value)\r\n"
    "Pixel vs2(Polyhedron _Value)\r\n"
    "Pixel vs3_Dummy(Vertex _Value)\r\n"
    "Pixel vs3(Vertex _Value)\r\n";

  using InputDesc_t = ::std::vector<D3D11_INPUT_ELEMENT_DESC>;

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pShader, 
    const InputDesc_t & _Layout,
    const ::std::string & _EntryPoint)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::DeviceContext DeviceContext;
    ::mock::DirectX11::Blob CompiledEffect;
    ::mock::DirectX11::InputLayout InputLayout;
    ::mock::DirectX11::VertexShader VertexShader;

    using namespace ::alicorn::extension::std;
    using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;
    using Shader_t = ::covellite::api::renderer::DirectX::Shader;

    ::mock::DirectX11::CompileDesc Desc;
    Desc.SrcData = Shader_t::Convert(VertexShaderPrefix) + 
      ::Predefined + ::Data + ::Input + Shader_t::Convert(ShaderData);
    Desc.SourceName = "[Covellite::Api]: " + _EntryPoint;
    Desc.pDefines = nullptr;
    Desc.pInclude = nullptr;
    Desc.Entrypoint = _EntryPoint;
    Desc.Target = "vs_4_0";
    Desc.Flags1 = (IS_RELEASE_CONFIGURATION) ? 0 :
      D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    Desc.Flags2 = 0;

    void * const pData = (void *)1811221233;
    const size_t DataSize = 1811221234;

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(DirectXProxy, CreateDeviceContext())
      .Times(1)
      .WillOnce(Return(&DeviceContext));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Shader"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    {
      InSequence Dummy;

      EXPECT_CALL(DirectXProxy, CompileGetCode())
        .Times(1)
        .WillOnce(Return(&CompiledEffect));

      EXPECT_CALL(DirectXProxy, Compile(Desc))
        .Times(1);

      EXPECT_CALL(CompiledEffect, GetBufferPointer())
        .Times(1)
        .WillOnce(Return(pData));

      EXPECT_CALL(CompiledEffect, GetBufferSize())
        .Times(1)
        .WillOnce(Return(DataSize));

      EXPECT_CALL(Device, CreateInputLayout(_Layout, pData, DataSize))
        .Times(1)
        .WillOnce(Return(&InputLayout));

      EXPECT_CALL(Device, CreateVertexShader(pData, DataSize, nullptr))
        .Times(1)
        .WillOnce(Return(&VertexShader));

      EXPECT_CALL(CompiledEffect, Release())
        .Times(1);
    }

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, IASetInputLayout(&InputLayout))
      .Times(1);

    EXPECT_CALL(DeviceContext, VSSetShader(&VertexShader, nullptr, 0))
      .Times(1);

    Render();

    EXPECT_CALL(InputLayout, Release())
      .Times(1);

    EXPECT_CALL(VertexShader, Release())
      .Times(1);
  };

  {
    const ::std::vector<D3D11_INPUT_ELEMENT_DESC> Layout =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32_UINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), uT("vs1") },
        });

      TestCallRender(pComponent, Layout, "vs1");
    }

    {
      const Object_t Data = { Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), uT("vs1") },
        }) };

      const auto pComponent = Component_t::Make({ { uT("service"), Data } });

      TestCallRender(pComponent, Layout, "vs1");
    }
  }

  {
    const ::std::vector<D3D11_INPUT_ELEMENT_DESC> Layout =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), uT("vs2") },
        });

      TestCallRender(pComponent, Layout, "vs2");
    }

    {
      const Object_t Data = { Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), uT("vs2") },
        }) };

      const auto pComponent = Component_t::Make({ { uT("service"), Data } });

      TestCallRender(pComponent, Layout, "vs2");
    }
  }

  {
    const ::std::vector<D3D11_INPUT_ELEMENT_DESC> Layout =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), uT("vs3") },
        });

      TestCallRender(pComponent, Layout, "vs3");
    }

    {
      const Object_t Data = { Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), uT("vs3") },
        }) };

      const auto pComponent = Component_t::Make({ { uT("service"), Data } });

      TestCallRender(pComponent, Layout, "vs3");
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_CreateVertex_Instance_InvalidValue)
{
  const ::std::string ShaderData =
    "Pixel vs1(Vertex _Value)\r\n"
    "Pixel vs2(Vertex _Value)\r\n";

  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader)
  {
    EXPECT_CALL(DirectXProxy, Compile(_))
      .Times(0);

    EXPECT_THROW(itCreator->second(_pShader), ::std::exception);
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("vs1") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
        { uT("instance"), uT("invalid1909231417") },
      });

    TestCall(pShader);
  }

  {
    const Object_t Data = { Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("vs2") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
        { uT("instance"), uT("invalid1909231418") },
      }) };

    TestCall(Component_t::Make({ { uT("service"), Data } }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_CreateVertex_Instance)
{
  using InputDesc_t = ::std::vector<D3D11_INPUT_ELEMENT_DESC>;

  const ::std::string VertexShaderPrefix =
    "#define COVELLITE_SHADER_DESKTOP\r\n"
    "#define COVELLITE_SHADER_HLSL\r\n"
    "#define COVELLITE_SHADER_VERTEX\r\n";

  const ::std::string ShaderData =
    "Pixel vs1(Vertex _Value)\r\n"
    "Pixel vs2(Vertex _Value)\r\n";

  ::std::string InputData{ ::Input.cbegin(), ::Input.cend() };

  ::boost::algorithm::replace_first(InputData,
    "/* place for instance variables */", ::std::string{} +
    "COVELLITE_IN float4 iValue1 COVELLITE_INPUT_SEMANTIC(TEXCOORD1);" + (char)0x5C + "\r\n"
    "COVELLITE_IN int4 iValue2 COVELLITE_INPUT_SEMANTIC(TEXCOORD2);" + (char)0x5C + "\r\n"
    "COVELLITE_IN int4 iValue3 COVELLITE_INPUT_SEMANTIC(TEXCOORD3);" + (char)0x5C + "\r\n"
    "COVELLITE_IN float4 iValue4 COVELLITE_INPUT_SEMANTIC(TEXCOORD4);" + (char)0x5C + "\r\n"
  );

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pShader,
    const InputDesc_t & _Layout,
    const ::std::string & _EntryPoint)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::DeviceContext DeviceContext;
    ::mock::DirectX11::Blob CompiledEffect;
    ::mock::DirectX11::InputLayout InputLayout;
    ::mock::DirectX11::VertexShader VertexShader;

    using namespace ::alicorn::extension::std;
    using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;
    using Shader_t = ::covellite::api::renderer::DirectX::Shader;

    ::mock::DirectX11::CompileDesc Desc;
    Desc.SrcData = Shader_t::Convert(VertexShaderPrefix) +
      ::Predefined + ::Data + Shader_t::Convert(InputData + ShaderData);
    Desc.SourceName = "[Covellite::Api]: " + _EntryPoint;
    Desc.pDefines = nullptr;
    Desc.pInclude = nullptr;
    Desc.Entrypoint = _EntryPoint;
    Desc.Target = "vs_4_0";
    Desc.Flags1 = (IS_RELEASE_CONFIGURATION) ? 0 :
      D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    Desc.Flags2 = 0;

    void * const pData = (void *)1811221233;
    const size_t DataSize = 1811221234;

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(DirectXProxy, CreateDeviceContext())
      .Times(1)
      .WillOnce(Return(&DeviceContext));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Shader"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    {
      InSequence Dummy;

      EXPECT_CALL(DirectXProxy, CompileGetCode())
        .Times(1)
        .WillOnce(Return(&CompiledEffect));

      EXPECT_CALL(DirectXProxy, Compile(Desc))
        .Times(1);

      EXPECT_CALL(CompiledEffect, GetBufferPointer())
        .Times(1)
        .WillOnce(Return(pData));

      EXPECT_CALL(CompiledEffect, GetBufferSize())
        .Times(1)
        .WillOnce(Return(DataSize));

      EXPECT_CALL(Device, CreateInputLayout(_Layout, pData, DataSize))
        .Times(1)
        .WillOnce(Return(&InputLayout));

      EXPECT_CALL(Device, CreateVertexShader(pData, DataSize, nullptr))
        .Times(1)
        .WillOnce(Return(&VertexShader));

      EXPECT_CALL(CompiledEffect, Release())
        .Times(1);
    }

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, IASetInputLayout(&InputLayout))
      .Times(1);

    EXPECT_CALL(DeviceContext, VSSetShader(&VertexShader, nullptr, 0))
      .Times(1);

    Render();

    EXPECT_CALL(InputLayout, Release())
      .Times(1);

    EXPECT_CALL(VertexShader, Release())
      .Times(1);
  };

  const ::std::vector<D3D11_INPUT_ELEMENT_DESC> Layout =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16 * 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_SINT,  1, 16 * 1, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_SINT,  1, 16 * 2, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    { "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16 * 3, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
  };

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
        { uT("entry"), uT("vs1") },
        { uT("instance"), uT("f4i4i4f4") },
      });

    TestCallRender(pComponent, Layout, "vs1");
  }

  {
    const Object_t Data = { Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
        { uT("entry"), uT("vs2") },
        { uT("instance"), uT("f4i4i4f4") },
      }) };

    TestCallRender(Component_t::Make({ { uT("service"), Data } }), Layout, "vs2");
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_Pixel)
{
  const ::std::string PixelShaderPrefix =
    "#define COVELLITE_SHADER_DESKTOP\r\n"
    "#define COVELLITE_SHADER_HLSL\r\n"
    "#define COVELLITE_SHADER_PIXEL\r\n";

  const ::std::string ShaderData =
    "float4 ps1(Pixel _Value)\r\n"
    "vec4 ps2_Dummy(Pixel _Value)\r\n"
    "vec4 ps2(Pixel _Value)\r\n";

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pShader,
    const ::std::string & _Entry)
  {
    const ::std::string PixelShaderPostfix =
      "float4 psMain(Pixel _Value) : SV_Target\r\n"
      "{\r\n"
      "  return " + _Entry + "(_Value);\r\n"
      "}\r\n";

    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::DeviceContext DeviceContext;
    ::mock::DirectX11::Blob CompiledEffect;
    ::mock::DirectX11::PixelShader PixelShader;

    using namespace ::alicorn::extension::std;
    using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;
    using Shader_t = ::covellite::api::renderer::DirectX::Shader;

    ::mock::DirectX11::CompileDesc Desc;
    Desc.SrcData = Shader_t::Convert(PixelShaderPrefix) + ::Predefined +
      ::Data + ::Input + Shader_t::Convert(ShaderData + PixelShaderPostfix);
    Desc.SourceName = "[Covellite::Api]: psMain";
    Desc.pDefines = nullptr;
    Desc.pInclude = nullptr;
    Desc.Entrypoint = "psMain";
    Desc.Target = "ps_4_0";
    Desc.Flags1 = (IS_RELEASE_CONFIGURATION) ? 0 :
      D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    Desc.Flags2 = 0;

    void * const pData = (void *)1809071822;
    const size_t DataSize = 1809071823;

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(DirectXProxy, CreateDeviceContext())
      .Times(1)
      .WillOnce(Return(&DeviceContext));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Shader"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    {
      InSequence Dummy;

      EXPECT_CALL(DirectXProxy, CompileGetCode())
        .Times(1)
        .WillOnce(Return(&CompiledEffect));

      EXPECT_CALL(DirectXProxy, Compile(Desc))
        .Times(1);

      EXPECT_CALL(CompiledEffect, GetBufferSize())
        .Times(1)
        .WillOnce(Return(DataSize));

      EXPECT_CALL(CompiledEffect, GetBufferPointer())
        .Times(1)
        .WillOnce(Return(pData));

      EXPECT_CALL(Device, CreatePixelShader(pData, DataSize, nullptr))
        .Times(1)
        .WillOnce(Return(&PixelShader));

      EXPECT_CALL(CompiledEffect, Release())
        .Times(1);
    }

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, PSSetShader(&PixelShader, nullptr, 0))
      .Times(1);

    Render();

    EXPECT_CALL(PixelShader, Release())
      .Times(1);
  };

  for (const auto & Entry : { uT("ps1"), uT("ps2") })
  {
    using namespace ::alicorn::extension::std;

    const auto strEntry = string_cast<::std::string, Encoding::Ascii128>(Entry);

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), Entry },
        });

      TestCallRender(pComponent, strEntry);
    }

    {
      const Object_t Data = { Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), Entry },
        }) };

      TestCallRender(Component_t::Make({ { uT("service"), Data } }), strEntry);
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_Pixel_NoReturn)
{
  const ::std::string PixelShaderPrefix =
    "#define COVELLITE_SHADER_DESKTOP\r\n"
    "#define COVELLITE_SHADER_HLSL\r\n"
    "#define COVELLITE_SHADER_PIXEL\r\n";

  const ::std::string ShaderData =
    "void ps1(Pixel _Value)\r\n";

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pShader,
    const ::std::string & _Entry)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::DeviceContext DeviceContext;
    ::mock::DirectX11::Blob CompiledEffect;
    ::mock::DirectX11::PixelShader PixelShader;

    using namespace ::alicorn::extension::std;
    using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;
    using Shader_t = ::covellite::api::renderer::DirectX::Shader;

    ::mock::DirectX11::CompileDesc Desc;
    Desc.SrcData = Shader_t::Convert(PixelShaderPrefix) + ::Predefined +
      ::Data + ::Input + Shader_t::Convert(ShaderData);
    Desc.SourceName = "[Covellite::Api]: " + _Entry;
    Desc.pDefines = nullptr;
    Desc.pInclude = nullptr;
    Desc.Entrypoint = _Entry;
    Desc.Target = "ps_4_0";
    Desc.Flags1 = (IS_RELEASE_CONFIGURATION) ? 0 :
      D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    Desc.Flags2 = 0;

    void * const pData = (void *)1809071822;
    const size_t DataSize = 1809071823;

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(DirectXProxy, CreateDeviceContext())
      .Times(1)
      .WillOnce(Return(&DeviceContext));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Shader"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    {
      InSequence Dummy;

      EXPECT_CALL(DirectXProxy, CompileGetCode())
        .Times(1)
        .WillOnce(Return(&CompiledEffect));

      EXPECT_CALL(DirectXProxy, Compile(Desc))
        .Times(1);

      EXPECT_CALL(CompiledEffect, GetBufferSize())
        .Times(1)
        .WillOnce(Return(DataSize));

      EXPECT_CALL(CompiledEffect, GetBufferPointer())
        .Times(1)
        .WillOnce(Return(pData));

      EXPECT_CALL(Device, CreatePixelShader(pData, DataSize, nullptr))
        .Times(1)
        .WillOnce(Return(&PixelShader));

      EXPECT_CALL(CompiledEffect, Release())
        .Times(1);
    }

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, PSSetShader(&PixelShader, nullptr, 0))
      .Times(1);

    Render();

    EXPECT_CALL(PixelShader, Release())
      .Times(1);
  };

  for (const auto & Entry : { uT("ps1") })
  {
    using namespace ::alicorn::extension::std;

    const auto strEntry = string_cast<::std::string, Encoding::Ascii128>(Entry);

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), Entry },
        });

      TestCallRender(pComponent, "ps1");
    }

    {
      const Object_t Data = { Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), Entry },
        }) };

      TestCallRender(Component_t::Make({ { uT("service"), Data } }), "ps1");
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_Pixel_Multitarget)
{
  const ::std::string PixelShaderPrefix =
    "#define COVELLITE_SHADER_DESKTOP\r\n"
    "#define COVELLITE_SHADER_HLSL\r\n"
    "#define COVELLITE_SHADER_PIXEL\r\n";

  const ::std::string ShaderData =
    "PsOutput ps1910101750(Pixel _Value)\r\n";

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pShader,
    const ::std::string & _Entry)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::DeviceContext DeviceContext;
    ::mock::DirectX11::Blob CompiledEffect;
    ::mock::DirectX11::PixelShader PixelShader;

    using namespace ::alicorn::extension::std;
    using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;
    using Shader_t = ::covellite::api::renderer::DirectX::Shader;

    ::mock::DirectX11::CompileDesc Desc;
    Desc.SrcData = Shader_t::Convert(PixelShaderPrefix) + ::Predefined +
      ::Data + ::Input + Shader_t::Convert(ShaderData);
    Desc.SourceName = "[Covellite::Api]: " + _Entry;
    Desc.pDefines = nullptr;
    Desc.pInclude = nullptr;
    Desc.Entrypoint = _Entry;
    Desc.Target = "ps_4_0";
    Desc.Flags1 = (IS_RELEASE_CONFIGURATION) ? 0 :
      D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    Desc.Flags2 = 0;

    void * const pData = (void *)1809071822;
    const size_t DataSize = 1809071823;

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(DirectXProxy, CreateDeviceContext())
      .Times(1)
      .WillOnce(Return(&DeviceContext));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Shader"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    {
      InSequence Dummy;

      EXPECT_CALL(DirectXProxy, CompileGetCode())
        .Times(1)
        .WillOnce(Return(&CompiledEffect));

      EXPECT_CALL(DirectXProxy, Compile(Desc))
        .Times(1);

      EXPECT_CALL(CompiledEffect, GetBufferSize())
        .Times(1)
        .WillOnce(Return(DataSize));

      EXPECT_CALL(CompiledEffect, GetBufferPointer())
        .Times(1)
        .WillOnce(Return(pData));

      EXPECT_CALL(Device, CreatePixelShader(pData, DataSize, nullptr))
        .Times(1)
        .WillOnce(Return(&PixelShader));

      EXPECT_CALL(CompiledEffect, Release())
        .Times(1);
    }

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, PSSetShader(&PixelShader, nullptr, 0))
      .Times(1);

    Render();

    EXPECT_CALL(PixelShader, Release())
      .Times(1);
  };

  for (const auto & Entry : { uT("ps1910101750") })
  {
    using namespace ::alicorn::extension::std;

    const auto strEntry = string_cast<::std::string, Encoding::Ascii128>(Entry);

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), Entry },
        });

      TestCallRender(pComponent, strEntry);
    }

    {
      const Object_t Data = { Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("content"), ::std::vector<uint8_t>{ ShaderData.cbegin(), ShaderData.cend() } },
          { uT("entry"), Entry },
        }) };

      TestCallRender(Component_t::Make({ { uT("service"), Data } }), strEntry);
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_CreateVertex_Fail)
{
  const auto TestCallRender = [](const Component_t::ComponentPtr_t _pBuffer)
  {
    ::mock::DirectX11::Proxy DirectXProxy;
    ::mock::DirectX11::Device Device;

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Buffer"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    InSequence Dummy;

    EXPECT_CALL(Device, CreateBuffer(_))
      .Times(1);

    EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(_pBuffer), ::std::exception);
  };

  const ::std::vector<::covellite::api::Vertex> VertexData;

  {
    {
      const auto pComponent = Component_t::Make(
        {
          { uT("content"), VertexData },
        });

      TestCallRender(pComponent);
    }

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Buffer") },
          { uT("content"), VertexData },
        });

      TestCallRender(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
    }
  }

  {
    using BufferMapper_t = ::covellite::api::cbBufferMap_t<::covellite::api::Vertex>;
    const BufferMapper_t Dummy = [](::covellite::api::Vertex *) { return false; };

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), VertexData },
      });

    const auto pComponent = Component_t::Make(
      {
        { uT("mapper"), Dummy },
        { uT("service"), Object_t{ pData } }
      });

    TestCallRender(pComponent);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_Vertex_Static)
{
  const auto TestCallRender = [](
    const Component_t::ComponentPtr_t & _pBuffer, 
    const auto & _RawData,
    size_t _VertexSize)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::DeviceContext DeviceContext;
    ::mock::DirectX11::Buffer Buffer;

    D3D11_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _RawData.data();

    D3D11_BUFFER_DESC Desc = { 0 };
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.ByteWidth = static_cast<UINT>(_RawData.size() * _VertexSize);
    Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(DirectXProxy, CreateDeviceContext())
      .Times(1)
      .WillOnce(Return(&DeviceContext));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Buffer"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    InSequence Dummy;

    EXPECT_CALL(Device, CreateBuffer(Desc, InitData))
      .Times(1)
      .WillOnce(Return(&Buffer));

    auto Render = itCreator->second(_pBuffer);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, IASetVertexBuffers(0, 1, &Buffer,
      static_cast<UINT>(_VertexSize), 0))
      .Times(1);

    Render();

    EXPECT_CALL(Buffer, Release())
      .Times(1);
  };

  {
    ::std::vector<::covellite::api::Vertex> VertexData =
    {
      {
        1.0f, 2.0f, 3.0f, 4.0f, 
        5.0f, 6.0f,
        7.0f, 8.0f, 9.0f, 0.0f
      },
      {
        10.0f, 11.0f, 12.0f, 13.0f, 
        14.0f, 15.0f, 
        16.0f, 17.0f, 18.0f, 19.0f
      },
    };
    IntroduceBufferSize(VertexData);

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("content"), VertexData },
        });

      TestCallRender(pComponent, VertexData, sizeof(::covellite::api::Vertex));
    }

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Buffer") },
          { uT("content"), VertexData },
        });

      const auto pComponent = Component_t::Make(
        { 
          { uT("service"), Object_t{ pData } } 
        });

      TestCallRender(pComponent, VertexData, sizeof(::covellite::api::Vertex));
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_Vertex_Dynamic)
{
  using BufferMapper_t = 
    ::covellite::api::cbBufferMap_t<::covellite::api::Vertex>;

  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Mapper, bool(::covellite::api::Vertex *));
  };

  const auto TestCallRender = [](
    const Component_t::ComponentPtr_t & _pBuffer,
    const void * _pRawData, size_t _RawDataSize, size_t _VertexSize)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    Proxy oProxy;
    Proxy::GetInstance() = &oProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::DeviceContext DeviceContext;
    ::mock::DirectX11::Buffer Buffer;

    D3D11_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pRawData;

    D3D11_BUFFER_DESC Desc = { 0 };
    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    Desc.ByteWidth = static_cast<UINT>(_RawDataSize * _VertexSize);
    Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    const BufferMapper_t Mapper = [&](::covellite::api::Vertex * _pData)
    {
      return oProxy.Mapper(_pData);
    };

    (*_pBuffer)[uT("mapper")] = Mapper;

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(DirectXProxy, CreateDeviceContext())
      .Times(1)
      .WillOnce(Return(&DeviceContext));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Buffer"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    InSequence Dummy;

    EXPECT_CALL(Device, CreateBuffer(Desc, InitData))
      .Times(1)
      .WillOnce(Return(&Buffer));

    auto Render = itCreator->second(_pBuffer);
    ASSERT_NE(nullptr, Render);

    {
      EXPECT_CALL(oProxy, Mapper(nullptr))
        .Times(1)
        .WillOnce(Return(false));

      EXPECT_CALL(DeviceContext, Map(_, _))
        .Times(0);

      EXPECT_CALL(oProxy, Mapper(_))
        .Times(0);

      EXPECT_CALL(DeviceContext, Unmap(_, _))
        .Times(0);

      EXPECT_CALL(DeviceContext, IASetVertexBuffers(0, 1, &Buffer,
        static_cast<UINT>(_VertexSize), 0))
        .Times(1);

      Render();
    }

    {
      D3D11_MAPPED_SUBRESOURCE Resource = { 0 };

      EXPECT_CALL(oProxy, Mapper(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(DeviceContext, Mapped(_, _))
        .Times(1)
        .WillOnce(Return(Resource));

      EXPECT_CALL(DeviceContext, Map(_, _))
        .Times(1)
        .WillOnce(Return(E_FAIL));

      EXPECT_CALL(oProxy, Mapper(_))
        .Times(0);

      EXPECT_CALL(DeviceContext, Unmap(_, _))
        .Times(0);

      EXPECT_CALL(DeviceContext, IASetVertexBuffers(_, _, _, _, _))
        .Times(0);

      EXPECT_THROW(Render(), ::std::exception);
    }

    {
      D3D11_MAPPED_SUBRESOURCE Resource = { 0 };
      Resource.pData = (void *)1908011249;

      EXPECT_CALL(oProxy, Mapper(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(DeviceContext, Mapped(&Buffer, D3D11_MAP_WRITE_NO_OVERWRITE))
        .Times(1)
        .WillOnce(Return(Resource));

      EXPECT_CALL(DeviceContext, Map(0, 0))
        .Times(1)
        .WillOnce(Return(S_OK));

      EXPECT_CALL(oProxy, 
        Mapper(reinterpret_cast<::covellite::api::Vertex *>(Resource.pData)))
        .Times(1);

      EXPECT_CALL(DeviceContext, Unmap(&Buffer, 0))
        .Times(1);

      EXPECT_CALL(DeviceContext, IASetVertexBuffers(0, 1, &Buffer,
        static_cast<UINT>(_VertexSize), 0))
        .Times(1);

      Render();
    }

    EXPECT_CALL(Buffer, Release())
      .Times(AtLeast(1));
  };

  ::std::vector<::covellite::api::Vertex> VertexData =
  {
    {
      1.0f, 2.0f, 3.0f, 4.0f, 
      5.0f, 6.0f,
      7.0f, 8.0f, 9.0f, 0.0f
    },
    {
      10.0f, 11.0f, 12.0f, 13.0f, 
      14.0f, 15.0f, 
      16.0f, 17.0f, 18.0f, 19.0f
    },
  };
  IntroduceBufferSize(VertexData);

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("content"), VertexData },
      });

    TestCallRender(pComponent,
      VertexData.data(), VertexData.size(), sizeof(::covellite::api::Vertex));
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), VertexData },
      });

    const auto pComponent = Component_t::Make({ { uT("service"), Object_t{ pData } } });

    TestCallRender(pComponent,
      VertexData.data(), VertexData.size(), sizeof(::covellite::api::Vertex));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Transform_Default)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::ObjectBuffer ConstantBuffer;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::Object);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  D3D11_BUFFER_DESC IndexBufferDesc = { 0 };
  IndexBufferDesc.ByteWidth = 1811221344;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, VSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, PSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBuffer(ConstantBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&ConstantBuffer));

  EXPECT_CALL(DeviceContext,
    VSSetConstantBuffers(COVELLITE_BUFFER_INDEX_OBJECT, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    PSSetConstantBuffers(COVELLITE_BUFFER_INDEX_OBJECT, 1, &ConstantBuffer))
    .Times(1);

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });
  const auto pTransform = Component_t::Make(
    {
      { uT("service"), Object_t{ pPosition, pRotation, pScale } }
    });

  const auto Render = itCreator->second(pTransform);
  ASSERT_NE(nullptr, Render);

  auto WorldMatrix = ::glm::identity<::glm::mat4>();

  auto TestCallRender = [&](
    float _X, float _Y, float _Z,
    float _A, float _B, float _C,
    float _Sx, float _Sy, float _Sz)
  {
    (*pPosition)[uT("x")] = _X;
    (*pPosition)[uT("y")] = _Y;
    (*pPosition)[uT("z")] = _Z;

    (*pRotation)[uT("x")] = _A;
    (*pRotation)[uT("y")] = _B;
    (*pRotation)[uT("z")] = _C;

    (*pScale)[uT("x")] = _Sx;
    (*pScale)[uT("y")] = _Sy;
    (*pScale)[uT("z")] = _Sz;

    WorldMatrix = ::glm::identity<::glm::mat4>();
    WorldMatrix = ::glm::scale(WorldMatrix, { _Sx, _Sy, _Sz });
    WorldMatrix = ::glm::rotate(WorldMatrix, _C, { 0.0f, 0.0f, 1.0f });
    WorldMatrix = ::glm::rotate(WorldMatrix, _B, { 0.0f, 1.0f, 0.0f });
    WorldMatrix = ::glm::rotate(WorldMatrix, _A, { 1.0f, 0.0f, 0.0f });
    WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });
    WorldMatrix = ::glm::transpose(WorldMatrix);

    ::Object ExpectMatrices;
    memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));
    ExpectMatrices.World = WorldMatrix;

    EXPECT_CALL(DeviceContext, UpdateSubresource(&ConstantBuffer, 0, nullptr,
      ExpectMatrices, 0, 0))
      .Times(1);

    Render();
  };

  // Два вызова, чтобы убедиться, что изменение исходных данных приводит 
  // к изменению результата рендеринга.

  TestCallRender(
    1956.0f, 1957.0f, 1958.0f,
    1204.0f, 1205.0f, 1206.0f,
    1152.0f, 1153.0f, 1154.0f);

  TestCallRender(
    1959.0f, 1960.0f, 1961.0f,
    1145.0f, 1146.0f, 1147.0f,
    1155.0f, 1157.0f, 1158.0f);

  EXPECT_CALL(ConstantBuffer, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Transform_Static)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::ObjectBuffer ConstantBuffer;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::Object);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  D3D11_BUFFER_DESC IndexBufferDesc = { 0 };
  IndexBufferDesc.ByteWidth = 1811221344;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, VSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, PSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBuffer(ConstantBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&ConstantBuffer));

  EXPECT_CALL(DeviceContext,
    VSSetConstantBuffers(COVELLITE_BUFFER_INDEX_OBJECT, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    PSSetConstantBuffers(COVELLITE_BUFFER_INDEX_OBJECT, 1, &ConstantBuffer))
    .Times(1);

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pTransform)
  {
    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    auto SetValues = [&](const bool _IsBuildMatrix,
      float _X, float _Y, float _Z,
      float _A, float _B, float _C,
      float _Sx, float _Sy, float _Sz)
    {
      (*pPosition)[uT("x")] = _X;
      (*pPosition)[uT("y")] = _Y;
      (*pPosition)[uT("z")] = _Z;

      (*pRotation)[uT("x")] = _A;
      (*pRotation)[uT("y")] = _B;
      (*pRotation)[uT("z")] = _C;

      (*pScale)[uT("x")] = _Sx;
      (*pScale)[uT("y")] = _Sy;
      (*pScale)[uT("z")] = _Sz;

      if (_IsBuildMatrix)
      {
        WorldMatrix = ::glm::identity<::glm::mat4>();
        WorldMatrix = ::glm::scale(WorldMatrix, { _Sx, _Sy, _Sz });
        WorldMatrix = ::glm::rotate(WorldMatrix, _C, { 0.0f, 0.0f, 1.0f });
        WorldMatrix = ::glm::rotate(WorldMatrix, _B, { 0.0f, 1.0f, 0.0f });
        WorldMatrix = ::glm::rotate(WorldMatrix, _A, { 1.0f, 0.0f, 0.0f });
        WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });
        WorldMatrix = ::glm::transpose(WorldMatrix);
      }
    };

    SetValues(true,
      1956.0f, 1957.0f, 1958.0f,
      1204.0f, 1205.0f, 1206.0f,
      1152.0f, 1153.0f, 1154.0f);

    const auto Render = itCreator->second(_pTransform);
    ASSERT_NE(nullptr, Render);

    auto TestCallRender = [&](void)
    {
      ::Object ExpectMatrices;
      memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));
      ExpectMatrices.World = WorldMatrix;

      EXPECT_CALL(DeviceContext,
        UpdateSubresource(&ConstantBuffer, 0, nullptr, ExpectMatrices, 0, 0))
        .Times(1);

      Render();
    };

    // Два вызова, чтобы убедиться, что изменение исходных данных не приводит 
    // к изменению результата рендеринга.

    TestCallRender();

    SetValues(false,
      1959.0f, 1960.0f, 1961.0f,
      1145.0f, 1146.0f, 1147.0f,
      1155.0f, 1157.0f, 1158.0f);

    TestCallRender();
  };

  const auto pTransform = Component_t::Make(
    {
      { uT("kind"), uT("Static") },
      { uT("service"), Object_t{ pPosition, pRotation, pScale } }
    });

  TestCallRender(pTransform);

  EXPECT_CALL(ConstantBuffer, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Transform_Billboard)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::ObjectBuffer ConstantBuffer;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::Object);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  D3D11_BUFFER_DESC IndexBufferDesc = { 0 };
  IndexBufferDesc.ByteWidth = 1908062000;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, VSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, PSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBuffer(ConstantBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&ConstantBuffer));

  EXPECT_CALL(DeviceContext,
    VSSetConstantBuffers(COVELLITE_BUFFER_INDEX_OBJECT, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    PSSetConstantBuffers(COVELLITE_BUFFER_INDEX_OBJECT, 1, &ConstantBuffer))
    .Times(1);

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pTransform)
  {
    const auto Render = itCreator->second(_pTransform);
    ASSERT_NE(nullptr, Render);

    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    auto TestCallRender = [&](float _X, float _Y, float _Z)
    {
      (*pPosition)[uT("x")] = _X;
      (*pPosition)[uT("y")] = _Y;
      (*pPosition)[uT("z")] = _Z;

      WorldMatrix = ::glm::identity<::glm::mat4>();
      WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });

      const auto ViewMatrix = ::glm::mat4
      {
        { 1.0f, 2.0f, 3.0f, 4.0f },
        { 5.0f, 6.0f, 7.0f, 8.0f },
        { 9.0f, 10.0f, 11.0f, 12.0f },
        { 13.0f, 14.0f, 15.0f, 16.0f },
      };

      GetCameraMartix(Example).View = ViewMatrix;

      auto TransposeViewMatrix = ViewMatrix;
      TransposeViewMatrix[0][3] = 0.0f;
      TransposeViewMatrix[1][3] = 0.0f;
      TransposeViewMatrix[2][3] = 0.0f;
      TransposeViewMatrix[3][0] = 0.0f;
      TransposeViewMatrix[3][1] = 0.0f;
      TransposeViewMatrix[3][2] = 0.0f;
      TransposeViewMatrix[3][3] = 1.0f;

      WorldMatrix = TransposeViewMatrix *
        ::glm::translate(::glm::identity<::glm::mat4>(), { _X, _Y, _Z });
      WorldMatrix = ::glm::transpose(WorldMatrix);

      ::Object ExpectMatrices;
      memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));
      ExpectMatrices.World = WorldMatrix;

      EXPECT_CALL(DeviceContext,
        UpdateSubresource(&ConstantBuffer, 0, nullptr, ExpectMatrices, 0, 0))
        .Times(1);

      Render();
    };

    // Два вызова, чтобы убедиться, что изменение исходных данных приводит 
    // к изменению результата рендеринга.

    TestCallRender(1956.0f, 1957.0f, 1958.0f);
    TestCallRender(1959.0f, 1960.0f, 1961.0f);
  };

  const auto pTransform = Component_t::Make(
    {
      { uT("kind"), uT("Billboard") },
      { uT("service"), Object_t{ pPosition } }
    });

  TestCallRender(pTransform);

  EXPECT_CALL(ConstantBuffer, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Transform_DefaultValues)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::ObjectBuffer ConstantBuffer;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::Object);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  D3D11_BUFFER_DESC IndexBufferDesc = { 0 };
  IndexBufferDesc.ByteWidth = 1908062000;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, VSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, PSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBuffer(ConstantBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&ConstantBuffer));

  EXPECT_CALL(DeviceContext,
    VSSetConstantBuffers(COVELLITE_BUFFER_INDEX_OBJECT, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    PSSetConstantBuffers(COVELLITE_BUFFER_INDEX_OBJECT, 1, &ConstantBuffer))
    .Times(1);

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent,
    const bool _IsFullTransform = true)
  {
    const auto Render = itCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    const auto ViewMatrix = ::glm::mat4
    {
      { 1.0f, 2.0f, 3.0f, 4.0f },
      { 5.0f, 6.0f, 7.0f, 8.0f },
      { 9.0f, 10.0f, 11.0f, 12.0f },
      { 13.0f, 14.0f, 15.0f, 16.0f },
    };

    GetCameraMartix(Example).View = ViewMatrix;

    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    if (_IsFullTransform)
    {
      WorldMatrix = ::glm::scale(WorldMatrix, { 1.0f, 1.0f, 1.0f });
      WorldMatrix = ::glm::transpose(WorldMatrix);
    }
    else
    {
      auto TransposeViewMatrix = ViewMatrix;
      TransposeViewMatrix[0][3] = 0.0f;
      TransposeViewMatrix[1][3] = 0.0f;
      TransposeViewMatrix[2][3] = 0.0f;
      TransposeViewMatrix[3][0] = 0.0f;
      TransposeViewMatrix[3][1] = 0.0f;
      TransposeViewMatrix[3][2] = 0.0f;
      TransposeViewMatrix[3][3] = 1.0f;

      WorldMatrix = TransposeViewMatrix;
      WorldMatrix = ::glm::transpose(WorldMatrix);
    }

    ::Object ExpectMatrices;
    memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));
    ExpectMatrices.World = WorldMatrix;

    EXPECT_CALL(DeviceContext,
      UpdateSubresource(&ConstantBuffer, 0, nullptr, ExpectMatrices, 0, 0))
      .Times(1);

    Render();
  };

  TestCallRender(Component_t::Make(
    {
      { uT("service"), Object_t{ pPosition, pRotation, pScale } }
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Static") },
      { uT("service"), Object_t{ pPosition, pRotation, pScale } }
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Billboard") },
      { uT("service"), Object_t{ pPosition } }
    }), false);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Transform_Combine)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto MatrixIdentity = ::glm::identity<::glm::mat4>();

  ::std::deque<Component_t::ComponentPtr_t> Components;

  const auto SetPosition = [&](float _X, float _Y, float _Z)
  {
    Components.push_front(Component_t::Make(
      {
        { uT("kind"), uT("Position") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      }));

    MatrixIdentity = ::glm::translate(MatrixIdentity, { _X, _Y, _Z });
  };

  const auto SetRotation = [&](float _X, float _Y, float _Z)
  {
    Components.push_front(Component_t::Make(
      {
        { uT("kind"), uT("Rotation") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      }));

    MatrixIdentity = ::glm::rotate(MatrixIdentity, _Z, { 0.0f, 0.0f, 1.0f });
    MatrixIdentity = ::glm::rotate(MatrixIdentity, _Y, { 0.0f, 1.0f, 0.0f });
    MatrixIdentity = ::glm::rotate(MatrixIdentity, _X, { 1.0f, 0.0f, 0.0f });
  };

  const auto SetScale = [&](float _X, float _Y, float _Z)
  {
    Components.push_front(Component_t::Make(
      {
        { uT("kind"), uT("Scale") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      }));

    MatrixIdentity = ::glm::scale(MatrixIdentity, { _X, _Y, _Z });
  };

  {
    MatrixIdentity = ::glm::identity<::glm::mat4>();
    Components.clear();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    MatrixIdentity = ::glm::transpose(MatrixIdentity);

    auto Render = itCreator->second(Component_t::Make(
      { 
        { uT("service"), Object_t{ Components.begin(), Components.end() } }
      }));
    ASSERT_NE(nullptr, Render);

    ::Object ExpectMatrices;
    memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));
    ExpectMatrices.World = MatrixIdentity;

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, ExpectMatrices, _, _))
      .Times(1);

    Render();
  }

  {
    MatrixIdentity = ::glm::identity<::glm::mat4>();
    Components.clear();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    MatrixIdentity = ::glm::transpose(MatrixIdentity);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Static") },
        { uT("service"), Object_t{ Components.begin(), Components.end() } }
      }));
    ASSERT_NE(nullptr, Render);

    ::Object ExpectMatrices;
    memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));
    ExpectMatrices.World = MatrixIdentity;

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, ExpectMatrices, _, _))
      .Times(1);

    Render();
  }

  {
    const auto ViewMatrix = ::glm::mat4
    {
      { 1.0f, 2.0f, 3.0f, 4.0f },
      { 5.0f, 6.0f, 7.0f, 8.0f },
      { 9.0f, 10.0f, 11.0f, 12.0f },
      { 13.0f, 14.0f, 15.0f, 16.0f },
    };

    GetCameraMartix(Example).View = ViewMatrix;

    auto TransposeViewMatrix = ViewMatrix;
    TransposeViewMatrix[0][3] = 0.0f;
    TransposeViewMatrix[1][3] = 0.0f;
    TransposeViewMatrix[2][3] = 0.0f;
    TransposeViewMatrix[3][0] = 0.0f;
    TransposeViewMatrix[3][1] = 0.0f;
    TransposeViewMatrix[3][2] = 0.0f;
    TransposeViewMatrix[3][3] = 1.0f;

    MatrixIdentity = TransposeViewMatrix;
    Components.clear();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetPosition(11.0f, 22.0f, 33.0f);
    SetPosition(111.0f, 222.0f, 333.0f);

    MatrixIdentity = ::glm::transpose(MatrixIdentity);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Billboard") },
        { uT("service"), Object_t{ Components.begin(), Components.end() } }
      }));
    ASSERT_NE(nullptr, Render);

    ::Object ExpectMatrices;
    memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));
    ExpectMatrices.World = MatrixIdentity;

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, ExpectMatrices, _, _))
      .Times(1);

    Render();
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_Constant_User)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  struct Data
  {
    uint8_t Value[6] = { 0x19, 0x09, 0x19, 0x18, 0x17, 0x00 };
  };

  Data oData;

  const BufferMapper_t BufferMapper = [&](void * _pData)
  {
    memcpy(_pData, &oData, sizeof(Data));
    return false;
  };

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Uint8Buffer ConstantBuffer;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(oData);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  D3D11_BUFFER_DESC IndexBufferDesc = { 0 };
  IndexBufferDesc.ByteWidth = 1909191816;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itConstBufferCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itConstBufferCreator);

  EXPECT_CALL(Device, CreateBuffer(ConstantBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&ConstantBuffer));

  auto Render = itConstBufferCreator->second(Component_t::Make(
    {
      { uT("mapper"), BufferMapper },
      { uT("size"), sizeof(Data) },
    }));
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(DeviceContext,
    VSSetConstantBuffers(COVELLITE_BUFFER_INDEX_USER, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    PSSetConstantBuffers(COVELLITE_BUFFER_INDEX_USER, 1, &ConstantBuffer))
    .Times(1);

  const ::std::vector<uint8_t> ExpectedData{ oData.Value, oData.Value + 5 };

  EXPECT_CALL(DeviceContext,
    UpdateSubresource(&ConstantBuffer, 0, nullptr, ExpectedData, 0, 0))
    .Times(1);

  Render(); // Вызывается BufferMapper

  EXPECT_CALL(ConstantBuffer, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_Orthographic_DefaultPosition)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::CameraBuffer ConstantBuffer;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::Camera);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  D3D11_BUFFER_DESC IndexBufferDesc = { 0 };
  IndexBufferDesc.ByteWidth = 1811221344;

  const auto Width = 1811211754.0f;
  const auto Height = 1811211755.0f;

  ::Camera Matrices;
  Matrices.Projection = ::glm::transpose(::glm::ortho(
    0.0f, Width, Height, 0.0f,
    1.0f, -1.0f));
  Matrices.View = ::glm::identity<::glm::mat4>();
  Matrices.ViewInverse = ::glm::transpose(::glm::inverse(Matrices.View));
  Matrices.View = ::glm::transpose(Matrices.View);

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, VSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, PSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBuffer(ConstantBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&ConstantBuffer));

  EXPECT_CALL(DeviceContext,
    VSSetConstantBuffers(COVELLITE_BUFFER_INDEX_CAMERA, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    PSSetConstantBuffers(COVELLITE_BUFFER_INDEX_CAMERA, 1, &ConstantBuffer))
    .Times(1);

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
    });

  D3D11_VIEWPORT Viewport = { 0 };
  Viewport.Width = Width;
  Viewport.Height = Height;

  auto Render = itCameraCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  InSequence Dummy;

  EXPECT_CALL(DeviceContext, RSGetViewports(1))
    .Times(1)
    .WillOnce(Return(Viewport));

  EXPECT_CALL(DeviceContext, 
    UpdateSubresource(&ConstantBuffer, _, _, Matrices, _, _))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_Orthographic)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::CameraBuffer ConstantBuffer;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::Camera);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  D3D11_BUFFER_DESC IndexBufferDesc = { 0 };
  IndexBufferDesc.ByteWidth = 1909161938;

  const auto X = 1902282004.0f;
  const auto Y = 1902282005.0f;
  const auto Width = 1902282006.0f;
  const auto Height = 1902282007.0f;

  const auto Projection = ::glm::transpose(::glm::ortho(
    X, X + Width,
    Y + Height, Y,
    1.0f, -1.0f));

  auto View = ::glm::identity<::glm::mat4>();
  const auto ViewInverse = ::glm::transpose(::glm::inverse(View));
  View = ::glm::transpose(View);

  ::Camera Matrices;

  Matrices.Projection = Projection;
  Matrices.View = View;
  Matrices.ViewInverse = ViewInverse;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, VSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, PSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBuffer(ConstantBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&ConstantBuffer));

  EXPECT_CALL(DeviceContext,
    VSSetConstantBuffers(COVELLITE_BUFFER_INDEX_CAMERA, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    PSSetConstantBuffers(COVELLITE_BUFFER_INDEX_CAMERA, 1, &ConstantBuffer))
    .Times(1);

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  const auto pPosition = Component_t::Make(
    {
      { uT("kind"), uT("Position") },
      { uT("x"), X },
      { uT("y"), Y },
    });

  const auto pCamera = Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
      { uT("service"), Object_t{ pPosition } }
    });

  auto Render = itCameraCreator->second(pCamera);
  ASSERT_NE(nullptr, Render);

  InSequence Dummy;

  D3D11_VIEWPORT Viewport = { 0 };
  Viewport.Width = Width;
  Viewport.Height = Height;

  EXPECT_CALL(DeviceContext, RSGetViewports(1))
    .Times(1)
    .WillOnce(Return(Viewport));

  EXPECT_CALL(DeviceContext, 
    UpdateSubresource(&ConstantBuffer, _, _, Matrices, _, _))
    .Times(1);

  Render();

  EXPECT_EQ(View, (::glm::mat4)(*pCamera)[uT("view")].Default(::glm::mat4{ 1.0f }));
  EXPECT_EQ(Projection, (::glm::mat4)(*pCamera)[uT("projection")].Default(::glm::mat4{ 1.0f }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_Perspective)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::CameraBuffer ConstantBuffer;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::Camera);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  D3D11_BUFFER_DESC IndexBufferDesc = { 0 };
  IndexBufferDesc.ByteWidth = 1909161939;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, VSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(DeviceContext, PSSetConstantBuffers(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBuffer(ConstantBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&ConstantBuffer));

  EXPECT_CALL(DeviceContext,
    VSSetConstantBuffers(COVELLITE_BUFFER_INDEX_CAMERA, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    PSSetConstantBuffers(COVELLITE_BUFFER_INDEX_CAMERA, 1, &ConstantBuffer))
    .Times(1);

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  const auto TestCallRender = [&](const Render_t & _CameraRender,
    float _Width, float _Height, const ::Camera & _Matrices)
  {
    InSequence Dummy;

    D3D11_VIEWPORT Viewport = { 0 };
    Viewport.Width = _Width;
    Viewport.Height = _Height;

    EXPECT_CALL(DeviceContext, RSGetViewports(1))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(DeviceContext, 
      UpdateSubresource(&ConstantBuffer, _, _, _Matrices, _, _))
      .Times(1);

    _CameraRender();
  };

  ::Camera Matrices;

  const auto GetProjection = [](float _AngleY, float _Width, float _Height,
    float _ZNear, float _ZFar)
  {
    const auto AngleY = _AngleY * 
      ::alicorn::extension::cpp::math::Constant<float>::DegreeToRadian;
    return ::glm::perspectiveFovRH(AngleY, _Width, _Height, _ZFar, _ZNear);
  };

  const auto GetView = [](
    float _X, float _Y, float _Z,
    float _A, float _B, float _C,
    float _Distance)
  {
    ::glm::mat4 Transform = ::glm::identity<::glm::mat4>();

    Transform = ::glm::translate(Transform,
      ::glm::vec3{ _X, _Y, _Z });
    Transform = ::glm::rotate(Transform,
      _C, ::glm::vec3{ 0.0f, 0.0f, 1.0f });
    Transform = ::glm::rotate(Transform,
      _B, ::glm::vec3{ 0.0f, 1.0f, 0.0f });
    Transform = ::glm::rotate(Transform,
      _A, ::glm::vec3{ 1.0f, 0.0f, 0.0f });

    return ::glm::lookAtRH(
      ::glm::vec3{ Transform * ::glm::vec4{ _Distance, 0.0f, 0.0f, 1.0f } },
      ::glm::vec3{ _X, _Y, _Z },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f });
  };

  // Default values
  {
    const auto AngleY = 90.0f;
    const auto Width = 1901011221.0f;
    const auto Height = 1901011222.0f;
    const auto ZNear = 0.01f;
    const auto ZFar = 200.0f;

    const auto View = GetView(
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.1f);

    Matrices.Projection = ::glm::transpose(
      GetProjection(AngleY, Width, Height, ZNear, ZFar));
    Matrices.View = ::glm::transpose(View);
    Matrices.ViewInverse = ::glm::transpose(::glm::inverse(View));

    const auto pCamera = Component_t::Make(
      {
        { uT("kind"), uT("Perspective") },
      });

    auto CameraRender = itCameraCreator->second(pCamera);
    ASSERT_NE(nullptr, CameraRender);

    TestCallRender(CameraRender, Width, Height, Matrices);

    EXPECT_EQ(Matrices.View, 
      (::glm::mat4)(*pCamera)[uT("view")].Default(::glm::mat4{ 1.0f }));
    EXPECT_EQ(Matrices.Projection,
      (::glm::mat4)(*pCamera)[uT("projection")].Default(::glm::mat4{ 1.0f }));
  }

  {
    const auto pPosition = Component_t::Make(
      {
        { uT("kind"), uT("Position") },
      });

    const auto pRotation = Component_t::Make(
      {
        { uT("kind"), uT("Rotation") },
      });

    const auto pCamera = Component_t::Make(
      {
        { uT("kind"), uT("Perspective") },
        { uT("service"), Object_t{ pPosition, pRotation } }
      });

    auto CameraRender = itCameraCreator->second(pCamera);
    ASSERT_NE(nullptr, CameraRender);

    {
      const auto AngleY = 123.0f;
      const auto Width = 1901011221.0f;
      const auto Height = 1901011222.0f;
      const auto ZNear = 19.10261248f;
      const auto ZFar = 191.0261249f;
      const auto Distance = 1234.0f;

      const auto X = 11327.0f;
      const auto Y = 11328.0f;
      const auto Z = 11329.0f;

      const auto A = 11330.0f;
      const auto B = 11331.0f;
      const auto C = 11332.0f;

      const auto View = GetView(
        X, Y, Z,
        A, B, C,
        Distance + 0.1f);

      Matrices.Projection = ::glm::transpose(
        GetProjection(AngleY, Width, Height, ZNear, ZFar));
      Matrices.View = ::glm::transpose(View);
      Matrices.ViewInverse = ::glm::transpose(::glm::inverse(View));

      (*pPosition)[uT("x")] = X;
      (*pPosition)[uT("y")] = Y;
      (*pPosition)[uT("z")] = Z;

      (*pRotation)[uT("x")] = A;
      (*pRotation)[uT("y")] = B;
      (*pRotation)[uT("z")] = C;

      (*pCamera)[uT("fov")] = AngleY;
      (*pCamera)[uT("znear")] = ZNear;
      (*pCamera)[uT("zfar")] = ZFar;
      (*pCamera)[uT("distance")] = Distance;

      TestCallRender(CameraRender, Width, Height, Matrices);

      EXPECT_EQ(Matrices.View,
        (::glm::mat4)(*pCamera)[uT("view")].Default(::glm::mat4{ 1.0f }));
      EXPECT_EQ(Matrices.Projection,
        (::glm::mat4)(*pCamera)[uT("projection")].Default(::glm::mat4{ 1.0f }));
    }

    // Второй вызов, чтобы убедиться, что значения можно менять во время
    // рендеринга
    {
      const auto AngleY = 456.0f;
      const auto Width = 1901011221.0f;
      const auto Height = 1901011222.0f;
      const auto ZNear = 191.0261248f;
      const auto ZFar = 1910.261249f;
      const auto Distance = 4567.0f;

      const auto X = 11333.0f;
      const auto Y = 11334.0f;
      const auto Z = 11335.0f;

      const auto A = 11336.0f;
      const auto B = 11337.0f;
      const auto C = 11338.0f;

      const auto View = GetView(
        X, Y, Z,
        A, B, C,
        Distance + 0.1f);

      Matrices.Projection = ::glm::transpose(
        GetProjection(AngleY, Width, Height, ZNear, ZFar));
      Matrices.View = ::glm::transpose(View);
      Matrices.ViewInverse = ::glm::transpose(::glm::inverse(View));

      (*pPosition)[uT("x")] = X;
      (*pPosition)[uT("y")] = Y;
      (*pPosition)[uT("z")] = Z;

      (*pRotation)[uT("x")] = A;
      (*pRotation)[uT("y")] = B;
      (*pRotation)[uT("z")] = C;

      (*pCamera)[uT("fov")] = AngleY;
      (*pCamera)[uT("znear")] = ZNear;
      (*pCamera)[uT("zfar")] = ZFar;
      (*pCamera)[uT("distance")] = Distance;

      TestCallRender(CameraRender, Width, Height, Matrices);

      EXPECT_EQ(Matrices.View, 
        (::glm::mat4)(*pCamera)[uT("view")].Default(::glm::mat4{ 1.0f }));
      EXPECT_EQ(Matrices.Projection, 
        (::glm::mat4)(*pCamera)[uT("projection")].Default(::glm::mat4{ 1.0f }));
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_DefaultViewport)
{
  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pCamera)
  {
    DXGI_SWAP_CHAIN_DESC Desc = { 0 };
    Desc.BufferDesc.Width = 2008111126;
    Desc.BufferDesc.Height = 2008111127;

    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)Desc.BufferDesc.Width;
    vp.Height = (FLOAT)Desc.BufferDesc.Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    InSequence Dummy;

    EXPECT_CALL(SwapChain, GetDesc())
      .Times(1)
      .WillOnce(Return(Desc));

    EXPECT_CALL(DeviceContext, RSSetViewports(::std::vector<D3D11_VIEWPORT>{ vp }))
      .Times(1);

    auto Render = itCreator->second(_pCamera);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(SwapChain, GetDesc())
      .Times(1)
      .WillOnce(Return(Desc));

    EXPECT_CALL(DeviceContext, RSSetViewports(::std::vector<D3D11_VIEWPORT>{ vp }))
      .Times(1);

    Render();
  };

  TestCallRender(Component_t::Make(
    { 
      { uT("kind"), uT("Orthographic") } 
    }));
  TestCallRender(Component_t::Make(
    { 
      { uT("kind"), uT("Perspective") } 
    }));
  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
      { uT("width"), 2009081145 },
    }));
  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Perspective") },
      { uT("height"), 2009081146 },
    }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_ScaleViewport)
{
  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const float _Scale,
    const Component_t::ComponentPtr_t & _pCamera)
  {
    DXGI_SWAP_CHAIN_DESC Desc = { 0 };
    Desc.BufferDesc.Width = 1024;
    Desc.BufferDesc.Height = 2048;

    D3D11_VIEWPORT vp;
    vp.Width = _Scale * Desc.BufferDesc.Width;
    vp.Height = _Scale * Desc.BufferDesc.Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    InSequence Dummy;

    EXPECT_CALL(SwapChain, GetDesc())
      .Times(1)
      .WillOnce(Return(Desc));

    EXPECT_CALL(DeviceContext, RSSetViewports(::std::vector<D3D11_VIEWPORT>{ vp }))
      .Times(1);

    auto Render = itCreator->second(_pCamera);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(SwapChain, GetDesc())
      .Times(1)
      .WillOnce(Return(Desc));

    EXPECT_CALL(DeviceContext, RSSetViewports(::std::vector<D3D11_VIEWPORT>{ vp }))
      .Times(1);

    Render();
  };

  TestCallRender(0.5f, Component_t::Make(
    { 
      { uT("kind"), uT("Orthographic") },
      { uT("scale"), 0.5f },
    }));
  TestCallRender(2.0f, Component_t::Make(
    { 
      { uT("kind"), uT("Perspective") },
      { uT("scale"), 2.0f },
    }));
  TestCallRender(0.25f, Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
      { uT("scale"), 0.25f },
      { uT("width"), 2009081145 },
      { uT("height"), 2009081146 },
   }));
  TestCallRender(4.0f, Component_t::Make(
    {
      { uT("kind"), uT("Perspective") },
      { uT("scale"), 4.0f },
      { uT("width"), 2009081147 },
      { uT("height"), 2009081148 },
   }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_UserViewport)
{
  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const UINT _Width, const UINT _Height,
    const Component_t::ComponentPtr_t & _pCamera)
  {
    DXGI_SWAP_CHAIN_DESC Desc = { 0 };
    Desc.BufferDesc.Width = 2008111126;
    Desc.BufferDesc.Height = 2008111127;

    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)_Width;
    vp.Height = (FLOAT)_Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    InSequence Dummy;

    EXPECT_CALL(SwapChain, GetDesc())
      .Times(1)
      .WillOnce(Return(Desc));

    EXPECT_CALL(DeviceContext, RSSetViewports(::std::vector<D3D11_VIEWPORT>{ vp }))
      .Times(1);

    auto Render = itCreator->second(_pCamera);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(SwapChain, GetDesc())
      .Times(1)
      .WillOnce(Return(Desc));

    EXPECT_CALL(DeviceContext, RSSetViewports(::std::vector<D3D11_VIEWPORT>{ vp }))
      .Times(1);

    Render();
  };

  TestCallRender(1024, 2048, Component_t::Make(
    { 
      { uT("kind"), uT("Orthographic") },
      { uT("width"), 1024 },
      { uT("height"), 2048 },
    }));
  TestCallRender(2048, 4096, Component_t::Make(
    { 
      { uT("kind"), uT("Perspective") },
      { uT("width"), 2048 },
      { uT("height"), 4096 },
    }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_DisableBlend)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pCamera)
  {
    const ::std::vector<FLOAT> Factor = { 0.0f, 0.0f, 0.0f, 0.0f };

    InSequence Dummy;

    EXPECT_CALL(Device, CreateBlendState(_))
      .Times(0);

    auto Render = itCreator->second(_pCamera);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetBlendState(nullptr, Factor, 0xFFFFFFFF))
      .Times(1);

    Render();
  };

  TestCallRender(Component_t::Make({ { uT("kind"), uT("Orthographic") } }));
  TestCallRender(Component_t::Make({ { uT("kind"), uT("Perspective") } }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_SetDefaultRenderTarget_And_DepthDisabled)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1)
    .WillOnce(Return(&RenderTargetView));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pCamera)
  {
    auto Render = itCreator->second(_pCamera);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(RenderTargetViews_t{ &RenderTargetView }, nullptr))
      .Times(1);

    Render();
  };

  TestCallRender(Component_t::Make({ { uT("kind"), uT("Orthographic") } }));
  TestCallRender(Component_t::Make({ { uT("kind"), uT("Perspective") } }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Index_CreateIndexBuffer_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  const ::std::vector<int> IndexData = { 1, 2, 3, 4, 5, 6 };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    InSequence Dummy;

    EXPECT_CALL(Device, CreateBuffer(_, _))
      .Times(1);

    EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(_pComponent), ::std::exception);
  };

  {
    const auto pPresent = Component_t::Make(
      {
        { uT("content"), IndexData },
      });

    TestCall(pPresent);
  }

  {
    const auto pIndexData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), IndexData },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pIndexData } } }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Index)
{
  ::std::vector<int> IndexData = { 1, 2, 3, 4, 5, 6 };
  IntroduceBufferSize(IndexData);

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pComponent)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::DeviceContext DeviceContext;
    ::mock::DirectX11::Buffer Buffer;

    D3D11_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = IndexData.data();

    D3D11_BUFFER_DESC Desc = { 0 };
    Desc.Usage = D3D11_USAGE_DEFAULT;
    // cppcheck-suppress sizeofDereferencedVoidPointer
    Desc.ByteWidth = sizeof(int) * (int)IndexData.size();
    Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(DirectXProxy, CreateDeviceContext())
      .Times(1)
      .WillOnce(Return(&DeviceContext));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itCreator = IExample.GetCreators().find(uT("Present"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    InSequence Dummy;

    EXPECT_CALL(Device, CreateBuffer(Desc, InitData))
      .Times(1)
      .WillOnce(Return(&Buffer));

    auto Render = itCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, 
      IASetIndexBuffer(&Buffer, DXGI_FORMAT_R32_UINT, 0))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
      .Times(1);

    EXPECT_CALL(DeviceContext, DrawIndexed(IndexData.size(), 0, 0))
      .Times(1);

    Render();

    EXPECT_CALL(Buffer, Release())
      .Times(1);
  };

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("content"), IndexData },
      });

    TestCallRender(pComponent);
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), IndexData },
      });

    TestCallRender(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Instance_CreateIndexBuffer_Fail)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  const ::std::vector<int> IndexData = { 1, 2, 3, 4, 5, 6 };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    InSequence Dummy;

    EXPECT_CALL(Device, CreateBuffer(_, _))
      .Times(1);

    EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(_pComponent), ::std::exception);
  };

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), BufferMapper_t{} },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("content"), IndexData },
        { uT("service"), Object_t{ pInstanceData } }
      });

    TestCall(pPresent);
  }

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), BufferMapper_t{} },
      });

    const auto pIndexData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), IndexData },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pInstanceData, pIndexData } } }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Instance_InvalidSizeAndCountValue)
{
  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  const ::std::vector<int> IndexData = { 1, 2, 3, 4, 5, 6 };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallMapper = [&](
    const Component_t::ComponentPtr_t & _pInstanceData,
    const auto _Mapper)
  {
    (*_pInstanceData)[uT("mapper")] = _Mapper;

    {
      const auto pPresent = Component_t::Make(
        {
          { uT("content"), IndexData },
          { uT("service"), Object_t{ _pInstanceData } }
        });

      EXPECT_THROW(itCreator->second(pPresent), ::std::exception);
    }

    {
      const auto pIndexData = Component_t::Make(
        {
          { uT("kind"), uT("Buffer") },
          { uT("content"), IndexData },
        });

      const auto pPresent = Component_t::Make(
        {
          { uT("service"), Object_t{ _pInstanceData, pIndexData } }
        });

      EXPECT_THROW(itCreator->second(pPresent), ::std::exception);
    }
  };

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pInstanceData)
  {
    using BufferMapperMaxSize_t = ::std::function<bool(void *)>;
    using BufferMapperSize_t = ::std::function<bool(void *, ::std::size_t &)>;

    TestCallMapper(_pInstanceData, BufferMapperMaxSize_t{});
    TestCallMapper(_pInstanceData, BufferMapperSize_t{});
  };

  TestCall(Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
    }));

  TestCall(Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("size"), ::std::size_t(16) },
    }));

  TestCall(Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("size"), ::std::size_t(257) },
      { uT("count"), ::std::size_t(1) },
    }));

  TestCall(Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("size"), ::std::size_t(64) },
      { uT("count"), ::std::size_t(7) },
    }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Instance_CreateInstanceBuffer_Fail)
{
  using BufferMapperMaxSize_t = ::std::function<bool(void *)>;
  using BufferMapperSize_t = ::std::function<bool(void *, ::std::size_t &)>;

  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  const ::std::vector<int> IndexData = { 1, 2, 3, 4, 5, 6 };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    InSequence Dummy;

    EXPECT_CALL(Device, CreateBuffer(_, _))
      .Times(1);

    EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
      .Times(1)
      .WillOnce(Return(S_OK));

    EXPECT_CALL(Device, CreateBuffer(_))
      .Times(1);

    EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(_pComponent), ::std::exception);
  };

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), BufferMapperMaxSize_t{} },
        { uT("size"), ::std::size_t(16) },
        { uT("count"), ::std::size_t(4) },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("content"), IndexData },
        { uT("service"), Object_t{ pInstanceData } }
      });

    TestCall(pPresent);
  }

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), BufferMapperMaxSize_t{} },
        { uT("size"), ::std::size_t(16) },
        { uT("count"), ::std::size_t(4) },
      });

    const auto pIndexData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), IndexData },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pInstanceData, pIndexData } } }));
  }

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), BufferMapperSize_t{} },
        { uT("size"), ::std::size_t(16) },
        { uT("count"), ::std::size_t(4) },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("content"), IndexData },
        { uT("service"), Object_t{ pInstanceData } }
      });

    TestCall(pPresent);
  }

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), BufferMapperSize_t{} },
        { uT("size"), ::std::size_t(16) },
        { uT("count"), ::std::size_t(4) },
      });

    const auto pIndexData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), IndexData },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pInstanceData, pIndexData } } }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Instance_Map_Fail)
{
  using BufferMapperMaxSize_t = ::std::function<bool(void *)>;
  using BufferMapperSize_t = ::std::function<bool(void *, ::std::size_t &)>;

  const ::std::vector<int> IndexData = { 1 };

  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer IndexBuffer;
  ::mock::DirectX11::Buffer InstanceBuffer;
  const ::std::size_t InstanceCount = 200;
  const ::std::size_t InstanceBlockSize = 16 * 34;
  const ::std::size_t InstanceBufferSize = InstanceCount * InstanceBlockSize;

  class MapperProxy
  {
  public:
    MOCK_METHOD1(Map, bool(void *));
  };

  MapperProxy oMapperProxy;

  const BufferMapperMaxSize_t MapperMaxSize = [&](void * _pData)
  {
    return oMapperProxy.Map(_pData);
  };

  const BufferMapperSize_t MapperSize = [&](void * _pData, ::std::size_t &)
  {
    return oMapperProxy.Map(_pData);
  };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    Render_t Render;

    {
      InSequence Dummy;

      EXPECT_CALL(Device, CreateBuffer(_, _))
        .Times(1)
        .WillOnce(Return(&IndexBuffer));

      EXPECT_CALL(Device, CreateBuffer(_))
        .Times(1)
        .WillOnce(Return(&InstanceBuffer));

      Render = itCreator->second(_pComponent);
      ASSERT_NE(nullptr, Render);

      EXPECT_CALL(oMapperProxy, Map(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      D3D11_MAPPED_SUBRESOURCE Resource = { 0 };

      EXPECT_CALL(DeviceContext, Mapped(&InstanceBuffer, _))
        .Times(1)
        .WillOnce(Return(Resource));

      EXPECT_CALL(DeviceContext, Map(0, 0))
        .Times(1)
        .WillOnce(Return(E_FAIL));

      EXPECT_CALL(oMapperProxy, Map(_))
        .Times(0);

      EXPECT_THROW(Render(), ::std::exception);
    }

    EXPECT_CALL(IndexBuffer, Release())
      .Times(AtLeast(1));

    EXPECT_CALL(InstanceBuffer, Release())
      .Times(AtLeast(1));
  };

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), MapperMaxSize },
        { uT("size"), InstanceBufferSize },
        { uT("count"), InstanceCount },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("content"), IndexData },
        { uT("service"), Object_t{ pInstanceData } }
      });

    TestCall(pPresent);
  }

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), MapperMaxSize },
        { uT("size"), InstanceBufferSize },
        { uT("count"), InstanceCount },
      });

    const auto pIndexData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), IndexData },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pInstanceData, pIndexData } } }));
  }

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), MapperSize },
        { uT("size"), InstanceBufferSize },
        { uT("count"), InstanceCount },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("content"), IndexData },
        { uT("service"), Object_t{ pInstanceData } }
      });

    TestCall(pPresent);
  }

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), MapperSize },
        { uT("size"), InstanceBufferSize },
        { uT("count"), InstanceCount },
      });

    const auto pIndexData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), IndexData },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pInstanceData, pIndexData } } }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Instance_MaxSize)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  ::std::vector<int> IndexData = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
  IntroduceBufferSize(IndexData);

  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer IndexBuffer;
  ::mock::DirectX11::Buffer InstanceBuffer;
  const ::std::size_t InstanceCount = 200;
  const ::std::size_t InstanceBlockSize = 16 * 34;
  const ::std::size_t InstanceBufferSize = InstanceCount * InstanceBlockSize;

  D3D11_BUFFER_DESC IndexDesc = { 0 };
  IndexDesc.Usage = D3D11_USAGE_DEFAULT;
  // cppcheck-suppress sizeofDereferencedVoidPointer
  IndexDesc.ByteWidth = sizeof(int) * (int)IndexData.size();
  IndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

  D3D11_SUBRESOURCE_DATA InitIndexData = { 0 };
  InitIndexData.pSysMem = IndexData.data();

  D3D11_BUFFER_DESC InstanceDesc = { 0 };
  InstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
  InstanceDesc.ByteWidth = InstanceBufferSize;
  InstanceDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  InstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  class MapperProxy
  {
  public:
    MOCK_METHOD1(Map, bool(void *));
  };

  MapperProxy oMapperProxy;

  const BufferMapper_t Mapper = [&](void * _pData)
  {
    return oMapperProxy.Map(_pData);
  };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pComponent)
  {
    Render_t Render;

    {
      InSequence Dummy;

      EXPECT_CALL(Device, CreateBuffer(IndexDesc, InitIndexData))
        .Times(1)
        .WillOnce(Return(&IndexBuffer));

      EXPECT_CALL(Device, CreateBuffer(InstanceDesc))
        .Times(1)
        .WillOnce(Return(&InstanceBuffer));

      Render = itCreator->second(_pComponent);
      ASSERT_NE(nullptr, Render);

      const auto TestCallDraw = [&](void)
      {
        EXPECT_CALL(DeviceContext,
          IASetVertexBuffers(1, 1, &InstanceBuffer, InstanceBlockSize, 0))
          .Times(1);

        EXPECT_CALL(DeviceContext,
          IASetIndexBuffer(&IndexBuffer, DXGI_FORMAT_R32_UINT, 0))
          .Times(1);

        EXPECT_CALL(DeviceContext,
          IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
          .Times(1);

        EXPECT_CALL(DeviceContext,
          DrawIndexedInstanced(IndexData.size(), InstanceCount, 0, 0, 0))
          .Times(1);
      };

      // ********************* Отрисовка без обновления *********************** //

      EXPECT_CALL(oMapperProxy, Map(nullptr))
        .Times(1)
        .WillOnce(Return(false));

      EXPECT_CALL(DeviceContext, Map(_, _))
        .Times(0);

      EXPECT_CALL(oMapperProxy, Map(_))
        .Times(0);

      EXPECT_CALL(DeviceContext, Unmap(_, _))
        .Times(0);

      TestCallDraw();
      Render();

      // ********************** Отрисовка с обновлением *********************** //

      D3D11_MAPPED_SUBRESOURCE Resource = { 0 };
      Resource.pData = (void *)1909240917;

      EXPECT_CALL(oMapperProxy, Map(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(DeviceContext, Mapped(&InstanceBuffer, D3D11_MAP_WRITE_NO_OVERWRITE))
        .Times(1)
        .WillOnce(Return(Resource));

      EXPECT_CALL(DeviceContext, Map(0, 0))
        .Times(1)
        .WillOnce(Return(S_OK));

      EXPECT_CALL(oMapperProxy, Map(Resource.pData))
        .Times(1);

      EXPECT_CALL(DeviceContext, Unmap(&InstanceBuffer, 0))
        .Times(1);

      TestCallDraw();
      Render();
    }

    EXPECT_CALL(IndexBuffer, Release())
      .Times(AtLeast(1));

    EXPECT_CALL(InstanceBuffer, Release())
      .Times(AtLeast(1));
  };

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), Mapper },
        { uT("size"), InstanceBufferSize },
        { uT("count"), InstanceCount },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("content"), IndexData },
        { uT("service"), Object_t{ pInstanceData } }
      });

    TestCall(pPresent);
  }

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), Mapper },
        { uT("size"), InstanceBufferSize },
        { uT("count"), InstanceCount },
      });

    const auto pIndexData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), IndexData },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pInstanceData, pIndexData } } }));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Instance_ChangeSize)
{
  using BufferMapper_t = ::std::function<bool(void *, ::std::size_t &)>;

  ::std::vector<int> IndexData = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
  IntroduceBufferSize(IndexData);

  ::mock::DirectX11::Proxy DirectXProxy;
  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer IndexBuffer;
  ::mock::DirectX11::Buffer InstanceBuffer;
  const ::std::size_t InstanceCount = 200;
  const ::std::size_t InstanceBlockSize = 16 * 34;
  const ::std::size_t InstanceBufferSize = InstanceCount * InstanceBlockSize;

  D3D11_BUFFER_DESC IndexDesc = { 0 };
  IndexDesc.Usage = D3D11_USAGE_DEFAULT;
  // cppcheck-suppress sizeofDereferencedVoidPointer
  IndexDesc.ByteWidth = sizeof(int) * (int)IndexData.size();
  IndexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

  D3D11_SUBRESOURCE_DATA InitIndexData = { 0 };
  InitIndexData.pSysMem = IndexData.data();

  D3D11_BUFFER_DESC InstanceDesc = { 0 };
  InstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
  InstanceDesc.ByteWidth = InstanceBufferSize;
  InstanceDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  InstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  class MapperProxy
  {
  public:
    MOCK_METHOD1(GetCount, ::std::size_t(::std::size_t));
    MOCK_METHOD1(Map, bool(void *));
  };

  MapperProxy oMapperProxy;

  const BufferMapper_t Mapper = [&](void * _pData, ::std::size_t & _Count)
  {
    _Count = oMapperProxy.GetCount(_Count);
    return oMapperProxy.Map(_pData);
  };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    Render_t Render;

    {
      InSequence Dummy;

      EXPECT_CALL(Device, CreateBuffer(IndexDesc, InitIndexData))
        .Times(1)
        .WillOnce(Return(&IndexBuffer));

      EXPECT_CALL(Device, CreateBuffer(InstanceDesc))
        .Times(1)
        .WillOnce(Return(&InstanceBuffer));

      Render = itCreator->second(_pComponent);
      ASSERT_NE(nullptr, Render);

      const auto TestCallDraw = [&](const ::std::size_t _Count)
      {
        EXPECT_CALL(DeviceContext,
          IASetVertexBuffers(1, 1, &InstanceBuffer, InstanceBlockSize, 0))
          .Times(1);

        EXPECT_CALL(DeviceContext,
          IASetIndexBuffer(&IndexBuffer, DXGI_FORMAT_R32_UINT, 0))
          .Times(1);

        EXPECT_CALL(DeviceContext,
          IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
          .Times(1);

        EXPECT_CALL(DeviceContext,
          DrawIndexedInstanced(IndexData.size(), _Count, 0, 0, 0))
          .Times(1);
      };

      // ************** Отрисовка без обновления буфера ********************* //

      EXPECT_CALL(oMapperProxy, GetCount(InstanceCount))
        .Times(1)
        .WillOnce(Return(InstanceCount / 2));

      EXPECT_CALL(oMapperProxy, Map(nullptr))
        .Times(1)
        .WillOnce(Return(false));

      EXPECT_CALL(DeviceContext, Map(_, _))
        .Times(0);

      EXPECT_CALL(oMapperProxy, Map(_))
        .Times(0);

      EXPECT_CALL(DeviceContext, Unmap(_, _))
        .Times(0);

      TestCallDraw(InstanceCount / 2);
      Render();

      // *** Отрисовка без обновления буфера (слишком большое количество) *** //

      EXPECT_CALL(oMapperProxy, GetCount(InstanceCount))
        .Times(1)
        .WillOnce(Return(InstanceCount * 2));

      EXPECT_CALL(oMapperProxy, Map(nullptr))
        .Times(1)
        .WillOnce(Return(false));

      EXPECT_CALL(DeviceContext, Map(_, _))
        .Times(0);

      EXPECT_CALL(oMapperProxy, Map(_))
        .Times(0);

      EXPECT_CALL(DeviceContext, Unmap(_, _))
        .Times(0);

      TestCallDraw(InstanceCount);
      Render();

      // ********************** Отрисовка с обновлением ********************* //

      D3D11_MAPPED_SUBRESOURCE Resource = { 0 };
      Resource.pData = (void *)1909240917;

      EXPECT_CALL(oMapperProxy, GetCount(InstanceCount))
        .Times(1)
        .WillOnce(Return(InstanceCount));

      EXPECT_CALL(oMapperProxy, Map(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(DeviceContext, Mapped(&InstanceBuffer, D3D11_MAP_WRITE_NO_OVERWRITE))
        .Times(1)
        .WillOnce(Return(Resource));

      EXPECT_CALL(DeviceContext, Map(0, 0))
        .Times(1)
        .WillOnce(Return(S_OK));

      EXPECT_CALL(oMapperProxy, GetCount(InstanceCount))
        .Times(1)
        .WillOnce(Return(InstanceCount / 4));

      EXPECT_CALL(oMapperProxy, Map(Resource.pData))
        .Times(1);

      EXPECT_CALL(DeviceContext, Unmap(&InstanceBuffer, 0))
        .Times(1);

      TestCallDraw(InstanceCount / 4);
      Render();

      // ****** Отрисовка с обновлением (слишком большое количество) ******** //

      EXPECT_CALL(oMapperProxy, GetCount(InstanceCount))
        .Times(1)
        .WillOnce(Return(0));

      EXPECT_CALL(oMapperProxy, Map(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(DeviceContext, Mapped(&InstanceBuffer, D3D11_MAP_WRITE_NO_OVERWRITE))
        .Times(1)
        .WillOnce(Return(Resource));

      EXPECT_CALL(DeviceContext, Map(0, 0))
        .Times(1)
        .WillOnce(Return(S_OK));

      EXPECT_CALL(oMapperProxy, GetCount(0))
        .Times(1)
        .WillOnce(Return(InstanceCount * 4));

      EXPECT_CALL(oMapperProxy, Map(Resource.pData))
        .Times(1);

      EXPECT_CALL(DeviceContext, Unmap(&InstanceBuffer, 0))
        .Times(1);

      TestCallDraw(InstanceCount);
      Render();
    }

    EXPECT_CALL(IndexBuffer, Release())
      .Times(AtLeast(1));

    EXPECT_CALL(InstanceBuffer, Release())
      .Times(AtLeast(1));
  };

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), Mapper },
        { uT("size"), InstanceBufferSize },
        { uT("count"), InstanceCount },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("content"), IndexData },
        { uT("service"), Object_t{ pInstanceData } }
      });

    TestCall(pPresent);
  }

  {
    const auto pInstanceData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("mapper"), Mapper },
        { uT("size"), InstanceBufferSize },
        { uT("count"), InstanceCount },
      });

    const auto pIndexData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), IndexData },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pInstanceData, pIndexData } } }));
  }
}

#define Updater_test DirectX11_test
#include "../Updater_test.hpp"

static const auto ImplClassName = uT("DirectX11");

#define Common_test DirectX11_test
#include "../Common_test.hpp"
