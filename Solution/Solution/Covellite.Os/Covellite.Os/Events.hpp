
#pragma once

namespace covellite
{

namespace events
{

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  Класс входит в проект \ref CovelliteOsPage \n
*  Класс событий окна операционной системы.
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  06 Июль 2018    \n
*  07 Февраль 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018 - 2019
*/
class Window_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  Идентификаторы событий.
  */
  enum Id
  {
    Activate = 0, ///< Активация окна программы; например, при разворачивании
                  ///< из панели задач или возвращении на первый план после
                  ///< перекрытия его другим окном (без параметров).
    Deactivate,   ///< Деактивация окна программы; например, при сворачивании
                  ///< окна в панель задач или перекрытии другим окном 
                  ///< (без параметров).
    Resize,       ///< Изменение размеров окна (без параметров):
                  ///< \b Windows:
                  ///< - При разворачивании на весь экран.
                  ///< - Возвращении в исходный размер после разворачивания
                  ///< на весь экран.
                  ///< - Возвращении в исходный размер после сворачивания
                  ///< в панель задач.
                  ///< - В конце перемещения края окна при изменении его 
                  ///< размеров.
                  ///< \b Android:
                  ///< - Поворот устройства.
  };
};

namespace { Window_t Window; }

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  Класс входит в проект \ref CovelliteOsPage \n
*  Класс событий курсора.
*
* \version
*  1.0.0.0        \n
* \date
*  06 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Cursor_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  Идентификаторы событий.
  */
  enum Id
  {
    Motion = 0, ///< Перемещение курсора (параметр - Position, содержащий новые 
                ///< координаты курсора).
    Touch,      ///< Нажатие курсора; в Windows - нажатие левой кнопки мыши,
                ///< в Android - касание экрана (без параметров).
    Release,    ///< Отпускание курсора (без параметров).
  };

public:
  /**
  * \ingroup CovelliteEventsParamsGroup
  * \brief
  *  Класс входит в проект \ref CovelliteOsPage \n
  *  Класс параметра для передачи сигналу координат курсора.
  */
  class Position final
  {
  public:
    const int32_t X;
    const int32_t Y;

  public:
    Position(int32_t _X, int32_t _Y) : X(_X), Y(_Y) {}
  };
};

namespace { Cursor_t Cursor; }

/**
* \ingroup CovelliteEventsTypeGroup
* \brief
*  Класс входит в проект \ref CovelliteOsPage \n
*  Класс событий кнопок клавиатуры.
*
* \version
*  1.0.0.0        \n
* \date
*  07 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Key_t final
{
public:
  /**
  * \ingroup CovelliteEventsIdGroup
  * \brief
  *  Идентификаторы событий.
  */
  enum Id
  {
    Pressed = 0,  ///< Нажатие символьной клавиши клавиатуры, сообщение будет
                  ///< повторятся при нажатии и удерживании клавиши (параметр - 
                  ///< Key_t::Code, содержащий UNICODE код нажатой клавиши).
    Down,         ///< Нажатие клавиши клавиатуры, сообщение генерируется 
                  ///< \b один раз при нажатии клавиши, повторов при удерживании
                  ///< не будет (параметр - Key_t::Code, содержащий ASCII код 
                  ///< нажатой клавиши).
    Up,           ///< Отпускание клавиши клавиатуры (параметр - Key_t::Code, 
                  ///< содержащий ASCII код нажатой клавиши).
    Menu,         ///< Нажатие кнопки меню программы; ALT + ПРОБЕЛ в Windows,
                  ///< левая нижняя кнопка в Android,  (без параметров).
    Back,         ///< Нажатие кнопки возврата к предыдущему слою; 
                  ///< ALT + <- в Windows, правая нижняя кнопка в Android
                  ///< (без параметров).
  };

public:
  /**
  * \ingroup CovelliteEventsParamsGroup
  * \brief
  *  Класс входит в проект \ref CovelliteOsPage \n
  *  Класс параметра для передачи сигналу координат курсора.
  */
  using Code = int32_t;

public:
  // 32 - это код, который генерирует java код при нажатии кнопки на 
  // клавиатуре (понадобилось из-за того, что в нативном коде невозможно 
  // получить код нажатой клавиши за пределами ASCII кодировки).
  static const int32_t APP_CMD_KEY_PRESSED = 32;
};

namespace { Key_t Key; }

} // namespace events

} // namespace covellite
