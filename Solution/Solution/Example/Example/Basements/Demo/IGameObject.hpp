
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
protected:
  using Objects_t = ::std::vector<Object_t>;

public:
  class Type final
  {
  public:
    enum Value
    {
      Skybox = 0x00,
      Camera = 0x08,

      // Объекты поверхности, не содержащие прозрачных пикселей
      None = 0x09,
      Sand,
      Rock,
      Well,

      Water = 0x10, // Вода - особый случай - прозрачная      

      // Объекты поверхности, содержащие прозрачные пиксели
      Grass = 0x11,
      Bush,
      Tree,

      // Маска объектов поверхности
      LandscapeMask = 0x07,
      
      Compass= 0x18, // Должен рендерится гарантированно последним
      Loader = 0x20,
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
  virtual Objects_t GetObject(const Any_t & = {}) const = 0;

public:
  virtual ~IGameObject(void) = default;
};

} // namespace model

} // namespace basement
