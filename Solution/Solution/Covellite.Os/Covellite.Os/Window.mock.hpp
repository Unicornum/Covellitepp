
#pragma once
#include <Covellite\Core\IWindow.hpp>
#include <Covellite\Os\Configuration.mock.hpp>
#include <Covellite\Os\IWindow.hpp>
#include <Covellite\Core\EventHandler.hpp>
#include <Covellite\Events.hpp>

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::Window::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::Window Example(Value1);

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

namespace os
{

class Window :
  public ::covellite::core::IWindow,
  public ::covellite::os::IWindow
{
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  using Configuration_t = ::mock::covellite::os::Configuration;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD0(Constructor, Id_t(void));
    MOCK_METHOD2(Subscribe, void(Id_t, EventHandlerPtr_t));
    MOCK_METHOD1(GetHandle, Handle_t(Id_t));
    MOCK_METHOD1(GetClientRect, Rect(Id_t));
  };

public:
  const Id_t m_Id = 0;
  Window & operator= (const Window &) = delete;
  bool operator== (const Window & _Value) const { return (m_Id == _Value.m_Id); }

public:
  void Subscribe(const EventHandlerPtr_t & _pEvents) override
  {
    Proxy::GetInstance()->Subscribe(m_Id, _pEvents);
  }

public:
  // Èםעונפויס events::IEvents:
  operator Events_t (void) const override
  {
    return m_Events;
  }

public:
  Handle_t Window::GetHandle(void) const override
  {
    return Proxy::GetInstance()->GetHandle(m_Id);
  }

  Rect GetClientRect(void) const override
  {
    return Proxy::GetInstance()->GetClientRect(m_Id);
  }

  const Configuration_t & GetConfiguration(void) const override
  {
    throw::std::exception{};
  }

private:
  Events_t m_Events;

public:
  Window(void) :
    m_Id(Proxy::GetInstance()->Constructor())
  {
  }
};

} // namespace os

} // namespace covellite

} // namespace mock
