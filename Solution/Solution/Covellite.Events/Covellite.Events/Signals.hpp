
#pragma once
#include <map>
#include <vector>
#include <boost/signals2.hpp>
#include <Covellite/Events/Events.hpp>

namespace covellite
{

namespace events
{

class Params;

/**
* \ingroup CovelliteEventsGroup
* \brief
*  Класс входит в проект \ref CovelliteEventsPage \n
*  Вспомогательный класс, хранящий список всех сигналов.
*  
* \version
*  1.0.0.0        \n
* \date
*  18 Июнь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Events::Signals
{
  using Signal_t = ::boost::signals2::signal<void(const Params &)>;
  using SignalsId_t = ::std::map<size_t, Signal_t>;
  using SignalsType_t = ::std::map<size_t, SignalsId_t>;

public:
  SignalsType_t m_Signals;
};

} // namespace events

} // namespace covellite
