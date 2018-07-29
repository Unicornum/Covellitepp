
#pragma once

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

namespace core
{

class Application
{
  using Run_t = ::std::function<void(void)>;
  using EventHandlerPtr_t = ::std::shared_ptr<::mock::covellite::core::EventHandler>;

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

protected:
  template<class TWindow, class ... TArgs>
  ::std::shared_ptr<TWindow> MakeWindow(const TArgs & ... _Args)
  {
    Proxy::GetInstance()->MakeWindow(m_Id, typeid(TWindow).name());
    return ::std::make_shared<TWindow>(_Args ...);
  }

public:
  EventHandlerPtr_t m_pEvents =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

public:
  explicit Application(Continuous) :
    m_Id(Proxy::GetInstance()->ContinuousConstructor()),
    Run([&](void) { Proxy::GetInstance()->Run(m_Id); })
  {

  }

  explicit Application(EventBased) :
    m_Id(Proxy::GetInstance()->EventBasedConstructor()),
    Run([&](void) { Proxy::GetInstance()->Run(m_Id); })
  {

  }

  virtual ~Application(void) = default;
};

} // namespace core

} // namespace covellite

} // namespace mock
