
#include "stdafx.h"
#include "DirectX9.hpp"
#include <alicorn/platform/winapi-check.hpp>
#include <Covellite/Api/Vertex.hpp>
#include "DxCheck.hpp"
#include "Component.hpp"

#include <d3d9.h>
#include <d3d9types.h>
#include <directxmath.h>
#pragma comment(lib, "d3d9.lib")

using namespace covellite::api::renderer;

DirectX9::DirectX9(const Data_t & _Data) :
  m_pDirect3D(Direct3DCreate9(D3D_SDK_VERSION))
{
  if (m_pDirect3D == NULL) throw STD_EXCEPTION << "Fail.";

  D3DPRESENT_PARAMETERS d3dpp = { 0 };
  d3dpp.Windowed = (_Data.IsFullScreen) ? FALSE : TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

  DX_CHECK m_pDirect3D->CreateDevice(
    D3DADAPTER_DEFAULT,  
    D3DDEVTYPE_HAL, 
    ::covellite::any_cast<HWND>(_Data.Handle),
    D3DCREATE_HARDWARE_VERTEXPROCESSING, 
    &d3dpp, 
    &m_pDevice);

  RECT ClientRect;
  WINAPI_CHECK ::GetClientRect(
    ::covellite::any_cast<HWND>(_Data.Handle), &ClientRect);

  ResizeWindow(ClientRect.right - ClientRect.left, 
    ClientRect.bottom - ClientRect.top);

  m_Creators =
  {
    { uT("Camera"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateCamera(_pData); } },
    { uT("State"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateState(_pData); } },
    { uT("Texture"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateTexture(_pData); } },
    { uT("Buffer"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateBuffer(_pData); } },
    { uT("DrawCall"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateDrawCall(_pData); } },
    { uT("Position"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreatePosition(_pData); } },
  };
}

DirectX9::~DirectX9(void) = default;

DirectX9::String_t DirectX9::GetUsingApi(void) const /*override*/
{
  return uT("DirectX 9");
}

void DirectX9::PresentFrame(void) /*override*/
{
  DX_CHECK m_pDevice->EndScene();
  DX_CHECK m_pDevice->Present(NULL, NULL, NULL, NULL);
}

void DirectX9::ResizeWindow(int32_t _Width, int32_t _Height) /*override*/
{
  m_FrameWidth = (FLOAT)_Width;
  m_FrameHeight = (FLOAT)_Height;
}

auto DirectX9::GetCreators(void) const -> const Creators_t & /*override*/
{
  return m_Creators;
}

auto DirectX9::CreateCamera(const ComponentPtr_t &) -> Render_t
{
  return [&]()
  {
    DX_CHECK m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    DX_CHECK m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    const auto Projection = DirectX::XMMatrixOrthographicOffCenterLH(
      0, m_FrameWidth, m_FrameHeight, 0, -1, 1);

    DirectX::XMFLOAT4X4 Matrix;
    XMStoreFloat4x4(&Matrix, Projection);

    DX_CHECK m_pDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&Matrix);
  };
}

auto DirectX9::CreateState(const ComponentPtr_t & _pComponent) const -> Render_t
{
  auto CreateBlendState = [&](const ComponentPtr_t & /*_pComponent*/) -> Render_t
  {
    return [=]()
    {
      DX_CHECK m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
      DX_CHECK m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
      DX_CHECK m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    };
  };

  auto CreateSamplerState = [&](const ComponentPtr_t & /*_pComponent*/) -> Render_t
  {
    return [=]()
    {
      DX_CHECK m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
      DX_CHECK m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    };
  };

  auto CreateScissorState = [&](const ComponentPtr_t & /*_pComponent*/) -> Render_t
  {
    const Component::Scissor Data{ _pComponent };

    Render_t ScissorEnabled = [&, _pComponent]()
    {
      const Component::Scissor Data{ _pComponent };

      RECT rect;
      rect.left = Data.Left;
      rect.right = Data.Right;
      rect.top = Data.Top;
      rect.bottom = Data.Bottom;

      DX_CHECK m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
      DX_CHECK m_pDevice->SetScissorRect(&rect);
    };

    Render_t ScissorDisabled = [&]()
    {
      DX_CHECK m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    };

    return (Data.IsEnabled) ? ScissorEnabled : ScissorDisabled;
  };

  Creators_t Creators =
  {
    { uT("Blend"), CreateBlendState },
    { uT("Sampler"), CreateSamplerState },
    { uT("Scissor"), CreateScissorState },
  };

  return Creators[_pComponent->Kind](_pComponent);
}

auto DirectX9::CreatePosition(const ComponentPtr_t & _pComponent) -> Render_t
{
  return [&, _pComponent]()
  {
    const Component::Position Data{ _pComponent };

    const auto World = ::DirectX::XMMatrixTranslation(Data.X, Data.Y, Data.Z);

    ::DirectX::XMFLOAT4X4 Matrix;
    XMStoreFloat4x4(&Matrix, World);

    DX_CHECK m_pDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&Matrix);
  };
}

auto DirectX9::CreateBuffer(const ComponentPtr_t & _pComponent) const->Render_t
{
  auto CreateVertexBuffer = [&](const ComponentPtr_t & _pComponent) -> Render_t
  {
    const Component::Buffer<Vertex::Polygon> Data{ _pComponent };

    class D3D9Vertex final
    {
    public:
      FLOAT x, y, z;
      DWORD colour;
      FLOAT u, v;

    public:
      inline static DWORD GetFVF(void)
      {
        return D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
      }
    };

    ComPtr_t<IDirect3DVertexBuffer9> pBuffer;
    DX_CHECK m_pDevice->CreateVertexBuffer(
      (UINT)(Data.Count * sizeof(D3D9Vertex)),
      D3DUSAGE_WRITEONLY,
      D3D9Vertex::GetFVF(),
      D3DPOOL_DEFAULT,
      &pBuffer,
      NULL);

    D3D9Vertex * pVertices = nullptr;
    DX_CHECK pBuffer->Lock(0, 0, (void**)&pVertices, 0);

    for (int i = 0; i < Data.Count; ++i)
    {
      pVertices[i].x = Data.pData[i].x;
      pVertices[i].y = Data.pData[i].y;
      pVertices[i].z = 0;

      pVertices[i].colour = D3DCOLOR_RGBA(
        (Data.pData[i].ABGRColor & 0x000000FF) >> 0,
        (Data.pData[i].ABGRColor & 0x0000FF00) >> 8,
        (Data.pData[i].ABGRColor & 0x00FF0000) >> 16,
        (Data.pData[i].ABGRColor & 0xFF000000) >> 24);

      pVertices[i].u = Data.pData[i].u;
      pVertices[i].v = Data.pData[i].v;
    }

    DX_CHECK pBuffer->Unlock();

    return [=]()
    {
      DX_CHECK m_pDevice->SetFVF(D3D9Vertex::GetFVF());
      DX_CHECK m_pDevice->SetStreamSource(0, pBuffer.Get(), 0, sizeof(D3D9Vertex));
    };
  };

  auto CreateIndexBuffer = [&](const ComponentPtr_t & _pComponent)->Render_t
  {
    const Component::Buffer<int> Data{ _pComponent };

    ComPtr_t<IDirect3DIndexBuffer9> pBuffer;
    DX_CHECK m_pDevice->CreateIndexBuffer(
      (UINT)(Data.Count * sizeof(unsigned int)),
      D3DUSAGE_WRITEONLY,
      D3DFMT_INDEX32,
      D3DPOOL_DEFAULT,
      &pBuffer,
      NULL);

    unsigned int * pIndices = nullptr;
    DX_CHECK pBuffer->Lock(0, 0, (void**)&pIndices, 0);

    memcpy(pIndices, Data.pData, sizeof(unsigned int) * Data.Count);

    DX_CHECK pBuffer->Unlock();

    return [=]()
    {
      DX_CHECK m_pDevice->SetIndices(pBuffer.Get());
    };
  };

  Creators_t Creators =
  {
    { uT("Vertex"), CreateVertexBuffer },
    { uT("Index"), CreateIndexBuffer },
  };

  return Creators[_pComponent->Kind](_pComponent);
}

auto DirectX9::CreateDrawCall(const ComponentPtr_t &) const -> Render_t
{
  return [=]()
  {
    IDirect3DVertexBuffer9 * pVertexBuffer = nullptr;
    UINT Offset = 0;
    UINT Stride = 0;
    DX_CHECK m_pDevice->GetStreamSource(0, &pVertexBuffer, &Offset, &Stride);

    D3DVERTEXBUFFER_DESC VertexInfo = { 0 };
    DX_CHECK pVertexBuffer->GetDesc(&VertexInfo);

    IDirect3DIndexBuffer9 * pIndexBuffer = nullptr;
    DX_CHECK m_pDevice->GetIndices(&pIndexBuffer);

    D3DINDEXBUFFER_DESC IndexInfo = { 0 };
    DX_CHECK pIndexBuffer->GetDesc(&IndexInfo);

    DX_CHECK m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
      VertexInfo.Size / Stride, 0, IndexInfo.Size / (3 * sizeof(int)));
    DX_CHECK m_pDevice->SetTexture(0, nullptr);
  };
}

auto DirectX9::CreateTexture(const ComponentPtr_t & _pComponent) const -> Render_t
{
  const Component::Texture Data{ _pComponent };

  ComPtr_t<IDirect3DTexture9> pTexture;
  DX_CHECK m_pDevice->CreateTexture(Data.Width, Data.Height,
    1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);

  D3DLOCKED_RECT locked_rect;
  DX_CHECK pTexture->LockRect(0, &locked_rect, NULL, 0);

  for (size_t y = 0; y < static_cast<size_t>(Data.Height); ++y)
  {
    for (size_t x = 0; x < static_cast<size_t>(Data.Width); ++x)
    {
      const byte * pSrcPixel = Data.pData +
        4 * (static_cast<size_t>(Data.Width) * y + x);
      byte * pDestPixel = ((byte *)locked_rect.pBits) +
        (static_cast<size_t>(locked_rect.Pitch) * y + x * 4);

      pDestPixel[0] = pSrcPixel[2];
      pDestPixel[1] = pSrcPixel[1];
      pDestPixel[2] = pSrcPixel[0];
      pDestPixel[3] = pSrcPixel[3];
    }
  }

  DX_CHECK pTexture->UnlockRect(0);

  return [=]()
  {
    DX_CHECK m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    DX_CHECK m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    DX_CHECK m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    DX_CHECK m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    DX_CHECK m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

    DX_CHECK m_pDevice->SetTexture(0, pTexture.Get());
  };
}
