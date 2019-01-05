
#pragma once
#include <deque>
#include <wrl.h>
#include <alicorn/std/string.hpp>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"
#include "fx/Data.h"

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
* \date
*  25 Август 2018    \n
*  17 Ноябрь 2018    \n
*  27 Декабрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class DirectX10 final :
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
  void SetViewport(int, int);

private:
  Render_t CreateState(const ComponentPtr_t &);
  Render_t CreateLight(const ComponentPtr_t &);
  Render_t CreateMaterial(const ComponentPtr_t &);
  Render_t CreateTexture(const ComponentPtr_t &);
  Render_t CreateShader(const ComponentPtr_t &);
  Render_t CreateBuffer(const ComponentPtr_t &);
  Render_t CreatePresent(const ComponentPtr_t &);
  Render_t GetDeptRender(const ComponentPtr_t &);
  Render_t CreateCamera(const ComponentPtr_t &);
  Render_t CreateGeometry(const ComponentPtr_t &);

private:
  Render_t CreateBlendState(bool);
  void PreRenderComponentsProcess(const PreRenders_t &);
  Renders_t GetPreRendersGeometry(void);

private:
  class Shader;
  class Buffer;

  template<class T>
  class ConstantBuffer final
  {
  public:
    T Data;

  public:
    void Update(const ComPtr_t<ID3D10Device> &);

  private:
    ComPtr_t<ID3D10Buffer> m_pBuffer;

  public:
    ConstantBuffer(void);
  };

private:
  FLOAT                         m_BkColor[4];
  Creators_t                    m_Creators;
  ::std::deque<ComponentPtr_t>  m_PreRenderComponent;
  String_t                      m_CurrentCameraId;

private:
  ComPtr_t<ID3D10Device>                          m_pDevice;
  ComPtr_t<IDXGISwapChain>                        m_pSwapChain;
  ComPtr_t<ID3D10RenderTargetView>                m_pRenderTargetView;
  ComPtr_t<ID3D10DepthStencilView>                m_pDepthStencilView;
  ConstantBuffer<::Matrices>                      m_WorldViewProjection;
  ::std::map<String_t, ConstantBuffer<::Lights>>  m_Lights;
  ConstantBuffer<::Lights>                        m_CurrentLights;

public:
  explicit DirectX10(const Renderer::Data &);
  ~DirectX10(void);
};

FACTORY_REGISTER_STRING_NAME(DirectX10);

} // namespace renderer

} // namespace api

} // namespace covellite
