
#pragma once
#include <wrl.h>
#include <alicorn/std/string.hpp>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11BlendState;
struct ID3D11RasterizerState;
struct ID3D11Buffer;
struct ID3D11SamplerState;

namespace covellite
{

namespace api
{

namespace render
{

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Реализация рендера графического API для Windows/DirectX11.
* \details
*  - Требуется Windows 7 и поддержка со стороны железа.
*  
* \version
*  1.0.0.0        \n
* \date
*  29 Август 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class DirectX11 final :
  public Registator_t<IGraphicApi>
{
  template<class T>
  using ComPtr_t = ::Microsoft::WRL::ComPtr<T>;

public:
  // Интерфейс IRender:
  void ClearWindow(void) override;
  void Present(void) override;
  void ResizeWindow(int32_t, int32_t) override;

public:
  // Интерфейс IGraphicApi:
  String_t GetUsingApi(void) const override;

public:
  ITexture * Create(const ITexture::Data &) override;
  void Destroy(ITexture *) override;
  IGeometry * Create(const IGeometry::Data &) override;
  void Destroy(IGeometry *) override;
  void EnableScissorRegion(int, int, int, int) override;
  void DisableScissorRegion(void) override;
  void Render(void) override;

private:
  void CreateDeviceAndSwapChain(const Data &);
  void SetViewport(int, int);
  void SetupEffect(void);

private:
  ComPtr_t<ID3D11Device>            m_pd3dDevice;
  ComPtr_t<ID3D11DeviceContext>     m_pImmediateContext;
  ComPtr_t<IDXGISwapChain>          m_pSwapChain;
  ComPtr_t<ID3D11RenderTargetView>  m_pRenderTargetView;

  ComPtr_t<ID3D11RasterizerState>   m_pScissorTestEnable;
  ComPtr_t<ID3D11RasterizerState>   m_pScissorTestDisable;
  ComPtr_t<ID3D11InputLayout>       m_pVertexLayout;
  ComPtr_t<ID3D11VertexShader>      m_pVertexShader;
  ComPtr_t<ID3D11PixelShader>       m_pPixelShader;
  ComPtr_t<ID3D11SamplerState>      m_pSamplerState;
  ComPtr_t<ID3D11BlendState>        m_pBlendEnable;

  ComPtr_t<ID3D11Buffer>            m_pConstantBuffer;

  FLOAT m_BkColor[4];

private:
  class Texture;
  class Buffer;
  class Geometry;

public:
  explicit DirectX11(const Data &);
  ~DirectX11(void);
};

FACTORY_REGISTER_STRING_NAME(DirectX11);

} // namespace render

} // namespace api

} // namespace covellite
