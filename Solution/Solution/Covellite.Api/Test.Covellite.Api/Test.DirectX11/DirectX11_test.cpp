
#include "stdafx.h"
#include <alicorn/std/vector.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса DirectX11.
*/

// Расположение класса DirectX11
#include "../../Covellite.Api/Renderer/DirectX11.hpp"

#include <Covellite/Api/Component.inl>
#include "../../Covellite.Api/Renderer/fx/Vertex.auto.hpp"
#include "../../Covellite.Api/Renderer/fx/Pixel.auto.hpp"
#include "../../Covellite.Api/Renderer/fx/VertexInput.auto.hpp"
#include "../../Covellite.Api/Renderer/fx/PixelInput.auto.hpp"

// Общий тестовый класс класса DirectX11
class DirectX11_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::DirectX11;
  using ITested_t = ::covellite::api::renderer::IGraphicApi;
  using Data_t = ::covellite::api::renderer::Renderer::Data;
  using Component_t = ::covellite::api::Component;

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
  }

private:
  ::mock::DirectX11::Device m_DefaultDevice;
  ::mock::DirectX11::DeviceContext m_DefaultDeviceContext;
  ::mock::DXGI::SwapChain m_DefaultSwapChain;
  DXGI_SWAP_CHAIN_DESC m_DefaultDesc = { 0 };
  ::mock::DirectX11::Buffer m_DefaultBuffer;
  D3D11_BUFFER_DESC m_DefaultBufferDesc;
  ::mock::DirectX11::Blob m_DefaultBlob;
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

  const Data_t Data = { 0 };

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

  Data_t Data = { 0 };
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

  Data_t Data = { 0 };
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
    Tested_t Example{ Data_t{} };

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
    Tested_t Example{ Data_t{} };

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

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
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

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
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

  EXPECT_THROW(Tested_t{ Data_t{} }, ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_GetUsingApi)
{
  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  const auto Result = IExample.GetUsingApi();
  EXPECT_EQ(uT("DirectX 11"), Result);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_ClearFrame)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::RenderTargetView RenderTargetView;

  Data_t Data = { 0 };
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
  ITested_t & IExample = Example;

  EXPECT_CALL(DeviceContext, ClearRenderTargetView(&RenderTargetView, Color))
    .Times(1);

  IExample.ClearFrame();
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

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  EXPECT_CALL(SwapChain, Present(0, 0))
    .Times(1);

  IExample.PresentFrame();
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
    Tested_t Example{ Data_t{} };
    ITested_t & IExample = Example;

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

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

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

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

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

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

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

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateBlendState(_))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
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

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateBlendState(BlendDesc))
    .Times(1)
    .WillOnce(Return(&BlendState));

  auto Render = itCreator->second(pComponent);

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

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateSamplerState(_))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
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

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateSamplerState(SamplerDesc))
    .Times(1)
    .WillOnce(Return(&SamplerState));

  auto Render = itCreator->second(pComponent);

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

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateRasterizerState(_))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
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
  Desc.CullMode = D3D11_CULL_NONE;
  Desc.FrontCounterClockwise = TRUE;
  Desc.ScissorEnable = TRUE;

  const auto X = 9061257;
  const auto Y = 9061258;
  const auto Width = 1809061259;
  const auto Height = 1809061300;

  const ::std::vector<D3D11_RECT> Rects =
  {
    { X, Y, X + Width, Y + Height },
  };

  const auto Type = uT("State");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Scissor") },
      { uT("is_enabled"), uT("true") },
      { uT("left"), X },
      { uT("top"), Y },
      { uT("right"), X + Width },
      { uT("bottom"), Y + Height },
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateRasterizerState(Desc))
    .Times(1)
    .WillOnce(Return(&Enable));

  auto Render = itCreator->second(pComponent);

  EXPECT_CALL(DeviceContext, RSSetScissorRects(Rects))
    .Times(1);

  EXPECT_CALL(DeviceContext, RSSetState(&Enable))
    .Times(1);

  Render();

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
  Desc.CullMode = D3D11_CULL_NONE;
  Desc.FrontCounterClockwise = TRUE;
  Desc.ScissorEnable = FALSE;

  const auto Type = uT("State");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Scissor") },
      { uT("is_enabled"), uT("false") },
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateRasterizerState(Desc))
    .Times(1)
    .WillOnce(Return(&Disable));

  auto Render = itCreator->second(pComponent);

  EXPECT_CALL(DeviceContext, RSSetState(&Disable))
    .Times(1);

  Render();

  EXPECT_CALL(Disable, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_CreateVertex_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  const auto Type = uT("Buffer");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Vertex") },
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_Vertex)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer Buffer;

  const ::std::vector<Tested_t::Vertex> VertexData =
  {
    { 1.0f, 2.0f, 1809081155, 3.0f, 4.0f },
    { 5.0f, 6.0f, 1809081156, 7.0f, 8.0f },
  };

  const auto Type = uT("Buffer");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Vertex") },
      { uT("data"), VertexData.data() },
      { uT("count"), VertexData.size() },
    });

  D3D11_SUBRESOURCE_DATA InitData = { 0 };
  InitData.pSysMem = VertexData.data();

  D3D11_BUFFER_DESC Desc = { 0 };
  Desc.Usage = D3D11_USAGE_DEFAULT;
  Desc.ByteWidth = sizeof(Tested_t::Vertex) * (int)VertexData.size();
  Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateBuffer(Desc, InitData))
    .Times(1)
    .WillOnce(Return(&Buffer));

  auto Render = itCreator->second(pComponent);

  EXPECT_CALL(DeviceContext, IASetVertexBuffers(0, 1, &Buffer,
    sizeof(Tested_t::Vertex), 0))
    .Times(1);

  Render();

  EXPECT_CALL(Buffer, Release())
    .Times(1);
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

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Buffer_Index)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer Buffer;

  const ::std::vector<int> IndexData = { 1, 2, 3, 4, 5, 6 };

  const auto Type = uT("Buffer");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Index") },
      { uT("data"), IndexData.data() },
      { uT("count"), IndexData.size() },
    });

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

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateBuffer(Desc, InitData))
    .Times(1)
    .WillOnce(Return(&Buffer));

  auto Render = itCreator->second(pComponent);

  EXPECT_CALL(DeviceContext, IASetIndexBuffer(&Buffer, DXGI_FORMAT_R32_UINT, 0))
    .Times(1);

  Render();

  EXPECT_CALL(Buffer, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_DrawCall_CreateBuffer_Fail)
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

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("DrawCall"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateBuffer(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_DrawCall)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Buffer ConstantBuffer;
  ::mock::DirectX11::Buffer IndexBuffer;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  D3D11_BUFFER_DESC ConstantBufferDesc = { 0 };
  ConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  ConstantBufferDesc.ByteWidth = sizeof(::covellite::api::renderer::ConstantBuffer);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  D3D11_BUFFER_DESC IndexBufferDesc = { 0 };
  IndexBufferDesc.ByteWidth = 1811221344;

  auto itCreator = IExample.GetCreators().find(uT("DrawCall"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateBuffer(ConstantBufferDesc, _))
    .Times(1)
    .WillOnce(Return(&ConstantBuffer));

  auto Render = itCreator->second(Component_t::Make({}));

  EXPECT_CALL(DeviceContext, UpdateSubresource(&ConstantBuffer, 0, nullptr,
    _, 0, 0))
    .Times(1);

  EXPECT_CALL(DeviceContext, VSSetConstantBuffers(0, 1, &ConstantBuffer))
    .Times(1);

  EXPECT_CALL(DeviceContext, IAGetIndexBuffer(DXGI_FORMAT_UNKNOWN, 0))
    .Times(1)
    .WillOnce(Return(&IndexBuffer));

  EXPECT_CALL(IndexBuffer, GetDesc())
    .Times(1)
    .WillOnce(Return(IndexBufferDesc));

  EXPECT_CALL(DeviceContext, IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST))
    .Times(1);

  EXPECT_CALL(DeviceContext,
    DrawIndexed(IndexBufferDesc.ByteWidth / sizeof(int), 0, 0))
    .Times(1);

  EXPECT_CALL(IndexBuffer, Release())
    .Times(1);

  Render();

  EXPECT_CALL(ConstantBuffer, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Camera)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;

  const auto Width = 1811211754;
  const auto Height = 1811211755;

  ::covellite::api::renderer::ConstantBuffer Constants;
  memset(&Constants, 0, sizeof(Constants));

  Constants.Projection = ::DirectX::XMMatrixTranspose(
    ::DirectX::XMMatrixOrthographicOffCenterLH(0,
    (FLOAT)Width, (FLOAT)Height, 0, -1, 1));

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  // Camera
  {
    D3D11_VIEWPORT Viewport = { 0 };
    Viewport.Width = (FLOAT)Width;
    Viewport.Height = (FLOAT)Height;

    auto itCreator = IExample.GetCreators().find(uT("Camera"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    auto Render = itCreator->second(Component_t::Make({}));

    EXPECT_CALL(DeviceContext, RSGetViewports(1))
      .Times(1)
      .WillOnce(Return(Viewport));

    Render();
  }

  // DrawCall
  {
    auto itCreator = IExample.GetCreators().find(uT("DrawCall"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    auto Render = itCreator->second(Component_t::Make({}));

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, Constants, _, _))
      .Times(1);

    Render();
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Position)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::DeviceContext DeviceContext;

  ::covellite::api::renderer::ConstantBuffer Constants;
  memset(&Constants, 0, sizeof(Constants));

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  // Position

  auto itPositionCreator = IExample.GetCreators().find(uT("Position"));
  ASSERT_NE(IExample.GetCreators().end(), itPositionCreator);

  const auto pPosition = Component_t::Make({});

  auto PositionRender = itPositionCreator->second(pPosition);

  // DrawCall

  auto itCreator = IExample.GetCreators().find(uT("DrawCall"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make({}));

  // Первый вызов
  {
    const auto X = 1811221354.0f;
    const auto Y = 1811221355.0f;
    const auto Z = 1811221356.0f;

    Constants.World = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixTranslation(X, Y, Z));

    pPosition->SetValue(uT("x"), X);
    pPosition->SetValue(uT("y"), Y);
    pPosition->SetValue(uT("z"), Z);

    PositionRender();

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, Constants, _, _))
      .Times(1);

    Render();
  }

  // Второй вызов
  {
    const auto X = 1811221405.0f;
    const auto Y = 1811221406.0f;
    const auto Z = 1811221407.0f;

    Constants.World = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixTranslation(X, Y, Z));

    InSequence Dummy;

    pPosition->SetValue(uT("x"), X);
    pPosition->SetValue(uT("y"), Y);
    pPosition->SetValue(uT("z"), Z);

    PositionRender();

    EXPECT_CALL(DeviceContext, UpdateSubresource(_, _, _, Constants, _, _))
      .Times(1);

    Render();
  }
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture_CreateTexture2D_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;

  const auto Type = uT("Texture");
  const auto pComponent = Component_t::Make(
    {
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateTexture2D(_, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
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

  const auto Type = uT("Texture");
  const auto pComponent = Component_t::Make(
    {
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

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

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Texture)
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

  const int Width = 11212026;
  const int Height = 1811212027;

  const auto Type = uT("Texture");
  const auto pComponent = Component_t::Make(
    {
      { uT("width"), Width },
      { uT("height"), Height },
      { uT("data"), BinaryData.data() },
      { uT("count"), BinaryData.size() },
    });

  D3D11_TEXTURE2D_DESC TextureDesc = { 0 };
  TextureDesc.Width = Width;
  TextureDesc.Height = Height;
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
  Init.SysMemPitch = (UINT)Width * 4;

  D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc = { 0 };
  SrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  SrvDesc.Texture2D.MipLevels = 1;

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  EXPECT_CALL(DirectXProxy, CreateDeviceContext())
    .Times(1)
    .WillOnce(Return(&DeviceContext));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(Device, CreateTexture2D(TextureDesc, Init))
    .Times(1)
    .WillOnce(Return(&Texture2D));

  EXPECT_CALL(Device, CreateShaderResourceView(&Texture2D, SrvDesc))
    .Times(1)
    .WillOnce(Return(&ShaderResourceView));

  EXPECT_CALL(Texture2D, Release())
    .Times(1);

  auto Render = itCreator->second(pComponent);

  EXPECT_CALL(DeviceContext, PSSetShaderResources(0, 1, &ShaderResourceView))
    .Times(1);

  Render();

  EXPECT_CALL(ShaderResourceView, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_Compile_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Blob ErrorMessage;
  char * Error = "Error1809081119";

  const auto Type = uT("Shader");
  const auto pComponent = Component_t::Make(
    {
      { uT("data"), (const uint8_t *)nullptr },
      { uT("count"), (size_t)0 }
    });

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
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

  EXPECT_STDEXCEPTION(itCreator->second(pComponent),
    (".+directx11\\.cpp\\([0-9]+\\): Failed: -2147467259 \\[" +
      ::std::string{ Error } +"\\]\\.").c_str());
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_CreateInputLayout_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::Blob CompiledEffect;

  const auto Type = uT("Shader");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Vertex") },
      { uT("data"), (const uint8_t *)nullptr },
      { uT("count"), (size_t)0 }
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CompileGetCode())
    .Times(1)
    .WillOnce(Return(&CompiledEffect));

  EXPECT_CALL(Device, CreateInputLayout(_, _, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(CompiledEffect, Release())
    .Times(1);

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_CreateVertexShader_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::Blob CompiledEffect;

  const auto Type = uT("Shader");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Vertex") },
      { uT("data"), (const uint8_t *)nullptr },
      { uT("count"), (size_t)0 }
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

  EXPECT_CALL(DirectXProxy, CompileGetCode())
    .Times(1)
    .WillOnce(Return(&CompiledEffect));

  EXPECT_CALL(Device, GetResult())
    .Times(AtLeast(1));

  EXPECT_CALL(Device, CreateVertexShader(_, _, _))
    .Times(1);

  EXPECT_CALL(Device, GetResult())
    .Times(1)
    .WillOnce(Return(E_FAIL));

  EXPECT_CALL(CompiledEffect, Release())
    .Times(1);

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_CreatePixelShader_Fail)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::Blob CompiledEffect;

  const auto Type = uT("Shader");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Pixel") },
      { uT("data"), (const uint8_t *)nullptr },
      { uT("count"), (size_t)0 }
    });

  using namespace ::testing;

  EXPECT_CALL(DirectXProxy, CreateDevice())
    .Times(1)
    .WillOnce(Return(&Device));

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  InSequence Dummy;

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

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_Vertex)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Blob CompiledEffect;
  ::mock::DirectX11::InputLayout InputLayout;
  ::mock::DirectX11::VertexShader VertexShader;

  const ::std::vector<D3D11_INPUT_ELEMENT_DESC> Layout =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32_UINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  const ::std::vector<uint8_t> Data = { 0x18, 0x11, 0x22, 0x12, 0x28 };

  const auto Type = uT("Shader");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Vertex") },
      { uT("data"), Data.data() },
      { uT("count"), Data.size() },
      { uT("version"), uT("Version1811221242") },
      { uT("entry"), uT("Entry1811221243") },
   });

  using namespace ::alicorn::extension::std;
  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  ::mock::DirectX11::CompileDesc Desc;
  Desc.SrcData = VertexInput + PixelInput + Data;
  Desc.SourceName = "[Covellite::Api]";
  Desc.pDefines = nullptr;
  Desc.pInclude = nullptr;
  Desc.Entrypoint = "Entry1811221243";
  Desc.Target = "Version1811221242";
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

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
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

    EXPECT_CALL(Device, CreateInputLayout(Layout, pData, DataSize))
      .Times(1)
      .WillOnce(Return(&InputLayout));

    EXPECT_CALL(Device, CreateVertexShader(pData, DataSize, nullptr))
      .Times(1)
      .WillOnce(Return(&VertexShader));

    EXPECT_CALL(CompiledEffect, Release())
      .Times(1);
  }

  auto Render = itCreator->second(pComponent);

  EXPECT_CALL(DeviceContext, IASetInputLayout(&InputLayout))
    .Times(1);

  EXPECT_CALL(DeviceContext, VSSetShader(&VertexShader, nullptr, 0))
    .Times(1);

  Render();

  EXPECT_CALL(InputLayout, Release())
    .Times(1);

  EXPECT_CALL(VertexShader, Release())
    .Times(1);
}

// ************************************************************************** //
TEST_F(DirectX11_test, /*DISABLED_*/Test_Shader_Pixel)
{
  using DirectXProxy_t = ::mock::DirectX11::Proxy;
  DirectXProxy_t DirectXProxy;
  DirectXProxy_t::GetInstance() = &DirectXProxy;

  ::mock::DirectX11::Device Device;
  ::mock::DirectX11::DeviceContext DeviceContext;
  ::mock::DirectX11::Blob CompiledEffect;
  ::mock::DirectX11::PixelShader PixelShader;

  const ::std::vector<uint8_t> Data = { 0x18, 0x11, 0x22, 0x12, 0x56 };

  const auto Type = uT("Shader");
  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Pixel") },
      { uT("data"), Data.data() },
      { uT("count"), Data.size() },
      { uT("version"), uT("Version1811221254") },
      { uT("entry"), uT("Entry1811221255") },
   });

  using namespace ::alicorn::extension::std;
  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  ::mock::DirectX11::CompileDesc Desc;
  Desc.SrcData = VertexInput + PixelInput + Data;
  Desc.SourceName = "[Covellite::Api]";
  Desc.pDefines = nullptr;
  Desc.pInclude = nullptr;
  Desc.Entrypoint = "Entry1811221255";
  Desc.Target = "Version1811221254";
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

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(Type);
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

  auto Render = itCreator->second(pComponent);

  EXPECT_CALL(DeviceContext, PSSetShader(&PixelShader, nullptr, 0))
    .Times(1);

  Render();

  EXPECT_CALL(PixelShader, Release())
    .Times(1);
}
