
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
*  Класс загрузчика сцены.
*
* \version
*  1.0.0.0        \n
* \date
*  31 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Loader final :
  public GameObject
{
public:
  Object_t GetObject(const Any_t &) const override;

private:
  Loader(void);
  friend GameObject;
};

} // namespace model

} // namespace basement
