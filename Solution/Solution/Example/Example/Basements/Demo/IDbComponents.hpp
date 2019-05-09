
#pragma once
#include <boost/signals2.hpp>
#include <alicorn/std/string.forward.hpp>
#include "Defines.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Интерфейсный класс базы данных компонентов.
*
* \version
*  1.0.0.0        \n
* \date
*  10 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class IDbComponents
{
protected:
  using ComponentId_t = ::alicorn::extension::std::String;

public:
  /**
  * \brief
  *  Функция получения игрового объекта по его иденитфикатору.
  */
  virtual const Object_t & GetObject(const Id_t) const = 0;

  /**
  * \brief
  *  Функция получения объекта компонента по его идентификатору.
  */
  virtual ComponentPtr_t GetComponent(const ComponentId_t &) const = 0;

public:
  /**
  * \brief
  *  Объект сигнала, оповещающего о добавлении игрового объекта.
  */
  ::boost::signals2::signal<void(Id_t)> DoAddObject;

  /**
  * \brief
  *  Объект сигнала, оповещающего об удалении игрового объекта.
  */
  ::boost::signals2::signal<void(Id_t)> DoRemoveObject;

public:
  virtual ~IDbComponents(void) = default;
};

} // namespace model

} // namespace basement
