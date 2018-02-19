
#pragma once

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::egl::Config::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::egl::Config Example(Value1);

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

class Config
{

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD2(Constructor, Id_t(Id_t, ANativeWindow *));
    MOCK_METHOD1(Destructor, void(Id_t));
  };

public:
  ::covellite::egl::EGLConfig m_Config = nullptr;

public:
  const Id_t m_Id = 0;
  Config & operator= (const Config &) = delete;
  bool operator== (const Config & _Value) const { return (m_Id == _Value.m_Id); }

public:
  Config(const Display & _Param1, ANativeWindow * _Param2) :
    m_Id(Proxy::GetInstance()->Constructor(_Param1.m_Id, _Param2))
  {

  }

  ~Config(void)
  {
    Proxy::GetInstance()->Destructor(m_Id);
  }
};

} // namespace egl

} // namespace covellite

} // namespace mock
