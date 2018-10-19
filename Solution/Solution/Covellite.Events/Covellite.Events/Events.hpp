
#pragma once
#include <vector>
#include <memory>

namespace boost { namespace signals2 { class connection; } }

namespace covellite
{

namespace events
{

class Params;

/**
* \ingroup CovelliteEventsGroup
* \brief
*  Класс входит в проект \ref CovelliteEventsPage \n
*  Класс для работы с событиями фреймворка.
* \details
*  - Подробнее про работу с событиями фреймворка см. \ref CovelliteEventsGroup.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  17 Июнь 2018    \n
*  18 Июнь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Events final
{
private:
  class Signal;
  class Signals;
  using SignalsPtr_t = ::std::shared_ptr<Signals>;
  using Connections_t = ::std::vector<::boost::signals2::connection>;

public:
  template<class TType>
  Signal operator[] (const TType &);

private:
  SignalsPtr_t  m_pSignals;
  Connections_t m_Connections;

public:
  Events(void);
  Events(const Events &);

public:
  ~Events(void);
};

} // namespace events

} // namespace covellite
