
#pragma once
#include <map>
#include <boost\signals2\signal.hpp>
#include <Covellite\Core\Params.hpp>
#include <Covellite\Core\Event.hpp>
#include <alicorn\platform\app-info.hpp>

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::core::EventHandler::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::core::EventHandler Example(Value1);

EXPECT_CALL(ExampleProxy, Function(ExampleId, Value2))
  .Times(1)
  .WillOnce(Return(Value3));

const auto Result = Example.Function(Value2);
EXPECT_EQ(Value3, Result);
*/

namespace mock
{

namespace covellite
{

namespace core
{

class EventHandler
{
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  using Command_t = int32_t;
  using Params_t = ::covellite::core::Params;
  using Event_t = ::covellite::core::Event;
  using Signal_t = ::boost::signals2::signal<void(const Params_t &)>;
  template<class TEventType>
  using OnEvent_t = ::std::map<TEventType, Signal_t>;
  using ElementId_t = ::std::string;
  using OnElement_t = OnEvent_t<ElementId_t>;
  using OnRocketEvent_t = ::std::map<Event_t::Rocket, OnElement_t>;

public:
  class Dummy {};

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD0(Constructor, Id_t(void));
    MOCK_METHOD0(DummyConstructor, Id_t(void));
    MOCK_METHOD1(PostCommandDrawWindow, void(Id_t));
    MOCK_METHOD2(Subscribe, void(Id_t, void *));
    MOCK_METHOD2(Unsubscribe, void(Id_t, void *));
  };

public:
  const Id_t m_Id = 0;
  EventHandler & operator= (const EventHandler &) = delete;
  bool operator== (const EventHandler & _Value) const { return (m_Id == _Value.m_Id); }

public:
  template<class TEventType>
  Signal_t & operator[] (TEventType _Command)
  { 
    return OnCommand[_Command]; 
  }

  void DoCommand(Command_t _Command, const Params_t & _Params) const
  {
    auto itCommand = OnCommand.find(_Command);
    if (itCommand == OnCommand.end()) return;

    itCommand->second(_Params);
  }

  OnElement_t & operator[] (Event_t::Rocket _Command)
  {
    return OnRocket[_Command];
  }

  template<Event_t::Rocket TEvent>
  void DoRocketCommand(ElementId_t _Command, const Params_t & _Params) const
  {
    auto itRocketCommand = OnRocket.find(TEvent);
    if (itRocketCommand == OnRocket.end()) return;

    auto itCommand = itRocketCommand->second.find(_Command);
    if (itCommand == itRocketCommand->second.end()) return;

    itCommand->second(_Params);
  }

  void PostCommandDrawWindow(void)
  {
    Proxy::GetInstance()->PostCommandDrawWindow(m_Id);

    auto & AndroidApp = AppInfo_t::Get<android_app>();

    if (AndroidApp.StepToDestroyRequest > 0)
    {
      AndroidApp.StepToDestroyRequest--;

      if (AndroidApp.StepToDestroyRequest == 0)
      {
        AndroidApp.destroyRequested = 1;
      }
    }
  }

public:
  template<class TSource>
  void Subscribe(TSource * _pSource)
  {
    Proxy::GetInstance()->Subscribe(m_Id, _pSource);
  }

  template<class TSource>
  void Unsubscribe(TSource * _pSource)
  {
    Proxy::GetInstance()->Unsubscribe(m_Id, _pSource);
  }

private:
  OnEvent_t<Command_t> OnCommand;
  OnRocketEvent_t      OnRocket;

public:
  EventHandler(void) :
    m_Id(Proxy::GetInstance()->Constructor())
  {

  }

  explicit EventHandler(Dummy) :
    m_Id(Proxy::GetInstance()->DummyConstructor())
  {

  }
};

} // namespace core

} // namespace covellite

} // namespace mock
