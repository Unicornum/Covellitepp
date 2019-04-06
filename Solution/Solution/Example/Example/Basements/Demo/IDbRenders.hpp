
#pragma once
#include "Defines.hpp"

namespace basement
{

namespace view
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ������������ ����� ���� ������ ��������.
*
* \version
*  1.0.0.0        \n
* \date
*  10 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class IDbRenders
{
public:
  virtual void AddObject(const Id_t, const Object_t &) = 0;
  virtual void RemoveObject(const Id_t, const Object_t &) = 0;

public:
  virtual ~IDbRenders(void) = default;
};

} // namespace view

} // namespace basement
