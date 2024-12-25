
#pragma once

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::gui::Initializer::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::gui::Initializer Example(Value1);

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

class Initializer
{
  using RenderInterfacePtr_t = 
    ::std::shared_ptr<CovelliteGui::RenderInterface>;
  using StringTranslatorPtr_t =
    ::std::shared_ptr<::mock::covellite::gui::IStringTranslator>;

public:
  class Data
  {
  public:
    RenderInterfacePtr_t m_pRenderInterface;
    StringTranslatorPtr_t m_pStringTranslator;

  public:
    bool operator== (const Data & _Data) const
    {
      if (m_pRenderInterface->m_Id != _Data.m_pRenderInterface->m_Id) return false;
      if (m_pStringTranslator->m_Id != _Data.m_pStringTranslator->m_Id) return false;
      return true;
    }
  };

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Constructor, Id_t(const Data &));
  };

public:
  const Id_t m_Id = 0;
  Initializer & operator= (const Initializer &) = delete;
  bool operator== (const Initializer & _Value) const { return (m_Id == _Value.m_Id); }

public:
  explicit Initializer(const Data & _Param1) :
    m_Id(Proxy::GetInstance()->Constructor(_Param1))
  {

  }

};

} // namespace gui

} // namespace covellite

} // namespace mock
