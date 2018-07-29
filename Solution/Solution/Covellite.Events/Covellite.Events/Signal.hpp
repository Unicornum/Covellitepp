
#pragma once
#include <boost/signals2.hpp>
#include <Covellite/Events/Events.hpp>

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsGroup
* \brief
*  Класс входит в проект \ref CovelliteEventsPage \n
*  Вспомогательный класс для доступа к сигналу конкретного события.
* \details
*  - Подробнее про работу с событиями фреймворка см. \ref CovelliteEventsGroup.
*
* \version
*  1.0.0.0        \n
* \date
*  17 Июнь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Events::Signal final
{
  using Signal_t = ::boost::signals2::signal<void(const Params &)>;
  using FnParams_t = ::std::function<void(const Params &)>;
  using FnVoid_t = ::std::function<void(void)>;

public:
  void Connect(const FnVoid_t &);
  void Connect(const FnParams_t &);
  bool operator() (void);
  bool operator() (const Params &);

private:
  bool Activate(const Params &);

private:
  Events & m_Events;
  Signal_t & m_Signal;

public:
  Signal(Events &, size_t, size_t);
};

} // namespace events

} // namespace covellite
