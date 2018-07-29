
#pragma once
#include <string>

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsGroup
* \brief
*  Класс входит в проект \ref CovelliteEventsPage \n
*  Класс событий ошибок во время работы приложения.
*
* \version
*  1.0.0.0        \n
* \date
*  16 Июнь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Error_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  Идентификаторы событий.
  */
  enum Id
  {
    Exception = 0,  ///< При вызове обработчика события было перехвачено
                    ///< исключение (параметр - Description, описание исключения).
  };

public:
  /**
  * \ingroup CovelliteEventsParamsGroup
  * \brief
  *  Класс входит в проект \ref CovelliteOsPage \n
  *  Класс параметра для передачи сигналу описания ошибки.
  */
  using Description = ::std::string;
};

namespace { Error_t Error; }

} // namespace events

} // namespace covellite
