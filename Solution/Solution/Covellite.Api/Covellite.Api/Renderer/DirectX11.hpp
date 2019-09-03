
#pragma once
#include <wrl.h>
#include "GraphicApi.hpp"

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
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ���������� ������� ������������ API ��� Windows/DirectX11.
* \details
*  - ��������� Windows 7 � ��������� �� ������� ������.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
*  1.3.0.0        \n
*  1.4.0.0        \n
* \date
*  29 ������ 2018    \n
*  20 ������ 2018    \n
*  29 ������� 2018    \n
*  23 ���� 2019    \n
*  11 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018 - 2019
*/
class DirectX11 final :
  public GraphicApi
{
  template<class T>
  using ComPtr_t = ::Microsoft::WRL::ComPtr<T>;
  using Renders_t = ::std::vector<Render_t>;
  using Data_t = ::covellite::api::renderer::SettingsData;

public:
  // ��������� IGraphicApi:
  String_t GetUsingApi(void) const override;
  void PresentFrame(void) override;
  void ResizeWindow(int32_t, int32_t) override;

protected:
  // ��������� GraphicApi:
  Render_t CreateCamera(const ComponentPtr_t &) override;
  Render_t CreateState(const ComponentPtr_t &) override;
  Render_t CreateFog(const ComponentPtr_t &) override;
  Render_t CreateMaterial(const ComponentPtr_t &) override;
  Render_t CreateLight(const ComponentPtr_t &) override;
  Render_t CreateTexture(const ComponentPtr_t &) override;
  Render_t CreateShader(const ComponentPtr_t &) override;
  Render_t CreateBuffer(const ComponentPtr_t &) override;
  Render_t CreateGeometry(const ComponentPtr_t &) override;

private:
  void CreateDeviceAndSwapChain(const Data_t &);
  void SetViewport(int, int);
  void CreateRenderTargetView(int, int);
  void CreateDepthStencilView(int, int);

private:
  Render_t CreateBlendState(bool);
  Render_t GetDepthState(const bool, const bool, const bool);
  Render_t GetPreRenderGeometry(const bool);
  Render_t GetPreRenderBillboardGeometry(void);

private:
  ComPtr_t<ID3D11Device>            m_pDevice;
  ComPtr_t<ID3D11DeviceContext>     m_pImmediateContext;
  ComPtr_t<IDXGISwapChain>          m_pSwapChain;
  ComPtr_t<ID3D11RenderTargetView>  m_pRenderTargetView;
  ComPtr_t<ID3D11DepthStencilView>  m_pDepthStencilView;

private:
  class Buffer;
  template<class>
  class ConstantBuffer;

public:
  explicit DirectX11(const Data_t &);
  ~DirectX11(void);

private:
  FRIEND_TEST(DirectX11_test, Test_Present_Geometry_Billboard);
  FRIEND_TEST(DirectX11_test, Test_Present_Geometry_CombineTransform);
};

FACTORY_REGISTER_STRING_NAME(DirectX11);

} // namespace renderer

} // namespace api

} // namespace covellite
