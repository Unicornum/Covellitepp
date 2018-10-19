
#pragma once
#include <Covellite/Predefined.hpp>

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsParamsGroup
* \brief
*  Класс входит в проект \ref CovelliteEventsPage \n
*  Вспомогательный класс для передачи параметров обработчикам событий.
* \details
*  - Нужен для того, чтобы у функций-обработчиков событий в качестве параметра
*  можно было указывать непосредственно один из классов
*  \ref CovelliteEventsParamsGroup.
*
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  23 Сентябрь 2017    \n
*  29 Июнь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017 - 2018
*/
class Params final
{
public:
  template<class T>
  inline operator const T & (void) const 
  { 
    return ::covellite::any_cast<const T &>(m_Params);
  }

private:
  const Any_t m_Params;

public:
  template<class T>
  inline Params(const T & _Value) : m_Params(_Value) {}
};

} // namespace events

} // namespace covellite
