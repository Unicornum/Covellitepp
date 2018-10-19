
#pragma once
#include <alicorn/std/string.hpp>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"

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
*  ��������������� �����, ���������� ��� OpenGL, ����� ��� Windows � Android
*  ������. 
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  06 ������ 2017    \n
*  27 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017 - 2018
*/
class OpenGLCommon :
  public Registator_t<IGraphicApi>
{
public:
  // ��������� IRender:
  void ClearWindow(void) final;
  void ResizeWindow(int32_t, int32_t) final;

public:
  // ��������� IGraphicApi:
  String_t GetUsingApi(void) const final;

public:
  // ��������� IGraphicApi:
  ITexture * Create(const ITexture::Data &) final;
  void Destroy(ITexture *) final;
  IGeometry * Create(const IGeometry::Data &) final;
  void Destroy(IGeometry *) final;
  void EnableScissorRegion(int, int, int, int) final;
  void DisableScissorRegion(void) final;
  void Render(void) final;

protected:
  virtual void DoOrtho(int32_t, int32_t) = 0;

private:
  const Data::Color m_BackgroundColor;
  const int         m_Top;
  const String_t    m_PreVersion;

public:
  OpenGLCommon(const Data &, const String_t &);
};

} // namespace renderer

} // namespace api

} // namespace covellite
