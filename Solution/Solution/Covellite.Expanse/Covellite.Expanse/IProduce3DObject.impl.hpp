
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
*  Интерфейсный класс создания/удаления 3D объектов.
*
* \version
*  1.0.0.0        \n
* \date
*  19 Июнь 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2020
*/
class IProduce3DObject
{
public:
  /**
  * \brief
  *  Функция добавления 3D объекта в 3D сцену.
  * \details
  *  Подробнее см. в описании класса C3DScene.
  */
  virtual void Add(const ObjectId_t, const C3DObject_t &) = 0;

  /**
  * \brief
  *  Функция удаления 3D объекта из 3D cцены.
  * \details
  *  Подробнее см. в описании класса C3DScene.
  */
  virtual void Remove(const ObjectId_t) = 0;

public:
  IProduce3DObject(void) = default;
  IProduce3DObject(const IProduce3DObject &) = delete;
  IProduce3DObject(IProduce3DObject &&) = delete;
  IProduce3DObject & operator= (const IProduce3DObject &) = delete;
  IProduce3DObject & operator= (IProduce3DObject &&) = delete;
  virtual ~IProduce3DObject(void) = default;
};

} // namespace expanse

} // namespace covellite
