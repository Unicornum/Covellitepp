
#pragma once
#include <Covellite/Covellite.hpp>
#include "Common.hpp"

namespace basement
{

/**
* \ingroup ExampleBasementGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс тестирования скорости рендеринга при использовании Covellite.Expanse.
*
* \version
*  1.0.0.0        \n
* \date
*  23 Июнь 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2020
*/
class Empty final :
  public Common
{
  using WindowExpanse_t = ::covellite::expanse::IWindow;

private:
  ::std::vector<ObjectId_t> m_Objects;

public:
  explicit Empty(WindowExpanse_t &);
  Empty(const Empty &) = delete;
  Empty(Empty &&) = delete;
  const Empty & operator= (const Empty &) = delete;
  Empty & operator= (Empty &&) = delete;
  ~Empty(void) = default;
};

} // namespace basement
