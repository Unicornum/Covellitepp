
#pragma once
#include "Defines.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Интерфейсный класс игрового объекта.
*
* \version
*  1.0.0.0        \n
* \date
*  28 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
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
  *  Функция получения типа игрового объекта.
  */
  virtual Type::Value GetType(void) const = 0;

  /**
  * \brief
  *  Функция получения уникального набора компонентов игрового объекта.
  */
  virtual Object_t GetObject(const Any_t & = {}) const = 0;

public:
  virtual ~IGameObject(void) = default;
};

} // namespace model

} // namespace basement
