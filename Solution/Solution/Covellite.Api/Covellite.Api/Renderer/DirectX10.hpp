
#pragma once
#include <wrl.h>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"
#include "CapturingServiceComponent.hpp"
#include "Updater.hpp"

struct ID3D10Device;
struct IDXGISwapChain;
struct ID3D10RenderTargetView;
struct ID3D10Buffer;
struct ID3D10Texture2D;
struct ID3D10DepthStencilView;

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
* \date
*  25 Август 2018    \n
*  17 Ноябрь 2018    \n
*  27 Декабрь 2018    \n
*  15 Февраль 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018 - 2019
*/
class DirectX10 final :
  public Registator_t<IGraphicApi>
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
  const Creators_t & GetCreators(void) const override;

private:
  static Creators_t GetCreators(DirectX10 *);
  void SetViewport(int, int);

private:
  Render_t CreateState(const ComponentPtr_t &);
  Render_t CreateLight(const ComponentPtr_t &);
  Render_t CreateMaterial(const ComponentPtr_t &);
  Render_t CreateFog(const ComponentPtr_t &);
  Render_t CreateTexture(const ComponentPtr_t &);
  Render_t CreateShader(const ComponentPtr_t &);
  Render_t CreateBuffer(const ComponentPtr_t &);
  Render_t CreatePresent(const ComponentPtr_t &);

private:
  class Buffer;
  class Data;

private:
  Render_t CreateBlendState(bool);
  Render_t GetDepthState(bool, bool, bool);
  Render_t CreateCamera(const ComponentPtr_t &);
  Render_t CreateGeometry(const ComponentPtr_t &);
  Render_t CreateBasePreRenderGeometry(void);
  Render_t CreateBillboardPreRenderGeometry(void);

private:
  const Creators_t m_Creators;

private:
  ComPtr_t<ID3D10Device>            m_pDevice;
  ComPtr_t<IDXGISwapChain>          m_pSwapChain;
  ComPtr_t<ID3D10RenderTargetView>  m_pRenderTargetView;
  ComPtr_t<ID3D10DepthStencilView>  m_pDepthStencilView;

private:
  ::std::shared_ptr<Data>   m_pData;
  CapturingServiceComponent m_ServiceComponents;
  Updater                   m_Updater;

public:
  explicit DirectX10(const Data_t &);
  ~DirectX10(void);
};

FACTORY_REGISTER_STRING_NAME(DirectX10);

} // namespace renderer

} // namespace api

} // namespace covellite
