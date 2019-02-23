
#pragma once
#include <vector>
#include <wrl.h>
#include <d3dcommon.h>

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
*  ����� ����� ������ ���������� DirectX.
*
* \version
*  1.0.0.0        \n
* \date
*  16 ������� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018 - 2019
*/
class DirectX final
{
  template<class T>
  using ComPtr_t = ::Microsoft::WRL::ComPtr<T>;
  using BinaryData_t = ::std::vector<uint8_t>;

public:
  class Shader final
  {
  public:
    static ComPtr_t<ID3DBlob> Compile(const BinaryData_t &, LPCSTR, LPCSTR);
  };
};

} // namespace renderer

} // namespace api

} // namespace covellite