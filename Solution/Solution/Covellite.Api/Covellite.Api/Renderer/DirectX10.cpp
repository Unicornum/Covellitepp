
#include "stdafx.h"
#include "DirectX10.hpp"
#include <directxmath.h>
#include <alicorn/std/vector.hpp>

#pragma warning(push)
#pragma warning(disable: 4996)
#include <Covellite/Api/Vertex.hpp>
#pragma warning(pop)

#include <Covellite/Api/Defines.hpp>
#include "DxCheck.hpp"
#include "DirectX.hpp"
#include "Component.hpp"
#include "Shaders/Shaders.hpp"
#include "GraphicApi.Constants.hpp"

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

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
  class Support<::Camera>
  {
  public:
    static UINT GetFlag(void) { return D3D10_BIND_CONSTANT_BUFFER; }
    static void SetConstantBuffer(const ComPtr_t<ID3D10Device> & /*_pDevice*/,
      const ComPtr_t<ID3D10Buffer> & /*_pBuffer*/)
    {
      //_pDevice->VSSetConstantBuffers(
      //  MATRICES_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
      //_pDevice->PSSetConstantBuffers(
      //  MATRICES_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
    }
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
      _pDevice->PSSetConstantBuffers(
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
      _pDevice->VSSetConstantBuffers(
        LIGHTS_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
      _pDevice->PSSetConstantBuffers(
        LIGHTS_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
    }
  };

  template<>
  class Support<::Fog>
  {
  public:
    static UINT GetFlag(void) { return D3D10_BIND_CONSTANT_BUFFER; }
    static void SetConstantBuffer(const ComPtr_t<ID3D10Device> & _pDevice,
      const ComPtr_t<ID3D10Buffer> & _pBuffer)
    {
      _pDevice->VSSetConstantBuffers(
        FOG_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
      _pDevice->PSSetConstantBuffers(
        FOG_BUFFER_INDEX, 1, _pBuffer.GetAddressOf());
    }
  };

  template<>
  class Support<int>
  {
  public:
    static UINT GetFlag(void) { return D3D10_BIND_INDEX_BUFFER; }
  };

  template<class T>
  static Render_t GetRender(
    const ComPtr_t<ID3D10Device> & _pDevice,
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

      const UINT stride = sizeof(T);
      const UINT offset = 0;
      _pDevice->IASetVertexBuffers(0, 1,
        _pBuffer.GetAddressOf(), &stride, &offset);
    };
  }

  template<>
  // cppcheck-suppress unusedFunction
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
  static ComPtr_t<ID3D10Buffer> CreateDynamic(
    ComPtr_t<ID3D10Device> _pDevice,
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

    ComPtr_t<ID3D10Buffer> pBuffer;
    DX_CHECK _pDevice->CreateBuffer(&bd, &InitData, &pBuffer);
    return pBuffer;
  }

  template<class T>
  static ComPtr_t<ID3D10Buffer> Create(ComPtr_t<ID3D10Device> _pDevice,
    const T * _pData)
  {
    const auto pBuffer = Create(_pDevice, _pData, 1);
    Support<T>::SetConstantBuffer(_pDevice, pBuffer);
    return pBuffer;
  }

  template<class T>
  static Creator_t GetCreator(const ComPtr_t<ID3D10Device> & _pDevice)
  {
    return [=](const ComponentPtr_t & _pComponent)
    {
      const Component::Buffer<T> BufferData{ _pComponent };

      return GetRender<T>(_pDevice,
        Create(_pDevice, BufferData.pData, BufferData.Count));
    };
  }

  template<class T>
  static Creator_t GetCreator(
    const ComPtr_t<ID3D10Device> & _pDevice,
    const cbBufferMap_t<T> & _cbMapper)
  {
    return [=](const ComponentPtr_t & _pComponent)
    {
      const Component::Buffer<T> BufferData{ _pComponent };

      return GetRender<T>(_pDevice,
        CreateDynamic(_pDevice, BufferData.pData, BufferData.Count), _cbMapper);
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
    m_pBuffer{ Buffer::Create(_pDevice, &m_Data) }
  {
  }
};

DirectX10::DirectX10(const Data_t & _Data)
{
  using ::alicorn::extension::cpp::IS_RELEASE_CONFIGURATION;

  const UINT DeviceFlags = 
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

  DXGI_SWAP_CHAIN_DESC Desc = { 0 };
  DX_CHECK m_pSwapChain->GetDesc(&Desc);

  SetViewport(Desc.BufferDesc.Width, Desc.BufferDesc.Height);

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

void DirectX10::ResizeWindow(int32_t _Width, int32_t _Height) /*override*/
{
  SetViewport(_Width, _Height);
}

auto DirectX10::CreateCamera(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto CameraId = _pComponent->Id;

  auto RenderLights = [=](void)
  {
    m_pConstants->SetCameraId(CameraId);
  };

  const auto DisableBlendRender = CreateBlendState(false);
  const auto DisableDepthRender = GetDepthState(false, false, false);

  const auto ServiceComponents = m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({}) },
      { uT("Rotation"), api::Component::Make({}) },
    });

  const Render_t CameraOrthographic = [=](void)
  {
    RenderLights();
    DisableBlendRender();
    DisableDepthRender();

    UINT ViewportCount = 1;
    D3D10_VIEWPORT Viewport = { 0 };
    m_pDevice->RSGetViewports(&ViewportCount, &Viewport);

    const Component::Position Pos{ ServiceComponents[0] };

    m_pConstants->Get<::Matrices>().Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixOrthographicOffCenterLH(
        Pos.X, Pos.X + Viewport.Width,
        Pos.Y + Viewport.Height, Pos.Y,
        1.0f, -1.0f));

    m_pConstants->Get<::Matrices>().View = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixIdentity());
  };

  const Render_t CameraPerspective = [=](void)
  {
    RenderLights();
    DisableBlendRender();
    DisableDepthRender();

    UINT ViewportCount = 1;
    D3D10_VIEWPORT Viewport = { 0 };
    m_pDevice->RSGetViewports(&ViewportCount, &Viewport);

    const auto AngleY = (float)::alicorn::extension::cpp::math::GreedToRadian *
      _pComponent->GetValue(uT("fov"), 90.0f);
    const auto aspectRatio = (float)Viewport.Width / (float)Viewport.Height;

    m_pConstants->Get<::Matrices>().Projection = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixPerspectiveFovRH(
        AngleY, aspectRatio, 0.01f, 200.0f));

    // Точка, куда смотрит камера - задается как компонент 
    // Transform.Position.
    const Component::Position Pos{ ServiceComponents[0] };
    const auto Look = ::DirectX::XMVectorSet(Pos.X, Pos.Y, Pos.Z, 1.0f);

    // Расстояние от камеры до Look.
    const auto Distance =
      _pComponent->GetValue(uT("distance"), 0.0f) + 0.1f;

    // Точка, где расположена камера - вычисляется на основе Look, Distance и
    // компонента Transform.Rotation.
    const Component::Rotation Rot{ ServiceComponents[1] };

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
    CameraPerspective : CameraOrthographic;
}

auto DirectX10::CreateState(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto CreateSamplerState = [&](void)
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

  const auto CreateScissorState = [&](void)
  {
    const Component::Scissor ScissorData{ _pComponent };

    D3D10_RASTERIZER_DESC rasterDesc = { 0 };
    rasterDesc.FillMode = D3D10_FILL_SOLID;
    rasterDesc.CullMode = D3D10_CULL_BACK;
    rasterDesc.FrontCounterClockwise = TRUE;
    rasterDesc.ScissorEnable = (ScissorData.IsEnabled) ? TRUE : FALSE;

    ComPtr_t<ID3D10RasterizerState> pScissor;
    DX_CHECK m_pDevice->CreateRasterizerState(&rasterDesc, &pScissor);

    const auto pScissorRect = 
      m_ServiceComponents.Get({ { uT("Rect"), _pComponent } })[0];

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
      m_pDevice->ClearRenderTargetView(
        m_pRenderTargetView.Get(), BackgroundColor.data());
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
    { uT("Depth"),      CreateDepthState      },
    { uT("Clear"),      CreateClearState      },
    { uT("AlphaTest"),  CreateAlphaTestState  },
  };

  return Creators[_pComponent->Kind]();
}

auto DirectX10::CreateFog(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  return DoCreateFog<::Fog>(_pComponent);
}

auto DirectX10::CreateMaterial(const ComponentPtr_t &) -> Render_t /*override*/
{
  return nullptr;
}

auto DirectX10::CreateLight(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  return DoCreateLight<::Lights>(_pComponent);
}

auto DirectX10::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
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

  const UINT MipLevels = 
    pTextureData->GetValue(uT("mipmapping"), false) ? 4 : 1;

  D3D10_TEXTURE2D_DESC textureDesc = { 0 };
  textureDesc.Width = TextureData.Width;
  textureDesc.Height = TextureData.Height;
  textureDesc.MipLevels = MipLevels; //0 - full set of subtextures
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  textureDesc.Usage = D3D10_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  textureDesc.MiscFlags = 0;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.SampleDesc.Quality = 0;

  ::std::vector<D3D10_SUBRESOURCE_DATA> Init{ MipLevels };

  for (size_t i = 0; i < MipLevels; i++)
  {
    memset(&Init[i], 0x00, sizeof(D3D10_SUBRESOURCE_DATA));
    Init[i].pSysMem = TextureData.pData;
    Init[i].SysMemPitch = TextureData.Width * 4;
  }

  ComPtr_t<ID3D10Texture2D> pTexture;
  DX_CHECK m_pDevice->CreateTexture2D(&textureDesc, Init.data(), &pTexture);

  D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc = { 0 };
  srvDesc.Format = textureDesc.Format;
  srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
  srvDesc.Texture2D.MostDetailedMip = textureDesc.MipLevels - 1;

  ComPtr_t<ID3D10ShaderResourceView> pShaderResourceView;
  DX_CHECK m_pDevice->CreateShaderResourceView(pTexture.Get(),
    &srvDesc, &pShaderResourceView);

  return [=](void)
  {
    m_pDevice->PSSetShaderResources(iDestination, 1, 
      pShaderResourceView.GetAddressOf());
  };
}

auto DirectX10::CreateShader(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
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
  auto Entry = ShaderData.Entry;

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

  const auto pCompiledShader = DirectX::Shader::Compile(
    DirectX::Shader::Convert(Define) + ShaderText, Entry.c_str(),
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
    return CreateVertexShader(
      {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
      });
  };

  const auto VertexGuiShader = [&](void)
  {
    return CreateVertexShader(
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR",    0, DXGI_FORMAT_R32_UINT, 0, 8, D3D10_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    });
  };

  const auto VertexTexturedShader = [&](void)
  {
    return CreateVertexShader(
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    });
  };

  const auto PixelShader = [&](void)
  {
    ComPtr_t<ID3D10PixelShader> pPixelShader;
    DX_CHECK m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),
      pCompiledShader->GetBufferSize(), &pPixelShader);

    return [=](void)
    {
      m_pDevice->PSSetShader(pPixelShader.Get());
      m_pConstants->Update<::Lights>();
      m_pConstants->Update<::Fog>();
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Vertex"),     VertexShader },
    { uT("Polygon"),    VertexGuiShader },
    { uT("Polyhedron"), VertexTexturedShader },
    { uT("Pixel"),      PixelShader },
  };

  return Creators[ShaderData.Kind]();
}

#pragma warning(push)
#pragma warning(disable: 4996)

auto DirectX10::CreateBuffer(const ComponentPtr_t & _pBuffer) -> Render_t /*override*/
{
  namespace vertex = ::covellite::api::vertex;
  using Content_t = ::std::vector<::covellite::api::Vertex>;

  const auto pBuffer = 
    m_ServiceComponents.Get({ { uT("Buffer"), _pBuffer } })[0];

  if (pBuffer->IsType<const vertex::Polygon *>(uT("data")))
  {
    return Buffer::GetCreator<vertex::Polygon>(m_pDevice)(pBuffer);
  }
  else if (pBuffer->IsType<const vertex::Polyhedron *>(uT("data")))
  {
    using BufferMapper_t = cbBufferMap_t<vertex::Polyhedron>;

    const auto & cbBufferMapper =
      _pBuffer->GetValue<const BufferMapper_t &>(uT("mapper"), nullptr);

    return (cbBufferMapper == nullptr) ? 
      Buffer::GetCreator<vertex::Polyhedron>(m_pDevice)(pBuffer) :
      Buffer::GetCreator<vertex::Polyhedron>(m_pDevice, cbBufferMapper)(pBuffer);
  }
  else if (pBuffer->IsType<const ::covellite::api::Vertex *>(uT("data")))
  {
    using BufferMapper_t = cbBufferMap_t<::covellite::api::Vertex>;

    const auto & cbBufferMapper =
      _pBuffer->GetValue<const BufferMapper_t &>(uT("mapper"), nullptr);

    return (cbBufferMapper == nullptr) ?
      Buffer::GetCreator<::covellite::api::Vertex>(m_pDevice)(pBuffer) :
      Buffer::GetCreator<::covellite::api::Vertex>(m_pDevice, cbBufferMapper)(pBuffer);
  }

  return Buffer::GetCreator<int>(m_pDevice)(pBuffer);
}

#pragma warning(pop)

auto DirectX10::CreateGeometry(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto GetStaticPreRenderGeometry = [this](void) -> Render_t
  {
    CreateBasePreRenderGeometry()();
    const auto World = m_pConstants->Get<::Matrices>().World;

    return [=](void)
    {
      m_pConstants->Get<::Matrices>().World = World;
      m_pConstants->Update<::Matrices>();
    };
  };

  const auto Variety = _pComponent->GetValue(uT("variety"), uT("Default"));

  const auto PreRender =
    (Variety == uT("Default")) ? CreateBasePreRenderGeometry() :
    (Variety == uT("Static")) ? GetStaticPreRenderGeometry() :
    (Variety == uT("Billboard")) ? CreateBillboardPreRenderGeometry() :
    throw STD_EXCEPTION << "Unexpected variety: " << Variety <<
    " [id=" << _pComponent->Id << "].";

  return [=](void)
  {
    PreRender();

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
    &descDSV, &m_pDepthStencilView);
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
      m_pDevice->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(),
        nullptr);
    };
  }

  D3D10_DEPTH_STENCIL_DESC dsDesc = { 0 };
  dsDesc.DepthEnable = true;
  dsDesc.DepthWriteMask = _IsOverwrite ?
    D3D10_DEPTH_WRITE_MASK_ALL : D3D10_DEPTH_WRITE_MASK_ZERO;
  dsDesc.DepthFunc = D3D10_COMPARISON_LESS;
  dsDesc.StencilEnable = false;

  ComPtr_t<ID3D10DepthStencilState> pDSState;
  DX_CHECK m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState);

  const Render_t RenderDepthEnabled = [=](void)
  {
    m_pDevice->OMSetDepthStencilState(pDSState.Get(), 1);
    m_pDevice->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(),
      m_pDepthStencilView.Get());
  };

  const Render_t RenderDepthClear = [=](void)
  {
    m_pDevice->OMSetDepthStencilState(pDSState.Get(), 1);
    m_pDevice->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(),
      m_pDepthStencilView.Get());
    m_pDevice->ClearDepthStencilView(m_pDepthStencilView.Get(),
      D3D10_CLEAR_DEPTH, 1.0f, 0);
  };

  return _IsClear ? RenderDepthClear : RenderDepthEnabled;
}

auto DirectX10::CreateBasePreRenderGeometry(void) -> Render_t
{
  Renders_t Result;

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

      m_pConstants->Get<::Matrices>().World *= 
        ::DirectX::XMMatrixRotationX(Rotation.X);
      m_pConstants->Get<::Matrices>().World *= 
        ::DirectX::XMMatrixRotationY(Rotation.Y);
      m_pConstants->Get<::Matrices>().World *= 
        ::DirectX::XMMatrixRotationZ(Rotation.Z);
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

  Result.push_back([=](void)
  {
    m_pConstants->Get<::Matrices>().World = ::DirectX::XMMatrixIdentity();
  });

  m_ServiceComponents.Process(
  {
    { uT("Position"), CreatePosition },
    { uT("Rotation"), CreateRotation },
    { uT("Scale"), CreateScale },
  });

  Result.push_back([=](void)
  {
    m_pConstants->Get<::Matrices>().World =
      ::DirectX::XMMatrixTranspose(m_pConstants->Get<::Matrices>().World);
    m_pConstants->Update<::Matrices>();
  });

  return [Result](void)
  {
    for (auto & Render : Result) Render();
  };
}

auto DirectX10::CreateBillboardPreRenderGeometry(void) -> Render_t
{
  Renders_t Result;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    Result.push_back([=](void)
    {
      const Component::Position Position{ _pPosition };

      m_pConstants->Get<::Matrices>().World *=
        ::DirectX::XMMatrixTranslation(Position.X, Position.Y, Position.Z);
    });
  };

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

  m_ServiceComponents.Process({ { uT("Position"), CreatePosition } });

  Result.push_back([=](void)
  {
    m_pConstants->Get<::Matrices>().World =
      ::DirectX::XMMatrixTranspose(m_pConstants->Get<::Matrices>().World);
    m_pConstants->Update<::Matrices>();
  });

  return [Result](void)
  {
    for (auto & Render : Result) Render();
  };
}
