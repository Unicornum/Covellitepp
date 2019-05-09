
#pragma once
#include <Covellite/Os/Window.mock.hpp>
#include <Covellite/App/IWindow.hpp>
#include <Covellite/Api/IWindow.hpp>

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

namespace api
{

class Window :
  public ::covellite::app::IWindow,
  public ::covellite::api::IWindow
{
  using WindowOs_t = ::covellite::os::IWindow;
  using String_t = ::alicorn::extension::std::String;
  using Rect_t = WindowOs_t::Rect;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Constructor, Id_t(Id_t));
    MOCK_METHOD1(GetClientRect, Rect_t(Id_t));
    MOCK_METHOD1(GetRenders, RendersPtr_t(Id_t));
  };

public:
  const Id_t m_Id = 0;
  Window & operator= (const Window &) = delete;
  bool operator== (const Window & _Value) const { return (m_Id == _Value.m_Id); }

public:
  operator Events_t (void) const override
  {
    return m_Events;
  }

public:
  Rect_t GetClientRect(void) const override
  {
    return Proxy::GetInstance()->GetClientRect(m_Id);
  }

  RendersPtr_t GetRenders(void) const override
  {
    return Proxy::GetInstance()->GetRenders(m_Id);
  }

private:
  Events_t m_Events;

public:
  explicit Window(const WindowOs_t & _Window) :
    m_Events(_Window),
    m_Id(Proxy::GetInstance()->Constructor(
      dynamic_cast<const ::mock::covellite::os::Window &>(_Window).m_Id))
  {

  }
};

} // namespace api

} // namespace covellite

} // namespace mock
