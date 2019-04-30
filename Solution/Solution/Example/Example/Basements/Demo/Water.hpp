
#pragma once
#include "GameObject.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс объекта отрисовки воды.
*
* \version
*  1.0.0.0        \n
* \date
*  30 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Water final :
  public GameObject
{
public:
  Objects_t GetObject(const Any_t &) const override;

private:
  Water(void);
  friend GameObject;
};

} // namespace model

} // namespace basement
