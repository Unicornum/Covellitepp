
#pragma once
#include <Covellite\Core\IWindow.hpp>
#include <Covellite\Gui\IWindow.hpp>
#include <Covellite\Gui\ILayer.hpp>

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

namespace Rocket { namespace Core { class Context; } }

namespace covellite
{

namespace gui
{

class Layer :
  public ::covellite::core::IWindow,
  public ::covellite::gui::ILayer
{
  using Utf8String_t = ::std::string;
  using DocumentId_t = ::std::string;
  using ElementId_t = ::std::string;
  using IWindow_t = ::covellite::gui::IWindow;

protected:
  using Context_t = ::mock::Rocket::Core::Context;
  using Path_t = ::boost::filesystem::path;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD2(Constructor, Id_t(IWindow_t *, Path_t));
    MOCK_METHOD2(Constructor, Id_t(Context_t *, Path_t));
    MOCK_METHOD1(Constructor, Id_t(Id_t));
    MOCK_METHOD2(SetText, void(Id_t, Utf8String_t));
    MOCK_METHOD1(GetText, Utf8String_t(Id_t));
    MOCK_METHOD2(SetStyle, void(Id_t, Utf8String_t));
    MOCK_METHOD1(SetFocus, void(Id_t));
    MOCK_METHOD2(Subscribe, void(Id_t, Id_t));
    MOCK_METHOD1(Show, void(Id_t));
    MOCK_METHOD1(Hide, void(Id_t));
    MOCK_METHOD1(GetId, DocumentId_t(Id_t));
    MOCK_METHOD2(GetElement, Id_t(Id_t, ElementId_t));
    MOCK_METHOD1(GetWidth, int(Id_t));
    MOCK_METHOD1(GetHeight, int(Id_t));
    MOCK_METHOD2(SetFontSize, void (Id_t, double));
  };

public:
  const Id_t m_Id = 0;
  Layer & operator= (const Layer &) = delete;
  bool operator== (const Layer & _Value) const { return (m_Id == _Value.m_Id); }

public:
  class Element
  {
  public:
    void SetText(const Utf8String_t & _Text)
    {
      Proxy::GetInstance()->SetText(m_Id, _Text);
    }

    Utf8String_t GetText(void)
    {
      return Proxy::GetInstance()->GetText(m_Id);
    }

    void SetStyle(const Utf8String_t & _Text)
    {
      Proxy::GetInstance()->SetStyle(m_Id, _Text);
    }

    void SetFocus(void)
    {
      Proxy::GetInstance()->SetFocus(m_Id);
    }

  public:
    const Id_t m_Id = 0;

  public:
    // cppcheck-suppress noExplicitConstructor
    Element(Id_t _Id) : m_Id(_Id) {}
  };

public:
  void Subscribe(const EventHandlerPtr_t & _pEvents) override
  {
    Proxy::GetInstance()->Subscribe(m_Id, _pEvents->m_Id);
  }

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

  void SetFontSize(double _Value = 1.0)
  {
    Proxy::GetInstance()->SetFontSize(m_Id, _Value);
  }

  public:
    explicit Layer(IWindow_t * _Param1, const Path_t & _Param2) :
      m_Id(Proxy::GetInstance()->Constructor(_Param1, _Param2))
    {

    }

  explicit Layer(Context_t * _Param1, const Path_t & _Param2) :
    m_Id(Proxy::GetInstance()->Constructor(_Param1, _Param2))
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
