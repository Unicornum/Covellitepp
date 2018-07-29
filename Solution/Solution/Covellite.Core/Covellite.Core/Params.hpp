
#pragma once
#include <boost/any.hpp>

/**
* \defgroup gCovelliteCoreParamsClasses Params
* \ingroup gCovelliteCoreClasses
*  Группа классов параметров, передаваемых обработчикам событий.
*/

namespace covellite
{

namespace core
{

namespace params
{

/**
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
*  Класс-заглушка для использования совместно с сигналами без параметров.
* \details
*  - Используется только для вызова обработчиков событий без параметров,
*  в качестве функции-обработчика использовать функцию без параметров.
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
class Empty final
{
};

/**
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
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
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
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

/**
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
*  Параметры, передаваемые событиям нажатия/отпускания кнопок клавиатуры.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
* \date
*  23 Сентябрь 2017    \n
*  01 Ноябрь 2017    \n
*  04 Ноябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class KeyCode final
{
  using RocketKeyCode_t = Rocket::Core::Input::KeyIdentifier;

public:
  int32_t AsciiKeyCode; ///< Виртуальный код нажатой клавиши конкретной
                        ///< операционной системы.

public:
  RocketKeyCode_t ToRocketKeyCode(void) const;
};

/**
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
*  Параметры, передаваемые событию ошибки.
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
class Error final
{
public:
  ::std::string Description;
};

/**
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
*  Класс информации об элементе, вызвавшем появление события.
* \details
*  - \b ВАЖНО!!! Все значения приводятся к нижнему регистру.
*  
* \version
*  1.0.0.0        \n
* \date
*  11 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class Click final
{
public:
  ::std::string m_DocumentId; ///< Идентификатор родительского документа (слоя)
                              ///< элемента, для которого вызывается событие.  
  ::std::string m_Tag;        ///< Название xml тега элемента.
  ::std::string m_Type;       ///< Тип элемента (значение атрибута \b type).
};

} // namespace params

/**
* \ingroup gCovelliteCoreClasses
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
*  Вспомогательный класс для передачи параметров обработчикам событий.
* \details
*  - Нужен для того, чтобы у функций-обработчиков событий в качестве параметра
*  можно было указывать непосредственно один из классов
*  \ref gCovelliteCoreParamsClasses.
*
* \version
*  1.0.0.0        \n
*  1.0.1.0        \n
* \date
*  23 Сентябрь 2017    \n
*  13 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class Params final
{
  using Params_t = ::boost::any;

public:
  template<class T>
  inline operator const T & (void) const { return ::boost::any_cast<const T &>(m_Params); }

private:
  const Params_t m_Params;

public:
  template<class T>
  inline Params(const T & _Value) : m_Params(_Value) {}
};

} // namespace core

} // namespace covellite
