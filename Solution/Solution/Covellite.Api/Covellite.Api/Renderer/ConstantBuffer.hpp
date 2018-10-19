
#pragma once
#include <directxmath.h>

namespace covellite
{

namespace api
{

namespace renderer
{

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Структура для передачи данных шейдеру, который используется DirectX.
*  
* \version
*  1.0.0.0        \n
* \date
*  10 Сентябрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class ConstantBuffer final
{
public:
  ::DirectX::XMMATRIX World;
  ::DirectX::XMMATRIX Projection;
  ::DirectX::XMFLOAT2 IsTextureDisabled;
};

} // namespace renderer

} // namespace api

} // namespace covellite
