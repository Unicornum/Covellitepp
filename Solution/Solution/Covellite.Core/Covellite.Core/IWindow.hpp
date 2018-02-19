﻿
#pragma once

namespace std { template<class> class shared_ptr; }

namespace covellite
{

namespace core
{

class EventHandler;

/**
* \ingroup gCovelliteCoreClasses
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
*  Интерфейсный класс, предоставляющий классам окон доступ к событиям фреймворка.
* \todo
*  Неудачное название класса.
*  
* \version
*  1.0.0.0        \n
*  1.0.1.0        \n
*  1.1.0.0        \n
*  2.0.0.0        \n
*  2.1.0.0        \n
* \date
*  19 Ноябрь 2016    \n
*  09 Декабрь 2016    \n
*  18 Сентябрь 2017    \n
*  19 Сентябрь 2017    \n
*  24 Сентябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/
class IWindow
{
protected:
  using EventHandler_t = covellite::core::EventHandler;
  using EventHandlerPtr_t = ::std::shared_ptr<EventHandler_t>;

public:
  virtual void Subscribe(const EventHandlerPtr_t &) = 0;

public:
  virtual ~IWindow(void) noexcept {}
};

} // namespace core

} // namespace covellite
