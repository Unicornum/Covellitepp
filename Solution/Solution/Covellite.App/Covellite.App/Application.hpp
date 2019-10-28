
#pragma once
#include <memory>
#include <stack>
#include <functional>
#include <Covellite/Events/Events.hpp>
#include <Covellite/App/IApplication.hpp>

class Application_test;

namespace covellite
{

namespace app
{

class IWindow;

/**
* \ingroup CovelliteAppGroup
* \brief
*  Класс входит в проект \ref CovelliteAppPage \n
*  Базовый класс приложения для Android.
* \details
*  - Содержит главный цикл приложения.
*  - Для создания объекта приложения следует создать класс-наследник от этого
*  класса и в его конструкторе подписаться на сообщения, которые следует
*  обрабатывать.
*  - Для того, чтобы на Android при смене ориентации устройства программа 
*  не завершалась, необходимо в AndroidManifest.xml добавить
* \code

<activity ...
  android:configChanges="keyboardHidden|orientation|screenSize">

* \endcode
*
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  17 Ноябрь 2016    \n
*  20 Июнь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2018
*/
class Application :
  public IApplication
{
  using Windows_t = ::std::stack<::std::shared_ptr<IWindow>>;
  using Run_t = ::std::function<void(void)>;
  using CreateApp_t = ::std::function<::std::unique_ptr<Application>(void)>;

public:
  class Continuous {}; ///< Непрерывная отрисовка экрана (режим работы игры).
  class EventBased {}; ///< Отрисовка экрана после полученного события (режим 
                       ///< работы обычного приложения, реагирующего на 
                       ///< действия пользователя).

public:
  // Итерфейс IApplication:
  operator Events_t (void) const final { return m_Events; }

public:
  template<class>
  static void Main(void *) noexcept;
  ::std::string GetCommandLine(void) const;
  /// Объект функции запуска главного цикла программы.
  const Run_t Run;

protected:
  template<class TWindow, class ... TArgs>
  TWindow & MakeWindow(TArgs && ...);

private:
  static void Main(CreateApp_t, void *) noexcept;
  VIRTUAL_MOCK bool PostCommand(bool);

protected:
  events::Events  m_Events;

private:
  bool            m_IsFocused = false;
  Windows_t       m_Windows;

private:
  explicit Application(const Run_t &);

public:
  explicit Application(Continuous);
  explicit Application(EventBased);
  virtual ~Application(void) noexcept;

private:
  friend Application_test;
  FRIEND_TEST(Application_test, /*DISABLED_*/Test_MakeWindow);
  FRIEND_TEST(Application_android_test, /*DISABLED_*/Test_OnTermWindow);
  FRIEND_TEST(Application_android_test, /*DISABLED_*/Test_PostCommand);
};

} // namespace app

} // namespace covellite

#include <Covellite\App\Application.inl>
