
#pragma once
#include <wrl.h>
#include "GraphicApi.hpp"

struct ID3D10Device;
struct IDXGISwapChain;
struct ID3D10RenderTargetView;
struct ID3D10DepthStencilView;
struct ID3D10RasterizerState;

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
*  Реализация рендера графического API для Windows/DirectX10.
* \details
*  - 'Наивная' реализация, не тестируется и не поддерживается, используйте
*  на свой страх и риск.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
*  1.3.0.0        \n
*  1.4.0.0        \n
* \date
*  25 Август 2018    \n
*  17 Ноябрь 2018    \n
*  27 Декабрь 2018    \n
*  15 Февраль 2019    \n
*  11 Август 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018 - 2019
*/
class DirectX10 final :
  public GraphicApi
{
  template<class T>
  using ComPtr_t = ::Microsoft::WRL::ComPtr<T>;
  using Renders_t = ::std::vector<Render_t>;

public:
  // Интерфейс IGraphicApi:
  String_t GetUsingApi(void) const override;
  void PresentFrame(void) override;
  void ResizeWindow(const Rect &) override;

protected:
  // Интерфейс GraphicApi:
  Render_t CreateCamera(const ComponentPtr_t &) override;
  Render_t CreateBkSurface(const ComponentPtr_t &) override;
  Render_t CreateState(const ComponentPtr_t &) override;
  Render_t CreateTexture(const ComponentPtr_t &) override;
  Render_t CreateTextureArray(const ComponentPtr_t &) override { throw STD_EXCEPTION << "Not Release!!!"; }
  Render_t CreateShader(const ComponentPtr_t &) override;
  Render_t CreateBuffer(const ComponentPtr_t &) override;
  Render_t CreateTransform(const ComponentPtr_t &) override;
  Render_t CreatePresentBuffer(const ComponentPtr_t &) override;

private:
  void SetViewport(const UINT, const UINT);
  Render_t CreateBlendState(bool);
  Render_t GetDepthState(bool, bool, bool);
  Render_t CreateBasePreRenderGeometry(const ComponentPtr_t &);
  Render_t CreateStaticPreRenderGeometry(const ComponentPtr_t &);
  Render_t CreateBillboardPreRenderGeometry(const ComponentPtr_t &);

private:
  ComPtr_t<ID3D10Device>            m_pDevice;
  ComPtr_t<IDXGISwapChain>          m_pSwapChain;
  ComPtr_t<ID3D10RasterizerState>   m_pDefaultRasterizerState;
  // Screen render targets
  ComPtr_t<ID3D10RenderTargetView>  m_pScreenRenderTargetView;
  ComPtr_t<ID3D10DepthStencilView>  m_pScreenDepthStencilView;
  // Active render targets
  ::std::vector<ID3D10RenderTargetView *> m_CurrentRenderTargets;
  ComPtr_t<ID3D10DepthStencilView>  m_pCurrentDepthStencilView;

private:
  class Buffer;
  template<class>
  class ConstantBuffer;
  class Texture;

public:
  explicit DirectX10(const Data_t &);
  DirectX10(const DirectX10 &) = delete;
  DirectX10(DirectX10 &&) = delete;
  DirectX10 & operator= (const DirectX10 &) = delete;
  DirectX10 & operator= (DirectX10 &&) = delete;
  ~DirectX10(void);
};

FACTORY_REGISTER_STRING_NAME(DirectX10);

} // namespace renderer

} // namespace api

} // namespace covellite
