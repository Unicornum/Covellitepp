
#pragma once

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsGroup
* \brief
*  Класс входит в проект \ref CovelliteAppPage \n
*  Класс событий отрисовки окна программы.
*  
* \version
*  1.0.0.0        \n
* \date
*  21 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Drawing_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  Идентификаторы событий.
  */
  enum Id
  {
    Do, ///< Активируется при необходимости отрисовки окна программы 
        ///< (без параметров).
  };
};

namespace { Drawing_t Drawing; }

} // namespace events

} // namespace covellite
