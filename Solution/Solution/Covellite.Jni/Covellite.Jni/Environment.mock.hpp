
#pragma once

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::jni::Environment::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::jni::Environment Example(Value1);

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

namespace jni
{

class Environment
{

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(GetStatusBarHeight, int32_t(Id_t));
    MOCK_METHOD1(ShowSoftKeyboard, void(Id_t));
    MOCK_METHOD0(Constructor, Id_t(void));
  };

public:
  const Id_t m_Id = 0;
  Environment & operator= (const Environment &) = delete;
  bool operator== (const Environment & _Value) const { return (m_Id == _Value.m_Id); }

public:
  int32_t GetStatusBarHeight(void)
  {
    return Proxy::GetInstance()->GetStatusBarHeight(m_Id);
  }

  void ShowSoftKeyboard(void)
  {
    Proxy::GetInstance()->ShowSoftKeyboard(m_Id);
  }

  Environment(void) :
    m_Id(Proxy::GetInstance()->Constructor())
  {

  }

};

} // namespace jni

} // namespace covellite

} // namespace mock
