
#include "stdafx.h"
#include <alicorn/std/vector.hpp>
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

namespace vertex = ::covellite::api::vertex;

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
      Top = 0;
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
  ::DirectX::XMFLOAT4A ARGBtoFloat4(uint32_t _HexColor)
  {
    return ::DirectX::XMFLOAT4A
    {
      ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
      ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
      ((_HexColor & 0x000000FF) >> 0) / 255.0f,
      ((_HexColor & 0xFF000000) >> 24) / 255.0f
    };
  }
};

// Образец макроса для подстановки в класс DirectX11 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(DirectX11_test, Test_Function);

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_RegisterIntoFactory)
{
  using namespace ::alicorn::modules::patterns;

  const Data_t oData;
  const ::covellite::api::renderer::SettingsData & Data = oData;

  auto pExample = factory::make_unique<ITested_t>(uT("DirectX11"), Data);
  EXPECT_NO_THROW(dynamic_cast<Tested_t &>(*pExample));
}

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
TEST_F(DirectX11_test, /*DISABLED_*/Test_RenderTargetView_SwapChain_GetDesc_Faill)
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

  EXPECT_CALL(SwapChain, GetResult(Eq("GetDesc")))
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_RenderTargetView_SwapChain_GetBuffer_Fail)
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

  EXPECT_CALL(SwapChain, GetResult(_))
    .Times(1);

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
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;
  ::mock::DirectX11::Texture2D Texture2D;
  ::mock::DirectX11::RenderTargetView RenderTargetView;

  DXGI_SWAP_CHAIN_DESC Desc = { 0 };
  Desc.BufferDesc.Width = 1809051954;
  Desc.BufferDesc.Height = 1809051955;

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

  EXPECT_CALL(SwapChain, GetDesc())
    .Times(1)
    .WillOnce(Return(Desc));

  EXPECT_CALL(SwapChain, ResizeBuffers(_, _, _, _, _))
    .Times(0);

  EXPECT_CALL(SwapChain, GetBuffer(0, __uuidof(ID3D11Texture2D)))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, CreateRenderTargetView(&Texture2D, nullptr))
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_RSSetViewports)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DXGI::SwapChain SwapChain;

  DXGI_SWAP_CHAIN_DESC Desc = { 0 };
  Desc.BufferDesc.Width = 1809051954;
  Desc.BufferDesc.Height = 1809051955;

  ::std::vector<D3D11_VIEWPORT> Viewports;

  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)Desc.BufferDesc.Width;
  vp.Height = (FLOAT)Desc.BufferDesc.Height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;

  Viewports.push_back(vp);

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CreateDeviceAndSwapChain(_))
    .Times(1);

  EXPECT_CALL(SwapChain, GetDesc())
    .Times(1)
    .WillOnce(Return(Desc));

  EXPECT_CALL(SwapChain, ResizeBuffers(_, _, _, _, _))
    .Times(0);

  EXPECT_CALL(DeviceContext, RSSetViewports(Viewports))
    .Times(1);

  Tested_t oExample{ Data_t{} };
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

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  {
    InSequence Dummy;

    EXPECT_CALL(SwapChain, GetDesc())
      .Times(1)
      .WillOnce(Return(Desc));

    EXPECT_CALL(Device, CreateTexture2D(TextureDesc, ZeroData))
      .Times(1)
      .WillOnce(Return(&Texture2D));

    EXPECT_CALL(Device, CreateDepthStencilView(&Texture2D, DeptStencilViewDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilView));

    EXPECT_CALL(Texture2D, Release())
      .Times(1);

  }

  Tested_t oExample{ Data_t{} };

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
TEST_F(DirectX11_test, /*DISABLED_*/Test_Matrices_CreateBuffer_Fail)
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

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Lights_CreateBuffer_Fail)
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

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
    .Times(1)
    .WillOnce(Return(S_OK));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
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

  EXPECT_THROW(IExample.ResizeWindow(0, 0), ::std::exception);
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

  EXPECT_THROW(IExample.ResizeWindow(0, 0), ::std::exception);
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

  EXPECT_THROW(IExample.ResizeWindow(0, 0), ::std::exception);
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

  EXPECT_THROW(IExample.ResizeWindow(0, 0), ::std::exception);
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

  EXPECT_THROW(IExample.ResizeWindow(0, 0), ::std::exception);
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
    ITested_t & IExample = oExample;

    {
      InSequence Dummy;

      EXPECT_CALL(RenderTargetViewBegin, Release())
        .Times(1);

      EXPECT_CALL(SwapChain, ResizeBuffers(2, Width, Height,
        DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH))
        .Times(1);

      EXPECT_CALL(SwapChain, GetBuffer(0, __uuidof(ID3D11Texture2D)))
        .Times(1)
        .WillOnce(Return(&Texture2D));

      EXPECT_CALL(Device, CreateRenderTargetView(&Texture2D, nullptr))
        .Times(1)
        .WillOnce(Return(&RenderTargetView));

      EXPECT_CALL(Texture2D, Release())
        .Times(1);

      IExample.ResizeWindow(Width, Height);
    }

    EXPECT_CALL(RenderTargetView, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_ResizeWindow_RSSetViewports)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;

  const auto Width = 1809061141;
  const auto Height = 1809061142;

  ::std::vector<D3D11_VIEWPORT> Viewports;

  D3D11_VIEWPORT vp;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  vp.Width = (FLOAT)Width;
  vp.Height = (FLOAT)Height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;

  Viewports.push_back(vp);

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  Tested_t oExample{ Data_t{} };
  ITested_t & IExample = oExample;

  EXPECT_CALL(DeviceContext, RSSetViewports(Viewports))
    .Times(1);

  IExample.ResizeWindow(Width, Height);
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
  ITested_t & IExample = oExample;

  {
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

    IExample.ResizeWindow(Width, Height);
  }

  EXPECT_CALL(DepthStencilView, Release())
    .Times(1);

  EXPECT_CALL(DeviceContext, Release())
    .Times(AtLeast(1));

  EXPECT_CALL(Device, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_BlendState_Fail)
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_BlendState)
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
  BlendDesc.RenderTarget[0].BlendEnable = TRUE;
  BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

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
TEST_F(DirectX11_test, /*DISABLED_*/Test_SamplerState_Fail)
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_SamplerState)
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_ScissorState_Fail)
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_ScissorState_Enabled)
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
    pComponent->SetValue(uT("left"), _X);
    pComponent->SetValue(uT("top"), _Y);
    pComponent->SetValue(uT("right"), _X + _Width);
    pComponent->SetValue(uT("bottom"), _Y + _Height);

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
TEST_F(DirectX11_test, /*DISABLED_*/Test_ScissorState_Enabled_FromData)
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

  const auto pData = Component_t::Make(
    {
      { uT("kind"), uT("Rect") },
    });

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

  {
    auto itDataCreator = IExample.GetCreators().find(uT("Data"));
    ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

    auto Render = itDataCreator->second(pData);
    EXPECT_EQ(nullptr, Render);
  }

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
    pData->SetValue(uT("left"), _X);
    pData->SetValue(uT("top"), _Y);
    pData->SetValue(uT("right"), _X + _Width);
    pData->SetValue(uT("bottom"), _Y + _Height);

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
TEST_F(DirectX11_test, /*DISABLED_*/Test_ScissorState_Disabled)
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Disabled)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;
  ::mock::DirectX11::DepthStencilView DepthStencilView;

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

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    EXPECT_CALL(Device, CreateDepthStencilState(_))
      .Times(0);

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(1, &RenderTargetView, nullptr))
      .Times(1);

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(_, _))
      .Times(0);

    Render();
  };

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") }
    }));

  TestCallRender(Component_t::Make(
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_NoClear_Overwrite)
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
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
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

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(1, &RenderTargetView, &DepthStencilView))
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_Clear_Overwrite)
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
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
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

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(1, &RenderTargetView, &DepthStencilView))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      ClearDepthStencilView(&DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0))
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_NoClear_NoOverwrite)
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
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
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

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(1, &RenderTargetView, &DepthStencilView))
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Depth_Enabled_Clear_NoOverwrite)
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
  DeptStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
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

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    EXPECT_CALL(Device, CreateDepthStencilState(DeptStencilDesc))
      .Times(1)
      .WillOnce(Return(&DepthStencilState));

    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, OMSetDepthStencilState(&DepthStencilState, 1))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(1, &RenderTargetView, &DepthStencilView))
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_Clear)
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

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pComponent,
    const ::std::vector<FLOAT> & _ExpectedColor)
  {
    auto Render = itCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

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
TEST_F(DirectX11_test, /*DISABLED_*/Test_State_AlphaTest)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  using namespace ::testing;

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("AlphaTest") },
    });

  auto Render = itCreator->second(pComponent);
  EXPECT_EQ(nullptr, Render);
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

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

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
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("destination"), uT("destination1907251220") },
      });

    auto Render = itDataCreator->second(pData);
    EXPECT_EQ(nullptr, Render);

    TestCall(Component_t::Make({}));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_PBR)
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

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _BinaryData,
    const int _Width, const int _Height, const ::std::size_t _Slot)
  {
    ::mock::DirectX11::Texture2D Texture2D;
    ::mock::DirectX11::ShaderResourceView ShaderResourceView;

    D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
    TextureDesc.Width = _Width;
    TextureDesc.Height = _Height;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.MiscFlags = 0;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;

    D3D11_SUBRESOURCE_DATA Init = { 0 };
    Init.pSysMem = _BinaryData.data();
    Init.SysMemPitch = (UINT)_Width * 4;

    D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
    SrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SrvDesc.Texture2D.MipLevels = 1;

    InSequence Dummy;

    EXPECT_CALL(Device, CreateTexture2D(TextureDesc, Init))
      .Times(1)
      .WillOnce(Return(&Texture2D));

    EXPECT_CALL(Device, CreateShaderResourceView(&Texture2D, SrvDesc))
      .Times(1)
      .WillOnce(Return(&ShaderResourceView));

    EXPECT_CALL(Texture2D, Release())
      .Times(1);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, 
      PSSetShaderResources(Eq(_Slot), 1, &ShaderResourceView))
      .Times(1);

    Render();

    EXPECT_CALL(ShaderResourceView, Release())
      .Times(1);
  };

  {
    const ::std::vector<uint8_t> BinaryData = { 0x19, 0x07, 0x25, 0x11, 0x51 };
    const int Width = 7251149;
    const int Height = 1907251150;

    const auto pComponent = Component_t::Make(
      {
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("data"), BinaryData.data() },
      });

    TestCall(pComponent, BinaryData, Width, Height, 0);
  }

  {
    const ::std::vector<uint8_t> BinaryData = { 0x18, 0x12, 0x29, 0x11, 0x57 };
    const int Width = 12291159;
    const int Height = 1812291200;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture")},
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("data"), BinaryData.data() },
      });

    auto Render = itDataCreator->second(pData);
    EXPECT_EQ(nullptr, Render);

    TestCall(Component_t::Make({}), BinaryData, Width, Height, 0);
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
    {
      const ::std::vector<uint8_t> BinaryData = { 0x19, 0x07, 0x25, 0x12, 0x06 };
      const int Width = 7251202;
      const int Height = 1907251203;

      const auto pComponent = Component_t::Make(
        {
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("data"), BinaryData.data() },
          { uT("destination"), Destinations[i] },
        });

      TestCall(pComponent, BinaryData, Width, Height, i);
    }

    {
      const ::std::vector<uint8_t> BinaryData = { 0x19, 0x07, 0x25, 0x12, 0x07 };
      const int Width = 7251204;
      const int Height = 1907251205;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture")},
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("data"), BinaryData.data() },
          { uT("destination"), Destinations[i] },
        });

      auto Render = itDataCreator->second(pData);
      EXPECT_EQ(nullptr, Render);

      TestCall(Component_t::Make({}), BinaryData, Width, Height, i);
    }
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
      { uT("data"), (const uint8_t *)ShaderData.data() },
      { uT("count"), ShaderData.size() },
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
    (".+\\.cpp\\([0-9]+\\): Failed: -2147467259 \\[" +
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
        { uT("data"), (const uint8_t *)ShaderData.data() },
        { uT("count"), ShaderData.size() },
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
        { uT("data"), (const uint8_t *)ShaderData.data() },
        { uT("count"), ShaderData.size() },
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
        { uT("data"), (const uint8_t *)ShaderData.data() },
        { uT("count"), ShaderData.size() },
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_DefaultData_Deprecated)
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
    const Component_t::ComponentPtr_t & _pData,
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
    Desc.SrcData = Shader_t::Convert(_Prefix) + ::Predefined + ::Data + ::Input +
      ::DefaultDeprecated + Shader_t::Convert(_Postfix);
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

    if (_pData != nullptr)
    {
      auto itCreator = IExample.GetCreators().find(uT("Data"));
      ASSERT_NE(IExample.GetCreators().end(), itCreator);

      auto Render = itCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

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

  for (const auto & Entry : { uT("VS"), uT("vsTextured") })
  {
    using namespace ::alicorn::extension::std;

    const auto strEntry = string_cast<::std::string, Locale::Ascii128>(Entry);

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("entry"), Entry },
        });

      TestCallRender(pComponent, nullptr, strEntry, "vs_4_0",
        VertexShaderPrefix, "");
    }

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("entry"), Entry },
        });

      const auto pComponent = Component_t::Make({ });

      TestCallRender(pComponent, pData, strEntry, "vs_4_0",
        VertexShaderPrefix, "");
    }
  }
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
    const Component_t::ComponentPtr_t & _pData,
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

    if (_pData != nullptr)
    {
      auto itCreator = IExample.GetCreators().find(uT("Data"));
      ASSERT_NE(IExample.GetCreators().end(), itCreator);

      auto Render = itCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

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

  for (const auto & Entry : { uT("vsGui"), uT("vsLights") })
  {
    using namespace ::alicorn::extension::std;

    const auto strEntry = string_cast<::std::string, Locale::Ascii128>(Entry);

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("entry"), Entry },
        });

      TestCallRender(pComponent, nullptr, strEntry, "vs_4_0",
        VertexShaderPrefix, "");
    }

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("entry"), Entry },
        });

      const auto pComponent = Component_t::Make({ });

      TestCallRender(pComponent, pData, strEntry, "vs_4_0",
        VertexShaderPrefix, "");
    }
  }

  for (const auto & Entry : { uT("psColored"), uT("psTextured") })
  {
    using namespace ::alicorn::extension::std;

    const auto strEntry = string_cast<::std::string, Locale::Ascii128>(Entry);

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

      TestCallRender(pComponent, nullptr, "psMain", "ps_4_0",
        PixelShaderPrefix, PixelShaderPostfix);
    }

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("entry"), Entry },
        });

      const auto pComponent = Component_t::Make({ });

      TestCallRender(pComponent, pData, "psMain", "ps_4_0",
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
    "Pixel vs3(Vertex _Value)\r\n";

  using InputDesc_t = ::std::vector<D3D11_INPUT_ELEMENT_DESC>;

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pShader, 
    const Component_t::ComponentPtr_t & _pData, 
    const InputDesc_t _Layout,
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

    if (_pData != nullptr)
    {
      auto itCreator = IExample.GetCreators().find(uT("Data"));
      ASSERT_NE(IExample.GetCreators().end(), itCreator);

      auto Render = itCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

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
          { uT("data"), (const uint8_t *)ShaderData.data() },
          { uT("count"), ShaderData.size() },
          { uT("entry"), uT("vs1") },
        });

      TestCallRender(pComponent, nullptr, Layout, "vs1");
    }

    {
      const auto pComponent = Component_t::Make({ });

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("data"), (const uint8_t *)ShaderData.data() },
          { uT("count"), ShaderData.size() },
          { uT("entry"), uT("vs1") },
        });

      TestCallRender(pComponent, pData, Layout, "vs1");
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
          { uT("data"), (const uint8_t *)ShaderData.data() },
          { uT("count"), ShaderData.size() },
          { uT("entry"), uT("vs2") },
        });

      TestCallRender(pComponent, nullptr, Layout, "vs2");
    }

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("data"), (const uint8_t *)ShaderData.data() },
          { uT("count"), ShaderData.size() },
          { uT("entry"), uT("vs2") },
        });

      const auto pComponent = Component_t::Make({});

      TestCallRender(pComponent, pData, Layout, "vs2");
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
          { uT("data"), (const uint8_t *)ShaderData.data() },
          { uT("count"), ShaderData.size() },
          { uT("entry"), uT("vs3") },
        });

      TestCallRender(pComponent, nullptr, Layout, "vs3");
    }

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("data"), (const uint8_t *)ShaderData.data() },
          { uT("count"), ShaderData.size() },
          { uT("entry"), uT("vs3") },
        });

      const auto pComponent = Component_t::Make({});

      TestCallRender(pComponent, pData, Layout, "vs3");
    }
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
    "float4 ps1(Pixel _Value) : SV_Target\r\n";

  const ::std::string PixelShaderPostfix =
    "float4 psMain(Pixel _Value) : SV_Target\r\n"
    "{\r\n"
    "  return ps1(_Value);\r\n"
    "}\r\n";

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pShader,
    const Component_t::ComponentPtr_t & _pData,
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
      ::Data + ::Input + Shader_t::Convert(ShaderData + PixelShaderPostfix);
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

    if (_pData != nullptr)
    {
      auto itCreator = IExample.GetCreators().find(uT("Data"));
      ASSERT_NE(IExample.GetCreators().end(), itCreator);

      auto Render = itCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

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

    const auto strEntry = string_cast<::std::string, Locale::Ascii128>(Entry);

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("data"), (const uint8_t *)ShaderData.data() },
          { uT("count"), ShaderData.size() },
          { uT("entry"), Entry },
        });

      TestCallRender(pComponent, nullptr, "psMain");
    }

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Shader") },
          { uT("data"), (const uint8_t *)ShaderData.data() },
          { uT("count"), ShaderData.size() },
          { uT("entry"), Entry },
        });

      const auto pComponent = Component_t::Make({ });

      TestCallRender(pComponent, pData, "psMain");
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_CreateVertex_Fail)
{
  const auto TestCallRender = [](
    const Component_t::ComponentPtr_t _pData,
    const Component_t::ComponentPtr_t _pBuffer)
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

    if (_pData != nullptr)
    {
      auto itCreator = IExample.GetCreators().find(uT("Data"));
      ASSERT_NE(IExample.GetCreators().end(), itCreator);

      const auto Render = itCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

    auto itCreator = IExample.GetCreators().find(uT("Buffer"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    InSequence Dummy;

    EXPECT_CALL(Device, CreateBuffer(_, _))
      .Times(1);

    EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(itCreator->second(_pBuffer), ::std::exception);
  };

  const vertex::Polygon * pPolygon = nullptr;

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("data"), pPolygon },
      });

    TestCallRender(nullptr, pComponent);
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("data"), pPolygon },
      });

    TestCallRender(pData, Component_t::Make({ }));
  }

  const vertex::Polyhedron * pPolyhedron = nullptr;

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("data"), pPolyhedron },
      });

    TestCallRender(nullptr, pComponent);
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("data"), pPolyhedron },
      });

    TestCallRender(pData, Component_t::Make({ }));
  }

  {
    using BufferMapper_t = ::covellite::api::cbBufferMap_t<vertex::Polyhedron>;
    const BufferMapper_t Dummy = [](vertex::Polyhedron *) { return false; };

    const auto pData = Component_t::Make(
      {
        { uT("data"), pPolyhedron },
      });

    const auto pComponent = Component_t::Make(
      {
        { uT("mapper"), Dummy },
      });

    TestCallRender(pData, pComponent);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_Vertex_Static)
{
  const auto TestCallRender = [](
    const Component_t::ComponentPtr_t & _pBuffer,
    const Component_t::ComponentPtr_t & _pData,
    const void * _pRawData, size_t _RawDataSize, size_t _VertexSize)
  {
    using DirectXProxy_t = ::mock::DirectX11::Proxy;
    DirectXProxy_t DirectXProxy;
    DirectXProxy_t::GetInstance() = &DirectXProxy;

    ::mock::DirectX11::Device Device;
    ::mock::DirectX11::DeviceContext DeviceContext;
    ::mock::DirectX11::Buffer Buffer;

    D3D11_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pRawData;

    D3D11_BUFFER_DESC Desc = { 0 };
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.ByteWidth = static_cast<UINT>(_RawDataSize * _VertexSize);
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

    if (_pData != nullptr)
    {
      auto itCreator = IExample.GetCreators().find(uT("Data"));
      ASSERT_NE(IExample.GetCreators().end(), itCreator);

      auto Render = itCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

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
    const ::std::vector<vertex::Polygon> VertexData =
    {
      { 1.0f, 2.0f, 1809081155, 3.0f, 4.0f },
      { 5.0f, 6.0f, 1809081156, 7.0f, 8.0f },
    };

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("data"), VertexData.data() },
          { uT("count"), VertexData.size() },
        });

      TestCallRender(pComponent, nullptr, 
        VertexData.data(), VertexData.size(), sizeof(vertex::Polygon));
    }

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Buffer") },
          { uT("data"), VertexData.data() },
          { uT("count"), VertexData.size() },
        });

      const auto pComponent = Component_t::Make({});

      TestCallRender(pComponent, pData, 
        VertexData.data(), VertexData.size(), sizeof(vertex::Polygon));
    }
  }

  {
    const ::std::vector<vertex::Polyhedron> VertexData =
    {
      { 
        1.0f, 2.0f, 3.0f, 
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f
      },
      { 
        9.0f, 10.0f, 11.0f,
        12.0f, 13.0f, 14.0f,
        15.0f, 16.0f
      },
    };

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("data"), VertexData.data() },
          { uT("count"), VertexData.size() },
        });

      TestCallRender(pComponent, nullptr, 
        VertexData.data(), VertexData.size(), sizeof(vertex::Polyhedron));
    }

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Buffer") },
          { uT("data"), VertexData.data() },
          { uT("count"), VertexData.size() },
        });

      const auto pComponent = Component_t::Make({});

      TestCallRender(pComponent, pData, 
        VertexData.data(), VertexData.size(), sizeof(vertex::Polyhedron));
    }
  }

  {
    const ::std::vector<::covellite::api::Vertex> VertexData =
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

    {
      const auto pComponent = Component_t::Make(
        {
          { uT("data"), VertexData.data() },
          { uT("count"), VertexData.size() },
        });

      TestCallRender(pComponent, nullptr,
        VertexData.data(), VertexData.size(), sizeof(::covellite::api::Vertex));
    }

    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Buffer") },
          { uT("data"), VertexData.data() },
          { uT("count"), VertexData.size() },
        });

      const auto pComponent = Component_t::Make({});

      TestCallRender(pComponent, pData,
        VertexData.data(), VertexData.size(), sizeof(::covellite::api::Vertex));
    }
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_Polyhedron_Dynamic)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<vertex::Polyhedron>;

  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Mapper, bool(vertex::Polyhedron *));
  };

  const auto TestCallRender = [](
    const Component_t::ComponentPtr_t & _pBuffer,
    const Component_t::ComponentPtr_t & _pData,
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

    const BufferMapper_t Mapper = [&](vertex::Polyhedron * _pData)
    {
      return oProxy.Mapper(_pData);
    };

    _pBuffer->SetValue(uT("mapper"), Mapper);

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(DirectXProxy, CreateDeviceContext())
      .Times(1)
      .WillOnce(Return(&DeviceContext));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    if (_pData != nullptr)
    {
      auto itCreator = IExample.GetCreators().find(uT("Data"));
      ASSERT_NE(IExample.GetCreators().end(), itCreator);

      auto Render = itCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

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

      EXPECT_CALL(oProxy, Mapper(reinterpret_cast<vertex::Polyhedron *>(Resource.pData)))
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

  const ::std::vector<vertex::Polyhedron> VertexData =
  {
    {
      1.0f, 2.0f, 3.0f,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f
    },
    {
      9.0f, 10.0f, 11.0f,
      12.0f, 13.0f, 14.0f,
      15.0f, 16.0f
    },
  };

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("data"), VertexData.data() },
        { uT("count"), VertexData.size() },
      });

    TestCallRender(pComponent, nullptr,
      VertexData.data(), VertexData.size(), sizeof(vertex::Polyhedron));
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("data"), VertexData.data() },
        { uT("count"), VertexData.size() },
      });

    const auto pComponent = Component_t::Make({ });

    TestCallRender(pComponent, pData,
      VertexData.data(), VertexData.size(), sizeof(vertex::Polyhedron));
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
    const Component_t::ComponentPtr_t & _pData,
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

    _pBuffer->SetValue(uT("mapper"), Mapper);

    using namespace ::testing;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(DirectXProxy, CreateDeviceContext())
      .Times(1)
      .WillOnce(Return(&DeviceContext));

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    if (_pData != nullptr)
    {
      auto itCreator = IExample.GetCreators().find(uT("Data"));
      ASSERT_NE(IExample.GetCreators().end(), itCreator);

      auto Render = itCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

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

  const ::std::vector<::covellite::api::Vertex> VertexData =
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

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("data"), VertexData.data() },
        { uT("count"), VertexData.size() },
      });

    TestCallRender(pComponent, nullptr,
      VertexData.data(), VertexData.size(), sizeof(::covellite::api::Vertex));
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("data"), VertexData.data() },
        { uT("count"), VertexData.size() },
      });

    const auto pComponent = Component_t::Make({ });

    TestCallRender(pComponent, pData,
      VertexData.data(), VertexData.size(), sizeof(::covellite::api::Vertex));
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_CreateIndex_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  const auto Type = uT("Buffer");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Index") },
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

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult(Eq("CreateBuffer")))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_Index)
{
  const ::std::vector<int> IndexData = { 1, 2, 3, 4, 5, 6 };

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pComponent,
    const Component_t::ComponentPtr_t & _pData)
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

    if (_pData != nullptr)
    {
      auto itCreator = IExample.GetCreators().find(uT("Data"));
      ASSERT_NE(IExample.GetCreators().end(), itCreator);

      auto Render = itCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

    auto itCreator = IExample.GetCreators().find(uT("Buffer"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    InSequence Dummy;

    EXPECT_CALL(Device, CreateBuffer(Desc, InitData))
      .Times(1)
      .WillOnce(Return(&Buffer));

    auto Render = itCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, IASetIndexBuffer(&Buffer, DXGI_FORMAT_R32_UINT, 0))
      .Times(1);

    Render();

    EXPECT_CALL(Buffer, Release())
      .Times(1);
  };

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Index") },
        { uT("data"), IndexData.data() },
        { uT("count"), IndexData.size() },
      });

    TestCallRender(pComponent, nullptr);
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("data"), IndexData.data() },
        { uT("count"), IndexData.size() },
      });

    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Index") },
      });

    TestCallRender(pComponent, pData);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Matrices_StructSizeAlign16Bytes)
{
  EXPECT_EQ(0, sizeof(::Matrices) % 16);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Geometry_UnknownVariety)
{
  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Geometry") },
      { uT("variety"), uT("Unknow1908061947") },
   });

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Geometry)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer ConstantBuffer;
  ::mock::DirectX11::Buffer IndexBuffer;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::Matrices);
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
    VSSetConstantBuffers(MATRICES_BUFFER_INDEX, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    PSSetConstantBuffers(MATRICES_BUFFER_INDEX, 1, &ConstantBuffer))
    .Times(1);

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  const auto TestCallRender = [&](
    const Render_t & _Render, 
    const ::Matrices & _Matrices)
  {
    EXPECT_CALL(DeviceContext, UpdateSubresource(&ConstantBuffer, 0, nullptr,
      _Matrices, 0, 0))
      .Times(1);

    EXPECT_CALL(DeviceContext, IAGetIndexBuffer(DXGI_FORMAT_UNKNOWN, 0))
      .Times(1)
      .WillOnce(Return(&IndexBuffer));

    EXPECT_CALL(IndexBuffer, GetDesc())
      .Times(1)
      .WillOnce(Return(IndexBufferDesc));

    EXPECT_CALL(DeviceContext,
      IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      DrawIndexed(IndexBufferDesc.ByteWidth / sizeof(int), 0, 0))
      .Times(1);

    EXPECT_CALL(IndexBuffer, Release())
      .Times(1);

    _Render();
  };

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Geometry") },
    });

  {
    ::Matrices DefaultMatrices;

    memset(&DefaultMatrices, 0, sizeof(DefaultMatrices));
    DefaultMatrices.World = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixIdentity());

    auto Render = itCreator->second(pComponent);
    ASSERT_NE(nullptr, Render);

    TestCallRender(Render, DefaultMatrices);
  }

  {
    ::Matrices ExpectMatrices;
    memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    const auto SetPosition = [&](float _X, float _Y, float _Z)
    {
      pPosition->SetValue(uT("x"), _X);
      pPosition->SetValue(uT("y"), _Y);
      pPosition->SetValue(uT("z"), _Z);

      ExpectMatrices.World = ::DirectX::XMMatrixTranslation(_X, _Y, _Z);
    };

    auto RotationRender = itDataCreator->second(pRotation);
    EXPECT_EQ(nullptr, RotationRender);

    const auto SetRotation = [&](float _X, float _Y, float _Z)
    {
      pRotation->SetValue(uT("x"), _X);
      pRotation->SetValue(uT("y"), _Y);
      pRotation->SetValue(uT("z"), _Z);

      ExpectMatrices.World *= ::DirectX::XMMatrixRotationX(_X);
      ExpectMatrices.World *= ::DirectX::XMMatrixRotationY(_Y);
      ExpectMatrices.World *= ::DirectX::XMMatrixRotationZ(_Z);
    };

    auto ScaleRender = itDataCreator->second(pScale);
    EXPECT_EQ(nullptr, ScaleRender);

    const auto SetScale = [&](float _X, float _Y, float _Z)
    {
      pScale->SetValue(uT("x"), _X);
      pScale->SetValue(uT("y"), _Y);
      pScale->SetValue(uT("z"), _Z);

      ExpectMatrices.World *= ::DirectX::XMMatrixScaling(_X, _Y, _Z);
    };

    auto Render = itCreator->second(pComponent);
    ASSERT_NE(nullptr, Render);

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);
    ExpectMatrices.World = ::DirectX::XMMatrixTranspose(ExpectMatrices.World);
    TestCallRender(Render, ExpectMatrices);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);
    ExpectMatrices.World = ::DirectX::XMMatrixTranspose(ExpectMatrices.World);
    TestCallRender(Render, ExpectMatrices);
  }

  EXPECT_CALL(ConstantBuffer, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Geometry_Static)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer ConstantBuffer;
  ::mock::DirectX11::Buffer IndexBuffer;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::Matrices);
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
    VSSetConstantBuffers(MATRICES_BUFFER_INDEX, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    PSSetConstantBuffers(MATRICES_BUFFER_INDEX, 1, &ConstantBuffer))
    .Times(1);

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  const auto TestCallRender = [&](
    const Render_t & _Render,
    const ::Matrices & _Matrices)
  {
    EXPECT_CALL(DeviceContext, 
      UpdateSubresource(&ConstantBuffer, 0, nullptr, _Matrices, 0, 0))
      .Times(1);

    EXPECT_CALL(DeviceContext, IAGetIndexBuffer(DXGI_FORMAT_UNKNOWN, 0))
      .Times(1)
      .WillOnce(Return(&IndexBuffer));

    EXPECT_CALL(IndexBuffer, GetDesc())
      .Times(1)
      .WillOnce(Return(IndexBufferDesc));

    EXPECT_CALL(DeviceContext,
      IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      DrawIndexed(IndexBufferDesc.ByteWidth / sizeof(int), 0, 0))
      .Times(1);

    EXPECT_CALL(IndexBuffer, Release())
      .Times(1);

    _Render();
  };

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Static") },
      });

    ::Matrices DefaultMatrices;
    memset(&DefaultMatrices, 0, sizeof(DefaultMatrices));

    DefaultMatrices.World = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixIdentity());

    auto Render = itCreator->second(pComponent);
    ASSERT_NE(nullptr, Render);

    TestCallRender(Render, DefaultMatrices);
  }

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Static") },
      });

    ::Matrices ExpectMatrices;
    memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));
    ExpectMatrices.World = ::DirectX::XMMatrixIdentity();

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    const auto SetPosition = [&](float _X, float _Y, float _Z)
    {
      pPosition->SetValue(uT("x"), _X);
      pPosition->SetValue(uT("y"), _Y);
      pPosition->SetValue(uT("z"), _Z);

      ExpectMatrices.World *= ::DirectX::XMMatrixTranslation(_X, _Y, _Z);
    };

    auto RotationRender = itDataCreator->second(pRotation);
    EXPECT_EQ(nullptr, RotationRender);

    const auto SetRotation = [&](float _X, float _Y, float _Z)
    {
      pRotation->SetValue(uT("x"), _X);
      pRotation->SetValue(uT("y"), _Y);
      pRotation->SetValue(uT("z"), _Z);

      ExpectMatrices.World *= ::DirectX::XMMatrixRotationX(_X);
      ExpectMatrices.World *= ::DirectX::XMMatrixRotationY(_Y);
      ExpectMatrices.World *= ::DirectX::XMMatrixRotationZ(_Z);
    };

    auto ScaleRender = itDataCreator->second(pScale);
    EXPECT_EQ(nullptr, ScaleRender);

    const auto SetScale = [&](float _X, float _Y, float _Z)
    {
      pScale->SetValue(uT("x"), _X);
      pScale->SetValue(uT("y"), _Y);
      pScale->SetValue(uT("z"), _Z);

      ExpectMatrices.World *= ::DirectX::XMMatrixScaling(_X, _Y, _Z);
    };

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);
    ExpectMatrices.World = ::DirectX::XMMatrixTranspose(ExpectMatrices.World);

    const auto StaticMatrices = ExpectMatrices;

    auto Render = itCreator->second(pComponent);
    ASSERT_NE(nullptr, Render);

    TestCallRender(Render, StaticMatrices);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    TestCallRender(Render, StaticMatrices);
  }

  EXPECT_CALL(ConstantBuffer, Release())
    .Times(1);
}

namespace covellite
{

namespace api
{

namespace renderer
{

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Geometry_Billboard)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer ConstantBuffer;
  ::mock::DirectX11::Buffer IndexBuffer;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::Matrices);
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
    VSSetConstantBuffers(MATRICES_BUFFER_INDEX, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    PSSetConstantBuffers(MATRICES_BUFFER_INDEX, 1, &ConstantBuffer))
    .Times(1);

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  const auto TestCallRender = [&](
    const Render_t & _Render,
    const ::Matrices & _Matrices)
  {
    oExample.m_pConstants->Get<::Matrices>().View = _Matrices.View;

    EXPECT_CALL(DeviceContext,
      UpdateSubresource(&ConstantBuffer, 0, nullptr, _Matrices, 0, 0))
      .Times(1);

    EXPECT_CALL(DeviceContext, IAGetIndexBuffer(DXGI_FORMAT_UNKNOWN, 0))
      .Times(1)
      .WillOnce(Return(&IndexBuffer));

    EXPECT_CALL(IndexBuffer, GetDesc())
      .Times(1)
      .WillOnce(Return(IndexBufferDesc));

    EXPECT_CALL(DeviceContext,
      IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      DrawIndexed(IndexBufferDesc.ByteWidth / sizeof(int), 0, 0))
      .Times(1);

    EXPECT_CALL(IndexBuffer, Release())
      .Times(1);

    _Render();
  };

  const auto GetView = [](
    float _X, float _Y, float _Z,
    float _A, float _B, float _C,
    float _Distance)
  {
    const auto Look = ::DirectX::XMVectorSet(_X, _Y, _Z, 1.0f);

    auto Transform =
      ::DirectX::XMMatrixRotationX(_A) *
      ::DirectX::XMMatrixRotationY(_B) *
      ::DirectX::XMMatrixRotationZ(_C) *
      ::DirectX::XMMatrixTranslation(_X, _Y, _Z);
    auto Eye = ::DirectX::XMVector3TransformCoord(
      ::DirectX::XMVectorSet(_Distance, 0.0f, 0.0f, 1.0f),
      Transform);

    return ::DirectX::XMMatrixLookAtRH(Eye, Look,
      ::DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
  };

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Billboard") },
      });

    auto Render = itCreator->second(pComponent);
    ASSERT_NE(nullptr, Render);

    ::Matrices DefaultMatrices;
    memset(&DefaultMatrices, 0, sizeof(DefaultMatrices));

    const auto Distance = 62028.0f;

    const auto X = 62029.0f;
    const auto Y = 62030.0f;
    const auto Z = 62031.0f;

    const auto A = 62032.0f;
    const auto B = 62033.0f;
    const auto C = 62034.0f;

    DefaultMatrices.View = GetView(
      X, Y, Z,
      A, B, C,
      Distance + 0.1f);

    ::DirectX::XMFLOAT4X4 Matrix;
    XMStoreFloat4x4(&Matrix, DefaultMatrices.View);
    Matrix._14 = 0.0f;
    Matrix._24 = 0.0f;
    Matrix._34 = 0.0f;
    Matrix._41 = 0.0f;
    Matrix._42 = 0.0f;
    Matrix._43 = 0.0f;
    Matrix._44 = 1.0f;
    DefaultMatrices.World = XMLoadFloat4x4(&Matrix);
    DefaultMatrices.World = ::DirectX::XMMatrixTranspose(DefaultMatrices.World);

    TestCallRender(Render, DefaultMatrices);
  }

  {
    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Billboard") },
      });

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    auto Render = itCreator->second(pComponent);
    ASSERT_NE(nullptr, Render);

    ::Matrices ExpectMatrices;
    memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));

    const auto Distance = 62021.0f;

    const auto X = 62022.0f;
    const auto Y = 62023.0f;
    const auto Z = 62024.0f;

    const auto A = 62025.0f;
    const auto B = 62026.0f;
    const auto C = 62027.0f;

    ExpectMatrices.View = GetView(
      X, Y, Z,
      A, B, C,
      Distance + 0.1f);

    ::DirectX::XMFLOAT4X4 Matrix;
    XMStoreFloat4x4(&Matrix, ExpectMatrices.View);
    Matrix._14 = 0.0f;
    Matrix._24 = 0.0f;
    Matrix._34 = 0.0f;
    Matrix._41 = 0.0f;
    Matrix._42 = 0.0f;
    Matrix._43 = 0.0f;
    Matrix._44 = 1.0f;

    const auto BillboardMatrix = XMLoadFloat4x4(&Matrix);

    const auto SetPosition = [&](float _X, float _Y, float _Z)
    {
      pPosition->SetValue(uT("x"), _X);
      pPosition->SetValue(uT("y"), _Y);
      pPosition->SetValue(uT("z"), _Z);

      ExpectMatrices.World *= ::DirectX::XMMatrixTranslation(_X, _Y, _Z);
    };

    ExpectMatrices.World = BillboardMatrix;
    SetPosition(1.0f, 2.0f, 3.0f);
    ExpectMatrices.World = ::DirectX::XMMatrixTranspose(ExpectMatrices.World);

    TestCallRender(Render, ExpectMatrices);

    ExpectMatrices.World = BillboardMatrix;
    SetPosition(11.0f, 22.0f, 33.0f);
    ExpectMatrices.World = ::DirectX::XMMatrixTranspose(ExpectMatrices.World);

    TestCallRender(Render, ExpectMatrices);
  }

  EXPECT_CALL(ConstantBuffer, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Geometry_CombineTransform)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  ::Matrices ExpectMatrices;

  const auto SetPosition = [&](float _X, float _Y, float _Z)
  {
    auto pPosition = Component_t::Make(
      { 
        { uT("kind"), uT("Position") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      });

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    ExpectMatrices.World *= ::DirectX::XMMatrixTranslation(_X, _Y, _Z);
  };

  const auto SetRotation = [&](float _X, float _Y, float _Z)
  {
    auto pRotation = Component_t::Make(
      {
        { uT("kind"), uT("Rotation") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      });

    auto RotationRender = itDataCreator->second(pRotation);
    EXPECT_EQ(nullptr, RotationRender);

    ExpectMatrices.World *= ::DirectX::XMMatrixRotationX(_X);
    ExpectMatrices.World *= ::DirectX::XMMatrixRotationY(_Y);
    ExpectMatrices.World *= ::DirectX::XMMatrixRotationZ(_Z);
  };

  const auto SetScale = [&](float _X, float _Y, float _Z)
  {
    auto pScale = Component_t::Make(
      {
        { uT("kind"), uT("Scale") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      });

    auto ScaleRender = itDataCreator->second(pScale);
    EXPECT_EQ(nullptr, ScaleRender);

    ExpectMatrices.World *= ::DirectX::XMMatrixScaling(_X, _Y, _Z);
  };

  {
    memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));
    ExpectMatrices.World = ::DirectX::XMMatrixIdentity();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    ExpectMatrices.World = ::DirectX::XMMatrixTranspose(ExpectMatrices.World);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Geometry") }
      }));
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, ExpectMatrices, _, _))
      .Times(1);

    Render();
  }

  {
    memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));
    ExpectMatrices.World = ::DirectX::XMMatrixIdentity();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    ExpectMatrices.World = ::DirectX::XMMatrixTranspose(ExpectMatrices.World);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Static") },
      }));
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, ExpectMatrices, _, _))
      .Times(1);

    Render();
  }

  {
    memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));

    ::DirectX::XMFLOAT4X4 Matrix;
    XMStoreFloat4x4(&Matrix, ExpectMatrices.View);
    Matrix._14 = 0.0f;
    Matrix._24 = 0.0f;
    Matrix._34 = 0.0f;
    Matrix._41 = 0.0f;
    Matrix._42 = 0.0f;
    Matrix._43 = 0.0f;
    Matrix._44 = 1.0f;

    ExpectMatrices.World = XMLoadFloat4x4(&Matrix);

    SetPosition(1.0f, 2.0f, 3.0f);
    SetPosition(11.0f, 22.0f, 33.0f);

    ExpectMatrices.World = ::DirectX::XMMatrixTranspose(ExpectMatrices.World);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Billboard") },
      }));
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, ExpectMatrices, _, _))
      .Times(1);

    Render();
  }
}

} // namespace renderer

} // namespace api

} // namespace covellite

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present_Geometry_DefaultDataValues)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;

  D3D11_BUFFER_DESC IndexBufferDesc = { 0 };
  IndexBufferDesc.ByteWidth = 1811221344;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  const auto TestCallRender = [&](
    const bool _IsPositionOnly, ::Matrices & _ExpectMatrices,
    const Component_t::ComponentPtr_t & _pComponent)
  {
    auto itCreator = IExample.GetCreators().find(uT("Present"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    auto RotationRender = itDataCreator->second(pRotation);
    EXPECT_EQ(nullptr, RotationRender);

    auto ScaleRender = itDataCreator->second(pScale);
    EXPECT_EQ(nullptr, ScaleRender);

    auto Render = itCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    _ExpectMatrices.World *= ::DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

    if (!_IsPositionOnly)
    {
      _ExpectMatrices.World = 
        _ExpectMatrices.World *
        ::DirectX::XMMatrixRotationX(0.0f) *
        ::DirectX::XMMatrixRotationY(0.0f) *
        ::DirectX::XMMatrixRotationZ(0.0f) *
        ::DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
    }

    _ExpectMatrices.World = ::DirectX::XMMatrixTranspose(_ExpectMatrices.World);

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, _ExpectMatrices, _, _))
      .Times(1);

    Render();
  };

  ::Matrices ExpectMatrices;
  memset(&ExpectMatrices, 0, sizeof(ExpectMatrices));

  ExpectMatrices.World = ::DirectX::XMMatrixIdentity();

  TestCallRender(false, ExpectMatrices, Component_t::Make(
    {
      { uT("kind"), uT("Geometry") }
    }));

  ExpectMatrices.World = ::DirectX::XMMatrixIdentity();

  TestCallRender(false, ExpectMatrices, Component_t::Make(
    {
      { uT("kind"), uT("Geometry") },
      { uT("variety"), uT("Static") },
    }));

  ::DirectX::XMFLOAT4X4 Matrix;
  XMStoreFloat4x4(&Matrix, ExpectMatrices.View);
  Matrix._44 = 1.0f;
  ExpectMatrices.World = XMLoadFloat4x4(&Matrix);

  TestCallRender(true, ExpectMatrices, Component_t::Make(
    {
      { uT("kind"), uT("Geometry") },
      { uT("variety"), uT("Billboard") },
    }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_Orthographic_DefaultPosition)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;

  const auto Width = 1811211754.0f;
  const auto Height = 1811211755.0f;

# pragma warning(push)
# pragma warning(disable: 6001)

  ::Matrices Matrices;

  Matrices.World = ::DirectX::XMMatrixTranspose(
    ::DirectX::XMMatrixIdentity());
  Matrices.Projection = ::DirectX::XMMatrixTranspose(
    ::DirectX::XMMatrixOrthographicOffCenterLH(0.0f, Width, Height, 0.0f, 1.0f, -1.0f));
  Matrices.View = ::DirectX::XMMatrixTranspose(
    ::DirectX::XMMatrixIdentity());

  ::DirectX::XMVECTOR Determinant;
  Matrices.ViewInverse = ::DirectX::XMMatrixTranspose(
    ::DirectX::XMMatrixInverse(&Determinant, ::DirectX::XMMatrixIdentity()));

# pragma warning(pop)

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  auto itPresentCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itPresentCreator);

  // Camera.Orthographic
  {
    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Orthographic") },
      });

    D3D11_VIEWPORT Viewport = { 0 };
    Viewport.Width = Width;
    Viewport.Height = Height;

    auto Render = itCameraCreator->second(pComponent);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, RSGetViewports(1))
      .Times(1)
      .WillOnce(Return(Viewport));

    Render();
  }

  // Present.Geometry
  {
    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
      });

    auto Render = itPresentCreator->second(pComponent);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, Matrices, _, _))
      .Times(1);

    Render();
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_Orthographic)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;

  const auto X = 1902282004.0f;
  const auto Y = 1902282005.0f;
  const auto Width = 1902282006.0f;
  const auto Height = 1902282007.0f;

# pragma warning(push)
# pragma warning(disable: 6001)

  ::Matrices Matrices;

  Matrices.World = ::DirectX::XMMatrixTranspose(
    ::DirectX::XMMatrixIdentity());
  Matrices.Projection = ::DirectX::XMMatrixTranspose(
    ::DirectX::XMMatrixOrthographicOffCenterLH(
      X, X + Width, 
      Y + Height, Y, 
      1.0f, -1.0f));
  Matrices.View = ::DirectX::XMMatrixTranspose(
    ::DirectX::XMMatrixIdentity());

  ::DirectX::XMVECTOR Determinant;
  Matrices.ViewInverse = ::DirectX::XMMatrixTranspose(
    ::DirectX::XMMatrixInverse(&Determinant, ::DirectX::XMMatrixIdentity()));

# pragma warning(pop)

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  auto itPresentCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itPresentCreator);

  // Camera.Orthographic
  {
    const auto pPosition = Component_t::Make(
      {
        { uT("kind"), uT("Position") },
        { uT("x"), X },
        { uT("y"), Y },
      });

    auto DataRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, DataRender);

    const auto pCamera = Component_t::Make(
      {
        { uT("kind"), uT("Orthographic") },
      });

    D3D11_VIEWPORT Viewport = { 0 };
    Viewport.Width = Width;
    Viewport.Height = Height;

    auto Render = itCameraCreator->second(pCamera);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, RSGetViewports(1))
      .Times(1)
      .WillOnce(Return(Viewport));

    Render();
  }

  // Present.Geometry
  {
    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
      });

    auto Render = itPresentCreator->second(pComponent);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, Matrices, _, _))
      .Times(1);

    Render();
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_Perspective)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  auto itPresentCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itPresentCreator);

  const auto pGeometry = Component_t::Make(
    {
      { uT("kind"), uT("Geometry") },
    });

  auto GeometryRender = itPresentCreator->second(pGeometry);
  ASSERT_NE(nullptr, GeometryRender);

  const auto TestCallRender = [&](Render_t & _CameraRender,
    float _Width, float _Height, const ::Matrices & _Matrices)
  {
    D3D11_VIEWPORT Viewport = { 0 };
    Viewport.Width = _Width;
    Viewport.Height = _Height;

    EXPECT_CALL(DeviceContext, RSGetViewports(1))
      .Times(1)
      .WillOnce(Return(Viewport));

    _CameraRender();

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, _Matrices, _, _))
      .Times(1);

    GeometryRender();
  };

  ::Matrices Matrices;
  Matrices.World = ::DirectX::XMMatrixTranspose(
    ::DirectX::XMMatrixIdentity());

  const auto GetProjection = [](float _AngleY, float _Width, float _Height)
  {
    return ::DirectX::XMMatrixPerspectiveFovRH(
      _AngleY * (float)::alicorn::extension::cpp::math::GreedToRadian,
      _Width / _Height, 0.01f, 200.0f);
  };

  const auto GetView = [](
    float _X, float _Y, float _Z,
    float _A, float _B, float _C,
    float _Distance)
  {
    const auto Look = ::DirectX::XMVectorSet(_X, _Y, _Z, 1.0f);

    auto Transform =
      ::DirectX::XMMatrixRotationX(_A) *
      ::DirectX::XMMatrixRotationY(_B) *
      ::DirectX::XMMatrixRotationZ(_C) *
      ::DirectX::XMMatrixTranslation(_X, _Y, _Z);
    auto Eye = ::DirectX::XMVector3TransformCoord(
      ::DirectX::XMVectorSet(_Distance, 0.0f, 0.0f, 1.0f),
      Transform);

    return ::DirectX::XMMatrixLookAtRH(Eye, Look,
      ::DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
  };

  const auto pCamera = Component_t::Make(
    {
      { uT("kind"), uT("Perspective") },
    });

  // Default values
  {
    const auto AngleY = 90.0f;
    const auto Width = 1901011221.0f;
    const auto Height = 1901011222.0f;

    Matrices.Projection = ::DirectX::XMMatrixTranspose(
      GetProjection(AngleY, Width, Height));

    const auto View = GetView(
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.1f);
    Matrices.View = ::DirectX::XMMatrixTranspose(View);

    ::DirectX::XMVECTOR Determinant;
    Matrices.ViewInverse = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixInverse(&Determinant, View));

    auto CameraRender = itCameraCreator->second(pCamera);
    ASSERT_NE(nullptr, CameraRender);

    TestCallRender(CameraRender, Width, Height, Matrices);
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

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    auto RotationRender = itDataCreator->second(pRotation);
    EXPECT_EQ(nullptr, RotationRender);

    auto CameraRender = itCameraCreator->second(pCamera);
    ASSERT_NE(nullptr, CameraRender);

    {
      const auto AngleY = 123.0f;
      const auto Distance = 1234.0f;
      const auto Width = 1901011221.0f;
      const auto Height = 1901011222.0f;

      const auto X = 11327.0f;
      const auto Y = 11328.0f;
      const auto Z = 11329.0f;

      const auto A = 11330.0f;
      const auto B = 11331.0f;
      const auto C = 11332.0f;

      Matrices.Projection = ::DirectX::XMMatrixTranspose(
        GetProjection(AngleY, Width, Height));

      const auto View = GetView(
        X, Y, Z,
        A, B, C,
        Distance + 0.1f);

      Matrices.View = ::DirectX::XMMatrixTranspose(View);

      ::DirectX::XMVECTOR Determinant;
      Matrices.ViewInverse = ::DirectX::XMMatrixTranspose(
        ::DirectX::XMMatrixInverse(&Determinant, View));

      pPosition->SetValue(uT("x"), X);
      pPosition->SetValue(uT("y"), Y);
      pPosition->SetValue(uT("z"), Z);

      pRotation->SetValue(uT("x"), A);
      pRotation->SetValue(uT("y"), B);
      pRotation->SetValue(uT("z"), C);

      pCamera->SetValue(uT("fov"), AngleY);
      pCamera->SetValue(uT("distance"), Distance);

      TestCallRender(CameraRender, Width, Height, Matrices);
    }

    {
      const auto AngleY = 456.0f;
      const auto Distance = 4567.0f;
      const auto Width = 1901011221.0f;
      const auto Height = 1901011222.0f;

      const auto X = 11333.0f;
      const auto Y = 11334.0f;
      const auto Z = 11335.0f;

      const auto A = 11336.0f;
      const auto B = 11337.0f;
      const auto C = 11338.0f;

      Matrices.Projection = ::DirectX::XMMatrixTranspose(
        GetProjection(AngleY, Width, Height));

      const auto View = GetView(
        X, Y, Z,
        A, B, C,
        Distance + 0.1f);

      Matrices.View = ::DirectX::XMMatrixTranspose(View);

      ::DirectX::XMVECTOR Determinant;
      Matrices.ViewInverse = ::DirectX::XMMatrixTranspose(
        ::DirectX::XMMatrixInverse(&Determinant, View));

      pPosition->SetValue(uT("x"), X);
      pPosition->SetValue(uT("y"), Y);
      pPosition->SetValue(uT("z"), Z);

      pRotation->SetValue(uT("x"), A);
      pRotation->SetValue(uT("y"), B);
      pRotation->SetValue(uT("z"), C);

      pCamera->SetValue(uT("fov"), AngleY);
      pCamera->SetValue(uT("distance"), Distance);

      TestCallRender(CameraRender, Width, Height, Matrices);
    }
  }
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera_Depth_Disabled)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;
  ::mock::DirectX11::DepthStencilView DepthStencilView;

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

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pCamera)
  {
    auto Render = itCreator->second(_pCamera);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(DeviceContext,
      OMSetRenderTargets(1, &RenderTargetView, nullptr))
      .Times(1);

    Render();
  };

  TestCallRender(Component_t::Make({ { uT("kind"), uT("Orthographic") } }));
  TestCallRender(Component_t::Make({ { uT("kind"), uT("Perspective") } }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Lights_StructSizeAlign16Bytes)
{
  EXPECT_EQ(0, sizeof(::Lights) % 16);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Lights_SendInfoToPixelShader)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::LightsBuffer LightsBuffer;

  D3D11_BUFFER_DESC LightsBufferDesc = { 0 };
  LightsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  LightsBufferDesc.ByteWidth = sizeof(::Lights);
  LightsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  ::Lights EmptyLights; // = { 0 } недостаточно!
  memset(&EmptyLights, 0, sizeof(EmptyLights));

  using namespace ::testing;

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pCamera)
  {
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

    EXPECT_CALL(Device, CreateBuffer(LightsBufferDesc, _))
      .Times(1)
      .WillOnce(Return(&LightsBuffer));

    EXPECT_CALL(DeviceContext,
      VSSetConstantBuffers(LIGHTS_BUFFER_INDEX, 1, &LightsBuffer))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      PSSetConstantBuffers(LIGHTS_BUFFER_INDEX, 1, &LightsBuffer))
      .Times(1);

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
    ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

    auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
    ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

    const ::std::string ShaderData{ "float4 ps(??? _Value) : SV_Target\r\n" };

    auto pPixelShader = Component_t::Make(
      {
        { uT("data"), (const uint8_t *)ShaderData.data() },
        { uT("count"), ShaderData.size() },
        { uT("entry"), uT("ps") },
      });

    auto PixelShaderRender = itShaderCreator->second(pPixelShader);
    ASSERT_NE(nullptr, PixelShaderRender);

    auto CameraRender = itCameraCreator->second(_pCamera);
    ASSERT_NE(nullptr, CameraRender);

    CameraRender(); // Первый раз может быть установлен мусор, поэтому...
    CameraRender();

    EXPECT_CALL(DeviceContext,
      UpdateSubresource(&LightsBuffer, 0, nullptr, EmptyLights, 0, 0))
      .Times(1);

    PixelShaderRender();
  };

  TestCallRender(Component_t::Make(
    {
      { uT("id"), uT("Camera.1") },
      { uT("kind"), uT("Orthographic") },
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("id"), uT("Camera.2") },
      { uT("kind"), uT("Perspective") },
    }));
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Light_Ambient)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  D3D11_BUFFER_DESC LightsBufferDesc = { 0 };
  LightsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  LightsBufferDesc.ByteWidth = sizeof(::Lights);
  LightsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::LightsBuffer LightsBuffer;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBuffer(LightsBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&LightsBuffer));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itLightCreator = IExample.GetCreators().find(uT("Light"));
  ASSERT_NE(IExample.GetCreators().end(), itLightCreator);

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  const ::std::string ShaderData{ "float4 ps(??? _Value) : SV_Target\r\n" };

  auto pPixelShader = Component_t::Make(
    {
      { uT("data"), (const uint8_t *)ShaderData.data() },
      { uT("count"), ShaderData.size() },
      { uT("entry"), uT("ps") },
    });

  ::Lights EmptyLights; // = { 0 } недостаточно!
  memset(&EmptyLights, 0, sizeof(EmptyLights));

  ::Lights DefaultLights; // = { 0 } недостаточно!
  memset(&DefaultLights, 0, sizeof(DefaultLights));
  DefaultLights.Ambient.Color = ARGBtoFloat4(0xFF000000);
  DefaultLights.Ambient.IsValid = 1;

  const auto Ambient = 0xFF031717;

  ::Lights ExpectedLights; // = { 0 } недостаточно!
  memset(&ExpectedLights, 0, sizeof(ExpectedLights));
  ExpectedLights.Ambient.Color = ARGBtoFloat4(Ambient);
  ExpectedLights.Ambient.IsValid = 1;

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera1,
    const Component_t::ComponentPtr_t & _pCamera2)
  {
    auto pLight = Component_t::Make(
      {
        { uT("kind"), uT("Ambient") },
      });

    auto LightRender = itLightCreator->second(pLight);
    ASSERT_NE(nullptr, LightRender);

    auto PixelShaderRender = itShaderCreator->second(pPixelShader);
    ASSERT_NE(nullptr, PixelShaderRender);

    auto Camera1Render = itCameraCreator->second(_pCamera1);
    ASSERT_NE(nullptr, Camera1Render);

    auto Camera2Render = itCameraCreator->second(_pCamera2);
    ASSERT_NE(nullptr, Camera2Render);

    // На случай, если при первом рендеринге будет записан мусор.
    Camera1Render();
    Camera2Render();

    const auto TestUsingLight = [&](const Render_t & _Camera,
      const Render_t & _Light, const ::Lights & _ExpectedLights)
    {
      _Camera();

      _Light();

      EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, _ExpectedLights, _, _))
        .Times(1);

      PixelShaderRender();
    };

    InSequence Dummy;

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, LightRender, EmptyLights);

    pLight->SetValue(uT("color"), Ambient);

    TestUsingLight(Camera1Render, LightRender, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, DefaultLights);

    TestUsingLight(Camera1Render, [](void) {}, ExpectedLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);
  };

  auto pCamera1 = Component_t::Make(
    {
      { uT("id"), uT("Camera.1") },
      { uT("kind"), uT("Orthographic") },
    });

  auto pCamera2 = Component_t::Make(
    {
      { uT("id"), uT("Camera.2") },
      { uT("kind"), uT("Orthographic") },
    });

  TestCallRender(pCamera1, pCamera2);

  auto pFocalCamera1 = Component_t::Make(
    {
      { uT("id"), uT("Camera.3") },
      { uT("kind"), uT("Perspective") },
    });

  auto pFocalCamera2 = Component_t::Make(
    {
      { uT("id"), uT("Camera.4") },
      { uT("kind"), uT("Perspective") },
    });

  TestCallRender(pFocalCamera1, pFocalCamera2);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Light_Direction)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  D3D11_BUFFER_DESC LightsBufferDesc = { 0 };
  LightsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  LightsBufferDesc.ByteWidth = sizeof(::Lights);
  LightsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::LightsBuffer LightsBuffer;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBuffer(LightsBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&LightsBuffer));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itLightCreator = IExample.GetCreators().find(uT("Light"));
  ASSERT_NE(IExample.GetCreators().end(), itLightCreator);

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  const ::std::string ShaderData{ "float4 ps(??? _Value) : SV_Target\r\n" };

  auto pPixelShader = Component_t::Make(
    {
      { uT("data"), (const uint8_t *)ShaderData.data() },
      { uT("count"), ShaderData.size() },
      { uT("entry"), uT("ps") },
    });

  ::Lights EmptyLights; // = { 0 } недостаточно!
  memset(&EmptyLights, 0, sizeof(EmptyLights));

  ::Lights DefaultLights; // = { 0 } недостаточно!
  memset(&DefaultLights, 0, sizeof(DefaultLights));
  DefaultLights.Direction.IsValid = 1;
  DefaultLights.Direction.Color = ARGBtoFloat4(0xFF000000);
  DefaultLights.Direction.Direction = { 1.0f, 0.0f, 0.0f, 0.0f };

  const auto Diffuse = 0xFF031836;

  const auto X = 1901031851.0f;
  const auto Y = 1901031852.0f;
  const auto Z = 1901031854.0f;

  ::Lights ExpectedLights; // = { 0 } недостаточно!
  memset(&ExpectedLights, 0, sizeof(ExpectedLights));
  ExpectedLights.Direction.IsValid = 1;
  ExpectedLights.Direction.Color = ARGBtoFloat4(Diffuse);
  ExpectedLights.Direction.Direction = { X, Y, Z, 0.0f };

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera1,
    const Component_t::ComponentPtr_t & _pCamera2)
  {
    auto pLight = Component_t::Make(
      {
        { uT("kind"), uT("Direction") },
      });

    auto pDirection = Component_t::Make(
      {
        { uT("kind"), uT("Direction") },
      });

    auto LightRender = itLightCreator->second(pLight);
    ASSERT_NE(nullptr, LightRender);

    auto DirectionRender = itDataCreator->second(pDirection);
    EXPECT_EQ(nullptr, DirectionRender);

    auto LightDataRender = itLightCreator->second(pLight);
    ASSERT_NE(nullptr, LightDataRender);

    auto PixelShaderRender = itShaderCreator->second(pPixelShader);
    ASSERT_NE(nullptr, PixelShaderRender);

    auto Camera1Render = itCameraCreator->second(_pCamera1);
    ASSERT_NE(nullptr, Camera1Render);

    auto Camera2Render = itCameraCreator->second(_pCamera2);
    ASSERT_NE(nullptr, Camera2Render);

    // На случай, если при первом рендеринге будет записан мусор.
    Camera1Render();
    Camera2Render();

    const auto TestUsingLight = [&](const Render_t & _Camera,
      const Render_t & _Light, const ::Lights & _ExpectedLights)
    {
      _Camera();

      _Light();

      EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, _ExpectedLights, _, _))
        .Times(1);

      PixelShaderRender();

    };

    InSequence Dummy;

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, LightRender, EmptyLights);

    pDirection->SetValue(uT("x"), 1.0f);
    pDirection->SetValue(uT("y"), 0.0f);
    pDirection->SetValue(uT("z"), 0.0f);

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, LightDataRender, DefaultLights);

    pDirection->SetValue(uT("x"), X);
    pDirection->SetValue(uT("y"), Y);
    pDirection->SetValue(uT("z"), Z);

    pLight->SetValue(uT("color"), Diffuse);

    TestUsingLight(Camera1Render, LightDataRender, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, DefaultLights);

    TestUsingLight(Camera1Render, [](void) {}, ExpectedLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);
  };

  auto pCamera1 = Component_t::Make(
    {
      { uT("id"), uT("Camera.1") },
      { uT("kind"), uT("Orthographic") },
    });

  auto pCamera2 = Component_t::Make(
    {
      { uT("id"), uT("Camera.2") },
      { uT("kind"), uT("Orthographic") },
    });

  TestCallRender(pCamera1, pCamera2);

  auto pFocalCamera1 = Component_t::Make(
    {
      { uT("id"), uT("Camera.3") },
      { uT("kind"), uT("Perspective") },
    });

  auto pFocalCamera2 = Component_t::Make(
    {
      { uT("id"), uT("Camera.4") },
      { uT("kind"), uT("Perspective") },
    });

  TestCallRender(pFocalCamera1, pFocalCamera2);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Light_Points)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  D3D11_BUFFER_DESC LightsBufferDesc = { 0 };
  LightsBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  LightsBufferDesc.ByteWidth = sizeof(::Lights);
  LightsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::LightsBuffer LightsBuffer;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBuffer(LightsBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&LightsBuffer));

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itLightCreator = IExample.GetCreators().find(uT("Light"));
  ASSERT_NE(IExample.GetCreators().end(), itLightCreator);

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  const ::std::string ShaderData{ "float4 ps(??? _Value) : SV_Target\r\n" };

  auto pPixelShader = Component_t::Make(
    {
      { uT("data"), (const uint8_t *)ShaderData.data() },
      { uT("count"), ShaderData.size() },
      { uT("entry"), uT("ps") },
    });

  ::Lights EmptyLights; // = { 0 } недостаточно!
  memset(&EmptyLights, 0, sizeof(EmptyLights));

  ::Lights DefaultLight; // = { 0 } недостаточно!
  memset(&DefaultLight, 0, sizeof(DefaultLight));
  DefaultLight.Points.Lights[0].Color = ARGBtoFloat4(0xFF000000);
  DefaultLight.Points.Lights[0].Position = { 0.0f, 0.0f, 0.0f, 1.0f };
  DefaultLight.Points.Lights[0].Attenuation = { 1.0f, 0.0f, 0.0f, 0.0f };
  DefaultLight.Points.UsedSlotCount = 1;

  class LightData
  {
  public:
    uint32_t Diffuse;
    float X, Y, Z;
    float Const, Linear, Exponent;
  };

  const ::std::vector<LightData> SourceData =
  {
    {
      0xFF031933,
      1901031927.0f, 1901031928.0f, 1901031929.0f,
      1901031930.0f, 1901031931.0f, 1901031932.0f,
    },
    {
      0xFF112233,
      1901032025.0f, 1901032026.0f, 1901032027.0f,
      1901032028.0f, 1901032029.0f, 1901032030.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
  };

  ::Lights DefaultLights; // = { 0 } недостаточно!
  memset(&DefaultLights, 0, sizeof(DefaultLights));
  DefaultLights.Points.UsedSlotCount =
    static_cast<uint32_t>(SourceData.size());

  ::Lights ExpectedLights; // = { 0 } недостаточно!
  memset(&ExpectedLights, 0, sizeof(ExpectedLights));
  ExpectedLights.Points.UsedSlotCount =
    static_cast<uint32_t>(SourceData.size());

  for (size_t i = 0; i < SourceData.size(); i++)
  {
    DefaultLights.Points.Lights[i] = DefaultLight.Points.Lights[0];

    ExpectedLights.Points.Lights[i].Color = 
      ARGBtoFloat4(SourceData[i].Diffuse);
    ExpectedLights.Points.Lights[i].Position =
    { SourceData[i].X, SourceData[i].Y, SourceData[i].Z, 1.0f };
    ExpectedLights.Points.Lights[i].Attenuation =
    { SourceData[i].Const, SourceData[i].Linear, SourceData[i].Exponent, 0.0f };
  }

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera1,
    const Component_t::ComponentPtr_t & _pCamera2)
  {
    auto pLight = Component_t::Make(
      {
        { uT("kind"), uT("Point") },
      });

    auto LightRender = itLightCreator->second(pLight);
    ASSERT_NE(nullptr, LightRender);

    ::std::vector<Component_t::ComponentPtr_t> Positions;
    ::std::vector<Component_t::ComponentPtr_t> Attenuations;
    ::std::vector<Component_t::ComponentPtr_t> Lights;
    ::std::vector<Render_t> LightRenders;

    for (auto & Value : SourceData)
    {
      ::boost::ignore_unused(Value);

      auto pPosition = Component_t::Make(
        {
          { uT("kind"), uT("Position") },
        });

      Positions.push_back(pPosition);

      auto PositionRender = itDataCreator->second(pPosition);
      EXPECT_EQ(nullptr, PositionRender);

      auto pAttenuation = Component_t::Make(
        {
          { uT("kind"), uT("Attenuation") },
        });

      auto AttenuationRender = itDataCreator->second(pAttenuation);
      EXPECT_EQ(nullptr, AttenuationRender);

      Attenuations.push_back(pAttenuation);

      auto pLocalLight = Component_t::Make(
        {
          { uT("kind"), uT("Point") },
        });

      auto LocalLightRender = itLightCreator->second(pLocalLight);
      ASSERT_NE(nullptr, LocalLightRender);

      Lights.push_back(pLocalLight);
      LightRenders.push_back(LocalLightRender);
    }

    const auto LightDataRender = [&](void)
    {
      for (auto & Render : LightRenders) Render();
    };

    auto PixelShaderRender = itShaderCreator->second(pPixelShader);
    ASSERT_NE(nullptr, PixelShaderRender);

    auto Camera1Render = itCameraCreator->second(_pCamera1);
    ASSERT_NE(nullptr, Camera1Render);

    auto Camera2Render = itCameraCreator->second(_pCamera2);
    ASSERT_NE(nullptr, Camera2Render);

    // На случай, если при первом рендеринге будет записан мусор.
    Camera1Render();
    Camera2Render();

    const auto TestUsingLight = [&](const Render_t & _Camera,
      const Render_t & _Light, const ::Lights & _ExpectedLights)
    {
      _Camera();

      _Light();

      EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, _ExpectedLights, _, _))
        .Times(1);

      PixelShaderRender();
    };

    InSequence Dummy;

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, LightRender, EmptyLights);

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, LightDataRender, DefaultLight);

    for (size_t i = 0; i < SourceData.size(); i++)
    {
      Positions[i]->SetValue(uT("x"), SourceData[i].X);
      Positions[i]->SetValue(uT("y"), SourceData[i].Y);
      Positions[i]->SetValue(uT("z"), SourceData[i].Z);

      Attenuations[i]->SetValue(uT("const"), SourceData[i].Const);
      Attenuations[i]->SetValue(uT("linear"), SourceData[i].Linear);
      Attenuations[i]->SetValue(uT("exponent"), SourceData[i].Exponent);

      Lights[i]->SetValue(uT("color"), SourceData[i].Diffuse);
    }

    TestUsingLight(Camera1Render, LightDataRender, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, DefaultLights);

    TestUsingLight(Camera1Render, [](void) {}, ExpectedLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);
  };

  auto pCamera1 = Component_t::Make(
    {
      { uT("id"), uT("Camera.1") },
      { uT("kind"), uT("Orthographic") },
    });

  auto pCamera2 = Component_t::Make(
    {
      { uT("id"), uT("Camera.2") },
      { uT("kind"), uT("Orthographic") },
    });

  TestCallRender(pCamera1, pCamera2);

  auto pFocalCamera1 = Component_t::Make(
    {
      { uT("id"), uT("Camera.3") },
      { uT("kind"), uT("Perspective") },
    });

  auto pFocalCamera2 = Component_t::Make(
    {
      { uT("id"), uT("Camera.4") },
      { uT("kind"), uT("Perspective") },
    });

  TestCallRender(pFocalCamera1, pFocalCamera2);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Light_Points_LimitCount)
{
  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itLightCreator = IExample.GetCreators().find(uT("Light"));
  ASSERT_NE(IExample.GetCreators().end(), itLightCreator);

  auto pLight = Component_t::Make(
    {
      { uT("kind"), uT("Point") },
    });

  auto LightRender = itLightCreator->second(pLight);
  ASSERT_NE(nullptr, LightRender);

  for (size_t i = 0; i < MAX_LIGHT_POINT_COUNT; i++)
  {
    LightRender();
  }

  // Лишний источник света - не должно упасть.
  LightRender();
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Fog_StructSizeAlign16Bytes)
{
  EXPECT_EQ(0, sizeof(::Fog) % 16);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Fog_SendInfoToPixelShader)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::FogBuffer Buffer;

  D3D11_BUFFER_DESC BufferDesc = { 0 };
  BufferDesc.Usage = D3D11_USAGE_DEFAULT;
  BufferDesc.ByteWidth = sizeof(::Fog);
  BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  using namespace ::testing;

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pData,
    const Component_t::ComponentPtr_t & _pFog,
    const uint32_t _Color,
    const ::Fog & _Expected)
  {
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

    EXPECT_CALL(Device, CreateBuffer(BufferDesc, _))
      .Times(1)
      .WillOnce(Return(&Buffer));

    EXPECT_CALL(DeviceContext,
      VSSetConstantBuffers(FOG_BUFFER_INDEX, 1, &Buffer))
      .Times(1);

    EXPECT_CALL(DeviceContext,
      PSSetConstantBuffers(FOG_BUFFER_INDEX, 1, &Buffer))
      .Times(1);

    const Tested_t oExample{ Data_t{} };
    const ITested_t & IExample = oExample;

    auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
    ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

    const ::std::string ShaderData{ "float4 ps(??? _Value) : SV_Target\r\n" };

    auto pPixelShader = Component_t::Make(
      {
        { uT("data"), (const uint8_t *)ShaderData.data() },
        { uT("count"), ShaderData.size() },
        { uT("entry"), uT("ps") },
      });

    auto PixelShaderRender = itShaderCreator->second(pPixelShader);
    ASSERT_NE(nullptr, PixelShaderRender);

    if (_pData != nullptr)
    {
      auto itDataCreator = IExample.GetCreators().find(uT("Data"));
      ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

      auto Render = itDataCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

    auto itFogCreator = IExample.GetCreators().find(uT("Fog"));
    ASSERT_NE(IExample.GetCreators().end(), itFogCreator);

    auto FogRender = itFogCreator->second(_pFog);
    ASSERT_NE(nullptr, FogRender);

    FogRender();

    EXPECT_CALL(DeviceContext,
      UpdateSubresource(&Buffer, 0, nullptr, _Expected, 0, 0))
      .Times(1);

    PixelShaderRender();

    if (_pData != nullptr)
    {
      ::Fog Expected = _Expected;
      Expected.Color = ARGBtoFloat4(_Color);
      Expected.Near += 2.0f;
      Expected.Far += 3.0f;
      Expected.Density += 4.0f;

      _pData->SetValue(uT("color"), _Color);
      _pData->SetValue(uT("near"), Expected.Near);
      _pData->SetValue(uT("far"), Expected.Far);
      _pData->SetValue(uT("density"), Expected.Density);

      FogRender();

      EXPECT_CALL(DeviceContext,
        UpdateSubresource(&Buffer, 0, nullptr, Expected, 0, 0))
        .Times(1);

      PixelShaderRender();
    }
  };

  ::Fog FogData;
  auto Color = 0xFFFFFFFF;
  FogData.Color = ARGBtoFloat4(Color);
  FogData.Near = 10.0f;
  FogData.Far = 100.0f;
  FogData.Density = 1.0f;

  const auto pDefaultFog = Component_t::Make({ });
  TestCallRender(nullptr, pDefaultFog, Color, FogData);

  const auto Styles = { uT("linear"), uT("exp"), uT("exp2") };

  for (const auto & Style : Styles)
  {
    Color -= 10;
    FogData.Color = ARGBtoFloat4(Color);
    FogData.Near += 11.0f;
    FogData.Far += 12.0f;
    FogData.Density += 13.0f;

    const auto pFog = Component_t::Make(
      {
        { uT("style"), Style },
        { uT("color"), Color },
        { uT("near"), FogData.Near },
        { uT("far"), FogData.Far },
        { uT("density"), FogData.Density },
      });

    TestCallRender(nullptr, pFog, Color, FogData);
  }

  for (const auto & Style : Styles)
  {
    Color -= 10;
    FogData.Color = ARGBtoFloat4(Color);
    FogData.Near += 11.0f;
    FogData.Far += 12.0f;
    FogData.Density += 13.0f;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Fog") },
        { uT("color"), Color },
        { uT("near"), FogData.Near },
        { uT("far"), FogData.Far },
        { uT("density"), FogData.Density },
      });

    const auto pFog = Component_t::Make(
      {
        { uT("style"), Style },
      });

    TestCallRender(pData, pFog, Color, FogData);
  }
}

#define Updater_test DirectX11_test
#include "../Updater_test.hpp"
