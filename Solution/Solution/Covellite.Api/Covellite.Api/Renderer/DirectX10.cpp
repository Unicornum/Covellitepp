
#include "stdafx.h"
#include "DirectX10.hpp"
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/lexical-cast.hpp>
#include <Covellite/Api/Vertex.hpp>
#include "DxCheck.hpp"
#include "Component.hpp"
#include "fx/Data.auto.hpp"
#include "fx/Input.auto.hpp"

#include <d3d10.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dcompiler.lib" )

using namespace covellite::api::renderer;

class DirectX10::Buffer
{
public:
  template<class T>
  class Support
  {
  public:
    static UINT GetFlag(void) { return D3D10_BIND_VERTEX_BUFFER; }
  };

  template<>
  class Support<::Matrices>
  {
  public:
    static UINT GetFlag(void) { return D3D10_BIND_CONSTANT_BUFFER; }
    static void SetConstantBuffer(const ComPtr_t<ID3D10Device> & _pDevice,
      const ComPtr_t<ID3D10Buffer> & _pBuffer)
    {
      _pDevice->VSSetConstantBuffers(
        MATRICES_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
    }
  };

  template<>
  class Support<::Lights>
  {
  public:
    static UINT GetFlag(void) { return D3D10_BIND_CONSTANT_BUFFER; }
    static void SetConstantBuffer(const ComPtr_t<ID3D10Device> & _pDevice,
      const ComPtr_t<ID3D10Buffer> & _pBuffer)
    {
      _pDevice->PSSetConstantBuffers(
        LIGHTS_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
    }
  };

  template<>
  class Support<::Material>
  {
  public:
    static UINT GetFlag(void) { return D3D10_BIND_CONSTANT_BUFFER; }
    static void SetConstantBuffer(const ComPtr_t<ID3D10Device> & _pDevice,
      const ComPtr_t<ID3D10Buffer> & _pBuffer)
    {
      _pDevice->PSSetConstantBuffers(
        MATERIAL_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
    }
  };

  template<>
  class Support<int>
  {
  public:
    static UINT GetFlag(void) { return D3D10_BIND_INDEX_BUFFER; }
  };

  template<class T>
  static Render_t GetRender(const ComPtr_t<ID3D10Device> & _pDevice,
    const ComPtr_t<ID3D10Buffer> & _pBuffer)
  {
    return [=](void)
    {
      const UINT stride = sizeof(T);
      const UINT offset = 0;
      _pDevice->IASetVertexBuffers(0, 1,
        _pBuffer.GetAddressOf(), &stride, &offset);
    };
  }

  template<>
  static Render_t GetRender<int>(const ComPtr_t<ID3D10Device> & _pDevice,
    const ComPtr_t<ID3D10Buffer> & _pBuffer)
  {
    return [=](void)
    {
      _pDevice->IASetIndexBuffer(_pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    };
  }

public:
  template<class T>
  static ComPtr_t<ID3D10Buffer> Create(ComPtr_t<ID3D10Device> _pDevice,
    const T * _pData, size_t _Count)
  {
    D3D10_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = static_cast<decltype(bd.ByteWidth)>(sizeof(T) * _Count);
    bd.BindFlags = Support<::std::remove_const<T>::type>::GetFlag();

    D3D10_SUBRESOURCE_DATA InitData = { 0 };
    InitData.pSysMem = _pData;

    ComPtr_t<ID3D10Buffer> pBuffer;
    DX_CHECK _pDevice->CreateBuffer(&bd, &InitData, &pBuffer);
    return pBuffer;
  }

  template<class T>
  static ComPtr_t<ID3D10Buffer> Create(ComPtr_t<ID3D10Device> _pDevice,
    const T * _pData)
  {
    auto pBuffer = Create(_pDevice, _pData, 1);
    Support<T>::SetConstantBuffer(_pDevice, pBuffer);
    return pBuffer;
  }

  template<class T>
  static Creator_t GetCreator(const ComPtr_t<ID3D10Device> & _pDevice)
  {
    return [&](const ComponentPtr_t & _pComponent) -> Render_t
    {
      const Component::Buffer<T> BufferData{ _pComponent };

      return GetRender<T>(_pDevice,
        Create(_pDevice, BufferData.pData, BufferData.Count));
    };
  }
};

template<class T>
DirectX10::ConstantBuffer<T>::ConstantBuffer(void)
{
  memset(&Data, 0x00, sizeof(Data));
}

template<class T>
void DirectX10::ConstantBuffer<T>::Update(
  const ComPtr_t<ID3D10Device> & _pDevice)
{
  if (m_pBuffer == nullptr)
  {
    m_pBuffer = Buffer::Create<T>(_pDevice, &Data);
  }

  _pDevice->UpdateSubresource(m_pBuffer.Get(), 0, NULL, &Data, 0, 0);

  // Здесь очищать Data нельзя, т.к. для буфера матриц эта функция вызывается
  // для каждого объекта, а матрицы вида/проекции устанавливаются при
  // рендеринге сцены один раз.
}

DirectX10::DirectX10(const Renderer::Data & _Data)
{
  m_BkColor[0] = _Data.BkColor.R;
  m_BkColor[1] = _Data.BkColor.G;
  m_BkColor[2] = _Data.BkColor.B;
  m_BkColor[3] = _Data.BkColor.A;

  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  const UINT createDeviceFlags = (IS_RELEASE_CONFIGURATION) ? 0 :
    D3D10_CREATE_DEVICE_DEBUG;

  DXGI_SWAP_CHAIN_DESC sd = { 0 };
  sd.BufferCount = 2;
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

  DX_CHECK D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, 
    NULL, createDeviceFlags, D3D10_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice);

  DXGI_SWAP_CHAIN_DESC Desc = { 0 };
  DX_CHECK m_pSwapChain->GetDesc(&Desc);

  SetViewport(Desc.BufferDesc.Width, Desc.BufferDesc.Height);

  auto ToPreRenderComponents = [&](const ComponentPtr_t & _pComponent) -> Render_t
  {
    m_PreRenderComponent.push_back(_pComponent);
    return nullptr;
  };

  m_Creators =
  {
    { uT("Data"), ToPreRenderComponents },
    { uT("State"), [&](const ComponentPtr_t & _pData) -> Render_t 
      { return CreateState(_pData); } },
    { uT("Light"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateLight(_pData); } },
    { uT("Material"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateMaterial(_pData); } },
    { uT("Texture"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateTexture(_pData); } },
    { uT("Shader"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateShader(_pData); } },
    { uT("Buffer"), [&](const ComponentPtr_t & _pData) -> Render_t 
      { return CreateBuffer(_pData); } },
    { uT("Present"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreatePresent(_pData); } },
  };
}

DirectX10::~DirectX10(void) = default;

DirectX10::String_t DirectX10::GetUsingApi(void) const /*override*/
{
  return uT("DirectX 10");
}

void DirectX10::ClearFrame(void) /*override*/
{
  m_pDevice->ClearRenderTargetView(m_pRenderTargetView.Get(), m_BkColor);
}

void DirectX10::PresentFrame(void) /*override*/
{
  m_pSwapChain->Present(0, 0);
}

void DirectX10::ResizeWindow(int32_t _Width, int32_t _Height) /*override*/
{
  SetViewport(_Width, _Height);
}

auto DirectX10::GetCreators(void) const -> const Creators_t & /*override*/
{
  return m_Creators;
}

void DirectX10::SetViewport(int _Width, int _Height)
{
  D3D10_VIEWPORT ViewPort = { 0 };
  ViewPort.TopLeftX = 0;
  ViewPort.TopLeftY = 0;
  ViewPort.Width = _Width;
  ViewPort.Height = _Height;
  ViewPort.MinDepth = 0.0f;
  ViewPort.MaxDepth = 1.0f;
  m_pDevice->RSSetViewports(1, &ViewPort);

  if (m_pRenderTargetView)
  {
    // Release the existing renderer target
    m_pRenderTargetView.Reset();

    // Resize the swap chain's buffer to the given dimensions
    m_pSwapChain->ResizeBuffers(2, _Width, _Height,
      DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
  }

  // Recreate Render Target
  ComPtr_t<ID3D10Texture2D> pBackBuffer;
  DX_CHECK m_pSwapChain->GetBuffer(0,
    __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);

  DX_CHECK m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL,
    &m_pRenderTargetView);

  m_pDepthStencilView.Reset();

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

  D3D10_DEPTH_STENCIL_DESC dsDesc = { 0 };

  // Depth test parameters
  dsDesc.DepthEnable = true;
  dsDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
  dsDesc.DepthFunc = D3D10_COMPARISON_LESS;

  // Stencil test parameters
  dsDesc.StencilEnable = false;

  // Create depth stencil state
  ComPtr_t<ID3D10DepthStencilState> pDSState;
  DX_CHECK m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState);

  m_pDevice->OMSetDepthStencilState(pDSState.Get(), 1);

  D3D10_DEPTH_STENCIL_VIEW_DESC descDSV = { 0 };
  descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;

  DX_CHECK m_pDevice->CreateDepthStencilView(pDepthBuffer.Get(),
    &descDSV, &m_pDepthStencilView);
}

auto DirectX10::CreateState(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto CreateSamplerState = [&](void) -> Render_t
  {
    D3D10_SAMPLER_DESC sampDesc = { 0 };
    sampDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
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

  auto CreateScissorState = [&](void) -> Render_t
  {
    const Component::Scissor ScissorData{ _pComponent };

    D3D10_RASTERIZER_DESC rasterDesc = { 0 };
    rasterDesc.FillMode = D3D10_FILL_SOLID;
    rasterDesc.CullMode = D3D10_CULL_BACK;
    rasterDesc.FrontCounterClockwise = TRUE;
    rasterDesc.ScissorEnable = (ScissorData.IsEnabled) ? TRUE : FALSE;

    ComPtr_t<ID3D10RasterizerState> pScissor;
    DX_CHECK m_pDevice->CreateRasterizerState(&rasterDesc, &pScissor);

    ComponentPtr_t pScissorRect = _pComponent;

    PreRenderComponentsProcess(
      {
        { uT("Rect"),
          [&](const ComponentPtr_t & _pComponent) { pScissorRect = _pComponent; } },
      });

    Render_t ScissorEnabled = [=](void)
    {
      const Component::Scissor ScissorData{ pScissorRect };

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

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Blend"), [&]() { return CreateBlendState(true); } },
    { uT("Sampler"), CreateSamplerState },
    { uT("Scissor"), CreateScissorState },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto DirectX10::CreateLight(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto * const pLights = &m_Lights;
  auto * const pCurrentCameraId = &m_CurrentCameraId;

  auto pPosition = ::covellite::api::Component::Make({ });
  auto pDirection = ::covellite::api::Component::Make({ { uT("x"), 1.0f } });
  auto pAttenuation = ::covellite::api::Component::Make({ });

  PreRenderComponentsProcess(
    {
      { uT("Position"),
        [&](const ComponentPtr_t & _pComponent) { pPosition = _pComponent; } },
      { uT("Direction"),
        [&](const ComponentPtr_t & _pComponent) { pDirection = _pComponent; } },
      { uT("Attenuation"),
        [&](const ComponentPtr_t & _pComponent) { pAttenuation = _pComponent; } },
    });

  auto CreateAmbient = [&](void) -> Render_t
  {
    return [=](void)
    {
      auto & Color = (*pLights)[*pCurrentCameraId].Data.Ambient.Color;

      Color.ARGBAmbient = _pComponent->GetValue(uT("ambient"), 0xFF000000);
      Color.ARGBDiffuse = _pComponent->GetValue(uT("diffuse"), 0xFF000000);
      Color.ARGBSpecular = _pComponent->GetValue(uT("specular"), 0xFF000000);
    };
  };

  auto CreateDirection = [&](void) -> Render_t
  {
    return [=](void)
    {
      const Component::Position Direction{ pDirection };

      auto & Light = (*pLights)[*pCurrentCameraId].Data.Direction;

      Light.Color.ARGBAmbient = _pComponent->GetValue(uT("ambient"), 0xFF000000);
      Light.Color.ARGBDiffuse = _pComponent->GetValue(uT("diffuse"), 0xFF000000);
      Light.Color.ARGBSpecular = _pComponent->GetValue(uT("specular"), 0xFF000000);
      Light.Direction = { Direction.X, Direction.Y, Direction.Z };
    };
  };

  auto CreatePoint = [&](void) -> Render_t
  {
    return [=](void)
    {
      const Component::Position Position{ pPosition };

      auto & Lights = (*pLights)[*pCurrentCameraId].Data.Points;
      auto & Light = Lights.Lights[Lights.UsedSlotCount++];

      Light.Color.ARGBAmbient = _pComponent->GetValue(uT("ambient"), 0xFF000000);
      Light.Color.ARGBDiffuse = _pComponent->GetValue(uT("diffuse"), 0xFF000000);
      Light.Color.ARGBSpecular = _pComponent->GetValue(uT("specular"), 0xFF000000);
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

auto DirectX10::CreateMaterial(const ComponentPtr_t & _pComponent) -> Render_t
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
    m_pDevice->PSSetConstantBuffers(MATERIAL_BUFFER_INDEX, 1,
      pBuffer.GetAddressOf());
    //m_pImmediateContext->UpdateSubresource(
    //  pBuffer.Get(), 0, NULL, &Material, 0, 0);
  };
}

auto DirectX10::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t
{
  ComponentPtr_t pData = _pComponent;

  PreRenderComponentsProcess(
  {
    { uT("Texture"),
      [&](const ComponentPtr_t & _pComponent) { pData = _pComponent; } },
  });

  const Component::Texture TextureData{ pData };

  D3D10_TEXTURE2D_DESC textureDesc = { 0 };
  textureDesc.Width = TextureData.Width;
  textureDesc.Height = TextureData.Height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.Usage = D3D10_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  textureDesc.MiscFlags = 0;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;

  D3D10_SUBRESOURCE_DATA Init = { 0 };
  Init.pSysMem = TextureData.pData;
  Init.SysMemPitch = TextureData.Width * 4;

  ComPtr_t<ID3D10Texture2D> pTexture;
  DX_CHECK m_pDevice->CreateTexture2D(&textureDesc, &Init, &pTexture);

  D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc = { 0 };
  srvDesc.Format = textureDesc.Format;
  srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
  srvDesc.Texture2D.MostDetailedMip = 0;

  ComPtr_t<ID3D10ShaderResourceView> pShaderResourceView;
  DX_CHECK m_pDevice->CreateShaderResourceView(pTexture.Get(),
    &srvDesc, &pShaderResourceView);

  return [=](void)
  {
    m_pDevice->PSSetShaderResources(0, 1, pShaderResourceView.GetAddressOf());
  };
}

class DirectX10::Shader
{
  using Creator_t = ::std::function<Render_t(void)>;

  template<class T>
  class Layout; // Not implement!

  template<>
  class Layout<api::Vertex::Gui>
  {
  public:
    static ::std::vector<D3D10_INPUT_ELEMENT_DESC> GetDesc(void)
    {
      return 
      {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32_UINT, 0, 8, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
      };
    }
  };

  template<>
  class Layout<api::Vertex::Textured>
  {
  public:
    static ::std::vector<D3D10_INPUT_ELEMENT_DESC> GetDesc(void)
    {
      return
      {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
      };
    }
  };

public:
  static ComPtr_t<ID3DBlob> Compile(const ::std::vector<uint8_t> & _Data,
    LPCSTR _pEntryPoint, LPCSTR _pTarget)
  {
    // 20 Ноябрь 2018 17:46 (unicornum.verum@gmail.com)
    TODO("Повтор кода из реализации DirectX11");

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
  static Creator_t GetCreator(const ComPtr_t<ID3D10Device> & _pDevice,
    const ComPtr_t<ID3DBlob> & _pCompiledShader)
  {
    return [&](void)
    {
      const auto LayoutDesc = Layout<T>::GetDesc();

      ComPtr_t<ID3D10InputLayout> pVertexLayout;
      DX_CHECK _pDevice->CreateInputLayout(
        LayoutDesc.data(), static_cast<UINT>(LayoutDesc.size()),
        _pCompiledShader->GetBufferPointer(), _pCompiledShader->GetBufferSize(),
        &pVertexLayout);

      ComPtr_t<ID3D10VertexShader> pVertexShader;
      DX_CHECK _pDevice->CreateVertexShader(
        _pCompiledShader->GetBufferPointer(), _pCompiledShader->GetBufferSize(),
        &pVertexShader);

      return [=](void)
      {
        _pDevice->IASetInputLayout(pVertexLayout.Get());
        _pDevice->VSSetShader(pVertexShader.Get());
      };
    };
  }
};

auto DirectX10::CreateShader(const ComponentPtr_t & _pComponent) -> Render_t
{
  ComponentPtr_t pData = _pComponent;

  PreRenderComponentsProcess(
    {
      { uT("Shader.HLSL"), 
        [&](const ComponentPtr_t & _pComponent) { pData = _pComponent; } },
    });

  const Component::Shader ShaderData{ pData };

  using namespace ::alicorn::extension::std;

  const auto CompleteShaderData = ::Data + ::Input +
    ::std::vector<uint8_t>{ ShaderData.pData, ShaderData.pData + ShaderData.Count };

  const auto pCompiledShader = Shader::Compile(CompleteShaderData,
    ShaderData.Entry.c_str(), ShaderData.Version.c_str());

  const auto PixelShader = [&](void)
  {
    ComPtr_t<ID3D10PixelShader> pPixelShader;
    DX_CHECK m_pDevice->CreatePixelShader(
      pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
      &pPixelShader);

    auto * const pCurrentLights = &m_CurrentLights;

    return [=](void)
    {
      m_pDevice->PSSetShader(pPixelShader.Get());

      pCurrentLights->Update(m_pDevice);
    };
  };

  using Vertex_t = ::covellite::api::Vertex;

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { 
      Vertex_t::Gui::GetName(),
      Shader::GetCreator<Vertex_t::Gui>(m_pDevice, pCompiledShader) 
    },
    { 
      Vertex_t::Textured::GetName(),
      Shader::GetCreator<Vertex_t::Textured>(m_pDevice, pCompiledShader) 
    },
    { 
      uT("Pixel"), 
      PixelShader
    },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto DirectX10::CreateBuffer(const ComponentPtr_t & _pComponent) -> Render_t
{
  using Vertex_t = ::covellite::api::Vertex;

  ComponentPtr_t pData = _pComponent;

  auto Prepare = 
    [&](const ComponentPtr_t & _pComponent) { pData = _pComponent; };

  PreRenderComponentsProcess(
    {
      { Vertex_t::Gui::GetName(), Prepare },
      { Vertex_t::Textured::GetName(), Prepare },
      { uT("Index"), Prepare },
    });

  Creators_t Creators =
  {
    { 
      Vertex_t::Gui::GetName(), 
      Buffer::GetCreator<Vertex_t::Gui>(m_pDevice) 
    },
    { 
      Vertex_t::Textured::GetName(), 
      Buffer::GetCreator<Vertex_t::Textured>(m_pDevice) 
    },
    { 
      uT("Index"), 
      Buffer::GetCreator<int>(m_pDevice) 
    },
  };

  return Creators[pData->GetValue(uT("kind"), uT("Unknown"))](pData);
}

auto DirectX10::CreatePresent(const ComponentPtr_t & _pComponent) -> Render_t
{
  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { 
      uT("Camera"),
      [&](void) -> Render_t { return CreateCamera(_pComponent); } 
    },
    { 
      uT("Geometry"),
      [&](void) -> Render_t { return CreateGeometry(_pComponent); } 
    },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto DirectX10::GetDeptRender(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto Dept = _pComponent->GetValue(uT("dept"), uT("Disabled"));

  Render_t RenderDeptEnabled = [=](void)
  {
    m_pDevice->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(),
      m_pDepthStencilView.Get());
    m_pDevice->ClearDepthStencilView(m_pDepthStencilView.Get(),
      D3D10_CLEAR_DEPTH, 1.0f, 0);
  };

  Render_t RenderDeptDisabled = [=](void)
  {
    m_pDevice->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(),
      nullptr);
  };

  return (Dept == uT("Enabled")) ? RenderDeptEnabled : RenderDeptDisabled;
}

auto DirectX10::CreateCamera(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto * const pWorldViewProjection = &m_WorldViewProjection;
  auto * const pLights = &m_Lights;
  auto * const pCurrentCameraId = &m_CurrentCameraId;
  auto * const pCurrentLights = &m_CurrentLights;
  const auto CameraId = _pComponent->Id;

  auto RenderDept = GetDeptRender(_pComponent);

  auto RenderLights = [=](void)
  {
    *pCurrentCameraId = CameraId;
    *pCurrentLights = (*pLights)[CameraId];

    (*pLights)[CameraId].Data.Points.UsedSlotCount = 0;
  };

  auto DisableBlendRender = CreateBlendState(false);

  Render_t CameraGui = [=](void)
  {
    RenderDept();
    RenderLights();
    DisableBlendRender();

    UINT ViewportCount = 1;
    D3D10_VIEWPORT Viewport = { 0 };
    m_pDevice->RSGetViewports(&ViewportCount, &Viewport);

    pWorldViewProjection->Data.Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixOrthographicOffCenterLH(0, (float)Viewport.Width,
      (float)Viewport.Height, 0, -1.0f, 1.0f));

    pWorldViewProjection->Data.View = ::DirectX::XMMatrixTranspose(
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

  Render_t CameraFocal = [=](void)
  {
    RenderDept();
    RenderLights();
    DisableBlendRender();

    UINT ViewportCount = 1;
    D3D10_VIEWPORT Viewport = { 0 };
    m_pDevice->RSGetViewports(&ViewportCount, &Viewport);

    const auto AngleY = (float)::alicorn::extension::cpp::math::GreedToRadian *
      _pComponent->GetValue(uT("angle"), 90.0f);
    const auto aspectRatio = (float)Viewport.Width / (float)Viewport.Height;

    pWorldViewProjection->Data.Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixPerspectiveFovLH(
        AngleY, aspectRatio, 0.01f, 100.0f));

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

    pWorldViewProjection->Data.View = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixLookAtLH(Eye, Look,
        ::DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)));
  };

  const auto Focal = _pComponent->GetValue(uT("focal"), uT("Disabled"));
  return (Focal == uT("Enabled")) ? CameraFocal : CameraGui;
}

auto DirectX10::CreateGeometry(const ComponentPtr_t &) -> Render_t
{
  auto PreRenders = GetPreRendersGeometry();

  return [=](void)
  {
    for (auto & Render : PreRenders) Render();

    ComPtr_t<ID3D10Buffer> pIndexBuffer;
    DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;
    UINT Offset = 0;
    m_pDevice->IAGetIndexBuffer(&pIndexBuffer, &Format, &Offset);

    D3D10_BUFFER_DESC Desc = { 0 };
    pIndexBuffer->GetDesc(&Desc);

    m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDevice->DrawIndexed((DWORD)(Desc.ByteWidth / sizeof(int)), 0, 0);
  };
}

auto DirectX10::CreateBlendState(bool _IsEnabled) -> Render_t
{
  ComPtr_t<ID3D10BlendState> pBlendState;

  if (_IsEnabled)
  {
    D3D10_BLEND_DESC BlendDesc = { 0 };
    BlendDesc.AlphaToCoverageEnable = FALSE;
    BlendDesc.BlendEnable[0] = TRUE;
    BlendDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
    BlendDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
    BlendDesc.BlendOp = D3D10_BLEND_OP_ADD;
    BlendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
    BlendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
    BlendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
    BlendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

    DX_CHECK m_pDevice->CreateBlendState(&BlendDesc, &pBlendState);
  }

  return [=](void)
  {
    m_pDevice->OMSetBlendState(pBlendState.Get(), nullptr, 0xFFFFFFFF);
  };
}

void DirectX10::PreRenderComponentsProcess(const PreRenders_t & _PreRenders)
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

auto DirectX10::GetPreRendersGeometry(void) -> Renders_t
{
  Renders_t Result;

  auto * const pWorldViewProjection = &m_WorldViewProjection;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    Result.push_back([=](void)
    {
      const Component::Position Position{ _pPosition };

      pWorldViewProjection->Data.World *=
        ::DirectX::XMMatrixTranslation(Position.X, Position.Y, Position.Z);
    });
  };

  auto CreateRotation = [&](const ComponentPtr_t & _pRotation)
  {
    Result.push_back([=](void)
    {
      const Component::Position Rotation{ _pRotation };

      pWorldViewProjection->Data.World *= 
        ::DirectX::XMMatrixRotationX(Rotation.X);
      pWorldViewProjection->Data.World *= 
        ::DirectX::XMMatrixRotationY(Rotation.Y);
      pWorldViewProjection->Data.World *= 
        ::DirectX::XMMatrixRotationZ(Rotation.Z);
    });
  };

  auto CreateScale = [&](const ComponentPtr_t & _pScale)
  {
    Result.push_back([=](void)
    {
      const Component::Position Scale{ _pScale };

      pWorldViewProjection->Data.World *=
        ::DirectX::XMMatrixScaling(Scale.X, Scale.Y, Scale.Z);
    });
  };

  Result.push_back([=](void)
  {
    pWorldViewProjection->Data.World = ::DirectX::XMMatrixIdentity();
  });

  PreRenderComponentsProcess(
  {
    { uT("Position"), CreatePosition },
    { uT("Rotation"), CreateRotation },
    { uT("Scale"), CreateScale },
  });

  Result.push_back([=](void)
  {
    pWorldViewProjection->Data.World =
      ::DirectX::XMMatrixTranspose(pWorldViewProjection->Data.World);
    pWorldViewProjection->Update(m_pDevice);
  });

  return Result;
}
