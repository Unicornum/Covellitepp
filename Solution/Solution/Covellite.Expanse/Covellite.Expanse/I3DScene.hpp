
#pragma once
#include <Covellite/Expanse/Defines.hpp>

namespace covellite
{

namespace expanse
{

/**
* \ingroup CovelliteExpanseGroup
* \brief
*  Класс входит в проект \ref CovelliteExpansePage \n
*  Интерфейс класса 3D сцены.
*  
* \version
*  1.0.0.0        \n
* \date
*  18 Июнь 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2020
*/
class I3DScene
{
public:
  /**
  * \brief
  *  Функция добавления идентификатора объекта в очередь рендеринга.
  * \details
  *  Подробнее см. в описании класса C3DScene.
  */
  virtual void Add(const size_t, const ObjectId_t, const size_t = 0) = 0;

  /**
  * \brief
  *  Функция добавления идентификатора объекта в очередь рендеринга.
  * \details
  *  Подробнее см. в описании класса C3DScene.
  */
  virtual void Add(const ObjectId_t) = 0;

public:
  virtual ~I3DScene(void) = default;
};

} // namespace expanse

} // namespace covellite
