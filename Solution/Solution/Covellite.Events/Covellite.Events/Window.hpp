
#pragma once
#include <Covellite/Events/Type.hpp>

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsGroup
* \brief
*  Класс входит в проект \ref CovelliteEventsPage \n
*  Класс событий уровня окна приложения.
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
class Window_t final :
  public Type
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  Идентификаторы событий.
  */
  enum Id
  {
    LostFocus = 0,  ///< Приложение потеряло фокус ввода (без параметров).
    GainedFocus,    ///< Приложение получило фокус ввода (без параметров).
    Resize,         ///< Изменение размеров окна программы, например,
                    ///< при повороте Android устройства (без параметров).
    Drawing,        ///< Сигнал вызывается при необходимости перерисовки окна.
                    ///< В зависимости от способа запуска программы 
                    ///< (см. Application::Run()), будет вызываться непрерывно,
                    ///< либо только после получения программой сообщения
                    ///< (без параметров).
    StartDrawing,   ///< Сигнал вызывается непосредственно перед событием 
                    ///< Drawing (без параметров).
    FinishDrawing,  ///< Сигнал вызывается сразу после события Drawing
                    ///< (без параметров).
    Close,          ///< Закрытие окна приложения (без параметров).
  };
};

namespace
{

Window_t Window;

} // namespace unnamed

} // namespace events

} // namespace covellite
