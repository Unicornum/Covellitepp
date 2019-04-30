
#pragma once
#include "GameObject.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� ������� ������ �����.
*
* \version
*  1.0.0.0        \n
* \date
*  28 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class Camera final :
  public GameObject
{
public:
  Objects_t GetObject(const Any_t &) const override;

private:
  Camera(void);
  friend GameObject;
};

} // namespace model

} // namespace basement
