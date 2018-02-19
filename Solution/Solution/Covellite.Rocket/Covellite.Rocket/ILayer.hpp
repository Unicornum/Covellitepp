
#pragma once

namespace covellite
{

namespace rocket
{

/**
* \ingroup gCovelliteRocketClasses
* \brief
*  Класс входит в проект \ref CovelliteRocketPage \n
*  Интерфейсный класс слоя (текущего отображаемого экрана).
*
* \version
*  1.0.0.0        \n
* \date
*  09 Декабрь 2016    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016
*/
class ILayer
{
public:
  virtual void Show(void) = 0;
  virtual void Hide(void) = 0;

public:
  virtual ~ILayer(void) noexcept {}
};

} // namespace rocket

} // namespace covellite
