
#pragma once
#include "IDbComponents.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс реализации базы данных компонентов.
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
class DbComponents final :
  public IDbComponents
{
  using String_t = ::alicorn::extension::std::String;

public:
  // Интерфейс IDbComponents
  const Object_t & GetObject(const Id_t) const override;
  ComponentPtr_t GetComponent(const String_t &) const override;

public:
  Id_t AddGameObject(const Object_t &);
  void RemoveGameObject(const Id_t);

private:
  class Id;

private:
  ::std::vector<Object_t> m_Objects;
  ::std::shared_ptr<Id>   m_pId;

public:
  DbComponents(void);
  ~DbComponents(void) noexcept;
};

} // namespace model

} // namespace basement
