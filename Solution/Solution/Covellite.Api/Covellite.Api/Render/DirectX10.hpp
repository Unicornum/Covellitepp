
#pragma once
#include <directxmath.h>
#include <alicorn/std/string.hpp>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"

struct ID3D10Device;
struct IDXGISwapChain;
struct ID3D10RenderTargetView;
struct ID3D10InputLayout;
struct ID3D10VertexShader;
struct ID3D10PixelShader;
struct ID3D10RasterizerState;
struct ID3D10BlendState;
struct ID3D10SamplerState;
struct ID3D10Buffer;

namespace covellite
{

namespace api
{

namespace render
{

struct ConstantBuffer
{
  ::DirectX::XMMATRIX mWorld;       // Матрица мира
  ::DirectX::XMMATRIX mProjection;  // Матрица проекции
  ::DirectX::XMFLOAT2 IsTextureDisabled;
};

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Реализация рендера графического API для Windows/DirectX10.
* \details
*  - 'Наивная' реализация, не тестируется и не поддерживается, используйте
*  на свой страх и риск.
*  
* \version
*  1.0.0.0        \n
* \date
*  25 Август 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class DirectX10 final :
  public Registator_t<IGraphicApi>
{
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
  void SetupEffect(void);
  void SetViewport(int, int);

private:
  ID3D10Device * m_pDevice;
  IDXGISwapChain * m_pSwapChain = nullptr;
  ID3D10RenderTargetView * m_pRenderTargetView = nullptr;
  ID3D10InputLayout * m_pVertexLayout = nullptr;
  ID3D10VertexShader * m_pVertexShader = nullptr;
  ID3D10PixelShader * m_pPixelShader = nullptr;
  ID3D10RasterizerState * m_pScissorTestEnable = nullptr;
  ID3D10RasterizerState * m_pScissorTestDisable = nullptr;
  ID3D10BlendState * m_pBlendEnable = nullptr;
  ID3D10SamplerState * m_pSamplerState = nullptr;
  ID3D10Buffer * m_pConstantBuffer = nullptr;

public:
  explicit DirectX10(const Data &);
  ~DirectX10(void) noexcept;
};

FACTORY_REGISTER_STRING_NAME(DirectX10);

} // namespace render

} // namespace api

} // namespace covellite
