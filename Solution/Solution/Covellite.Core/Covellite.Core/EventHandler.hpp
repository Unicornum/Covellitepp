
#pragma once
#include <map>
#include <boost\signals2\signal.hpp>
#include <boost\noncopyable.hpp>
#include <Covellite\Core\Event.hpp>

class EventHandler_test;

namespace covellite
{

namespace core
{

class Params;
class Message;

/**
* \ingroup CovelliteCoreGroup
* \brief
*  Класс входит в проект \ref CovelliteCorePage \n
*  Класс обработчика сообщений операционной системы.
* \details
*  - Преобразует события конкретной операционной системы в универсальные 
*  события фреймворка.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
*  1.3.0.0        \n
*  1.4.0.0        \n
* \date
*  18 Сентябрь 2017    \n
*  25 Сентябрь 2017    \n
*  09 Октябрь 2017    \n
*  13 Октябрь 2017    \n
*  24 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class EventHandler final :
  ::boost::noncopyable
{
  using Command_t = int32_t;
  using Signal_t = ::boost::signals2::signal<void(const Params &)>;
  template<class TEventType>
  using OnEvent_t = ::std::map<TEventType, Signal_t>;
  using ElementId_t = ::std::string;
  using OnElement_t = OnEvent_t<ElementId_t>;
  using OnRocketEvent_t = ::std::map<Event::Rocket, OnElement_t>;

public:
  template<class TEventType>
  Signal_t & operator[] (TEventType) = delete;
  OnElement_t & operator[] (Event::Rocket);
  template<class TEventType>
  bool DoCommand(TEventType, const Params &);
  void PostCommandDrawWindow(void);

public:
  template<class TSource>
  void Subscribe(TSource *);
  template<class TSource>
  void Unsubscribe(TSource *);

private:
  void OnDestroy(void);

private:
  OnEvent_t<Event::System>    OnSystem;
  OnEvent_t<Event::Window>    OnWindow;
  OnEvent_t<Event::Input>     OnInput;
  OnEvent_t<Event::Framework> OnFramework;
  OnRocketEvent_t             OnRocket;

private:
  class ClickEventListener;
  using ClickEventListenerPtr_t = ::std::unique_ptr<ClickEventListener>;
  ClickEventListenerPtr_t     m_pClickEventListener;
  ::std::function<void(void)> m_fnCleaner;
  bool                        m_IsPaused = false;

public:
  class Dummy {};
  EventHandler(void);
  explicit EventHandler(Dummy);
  ~EventHandler(void) noexcept;

private:
  friend EventHandler_test;
  FRIEND_TEST(EventHandler_windows_test, /*DISABLED_*/Test_OnDestroy);
  FRIEND_TEST(EventHandler_android_test, /*DISABLED_*/Test_OnDestroy);
};

} // namespace core

} // namespace covellite

#include <Covellite\Core\EventHandler.inl>
