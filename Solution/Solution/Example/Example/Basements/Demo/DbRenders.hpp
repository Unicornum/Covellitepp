
#pragma once
#include "Defines.hpp"
#include "IDbRenders.hpp"

namespace basement
{

namespace view
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс реализации базы данных рендеров.
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
class DbRenders final :
  public IDbRenders
{
public:
  // Интерфейс IDbRenders:
  void AddObject(const Id_t, const Object_t &) override;
  void RemoveObject(const Id_t, const Object_t &) override;

public:
  const Renders_t & GetRenders(const Id_t) const;

private:
  const RendersPtr_t       m_pRenders;
  ::std::vector<Renders_t> m_Renders;

public:
  explicit DbRenders(const RendersPtr_t &);
  ~DbRenders(void) noexcept;
};

} // namespace view

} // namespace basement
