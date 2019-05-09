
#pragma once
#include <typeinfo>
#include <Covellite\Events\Events.hpp>
#include <Covellite\Api\Window.mock.hpp>

/*
An example of use:

using ExampleProxy_t = ::mock::covellite::gui::Window::Proxy;
ExampleProxy_t ExampleProxy;
ExampleProxy_t::GetInstance() = &ExampleProxy;

const ::mock::Id_t ExampleId = 1607301544;

using namespace ::testing;

InSequence Dummy;

EXPECT_CALL(ExampleProxy, Constructor(Value1))
  .Times(1)
  .WillOnce(Return(ExampleId));

::mock::covellite::gui::Window Example(Value1);

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

class Window :
  public ::covellite::gui::IWindow
{
  using WindowApi_t = ::covellite::api::IWindow;
  using WindowApiPtr_t = ::std::shared_ptr<::covellite::api::IWindow>;
  using Path_t = ::boost::filesystem::path;
  using Utf8String_t = ::std::string;
  using StringBank_t = ::std::map<Utf8String_t, Utf8String_t>;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Constructor, Id_t(const void *));
    MOCK_METHOD1(Set, void(StringBank_t));
    MOCK_METHOD2(AddLayer, Id_t(Id_t, ::std::string));
    MOCK_METHOD2(PushLayer, void(Id_t, ::std::string));
    MOCK_METHOD1(Back, void(Id_t));

  public:
    MOCK_METHOD1(DoDrawWindow, void(Id_t));
  };

private:
  void DoDrawWindow(void)
  {
    Proxy::GetInstance()->DoDrawWindow(m_Id);
  }

public:
  const Id_t m_Id = 0;
  Window & operator= (const Window &) = delete;
  bool operator== (const Window & _Value) const { return (m_Id == _Value.m_Id); }

public:
  // ��������� events::IEvents:
  operator Events_t (void) const override
  {
    return m_Events;
  }

public:
  void Set(const StringBank_t & _Bank)
  {
    Proxy::GetInstance()->Set(_Bank);
  }

  template<class TLayer>
  void PushLayer(void)
  {
    Proxy::GetInstance()->PushLayer(m_Id, typeid(TLayer).name());
  }

  void Back(void)
  {
    Proxy::GetInstance()->Back(m_Id);
  }

private:
  Events_t m_Events;

public:
  explicit Window(const WindowApi_t & _WindowsApi) :
    m_Id(Proxy::GetInstance()->Constructor(&_WindowsApi))
  {

  }
};

} // namespace gui

} // namespace covellite

} // namespace mock
