
#pragma once
#include <Covellite/Predefined.hpp>
#include <Covellite/Events.hpp>
#include <Covellite/App/IApplication.hpp>
#include <Covellite/App/IWindow.hpp>
#include <Covellite/Os/Configuration.mock.hpp>
#include <Covellite/Os/IWindow.hpp>

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
  public ::covellite::app::IWindow,
  public ::covellite::os::IWindow
{
  using Any_t = ::covellite::Any_t;
  using Configuration_t = ::mock::covellite::os::Configuration;
  using IApplication_t = ::covellite::app::IApplication;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Constructor, Id_t(const IApplication_t *));
    MOCK_METHOD1(GetHandle, Any_t(Id_t));
    MOCK_METHOD1(GetClientRect, Rect(Id_t));
  };

public:
  const Id_t m_Id = 0;
  Window & operator= (const Window &) = delete;
  bool operator== (const Window & _Value) const { return (m_Id == _Value.m_Id); }

public:
  // Èםעונפויס events::IEvents:
  operator Events_t (void) const override
  {
    return m_Events;
  }

public:
  Any_t Window::GetHandle(void) const override
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
  Window(const IApplication_t & _Application) :
    m_Id(Proxy::GetInstance()->Constructor(&_Application))
  {
  }
};

} // namespace os

} // namespace covellite

} // namespace mock
