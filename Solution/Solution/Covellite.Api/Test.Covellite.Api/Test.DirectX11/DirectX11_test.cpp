
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса DirectX11.
*/

// Расположение класса DirectX11
#include "..\..\Covellite.Api\Renderer\DirectX11.hpp"

#include "../../Covellite.Api/Renderer/fx/Render.auto.hpp"

// Общий тестовый класс класса DirectX11
class DirectX11_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::DirectX11;
  using IRender_t = ::covellite::api::renderer::IRenderer;
  using IGraphicApi_t = ::covellite::api::renderer::IGraphicApi;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<HRESULT>::Set(S_OK);
    ::testing::DefaultValue<ID3D11Device *>::Set(&m_DefaultDevice);
    ::testing::DefaultValue<ID3D11DeviceContext *>::Set(&m_DefaultDeviceContext);
    ::testing::DefaultValue<ID3D11RasterizerState *>::Set(&m_DefaultRasterizerState);
    ::testing::DefaultValue<ID3DBlob *>::Set(&m_DefaultBlob);
    ::testing::DefaultValue<ID3D11InputLayout *>::Set(&m_DefaultInputLayout);
    ::testing::DefaultValue<ID3D11SamplerState *>::Set(&m_DefaultSamplerState);
    ::testing::DefaultValue<IDXGISwapChain *>::Set(&m_DefaultSwapChain);
    ::testing::DefaultValue<DXGI_SWAP_CHAIN_DESC>::Set(m_DefaultDesc);
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<HRESULT>::Clear();
    ::testing::DefaultValue<ID3D11Device *>::Clear();
    ::testing::DefaultValue<ID3D11DeviceContext *>::Clear();
    ::testing::DefaultValue<ID3D11RasterizerState *>::Clear();
    ::testing::DefaultValue<ID3DBlob *>::Clear();
    ::testing::DefaultValue<ID3D11InputLayout *>::Clear();
    ::testing::DefaultValue<ID3D11SamplerState *>::Clear();
    ::testing::DefaultValue<IDXGISwapChain *>::Clear();
    ::testing::DefaultValue<DXGI_SWAP_CHAIN_DESC>::Clear();
  }

private:
  ::mock::DirectX11::Device m_DefaultDevice;
  ::mock::DirectX11::DeviceContext m_DefaultDeviceContext;
  ::mock::DirectX11::RasterizerState m_DefaultRasterizerState;
  ::mock::DirectX11::Blob m_DefaultBlob;
  ::mock::DirectX11::InputLayout m_DefaultInputLayout;
  ::mock::DirectX11::SamplerState m_DefaultSamplerState;
  ::mock::DXGI::SwapChain m_DefaultSwapChain;
  DXGI_SWAP_CHAIN_DESC m_DefaultDesc = { 0 };
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
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateDeviceAndSwapChain_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceAndSwapChain(_))
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateDeviceAndSwapChain_Windowed)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  Tested_t::Data Data = { 0 };
  Data.Handle = (HWND)1809051128;
  Data.IsFullScreen = false;

  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  ::mock::DirectX11::DeviceDesc Desc = { 0 };
  Desc.DriverType = D3D_DRIVER_TYPE_HARDWARE;
  Desc.Flags = (IS_RELEASE_CONFIGURATION) ? 0 : D3D11_CREATE_DEVICE_DEBUG;
  Desc.FeatureLevels = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
  Desc.SDKVersion = D3D11_SDK_VERSION;
  Desc.SwapChainDesc.OutputWindow = Data.Handle;
  Desc.SwapChainDesc.Windowed = TRUE;
  Desc.SwapChainDesc.BufferCount = 2;
  Desc.SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  Desc.SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  Desc.SwapChainDesc.SampleDesc.Count = 1;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceAndSwapChain(Desc))
    .Times(1);

  Tested_t Example{ Data };
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateDeviceAndSwapChain_FullScreen)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  Tested_t::Data Data = { 0 };
  Data.Handle = (HWND)1809051304;
  Data.IsFullScreen = true;

  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  ::mock::DirectX11::DeviceDesc Desc = { 0 };
  Desc.DriverType = D3D_DRIVER_TYPE_HARDWARE;
  Desc.Flags = (IS_RELEASE_CONFIGURATION) ? 0 : D3D11_CREATE_DEVICE_DEBUG;
  Desc.FeatureLevels = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
  Desc.SDKVersion = D3D11_SDK_VERSION;
  Desc.SwapChainDesc.OutputWindow = Data.Handle;
  Desc.SwapChainDesc.Windowed = FALSE;
  Desc.SwapChainDesc.BufferCount = 2;
  Desc.SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  Desc.SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  Desc.SwapChainDesc.SampleDesc.Count = 1;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceAndSwapChain(Desc))
    .Times(1);

  Tested_t Example{ Data };
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
    Tested_t Example{ Tested_t::Data{} };

    EXPECT_CALL(Device, Release())
      .Times(1);

    EXPECT_CALL(DeviceContext, Release())
      .Times(1);

    EXPECT_CALL(SwapChain, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Constructor_SetViewport)
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

  EXPECT_CALL(DeviceContext, OMSetRenderTargets(1, &RenderTargetView, nullptr))
    .Times(1);

  EXPECT_CALL(DeviceContext, RSSetViewports(Viewports))
    .Times(1);

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  {
    Tested_t Example{ Tested_t::Data{} };

    EXPECT_CALL(RenderTargetView, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_SwapChain_GetDesc_Faill)
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

  EXPECT_CALL(SwapChain, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_CALL(DeviceContext, Release())
    .Times(1);

  EXPECT_CALL(SwapChain, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_SwapChain_GetBuffer_Fail)
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

  EXPECT_CALL(SwapChain, GetResult())
    .Times(2)
    .WillOnce(Return(S_OK))
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_CALL(DeviceContext, Release())
    .Times(1);

  EXPECT_CALL(SwapChain, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Device_CreateRenderTargetView_Fail)
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

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_CALL(DeviceContext, Release())
    .Times(1);

  EXPECT_CALL(SwapChain, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateConstantBuffer_Fail)
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

  EXPECT_CALL(Device, GetResult())
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_ClearWindow)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;

  Tested_t::Data Data = { 0 };
  Data.BkColor.R = 1809061119.0f;
  Data.BkColor.G = 1809061120.0f;
  Data.BkColor.B = 1809061121.0f;
  Data.BkColor.A = 1809061122.0f;

  const ::std::vector<FLOAT> Color =
  {
    Data.BkColor.R,
    Data.BkColor.G,
    Data.BkColor.B,
    Data.BkColor.A
  };

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

  Tested_t Example{ Data };
  IRender_t & IExample = Example;

  EXPECT_CALL(DeviceContext, ClearRenderTargetView(&RenderTargetView, Color))
    .Times(1);

  IExample.ClearWindow();
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Present)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DXGI::SwapChain SwapChain;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateSwapChain())
    .Times(1)
    .WillOnce(Return(&SwapChain));

  Tested_t Example{ Tested_t::Data{} };
  IRender_t & IExample = Example;

  EXPECT_CALL(SwapChain, Present(0, 0))
    .Times(1);

  IExample.Present();
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_ResizeWindow)
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

  ::std::vector<D3D11_VIEWPORT> Viewports;

  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)Width;
  vp.Height = (FLOAT)Height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;

  Viewports.push_back(vp);

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
    Tested_t Example{ Tested_t::Data{} };
    IRender_t & IExample = Example;

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

      EXPECT_CALL(DeviceContext, OMSetRenderTargets(1, &RenderTargetView, nullptr))
        .Times(1);

      EXPECT_CALL(DeviceContext, RSSetViewports(Viewports))
        .Times(1);

      EXPECT_CALL(Texture2D, Release())
        .Times(1);

      IExample.ResizeWindow(Width, Height);
    }

    EXPECT_CALL(RenderTargetView, Release())
      .Times(1);
  }
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

  Tested_t Example{ Tested_t::Data{} };
  IRender_t & IExample = Example;

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

  Tested_t Example{ Tested_t::Data{} };
  IRender_t & IExample = Example;

  EXPECT_CALL(SwapChain, GetResult())
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

  Tested_t Example{ Tested_t::Data{} };
  IRender_t & IExample = Example;

  InSequence Dummy;

  EXPECT_CALL(SwapChain, GetBuffer(_, _))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, CreateRenderTargetView(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  EXPECT_THROW(IExample.ResizeWindow(0, 0), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_GetUsingApi)
{
  const Tested_t Example{ Tested_t::Data{} };
  const IGraphicApi_t & IExample = Example;

  const auto Result = IExample.GetUsingApi();
  EXPECT_EQ(uT("DirectX 11"), Result);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateTexture)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Texture2D Texture2D;
  ::mock::DirectX11::ShaderResourceView ShaderResourceView;

  const ::std::vector<uint8_t> BinaryData =
  {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
  };

  Tested_t::ITexture::Data Data = { 0 };
  Data.Width = 1809062028;
  Data.Height = 1809062029;
  Data.pData = BinaryData.data();

  D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
  TextureDesc.Width = Data.Width;
  TextureDesc.Height = Data.Height;
  TextureDesc.MipLevels = 1;
  TextureDesc.ArraySize = 1;
  TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  TextureDesc.Usage = D3D11_USAGE_DEFAULT;
  TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  TextureDesc.MiscFlags = 0;
  TextureDesc.SampleDesc.Count = 1;
  TextureDesc.SampleDesc.Quality = 0;

  D3D11_SUBRESOURCE_DATA Init = { 0 };
  Init.pSysMem = BinaryData.data();
  Init.SysMemPitch = Data.Width * 4;

  D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
  SrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  SrvDesc.Texture2D.MipLevels = 1;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(Device, CreateTexture2D(TextureDesc, Init))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, CreateShaderResourceView(&Texture2D, SrvDesc))
    .Times(1)
    .WillOnce(Return(&ShaderResourceView));

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  EXPECT_CALL(Device, Release())
    .Times(1);

  auto * pResult = IExample.Create(Data);

  EXPECT_CALL(DeviceContext, PSSetShaderResources(0, 1, &ShaderResourceView))
    .Times(1);

  pResult->Render();

  EXPECT_CALL(ShaderResourceView, Release())
    .Times(1);

  delete pResult;
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateTexture_CreateTexture2D_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Texture2D Texture2D;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(Device, CreateTexture2D(_, _))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, GetResult())
    .Times(AtLeast(1))
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_THROW(IExample.Create(Tested_t::ITexture::Data{}), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateTexture_CreateShaderResourceView_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Texture2D Texture2D;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(Device, CreateTexture2D(_, _))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, GetResult())
    .Times(1);

  EXPECT_CALL(Device, CreateShaderResourceView(_, _))
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_THROW(IExample.Create(Tested_t::ITexture::Data{}), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_DestroyTexture)
{
  class Texture :
    public ::covellite::api::renderer::DirectX11::ITexture
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD0(Destructor, void(void));
    };

  public:
    void Render(void) override {}

  public:
    ~Texture(void)
    {
      Proxy::GetInstance()->Destructor();
    }
  };

  Texture::Proxy Proxy;
  Texture::Proxy::GetInstance() = &Proxy;

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  Texture * pTexture = new Texture;

  using namespace ::testing;

  EXPECT_CALL(Proxy, Destructor())
    .Times(1);

  IExample.Destroy(pTexture);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateGeometry_CreateVertexBuffer)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer Buffer;

  ::std::vector<Tested_t::Vertex> VertexData = 
  {
    { 1.0f, 2.0f, 1809081155, 3.0f, 4.0f },
    { 5.0f, 6.0f, 1809081156, 7.0f, 8.0f },
  };

  Tested_t::IGeometry::Data Data = { 0 };
  Data.pVertices = VertexData.data();
  Data.VerticesCount = (int)VertexData.size();

  D3D11_SUBRESOURCE_DATA InitData = { 0 };
  InitData.pSysMem = VertexData.data();

  D3D11_BUFFER_DESC Desc = { 0 };
  Desc.Usage = D3D11_USAGE_DEFAULT;
  Desc.ByteWidth = sizeof(Tested_t::Vertex) * (int)VertexData.size();
  Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  InSequence Dummy;

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(Device, CreateBuffer(Desc, InitData))
    .Times(1)
    .WillOnce(Return(&Buffer));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(1);

  auto * pGeometry = IExample.Create(Data);

  EXPECT_CALL(Buffer, Release())
    .Times(1);

  delete pGeometry;
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateGeometry_CreateVertexBuffer_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  InSequence Dummy;

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_THROW(IExample.Create(Tested_t::IGeometry::Data{}), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateGeometry_CreateIndexBuffer)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer Buffer;

  ::std::vector<int> IndexData = { 1, 2, 3, 4, 5, 6 };

  Tested_t::IGeometry::Data Data = { 0 };
  Data.pIndices = IndexData.data();
  Data.IndicesCount = (int)IndexData.size();

  D3D11_SUBRESOURCE_DATA InitData = { 0 };
  InitData.pSysMem = IndexData.data();

  D3D11_BUFFER_DESC Desc = { 0 };
  Desc.Usage = D3D11_USAGE_DEFAULT;
  Desc.ByteWidth = sizeof(int) * (int)IndexData.size();
  Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  InSequence Dummy;

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(1);

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(Device, CreateBuffer(Desc, InitData))
    .Times(1)
    .WillOnce(Return(&Buffer));

  EXPECT_CALL(Device, Release())
    .Times(1);

  auto * pGeometry = IExample.Create(Data);

  EXPECT_CALL(Buffer, Release())
    .Times(1);

  delete pGeometry;
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateGeometry_CreateIndexBuffer_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  InSequence Dummy;

  // Vertex buffer

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(1);

  // Index buffer

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_THROW(IExample.Create(Tested_t::IGeometry::Data{}), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateGeometry_Update_WithTexture)
{
  class Texture :
    public Tested_t::ITexture
  {
  public:
    MOCK_METHOD0(Render, void(void));
  };

  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;
  Texture Texture;

  const FLOAT X = 1809081821.0f;
  const FLOAT Y = 1809081822.0f;
  const auto Width = 1809081920;
  const auto Height = 1809081921;

  D3D11_VIEWPORT Viewport = { 0 };
  Viewport.Width = (FLOAT)Width;
  Viewport.Height = (FLOAT)Height;

  ::covellite::api::renderer::ConstantBuffer ConstantBuffer =
  {
    ::DirectX::XMMatrixTranspose(::DirectX::XMMatrixTranslation(X, Y, 0)),
    ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixOrthographicOffCenterLH(0, (float)Width, (float)Height,
        0, -1, 1)),
    ::DirectX::XMFLOAT2{ 1.0f, 1.0f },
  };

  Tested_t::IGeometry::Data Data = { 0 };
  Data.pTexture = &Texture;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  auto * pGeometry = IExample.Create(Data);

  InSequence Dummy;

  EXPECT_CALL(DeviceContext, RSGetViewports(1))
    .Times(1)
    .WillOnce(Return(Viewport));

  pGeometry->Update(X, Y);

  EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, ConstantBuffer, 0, 0))
    .Times(1);

  pGeometry->Render();

  delete pGeometry;
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateGeometry_Update_WithoutTexture)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;

  const FLOAT X = 1809081922.0f;
  const FLOAT Y = 1809081923.0f;
  const auto Width = 1809081924;
  const auto Height = 1809081925;

  D3D11_VIEWPORT Viewport = { 0 };
  Viewport.Width = (FLOAT)Width;
  Viewport.Height = (FLOAT)Height;

  ::covellite::api::renderer::ConstantBuffer ConstantBuffer =
  {
    ::DirectX::XMMatrixTranspose(::DirectX::XMMatrixTranslation(X, Y, 0)),
    ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixOrthographicOffCenterLH(0, (float)Width, (float)Height,
        0, -1, 1)),
    ::DirectX::XMFLOAT2{ 0.0f, 0.0f },
  };

  Tested_t::IGeometry::Data Data = { 0 };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  auto * pGeometry = IExample.Create(Data);

  InSequence Dummy;

  EXPECT_CALL(DeviceContext, RSGetViewports(1))
    .Times(1)
    .WillOnce(Return(Viewport));

  pGeometry->Update(X, Y);

  EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, ConstantBuffer, 0, 0))
    .Times(1);

  pGeometry->Render();

  delete pGeometry;
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateGeometry_Render_WithTexture)
{
  class Texture :
    public Tested_t::ITexture
  {
  public:
    MOCK_METHOD0(Render, void(void));
  };

  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer ConstantBuffer;
  ::mock::DirectX11::Buffer VertexBuffer;
  ::mock::DirectX11::Buffer IndexBuffer;
  const auto IndexCount = 9081950 * 3;
  Texture Texture;

  Tested_t::IGeometry::Data Data = { 0 };
  Data.IndicesCount = IndexCount;
  Data.pTexture = &Texture;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::covellite::api::renderer::ConstantBuffer);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  D3D11_SUBRESOURCE_DATA InitData = { 0 };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(ConstantBufferDesc, InitData))
    .Times(1)
    .WillOnce(Return(&ConstantBuffer));

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(2)
    .WillRepeatedly(Return(&Device));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(2)
    .WillOnce(Return(&VertexBuffer))
    .WillOnce(Return(&IndexBuffer));

  auto * pGeometry = IExample.Create(Data);

  InSequence Dummy;

  EXPECT_CALL(DeviceContext, IASetVertexBuffers(0, 1, &VertexBuffer,
    sizeof(Tested_t::Vertex), 0))
    .Times(1);

  EXPECT_CALL(DeviceContext, IASetIndexBuffer(&IndexBuffer, 
    DXGI_FORMAT_R32_UINT, 0))
    .Times(1);

  EXPECT_CALL(DeviceContext, UpdateSubresource(&ConstantBuffer, 0, nullptr, 
    _, _, _))
    .Times(1);

  EXPECT_CALL(DeviceContext, VSSetConstantBuffers(0, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(Texture, Render())
    .Times(1);

  EXPECT_CALL(DeviceContext, IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
    .Times(1);

  EXPECT_CALL(DeviceContext, DrawIndexed(IndexCount, 0, 0))
    .Times(1);

  pGeometry->Render();

  delete pGeometry;
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateGeometry_Render_WithoutTexture)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer ConstantBuffer;
  ::mock::DirectX11::Buffer VertexBuffer;
  ::mock::DirectX11::Buffer IndexBuffer;
  const auto IndexCount = 9081950 * 3;

  Tested_t::IGeometry::Data Data = { 0 };
  Data.IndicesCount = IndexCount;
  Data.pTexture = nullptr;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::covellite::api::renderer::ConstantBuffer);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  D3D11_SUBRESOURCE_DATA InitData = { 0 };

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateBuffer(ConstantBufferDesc, InitData))
    .Times(1)
    .WillOnce(Return(&ConstantBuffer));

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  EXPECT_CALL(DeviceContext, GetDevice())
    .Times(2)
    .WillRepeatedly(Return(&Device));

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(2)
    .WillOnce(Return(&VertexBuffer))
    .WillOnce(Return(&IndexBuffer));

  auto * pGeometry = IExample.Create(Data);

  InSequence Dummy;

  EXPECT_CALL(DeviceContext, IASetVertexBuffers(0, 1, &VertexBuffer,
    sizeof(Tested_t::Vertex), 0))
    .Times(1);

  EXPECT_CALL(DeviceContext, IASetIndexBuffer(&IndexBuffer,
    DXGI_FORMAT_R32_UINT, 0))
    .Times(1);

  EXPECT_CALL(DeviceContext, UpdateSubresource(&ConstantBuffer, 0, nullptr,
    _, _, _))
    .Times(1);

  EXPECT_CALL(DeviceContext, VSSetConstantBuffers(0, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext, IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
    .Times(1);

  EXPECT_CALL(DeviceContext, DrawIndexed(IndexCount, 0, 0))
    .Times(1);

  pGeometry->Render();

  delete pGeometry;
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_DestroyGeometry)
{
  class Geometry :
    public ::covellite::api::renderer::DirectX11::IGeometry
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD0(Destructor, void(void));
    };

  public:
    void Update(float, float) override {}
    void Render(void) override {}

  public:
    ~Geometry(void)
    {
      Proxy::GetInstance()->Destructor();
    }
  };

  Geometry::Proxy Proxy;
  Geometry::Proxy::GetInstance() = &Proxy;

  Tested_t Example{ Tested_t::Data{} };
  IGraphicApi_t & IExample = Example;

  Geometry * pGeometry = new Geometry;

  using namespace ::testing;

  EXPECT_CALL(Proxy, Destructor())
    .Times(1);

  IExample.Destroy(pGeometry);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_EnableScissorRegion)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RasterizerState Enable;

  const auto X = 9061257;
  const auto Y = 9061258;
  const auto Width = 1809061259;
  const auto Height = 1809061300;

  D3D11_RASTERIZER_DESC Desc = { 0 };
  Desc.FillMode = D3D11_FILL_SOLID;
  Desc.CullMode = D3D11_CULL_NONE;
  Desc.FrontCounterClockwise = TRUE;
  Desc.ScissorEnable = TRUE;

  const ::std::vector<D3D11_RECT> Rects =
  {
    { X, Y, X + Width, Y + Height },
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateRasterizerState(Desc))
    .Times(1)
    .WillOnce(Return(&Enable));

  EXPECT_CALL(Device, CreateRasterizerState(_))
    .Times(1);

  {
    Tested_t Example{ Tested_t::Data{} };
    IGraphicApi_t & IExample = Example;

    EXPECT_CALL(DeviceContext, RSSetScissorRects(Rects))
      .Times(1);

    EXPECT_CALL(DeviceContext, RSSetState(&Enable))
      .Times(1);

    IExample.EnableScissorRegion(X, Y, Width, Height);

    EXPECT_CALL(Enable, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Device_CreateRasterizerState_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  using namespace ::testing;

  {
    InSequence Dummy;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(Device, GetResult())
      .Times(AtLeast(1));

    EXPECT_CALL(Device, CreateRasterizerState(_))
      .Times(1);

    EXPECT_CALL(Device, GetResult())
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
  }

  {
    InSequence Dummy;

    EXPECT_CALL(DirectXProxy, CreateDevice())
      .Times(1)
      .WillOnce(Return(&Device));

    EXPECT_CALL(Device, GetResult())
      .Times(AtLeast(1));

    EXPECT_CALL(Device, CreateRasterizerState(_))
      .Times(1);

    EXPECT_CALL(Device, GetResult())
      .Times(1)
      .WillOnce(Return(S_OK));

    EXPECT_CALL(Device, CreateRasterizerState(_))
      .Times(1);

    EXPECT_CALL(Device, GetResult())
      .Times(1)
      .WillOnce(Return(E_FAIL));

    EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_DisableScissorRegion)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RasterizerState Disabled;

  D3D11_RASTERIZER_DESC Desc = { 0 };
  Desc.FillMode = D3D11_FILL_SOLID;
  Desc.CullMode = D3D11_CULL_NONE;
  Desc.FrontCounterClockwise = TRUE;
  Desc.ScissorEnable = FALSE;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  EXPECT_CALL(Device, CreateRasterizerState(_))
    .Times(1);

  EXPECT_CALL(Device, CreateRasterizerState(Desc))
    .Times(1)
    .WillOnce(Return(&Disabled));

  {
    Tested_t Example{ Tested_t::Data{} };
    IGraphicApi_t & IExample = Example;

    EXPECT_CALL(DeviceContext, RSSetState(&Disabled))
      .Times(1);

    IExample.DisableScissorRegion();

    EXPECT_CALL(Disabled, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CompileVS_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Blob ErrorMessage;
  char * Error = "Error1809081119";

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

  EXPECT_STDEXCEPTION(Tested_t{ Tested_t::Data{} },
    (".+directx11\\.cpp\\([0-9]+\\): Failed: -2147467259 \\[" + 
    ::std::string{ Error } + "\\]\\.").c_str());
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Render_VertexShader)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Blob CompiledEffect;
  ::mock::DirectX11::VertexShader VertexShader;

  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  ::mock::DirectX11::CompileDesc Desc;
  Desc.SrcData = ::Render;
  Desc.SourceName = "";
  Desc.pDefines = nullptr;
  Desc.pInclude = nullptr;
  Desc.Entrypoint = "VS";
  Desc.Target = "vs_4_0";
  Desc.Flags1 = (IS_RELEASE_CONFIGURATION) ? 0 :
    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
  Desc.Flags2 = 0;

  void * pData = (void *)1809071758;
  const size_t DataSize = 1809071759;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

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

  EXPECT_CALL(Device, CreateVertexShader(pData, DataSize, nullptr))
    .Times(1)
    .WillOnce(Return(&VertexShader));

  EXPECT_CALL(CompiledEffect, Release())
    .Times(1);

  EXPECT_CALL(DirectXProxy, CompileGetCode())
    .Times(1);

  EXPECT_CALL(DirectXProxy, Compile(_))
    .Times(1);

  {
    Tested_t Example{ Tested_t::Data{} };
    IGraphicApi_t & IExample = Example;

    EXPECT_CALL(DeviceContext, IASetInputLayout(_))
      .Times(1);

    EXPECT_CALL(DeviceContext, VSSetShader(&VertexShader, nullptr, 0))
      .Times(1);

    IExample.Render();

    EXPECT_CALL(VertexShader, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateVertexShader_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::Blob CompiledEffect;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  InSequence Dummy;

  EXPECT_CALL(Device, GetResult())
    .Times(AtLeast(1));

  EXPECT_CALL(DirectXProxy, CompileGetCode())
    .Times(1)
    .WillOnce(Return(&CompiledEffect));

  EXPECT_CALL(Device, CreateVertexShader(_, _, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(CompiledEffect, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CompilePS_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Blob ErrorMessage;
  char * Error = "Error1809081140";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CompileGetErrorMsg())
    .Times(1);

  EXPECT_CALL(DirectXProxy, Compile(_))
    .Times(1);

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

  EXPECT_STDEXCEPTION(Tested_t{ Tested_t::Data{} },
    (".+directx11\\.cpp\\([0-9]+\\): Failed: -2147467259 \\[" +
      ::std::string{ Error } +"\\]\\.").c_str());
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Render_VertexLayout)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Blob CompiledEffect;
  ::mock::DirectX11::InputLayout InputLayout;

  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  ::mock::DirectX11::CompileDesc Desc;
  Desc.SrcData = ::Render;
  Desc.SourceName = "";
  Desc.pDefines = nullptr;
  Desc.pInclude = nullptr;
  Desc.Entrypoint = "VS";
  Desc.Target = "vs_4_0";
  Desc.Flags1 = (IS_RELEASE_CONFIGURATION) ? 0 :
    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
  Desc.Flags2 = 0;

  const ::std::vector<D3D11_INPUT_ELEMENT_DESC> Layout =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32_UINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  void * pData = (void *)1809071329;
  const size_t DataSize = 1809071330;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

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

  EXPECT_CALL(Device, CreateInputLayout(Layout, pData, DataSize))
    .Times(1)
    .WillOnce(Return(&InputLayout));

  EXPECT_CALL(CompiledEffect, Release())
    .Times(1);

  EXPECT_CALL(DirectXProxy, CompileGetCode())
    .Times(1);

  EXPECT_CALL(DirectXProxy, Compile(_))
    .Times(1);

  {
    Tested_t Example{ Tested_t::Data{} };
    IGraphicApi_t & IExample = Example;

    EXPECT_CALL(DeviceContext, IASetInputLayout(&InputLayout))
      .Times(1);

    IExample.Render();

    EXPECT_CALL(InputLayout, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateInputLayout_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::Blob CompiledEffect;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  InSequence Dummy;

  EXPECT_CALL(Device, GetResult())
    .Times(AtLeast(1));

  EXPECT_CALL(DirectXProxy, CompileGetCode())
    .Times(1)
    .WillOnce(Return(&CompiledEffect));

  EXPECT_CALL(Device, GetResult())
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateInputLayout(_, _, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(CompiledEffect, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Render_PixelShader)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Blob CompiledEffect;
  ::mock::DirectX11::PixelShader PixelShader;

  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  ::mock::DirectX11::CompileDesc Desc;
  Desc.SrcData = ::Render;
  Desc.SourceName = "";
  Desc.pDefines = nullptr;
  Desc.pInclude = nullptr;
  Desc.Entrypoint = "PS";
  Desc.Target = "ps_4_0";
  Desc.Flags1 = (IS_RELEASE_CONFIGURATION) ? 0 :
    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
  Desc.Flags2 = 0;

  void * pData = (void *)1809071822;
  const size_t DataSize = 1809071823;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CompileGetCode())
    .Times(1);

  EXPECT_CALL(DirectXProxy, Compile(_))
    .Times(1);

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

  {
    Tested_t Example{ Tested_t::Data{} };
    IGraphicApi_t & IExample = Example;

    EXPECT_CALL(DeviceContext, VSSetShader(_, _, _))
      .Times(1);

    EXPECT_CALL(DeviceContext, PSSetShader(&PixelShader, nullptr, 0))
      .Times(1);

    IExample.Render();

    EXPECT_CALL(PixelShader, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreatePixelShader_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::Blob CompiledEffect;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  InSequence Dummy;

  EXPECT_CALL(Device, GetResult())
    .Times(AtLeast(1));

  EXPECT_CALL(DirectXProxy, CompileGetCode())
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(AtLeast(1));

  EXPECT_CALL(DirectXProxy, CompileGetCode())
    .Times(1)
    .WillOnce(Return(&CompiledEffect));

  EXPECT_CALL(Device, CreatePixelShader(_, _, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(CompiledEffect, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Render_SamplerState)
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

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  InSequence Dummy;

  EXPECT_CALL(Device, CreateSamplerState(SamplerDesc))
    .Times(1)
    .WillOnce(Return(&SamplerState));

  {
    Tested_t Example{ Tested_t::Data{} };
    IGraphicApi_t & IExample = Example;

    EXPECT_CALL(DeviceContext, PSSetShader(_, _, _))
      .Times(1);

    EXPECT_CALL(DeviceContext, PSSetSamplers(0, 1, &SamplerState))
      .Times(1);

    IExample.Render();

    EXPECT_CALL(SamplerState, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateSamplerState_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  InSequence Dummy;

  EXPECT_CALL(Device, GetResult())
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateSamplerState(_))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Render_BlendState)
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

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  InSequence Dummy;

  EXPECT_CALL(Device, CreateBlendState(BlendDesc))
    .Times(1)
    .WillOnce(Return(&BlendState));

  {
    Tested_t Example{ Tested_t::Data{} };
    IGraphicApi_t & IExample = Example;

    EXPECT_CALL(DeviceContext, PSSetSamplers(_, _, _))
      .Times(1);

    EXPECT_CALL(DeviceContext, OMSetBlendState(&BlendState, Factor, 0xFFFFFFFF))
      .Times(1);

    IExample.Render();

    EXPECT_CALL(BlendState, Release())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_CreateBlendState_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  InSequence Dummy;

  EXPECT_CALL(Device, GetResult())
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateBlendState(_))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(Device, Release())
    .Times(1);

  EXPECT_THROW(Tested_t{ Tested_t::Data{} }, ::std::exception);
}
