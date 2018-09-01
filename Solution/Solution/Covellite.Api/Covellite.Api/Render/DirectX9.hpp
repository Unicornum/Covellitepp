
#pragma once
#include <alicorn/std/string.hpp>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"

struct IDirect3D9;
struct IDirect3DDevice9;

namespace covellite
{

namespace api
{

namespace render
{

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ���������� ������� ������������ API ��� Windows/DirectX9.
* \details
*  - '�������' ����������, �� ����������� � �� ��������������, �����������
*  �� ���� ����� � ����.
*
* \version
*  1.0.0.0        \n
* \date
*  28 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class DirectX9 final :
  public Registator_t<IGraphicApi>
{
public:
  // ��������� IRender:
  void ClearWindow(void) override;
  void Present(void) override;
  void ResizeWindow(int32_t, int32_t) override;

public:
  // ��������� IGraphicApi:
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
  HWND m_hWnd;
  const Data::Color m_BackgroundColor;
  IDirect3D9 * m_pDirect3D;
  IDirect3DDevice9 * m_pDevice = nullptr;

public:
  explicit DirectX9(const Data &);
  ~DirectX9(void);
};

FACTORY_REGISTER_STRING_NAME(DirectX9);

} // namespace render

} // namespace api

} // namespace covellite
