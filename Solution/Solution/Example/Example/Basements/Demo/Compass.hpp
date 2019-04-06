
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
*  Класс объекта компаса.
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
class Compass final :
  public GameObject
{
public:
  Object_t GetObject(const Any_t &) const override;

private:
  Compass(void);
  friend GameObject;
};

} // namespace model

} // namespace basement
