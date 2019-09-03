
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
*  Класс входит в проект \ref CovelliteApiPage \n
*  класс общей логики реализаций DirectX.
*
* \version
*  1.0.0.0        \n
* \date
*  16 Февраль 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018 - 2019
*/
class DirectX final
{
  template<class T>
  using ComPtr_t = ::Microsoft::WRL::ComPtr<T>;
  using BinaryData_t = ::std::vector<uint8_t>;
  using String_t = ::alicorn::extension::std::String;

public:
  class Shader final
  {
  public:
    static ComPtr_t<ID3DBlob> Compile(const BinaryData_t &, LPCSTR, LPCSTR);
    static ::std::string GetVersion(const String_t &);
    static BinaryData_t Convert(const ::std::string & _Source);
  };
};

} // namespace renderer

} // namespace api

} // namespace covellite
