
#include "stdafx.h"
#include "DirectX9.hpp"
#include <alicorn/platform/winapi-check.hpp>

#include <d3d9.h>
#include <d3d9types.h>
#include <directxmath.h>
#pragma comment(lib, "d3d9.lib")

using namespace covellite::api::renderer;

class D3D9Texture final :
  public IGraphicApi::ITexture
{
public:
  void Render(void) override
  {
    auto Result = m_pDevice->SetTexture(0, m_pTexture);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
  }

private:
  IDirect3DDevice9 * m_pDevice;
  IDirect3DTexture9 * m_pTexture;

public:
  D3D9Texture(IDirect3DDevice9 * _pDevice, const Data _Source) :
    m_pDevice(_pDevice)
  {
    auto Result = m_pDevice->CreateTexture(_Source.Width, _Source.Height,
      1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture, NULL);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

    D3DLOCKED_RECT locked_rect;
    Result = m_pTexture->LockRect(0, &locked_rect, NULL, 0);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

    for (int y = 0; y < _Source.Height; ++y)
    {
      for (int x = 0; x < _Source.Width; ++x)
      {
        const byte * pSrcPixel = _Source.pData + 
          static_cast<size_t>(4 * (_Source.Width * y + x));
        byte * pDestPixel = ((byte *)locked_rect.pBits) + 
          static_cast<size_t>(locked_rect.Pitch * y + x * 4);

        pDestPixel[0] = pSrcPixel[2];
        pDestPixel[1] = pSrcPixel[1];
        pDestPixel[2] = pSrcPixel[0];
        pDestPixel[3] = pSrcPixel[3];
      }
    }

    Result = m_pTexture->UnlockRect(0);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
  }

  ~D3D9Texture(void)
  {
    m_pTexture->Release();
  }
};

class D3D9Geometry final :
  public IGraphicApi::IGeometry
{
  class D3D9Vertex final
  {
  public:
    FLOAT x, y, z;
    DWORD colour;
    FLOAT u, v;

  public:
    static DWORD GetFVF(void) 
    {
      return D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
    }
  };

public:
  void Update(FLOAT _X, FLOAT _Y) override
  {
    const auto World = ::DirectX::XMMatrixTranslation(_X, _Y, 0);

    ::DirectX::XMFLOAT4X4 Matrix;
    XMStoreFloat4x4(&Matrix, World);

    auto Result = m_pDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&Matrix);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
  }

  void Render(void) override
  {
    auto Result = m_pDevice->SetFVF(D3D9Vertex::GetFVF());
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

    Result = m_pDevice->SetStreamSource(
      0, m_pVertexBuffer, 0, sizeof(D3D9Vertex));
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

    Result = m_pDevice->SetIndices(m_pIndexBuffer);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

    if (m_pTexture != nullptr)
    {
      m_pTexture->Render();
    }
    else
    {
      Result = m_pDevice->SetTexture(0, NULL);
      if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
    }

    Result = m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 
      m_VerticesCount, 0, m_PrimitivesCount);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
  }

private:
  IDirect3DDevice9 * m_pDevice;
  IDirect3DVertexBuffer9 * m_pVertexBuffer = nullptr;
  IDirect3DIndexBuffer9 * m_pIndexBuffer = nullptr;
  IGraphicApi::ITexture * m_pTexture;
  DWORD m_VerticesCount;
  DWORD m_PrimitivesCount;

public:
  D3D9Geometry(
    IDirect3DDevice9 * _pDevice,
    const IGeometry::Data & _SourceData) :
    m_pDevice(_pDevice),
    m_pTexture(_SourceData.pTexture),
    m_VerticesCount(_SourceData.VerticesCount),
    m_PrimitivesCount(_SourceData.IndicesCount / 3)
  {
    auto Result = m_pDevice->CreateVertexBuffer(
      _SourceData.VerticesCount * sizeof(D3D9Vertex), 
      D3DUSAGE_WRITEONLY, 
      D3D9Vertex::GetFVF(),
      D3DPOOL_DEFAULT, 
      &m_pVertexBuffer,
      NULL);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

    D3D9Vertex * pVertices = nullptr;
    Result = m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

    for (int i = 0; i < _SourceData.VerticesCount; ++i)
    {
      pVertices[i].x = _SourceData.pVertices[i].x;
      pVertices[i].y = _SourceData.pVertices[i].y;
      pVertices[i].z = 0;

      pVertices[i].colour = D3DCOLOR_RGBA(
        (_SourceData.pVertices[i].Color & 0x000000FF) >>  0,
        (_SourceData.pVertices[i].Color & 0x0000FF00) >>  8,
        (_SourceData.pVertices[i].Color & 0x00FF0000) >> 16,
        (_SourceData.pVertices[i].Color & 0xFF000000) >> 24);

      pVertices[i].u = _SourceData.pVertices[i].u;
      pVertices[i].v = _SourceData.pVertices[i].v;
    }

    Result = m_pVertexBuffer->Unlock();
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

    Result = m_pDevice->CreateIndexBuffer(
      _SourceData.IndicesCount * sizeof(unsigned int),
      D3DUSAGE_WRITEONLY, 
      D3DFMT_INDEX32, 
      D3DPOOL_DEFAULT, 
      &m_pIndexBuffer,
      NULL);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

    unsigned int * pIndices = nullptr;
    Result = m_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

    memcpy(pIndices, _SourceData.pIndices, 
      sizeof(unsigned int) * _SourceData.IndicesCount);

    Result = m_pIndexBuffer->Unlock();
    if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
  }

  ~D3D9Geometry(void)
  {
    if (m_pVertexBuffer != nullptr)
    {
      m_pVertexBuffer->Release();
      m_pVertexBuffer = nullptr;
    }

    if (m_pIndexBuffer != nullptr)
    {
      m_pIndexBuffer->Release();
      m_pIndexBuffer = nullptr;
    }
  }
};


DirectX9::DirectX9(const Data & _Data) :
  m_hWnd(_Data.Handle),
  m_BackgroundColor(_Data.BkColor),
  m_pDirect3D(Direct3DCreate9(D3D_SDK_VERSION))
{
  if (m_pDirect3D == NULL) throw STD_EXCEPTION << "Fail.";

  D3DPRESENT_PARAMETERS d3dpp = { 0 };
  d3dpp.Windowed = (_Data.IsFullScreen) ? FALSE : TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

  const auto Result = m_pDirect3D->CreateDevice(
    D3DADAPTER_DEFAULT,  
    D3DDEVTYPE_HAL, 
    m_hWnd,
    D3DCREATE_HARDWARE_VERTEXPROCESSING, 
    &d3dpp, 
    &m_pDevice);
  if (FAILED(Result)) throw STD_EXCEPTION << "Fail.";

  RECT ClientRect;
  WINAPI_CHECK ::GetClientRect(m_hWnd, &ClientRect);

  ResizeWindow(ClientRect.right - ClientRect.left, 
    ClientRect.bottom - ClientRect.top);
}

DirectX9::~DirectX9(void)
{
  if (m_pDevice != nullptr)
  {
    m_pDevice->SetTexture(0, NULL);
    m_pDevice->SetStreamSource(0, NULL, 0, 0);
    m_pDevice->SetIndices(NULL);
    m_pDevice->Release();
    m_pDevice = nullptr;
  }

  if (m_pDirect3D != nullptr)
  {
    m_pDirect3D->Release();
    m_pDirect3D = NULL;
  }
}

void DirectX9::ClearWindow(void) /*override*/
{
  const D3DCOLOR BkColor = D3DCOLOR_COLORVALUE(m_BackgroundColor.R,
    m_BackgroundColor.G, m_BackgroundColor.B, m_BackgroundColor.A);

  auto Result = m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, BkColor, 1.0f, 0);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  Result = m_pDevice->BeginScene();
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
}

void DirectX9::Present(void) /*override*/
{
  auto Result = m_pDevice->EndScene();
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  Result = m_pDevice->Present(NULL, NULL, NULL, NULL);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
}

void DirectX9::ResizeWindow(int32_t _Width, int32_t _Height) /*override*/
{
  const auto Projection = DirectX::XMMatrixOrthographicOffCenterLH(
    0, (FLOAT)_Width, (FLOAT)_Height, 0, -1, 1);

  DirectX::XMFLOAT4X4 Matrix;
  XMStoreFloat4x4(&Matrix, Projection);

  auto Result = m_pDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&Matrix);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
}

DirectX9::String_t DirectX9::GetUsingApi(void) const /*override*/
{
  return uT("DirectX 9");
}

DirectX9::ITexture * DirectX9::Create(const ITexture::Data & _SourceData) /*override*/
{
  return new D3D9Texture(m_pDevice, _SourceData);
}

void DirectX9::Destroy(ITexture * _pTexture) /*override*/
{
  delete _pTexture;
}

DirectX9::IGeometry * DirectX9::Create(const IGeometry::Data & _SourceData) /*override*/
{
  return new D3D9Geometry(m_pDevice, _SourceData);
}

void DirectX9::Destroy(IGeometry * _pGeometry) /*override*/
{
  delete _pGeometry;
}

void DirectX9::EnableScissorRegion(int _X, int _Y, int _Width, int _Height) /*override*/
{
  auto Result = m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  RECT Rect;
  Rect.left = _X;
  Rect.right = _X + _Width;
  Rect.top = _Y;
  Rect.bottom = _Y + _Height;

  Result = m_pDevice->SetScissorRect(&Rect);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
}

void DirectX9::DisableScissorRegion(void) /*override*/
{
  auto Result = m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
}

void DirectX9::Render(void) /*override*/
{
  auto Result = m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  // Enable alpha-blending for Rocket.
  Result = m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  Result = m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  Result = m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  // Set up the texture stage states for the diffuse texture.
  Result = m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  Result = m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  Result = m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  Result = m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  Result = m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  Result = m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  Result = m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";

  // Disable lighting for Rocket.
  Result = m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
  if (FAILED(Result)) throw STD_EXCEPTION << "Failed";
}
