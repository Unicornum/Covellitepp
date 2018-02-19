
#pragma once

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::egl::Display::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::egl::Display Example(Value1);

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

class Display
{
public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD0(Constructor, Id_t(void));
    MOCK_METHOD1(Destructor, void(Id_t));
  };

public:
  const Id_t m_Id = 0;
  Display & operator= (const Display &) = delete;
  bool operator== (const Display & _Value) const { return (m_Id == _Value.m_Id); }

public:
  ::covellite::egl::EGLDisplay m_Display = nullptr;

public:
  Display(void) :
    m_Id(Proxy::GetInstance()->Constructor()),
    m_Display(eglGetDisplay(::covellite::egl::EGL_DEFAULT_DISPLAY))
  {

  }
  ~Display(void)
  {
    Proxy::GetInstance()->Destructor(m_Id);
  }
};

} // namespace egl

} // namespace covellite

} // namespace mock
