
#pragma once
#include <Covellite/Events/Events.hpp>
#include <Covellite/App/IApplication.hpp>
#include <Covellite/App/IWindow.hpp>

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::Application::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::Application Example(Value1);

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

namespace app
{

class Application :
  public ::covellite::app::IApplication
{
  using Events_t = ::covellite::events::Events;
  using Run_t = ::std::function<void(void)>;
  using Windows_t = ::std::stack<::std::shared_ptr<::covellite::app::IWindow>>;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD0(ContinuousConstructor, Id_t(void));
    MOCK_METHOD0(EventBasedConstructor, Id_t(void));
    MOCK_METHOD2(Main, void(::std::string, void *));
    MOCK_METHOD1(Run, void(Id_t));
    MOCK_METHOD2(MakeWindow, void(Id_t, ::std::string));
  };

public:
  const Id_t m_Id = 0;
  Application & operator= (const Application &) = delete;
  bool operator== (const Application & _Value) const { return (m_Id == _Value.m_Id); }

public:
  class Continuous {};
  class EventBased {};

public:
  template<class TApplication>
  static void Main(void * _pParams) noexcept
  {
    Proxy::GetInstance()->Main(typeid(TApplication).name(), _pParams);
  }

  const Run_t Run;

public:
  operator Events_t (void) const final { return m_Events; }

protected:
  template<class TWindow, class ... TArgs>
  TWindow & MakeWindow(TArgs && ... _Args)
  {
    Proxy::GetInstance()->MakeWindow(m_Id, typeid(TWindow).name());

    auto pWindow = ::std::make_shared<TWindow>(_Args ...);
    m_Windows.push(pWindow);
    return *pWindow;
  }

protected:
  Events_t  m_Events;
  Windows_t m_Windows;

public:
  explicit Application(Continuous) :
    m_Id(Proxy::GetInstance()->ContinuousConstructor()),
    Run([=](void) { Proxy::GetInstance()->Run(m_Id); })
  {

  }

  explicit Application(EventBased) :
    m_Id(Proxy::GetInstance()->EventBasedConstructor()),
    Run([=](void) { Proxy::GetInstance()->Run(m_Id); })
  {

  }

  virtual ~Application(void) = default;
};

} // namespace app

} // namespace covellite

} // namespace mock
