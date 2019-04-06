
#pragma once
#include <Covellite\Gui\ILayer.hpp>

/*
An example of use:

using ExampleProxy_t = ::mock::alicorn::extension::android::Layers::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::alicorn::extension::android::Layers Example(Value1);

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

namespace gui
{

class Layers
{
  using LayerPtr_t = ::std::shared_ptr<::covellite::gui::ILayer>;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD0(Constructor, Id_t(void));
    MOCK_METHOD2(Push, void(Id_t, Id_t));
    MOCK_METHOD1(Pop, bool(Id_t));
  };

public:
  const Id_t m_Id = 0;
  Layers & operator= (const Layers &) = delete;
  bool operator== (const Layers & _Value) const { return (m_Id == _Value.m_Id); }

public:
  void Push(const LayerPtr_t & _pLayer)
  {
    Proxy::GetInstance()->Push(m_Id, 
      dynamic_cast<const ::mock::covellite::gui::Layer &>(*_pLayer).m_Id);
  }

  bool Pop(void)
  {
    return Proxy::GetInstance()->Pop(m_Id);
  }

public:
  Layers(void) :
    m_Id(Proxy::GetInstance()->Constructor())
  {

  }
};

} // namespace gui

} // namespace covellite

} // namespace mock
