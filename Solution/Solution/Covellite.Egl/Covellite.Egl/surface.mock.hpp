
#pragma once

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::egl::Surface::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::egl::Surface Example(Value1);

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

class Surface
{

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD3(Constructor, Id_t(Id_t, Id_t, ANativeWindow *));
    MOCK_METHOD1(Destructor, void(Id_t));
    MOCK_METHOD1(SwapBuffers, void(Id_t));
    MOCK_METHOD1(GetWidth, int32_t(Id_t));
    MOCK_METHOD1(GetHeight, int32_t(Id_t));
  };

public:
  const Id_t m_Id = 0;
  Surface & operator= (const Surface &) = delete;
  bool operator== (const Surface & _Value) const { return (m_Id == _Value.m_Id); }

public:
  ::covellite::egl::EGLSurface m_Surface = nullptr;

public:
  void SwapBuffers(void) const
  {
    Proxy::GetInstance()->SwapBuffers(m_Id);
  }

  int32_t GetWidth(void) const
  {
    return Proxy::GetInstance()->GetWidth(m_Id);
  }

  int32_t GetHeight(void) const
  {
    return Proxy::GetInstance()->GetHeight(m_Id);
  }

  Surface(const Display & _Display, const Config & _Config,
    ANativeWindow * _pActivity) :
    m_Id(Proxy::GetInstance()->Constructor(_Display.m_Id, _Config.m_Id, _pActivity))
  {

  }
  ~Surface(void)
  {
    Proxy::GetInstance()->Destructor(m_Id);
  }
};

} // namespace egl

} // namespace covellite

} // namespace mock
