
#pragma once

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::core::Window::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::core::Window Example(Value1);

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

class Window
{

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD0(Constructor, Id_t(void));
    MOCK_METHOD1(GetStatusBarHeight, int32_t(Id_t));
    MOCK_METHOD1(Exit, void(Id_t));
  };

public:
  const Id_t m_Id = 0;
  Window & operator= (const Window &) = delete;
  bool operator== (const Window & _Value) const { return (m_Id == _Value.m_Id); }

public:
  int32_t GetStatusBarHeight(void) const
  {
    return Proxy::GetInstance()->GetStatusBarHeight(m_Id);
  }

  void Exit(void) const
  {
    Proxy::GetInstance()->Exit(m_Id);
  }

public:
  Window(void) :
    m_Id(Proxy::GetInstance()->Constructor())
  {

  }
};

} // namespace core

} // namespace covellite

} // namespace mock
