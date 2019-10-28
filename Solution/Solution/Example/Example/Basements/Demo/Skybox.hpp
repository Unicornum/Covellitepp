
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
*  Класс скайбокса.
*
* \version
*  1.0.0.0        \n
* \date
*  29 Апрель 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Skybox final :
  public GameObject
{
public:
  Objects_t GetObject(const Any_t &) const override;

private:
  const bool m_IsNightMode;

private:
  Skybox(void);
  friend GameObject;
};

} // namespace model

} // namespace basement
