
#pragma once
#include <alicorn/std/string.forward.hpp>
#include "IDbComponents.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс реализации базы данных updater'ов игровых объектов.
*
* \version
*  1.0.0.0        \n
* \date
*  25 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class DbUpdaters final
{
public:
  void AddUpdater(const Id_t &, const Updater_t &);
  void RemoveUpdater(const Id_t);
  void CallUpdater(const Id_t) const;

private:
  ::std::map<Id_t, Updater_t> m_Objects;

public:
  DbUpdaters(void);
  ~DbUpdaters(void);
};

} // namespace model

} // namespace basement
