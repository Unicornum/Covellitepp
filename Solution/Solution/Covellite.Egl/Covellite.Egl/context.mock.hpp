
#pragma once

/*
An example of use:

using ExampleProxy_t = ::mock::egl::Context::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::egl::Context Example(Value1);

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

namespace egl
{

class Context
{

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD2(MakeCurrent, void(Id_t, Id_t));
    MOCK_METHOD3(Constructor, Id_t(Id_t, Id_t, const int));
    MOCK_METHOD1(Destructor, void(Id_t));
  };

public:
  const Id_t m_Id = 0;
  Context & operator= (const Context &) = delete;
  bool operator== (const Context & _Value) const { return (m_Id == _Value.m_Id); }

public:
  void MakeCurrent(const Surface & _Param1) const
  {
    Proxy::GetInstance()->MakeCurrent(m_Id, _Param1.m_Id);
  }

  Context(const Display & _Param1, const Config & _Param2, const int _Version) :
    m_Id(Proxy::GetInstance()->Constructor(_Param1.m_Id, _Param2.m_Id, _Version))
  {

  }

  ~Context(void)
  {
    Proxy::GetInstance()->Destructor(m_Id);
  }
};

} // namespace egl

} // namespace covellite

} // namespace mock
