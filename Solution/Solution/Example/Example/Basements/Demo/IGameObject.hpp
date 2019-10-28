
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
  class Support final
  {
  public:
    enum Value
    {
      Skybox = 0, // Перед камерой основной сцены!
      Camera,

      Invalid,
    };
  };

  class Landscape final
  {
  public:
    enum Value
    {
      // Объекты поверхности, не содержащие прозрачных пикселей
      None = Support::Invalid, // После камеры!
      Sand,
      Rock,
      Well,

      // Объекты поверхности, содержащие прозрачные пиксели
      Grass,
      Bush,
      Tree,

      Invalid,
    };
  };

  class Extra final
  {
  public:
    enum Value
    {
      Water = Landscape::Invalid, // Вода - после объектов поверхности!
      Loader,  // Должен рендерится последним, т.к. его updater удаляет
               // объекты, которые еще находятся в списке рендеринга.

      Invalid,
    };
  };

  class Another final
  {
  public:
    enum Value
    {
      Animated = Extra::Invalid,
      Particles,
      Shadows,

      Invalid,
    };
  };

public:
  /**
  * \brief
  *  Функция получения типа игрового объекта.
  */
  virtual size_t GetType(void) const = 0;

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
