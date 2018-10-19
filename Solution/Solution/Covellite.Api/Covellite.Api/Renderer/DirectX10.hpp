
#pragma once
#include <wrl.h>
#include "CommonImpl.hpp"
#include "ConstantBuffer.hpp"

struct ID3D10Device;
struct IDXGISwapChain;
struct ID3D10RenderTargetView;

namespace covellite
{

namespace api
{

namespace renderer
{

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ���������� ������� ������������ API ��� Windows/DirectX10.
* \details
*  - '�������' ����������, �� ����������� � �� ��������������, �����������
*  �� ���� ����� � ����.
*  
* \version
*  1.0.0.0        \n
* \date
*  25 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class DirectX10 final :
  public CommonImpl
{
  template<class T>
  using ComPtr_t = ::Microsoft::WRL::ComPtr<T>;

public:
  // ��������� IRender:
  void ClearWindow(void) override;
  void Present(void) override;
  void ResizeWindow(int32_t, int32_t) override;

public:
  // ��������� IGraphicApi:
  String_t GetUsingApi(void) const override;

protected:
  const Creators_t & GetCreators(void) const override;

private:
  void SetViewport(int, int);

  Render_t CreateCamera(const Component &);
  Render_t CreateBlendState(const Component &) const;
  Render_t CreateSamplerState(const Component &) const;
  Render_t CreateScissor(const Component &) const;
  Render_t CreatePosition(const Component &);
  Render_t CreateVertexBuffer(const Component &) const;
  Render_t CreateIndexBuffer(const Component &) const;
  Render_t CreateDrawCall(const Component &) const;
  Render_t CreateTexture(const Component &) const;
  Render_t CreateShader(const Component &) const;

private:
  FLOAT m_BkColor[4];
  renderer::ConstantBuffer m_Constants;
  Creators_t m_Creators;

private:
  ComPtr_t<ID3D10Device> m_pDevice;
  ComPtr_t<IDXGISwapChain> m_pSwapChain;
  ComPtr_t<ID3D10RenderTargetView> m_pRenderTargetView;

private:
  class Buffer;

public:
  explicit DirectX10(const Data &);
  ~DirectX10(void);
};

FACTORY_REGISTER_STRING_NAME(DirectX10);

} // namespace renderer

} // namespace api

} // namespace covellite
