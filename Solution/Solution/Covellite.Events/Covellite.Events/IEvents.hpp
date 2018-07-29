
#pragma once

namespace covellite
{

namespace events
{

class Events;

/**
* \ingroup CovelliteEventsGroup
* \brief
*  Класс входит в проект \ref CovelliteEventsPage \n
*  Интерфейсный класс для классов, которые должны использовать события 
*  фреймворка.
* \details
*  - Предназначен для наследования интерфесными классами, которые будут 
*  использоваться для передачи объекта событий фреймворка далее по иерархии 
*  классов.
*  
* \version
*  1.0.0.0        \n
* \date
*  17 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class IEvents
{
protected:
  using Events_t = ::covellite::events::Events;

public:
  virtual operator Events_t (void) const = 0;

public:
  virtual ~IEvents(void) = default;
};

} // namespace events

} // namespace covellite
