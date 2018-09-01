
#pragma once

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  Класс входит в проект \ref CovelliteAppPage \n
*  Класс событий уровня приложения.
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
class Application_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  Идентификаторы событий.
  */
  enum Id
  {
    Start = 0,  ///< Активируется при запуске программы (без параметров). \n
                ///< \b ВАЖНО! В отличие от Windows (где событие WM_CREATE 
                ///< приходит \b после создания окна), это событие служит 
                ///< указанием, что окно \b должно быть создано, поэтому его 
                ///< следует использовать для создания объектов окон в классе 
                ///< приложения.
    Update,     ///< Активируется при необходимости обновления состояния 
                ///< программы (без параметров).
    Exit,       ///< - Активируется при завершении работы программы 
                ///< (без параметров).
                ///< - Можно вызвать из любого места программы для ее 
                ///< завершения (без параметров).
  };
};

namespace { Application_t Application; }

} // namespace events

} // namespace covellite
