
#pragma once
#include <wrl.h>
#include <alicorn/std/string.hpp>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"
#include "ConstantBuffer.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

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
* \date
*  29 Август 2018    \n
*  20 Ноябрь 2018    \n
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
  // Интерфейс IGraphicApi:
  String_t GetUsingApi(void) const override;
  void ClearFrame(void) override;
  void PresentFrame(void) override;
  void ResizeWindow(int32_t, int32_t) override;
  const Creators_t & GetCreators(void) const override;

private:
  void CreateDeviceAndSwapChain(const Renderer::Data &);
  void SetViewport(int, int);

private:
  Render_t CreateCamera(const ComponentPtr_t &);
  Render_t CreateState(const ComponentPtr_t &) const;
  Render_t CreatePosition(const ComponentPtr_t &);
  Render_t CreateBuffer(const ComponentPtr_t &) const;
  Render_t CreateDrawCall(const ComponentPtr_t &) const;
  Render_t CreateTexture(const ComponentPtr_t &) const;
  Render_t CreateShader(const ComponentPtr_t &) const;

private:
  FLOAT m_BkColor[4];
  ConstantBuffer m_Constants;
  Creators_t m_Creators;

private:
  ComPtr_t<ID3D11Device>            m_pDevice;
  ComPtr_t<ID3D11DeviceContext>     m_pImmediateContext;
  ComPtr_t<IDXGISwapChain>          m_pSwapChain;
  ComPtr_t<ID3D11RenderTargetView>  m_pRenderTargetView;

private:
  class Buffer;

public:
  explicit DirectX11(const Renderer::Data &);
  ~DirectX11(void);
};

FACTORY_REGISTER_STRING_NAME(DirectX11);

} // namespace renderer

} // namespace api

} // namespace covellite
