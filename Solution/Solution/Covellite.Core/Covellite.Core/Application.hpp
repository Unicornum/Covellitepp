
#pragma once
#include <memory>
#include <stack>
#include <functional>

class Application_test;

namespace covellite
{

namespace core
{

class IWindow;
class EventHandler;

/**
* \ingroup gCovelliteCoreClasses
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
*  Базовый класс приложения для Android.
* \details
*  - Содержит главный цикл приложения.
*  - Для создания объекта приложения следует создать класс-наследник от этого
*  класса и в его конструкторе подписаться на сообщения, которые следует
*  обрабатывать.
*  - Для того, чтобы при смене ориентации устройства программа не завершалась,
*  необходимо в AndroidManifest.xml добавить
* \code

<activity ...
  android:configChanges="keyboardHidden|orientation|screenSize">

* \endcode
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
*  1.3.0.0        \n
*  1.4.0.0        \n
*  1.4.1.0        \n
*  1.5.0.0        \n
* \date
*  17 Ноябрь 2016    \n
*  26 Декабрь 2016    \n
*  18 Сентябрь 2017    \n
*  26 Сентябрь 2017    \n
*  01 Октябрь 2017    \n
*  13 Октябрь 2017    \n
*  24 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/
class Application
{
  using Windows_t = ::std::stack<::std::shared_ptr<IWindow>>;
  using EventHandlerPtr_t = ::std::shared_ptr<covellite::core::EventHandler>;
  using Run_t = ::std::function<void(void)>;
  using CreateApp_t = ::std::function<::std::unique_ptr<Application>(void)>;

public:
  class Continuous {}; ///< Непрерывная отрисовка экрана (режим работы игры).
  class EventBased {}; ///< Отрисовка экрана после полученного события (режим 
                       ///< работы обычного приложения, реагирующего на 
                       ///< действия пользователя).

public:
  template<class>
  static void Main(void *) noexcept;
  ::std::string GetCommandLine(void) const;
  /// Объект функции запуска главного цикла программы.
  const Run_t Run;

protected:
  template<class TWindow, class ... TArgs>
  ::std::shared_ptr<TWindow> MakeWindow(TArgs && ...);

private:
  static void Main(CreateApp_t, void *) noexcept;
  VIRTUAL_MOCK bool PostCommand(bool) const;

protected:
  EventHandlerPtr_t m_pEvents;

private:
  bool      m_IsFocused = false;
  Windows_t m_Windows;

private:
  explicit Application(const Run_t &);

public:
  explicit Application(Continuous);
  explicit Application(EventBased);
  virtual ~Application(void) = default;

private:
  friend Application_test;
  FRIEND_TEST(Application_test, /*DISABLED_*/Test_MakeWindow);
  FRIEND_TEST(Application_android_test, /*DISABLED_*/Test_PostCommand);
};

} // namespace core

} // namespace covellite

#include <Covellite\Core\Application.inl>
