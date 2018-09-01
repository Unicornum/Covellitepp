
#pragma once
#include <alicorn\std\check-proxy.hpp>
#include <alicorn\platform.lib.hpp>

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
*  ��������������� ����� ������ ������ ������� DX_CHECK.
*  
* \version
*  1.0.0.0        \n
* \date
*  30 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class DxCheck final :
  public ::alicorn::extension::std::CheckProxy<DxCheck>
{
public:
  static bool IsSuccessfull(long);
  static ::std::string GetMessageText(long);
};

} // namespace render

} // namespace api

} // namespace covellite

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ������ �������� ������ ������� DirectX.
*
* \version
*  1.0.0.0        \n
* \date
*  30 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
#define DX_CHECK \
  CHECK_PROXY(::covellite::api::render::DxCheck)
