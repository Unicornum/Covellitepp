
#pragma once
#include <deque>
#include <wrl.h>
#include <alicorn/std/string.hpp>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"
#include "fx/Data.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;
struct ID3D11DepthStencilView;

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
* \date
*  29 Август 2018    \n
*  20 Ноябрь 2018    \n
*  29 Декабрь 2018    \n
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
  using Renders_t = ::std::vector<Render_t>;
  using PreRender_t = ::std::function<void(const ComponentPtr_t &)>;
  using PreRenders_t = ::std::map<String_t, PreRender_t>;

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
  void CreateRenderTargetView(int, int);
  void CreateDepthStencilView(int, int);

private:
  Render_t CreateState(const ComponentPtr_t &);
  Render_t CreateLight(const ComponentPtr_t &);
  Render_t CreateMaterial(const ComponentPtr_t &);
  Render_t CreateTexture(const ComponentPtr_t &);
  Render_t CreateShader(const ComponentPtr_t &);
  Render_t CreateBuffer(const ComponentPtr_t &);
  Render_t CreatePresent(const ComponentPtr_t &);
  Render_t CreateCamera(const ComponentPtr_t &);
  Render_t CreateGeometry(const ComponentPtr_t &);

private:
  Render_t CreateBlendState(bool);
  Render_t GetLightsRender(void);
  Render_t GetDeptRender(const ComponentPtr_t &);
  Renders_t GetPreRendersGeometry(void);
  ComponentPtr_t GetPreRenderComponent(const ComponentPtr_t &, const String_t &);
  void PreRenderComponentsProcess(const PreRenders_t &);

private:
  FLOAT                           m_BkColor[4];
  Creators_t                      m_Creators;
  ::std::deque<ComponentPtr_t>    m_PreRenderComponent;
  ::Matrices                      m_Constants;
  ::std::map<String_t, ::Lights>  m_Lights;
  String_t                        m_CurrentCameraId;
  ::Lights                        m_CurrentLights;

private:
  ComPtr_t<ID3D11Device>            m_pDevice;
  ComPtr_t<ID3D11DeviceContext>     m_pImmediateContext;
  ComPtr_t<IDXGISwapChain>          m_pSwapChain;
  ComPtr_t<ID3D11RenderTargetView>  m_pRenderTargetView;
  ComPtr_t<ID3D11DepthStencilView>  m_pDepthStencilView;
  ComPtr_t<ID3D11Buffer>            m_pMatrices;
  ComPtr_t<ID3D11Buffer>            m_pLights;

private:
  class Shader;
  class Buffer;

public:
  explicit DirectX11(const Renderer::Data &);
  ~DirectX11(void);
};

FACTORY_REGISTER_STRING_NAME(DirectX11);

} // namespace renderer

} // namespace api

} // namespace covellite
