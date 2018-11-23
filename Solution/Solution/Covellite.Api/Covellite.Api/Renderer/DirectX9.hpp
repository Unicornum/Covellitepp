
#pragma once
#include <wrl.h>
#include <alicorn/std/string.hpp>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"
#include "Renderer.hpp"

struct IDirect3D9;
struct IDirect3DDevice9;

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
*  ���������� ������� ������������ API ��� Windows/DirectX9.
* \details
*  - '�������' ����������, �� ����������� � �� ��������������, �����������
*  �� ���� ����� � ����.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  28 ������ 2018    \n
*  18 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class DirectX9 final :
  public Registator_t<IGraphicApi>
{
  template<class T>
  using ComPtr_t = ::Microsoft::WRL::ComPtr<T>;

  // ��������� IGraphicApi:
  String_t GetUsingApi(void) const override;
  void ClearFrame(void) override;
  void PresentFrame(void) override;
  void ResizeWindow(int32_t, int32_t) override;
  const Creators_t & GetCreators(void) const override;

private:
  Render_t CreateCamera(const ComponentPtr_t &);
  Render_t CreateState(const ComponentPtr_t &) const;
  Render_t CreatePosition(const ComponentPtr_t &);
  Render_t CreateBuffer(const ComponentPtr_t &) const;
  Render_t CreateDrawCall(const ComponentPtr_t &) const;
  Render_t CreateTexture(const ComponentPtr_t &) const;

private:
  const DWORD m_BackgroundColor;
  Creators_t m_Creators;
  FLOAT m_FrameWidth = 0.0f;
  FLOAT m_FrameHeight = 0.0f;

private:
  ComPtr_t<IDirect3D9>       m_pDirect3D;
  ComPtr_t<IDirect3DDevice9> m_pDevice;

public:
  explicit DirectX9(const Renderer::Data &);
  ~DirectX9(void);
};

FACTORY_REGISTER_STRING_NAME(DirectX9);

} // namespace renderer

} // namespace api

} // namespace covellite
