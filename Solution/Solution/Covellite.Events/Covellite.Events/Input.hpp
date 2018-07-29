
#pragma once
#include <Covellite/Events/Type.hpp>
#include <Covellite/Events/Params.hpp>

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsGroup
* \brief
*  Класс входит в проект \ref CovelliteEventsPage \n
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
class Input_t final :
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
    Motion = 0,   ///< Сигнал вызывается при движении мыши по окну/пальца по 
                  ///< экрану (параметр - params::Motion).
    Touch,        ///< Сигнал вызывается при при нажатии левой кнопки мыши/
                  ///< касании пальца экрана (без параметров).
    Release,      ///< Сигнал вызывается при отпускании левой кнопки мыши/
                  ///< отрыве пальца от экрана (без параметров).
    KeyPressed,   ///< Сигнал вызывается при нажатии кнопки на клавиатуре;
                  ///< используется для передачи Unicode кода символа нажатой
                  ///< кнопки текстовым полям ввода (параметр - 
                  ///< params::KeyPressed).
    KeyDown,      ///< Сигнал вызывается при нажатии кнопки на клавиатуре;
                  ///< используется для перехвата виртуального кода кнопки
                  ///< (параметр - int32_t).
    KeyUp,        ///< Сигнал вызывается при отпускании кнопки на клавиатуре;
                  ///< используется для перехвата виртуального кода кнопки
                  ///< (параметр - int32_t).
    Back,         ///< Сигнал перехода к предыдущему слою. Генерируется при 
                  ///< нажатии Alt + Left в Windows и Back (нижней правой) в
                  ///< Android (без параметров).
  };
};

namespace
{

Input_t Input;

} // namespace unnamed

namespace params
{

/**
* \ingroup CovelliteEventsParamsGroup
* \brief
*  Класс входит в проект \ref CovelliteEventsPage \n
*  Параметры, передаваемые событию движения пальца по экрану.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  23 Сентябрь 2017    \n
*  24 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class Motion final
{
public:
  int32_t X;      ///< X координата касания в пикселях.
  int32_t Y;      ///< Y координата касания в пикселях.
};

/**
* \ingroup CovelliteEventsParamsGroup
* \brief
*  Класс входит в проект \ref CovelliteEventsPage \n
*  Параметры, передаваемые событию нажатия кнопки клавиатуры.
*
* \version
*  1.0.0.0        \n
* \date
*  23 Сентябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class KeyPressed final
{
public:
  int32_t UnicodeKeyCode;
};

} // namespace params

} // namespace events

} // namespace covellite
