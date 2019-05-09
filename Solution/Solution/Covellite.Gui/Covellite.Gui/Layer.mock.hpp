
#pragma once
#include <alicorn/std/string.forward.hpp>
#include <Covellite/Gui/IWindow.hpp>
#include <Covellite/Gui/ILayer.hpp>

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::gui::Layer::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::gui::Layer Example(Value1);

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

class Layer :
  public ::covellite::gui::ILayer
{
  using DocumentId_t = ::std::string;
  using ElementId_t = ::std::string;
  using IWindow_t = ::covellite::gui::IWindow;

protected:
  using String_t = ::alicorn::extension::std::String;
  using Path_t = ::boost::filesystem::path;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD2(Constructor, Id_t(IWindow_t *, Path_t));
    MOCK_METHOD3(Constructor, Id_t(IWindow_t *, Path_t, ::std::string));
    MOCK_METHOD1(Constructor, Id_t(Id_t));
    MOCK_METHOD1(SetFocus, void(Id_t));
    MOCK_METHOD2(SetMeaning, void(Id_t, String_t));
    MOCK_METHOD1(GetMeaning, String_t(Id_t));
    MOCK_METHOD2(SetClassStyle, void(Id_t, String_t));
    MOCK_METHOD2(Subscribe, void(Id_t, Id_t));
    MOCK_METHOD1(Show, void(Id_t));
    MOCK_METHOD1(Hide, void(Id_t));
    MOCK_METHOD1(GetId, DocumentId_t(Id_t));
    MOCK_METHOD2(GetElement, Id_t(Id_t, ElementId_t));
    MOCK_METHOD1(GetWidth, int(Id_t));
    MOCK_METHOD1(GetHeight, int(Id_t));
    MOCK_METHOD2(EmployFontSize, float (Id_t, float));
  };

public:
  const Id_t m_Id = 0;
  Layer & operator= (const Layer &) = delete;
  bool operator== (const Layer & _Value) const { return (m_Id == _Value.m_Id); }

public:
  class Element
  {
  public:
    void SetFocus(void)
    {
      Proxy::GetInstance()->SetFocus(m_Id);
    }

    void SetMeaning(const String_t & _Value)
    {
      Proxy::GetInstance()->SetMeaning(m_Id, _Value);
    }

    String_t GetMeaning(void) const
    {
      return Proxy::GetInstance()->GetMeaning(m_Id);
    }

    void SetClassStyle(const String_t & _Value)
    {
      Proxy::GetInstance()->SetClassStyle(m_Id, _Value);
    }

  public:
    const Id_t m_Id = 0;

  public:
    // cppcheck-suppress noExplicitConstructor
    Element(Id_t _Id) : m_Id(_Id) {}
  };

public:
  void Show(void)
  {
    Proxy::GetInstance()->Show(m_Id);
  }

  void Hide(void)
  {
    Proxy::GetInstance()->Hide(m_Id);
  }

  DocumentId_t GetId(void) const
  {
    return Proxy::GetInstance()->GetId(m_Id);
  }

  Element GetElement(const ElementId_t & _Id) const
  {
    return Proxy::GetInstance()->GetElement(m_Id, _Id);
  }

  int GetWidth(void) const
  {
    return Proxy::GetInstance()->GetWidth(m_Id);
  }

  int GetHeight(void) const
  {
    return Proxy::GetInstance()->GetHeight(m_Id);
  }

  float EmployFontSize(float _Value)
  {
    return Proxy::GetInstance()->EmployFontSize(m_Id, _Value);
  }

  public:
    Layer(IWindow_t & _Param1, const Path_t & _Param2) :
      m_Id(Proxy::GetInstance()->Constructor(&_Param1, _Param2))
    {

    }

    Layer(IWindow_t & _Param1, const Path_t & _Param2, const ::std::string & _Param3) :
      m_Id(Proxy::GetInstance()->Constructor(&_Param1, _Param2, _Param3))
    {

    }

  explicit Layer(::mock::Id_t _Param1) :
    m_Id(Proxy::GetInstance()->Constructor(_Param1))
  {

  }
};

} // namespace gui

} // namespace covellite

} // namespace mock
