
#pragma once
#include "Defines.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ������������ ����� �������� �������.
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
class IGameObject
{
public:
  class Type final
  {
  public:
    enum Value
    {
      Camera = 0,
      Loader,
      None,
      Tree,
      Bush,
      Rock,
      Grass,
      Sand,
      Water,
      Compass,
    };
  };

public:
  /**
  * \brief
  *  ������� ��������� ���� �������� �������.
  */
  virtual Type::Value GetType(void) const = 0;

  /**
  * \brief
  *  ������� ��������� ����������� ������ ����������� �������� �������.
  */
  virtual Object_t GetObject(const Any_t & = {}) const = 0;

public:
  virtual ~IGameObject(void) = default;
};

} // namespace model

} // namespace basement
