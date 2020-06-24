
#pragma once

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::expanse::Window::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::expanse::Window Example(Value1);

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

namespace expanse
{

class Window
{
  using WindowApi_t = ::covellite::api::IWindow;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD2(CreateObject, ObjectId_t(Id_t, const GameObject_t &));
    MOCK_METHOD2(RemoveObject, void(Id_t, const ObjectId_t));
    MOCK_METHOD4(Add, void(Id_t, size_t, ObjectId_t, size_t));
    MOCK_METHOD1(Constructor, Id_t(const WindowApi_t &));
  };

public:
  const Id_t m_Id = 0;
  Window & operator= (const Window &) = delete;
  bool operator== (const Window & _Value) const { return (m_Id == _Value.m_Id); }

public:
  ObjectId_t CreateObject(const GameObject_t & _Param1)
  {
    return Proxy::GetInstance()->CreateObject(m_Id, _Param1);
  }

  void RemoveObject(const ObjectId_t _Param1)
  {
    Proxy::GetInstance()->RemoveObject(m_Id, _Param1);
  }

  void Add(const size_t _Param1, const ObjectId_t _Param2, const size_t _Param3 = 0)
  {
    Proxy::GetInstance()->Add(m_Id, _Param1, _Param2, _Param3);
  }

  Window(const WindowApi_t & _Param1) :
    m_Id(Proxy::GetInstance()->Constructor(_Param1))
  {

  }

};

} // namespace expanse

} // namespace covellite

} // namespace mock
