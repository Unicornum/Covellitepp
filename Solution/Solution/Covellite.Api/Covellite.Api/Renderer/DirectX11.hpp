
#pragma once
#include <wrl.h>
#include "GraphicApi.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;
struct ID3D11DepthStencilView;
class DirectX11_test;

namespace covellite
{

namespace api
{

namespace renderer
{

/**
* \ingroup CovelliteApiRendererGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Реализация рендера графического API для Windows/DirectX11.
* \details
*  - Требуется Windows 7 и поддержка со стороны железа.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
*  1.3.0.0        \n
*  1.4.0.0        \n
* \date
*  29 Август 2018    \n
*  20 Ноябрь 2018    \n
*  29 Декабрь 2018    \n
*  23 Июль 2019    \n
*  11 Август 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018 - 2019
*/
class DirectX11 final :
  public GraphicApi
{
  template<class T>
  using ComPtr_t = ::Microsoft::WRL::ComPtr<T>;
  using Renders_t = ::std::vector<Render_t>;
  using Data_t = ::covellite::api::renderer::SettingsData;

public:
  // Интерфейс IGraphicApi:
  String_t GetUsingApi(void) const override;
  void PresentFrame(void) override;
  void ResizeWindow(int32_t, int32_t) override;

protected:
  // Интерфейс GraphicApi:
  Render_t CreateCamera(const ComponentPtr_t &) override;
  Render_t CreateBkSurface(const ComponentPtr_t &) override;
  Render_t CreateState(const ComponentPtr_t &) override;
  Render_t CreateFog(const ComponentPtr_t &) override;
  Render_t CreateTexture(const ComponentPtr_t &) override;
  Render_t CreateShader(const ComponentPtr_t &) override;
  Render_t CreateBuffer(const ComponentPtr_t &) override;
  Render_t CreateTransform(const ComponentPtr_t &) override;
  Render_t CreatePresentBuffer(const ComponentPtr_t &) override;

private:
  // deprecated
  Render_t CreateLight(const ComponentPtr_t &) override;
  Render_t CreateGeometry(const ComponentPtr_t &) override;

private:
  void CreateDeviceAndSwapChain(const Data_t &);
  void SetViewport(int, int);
  void CreateRenderTargetView(int, int);
  void CreateDepthStencilView(int, int);

private:
  Render_t CreateBlendState(bool);
  Render_t GetDepthState(const bool, const bool, const bool);
  template<class>
  Render_t CreateDefaultTransformRender(const ComponentPtr_t &);
  template<class>
  Render_t CreateStaticTransformRender(const ComponentPtr_t &);
  template<class, class>
  Render_t CreateBillboardTransformRender(const ComponentPtr_t &);

private:
  ComPtr_t<ID3D11Device>                  m_pDevice;
  ComPtr_t<ID3D11DeviceContext>           m_pImmediateContext;
  ComPtr_t<IDXGISwapChain>                m_pSwapChain;
  ComPtr_t<ID3D11RenderTargetView>        m_pScreenRenderTargetView;
  ComPtr_t<ID3D11DepthStencilView>        m_pScreenDepthStencilView;
  ::std::vector<ID3D11RenderTargetView *> m_CurrentRenderTargets;
  ComPtr_t<ID3D11DepthStencilView>        m_pCurrentDepthStencilView;

public:
  class Texture;

private:
  class Buffer;
  template<class>
  class ConstantBuffer;

public:
  explicit DirectX11(const Data_t &);
  ~DirectX11(void);

private:
  friend DirectX11_test;
};

FACTORY_REGISTER_STRING_NAME(DirectX11);

} // namespace renderer

} // namespace api

} // namespace covellite
