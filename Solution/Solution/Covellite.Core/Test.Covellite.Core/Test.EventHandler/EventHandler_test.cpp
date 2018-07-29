
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса EventHandler.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define CommandHandlerApp CommandHandlerApp_dummy
#define CommandHandlerInput CommandHandlerInput_dummy

// Расположение класса EventHandler
#include "..\..\Covellite.Core\EventHandler.android.cpp"
#include "..\..\Covellite.Core\EventHandler.cpp"
#include "..\..\Covellite.Core\ClickEventListener.cpp"

// Общий тестовый класс класса EventHandler
class EventHandler_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::EventHandler;
  using EventListener_t = ::mock::Rocket::Core::EventListener;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }

protected:
  EventListener_t * GetClickEventListener(Tested_t & _Example)
  {
    return _Example.m_pClickEventListener.get();
  }

protected:
  class Tested :
    public Tested_t
  {
  public:
    MOCK_METHOD1(DoError, void(::std::string));
    MOCK_METHOD3(DoClick, void(::std::string, ::std::string, ::std::string));

  private:
    void OnError(const ::covellite::core::params::Error & _Value)
    {
      DoError(_Value.Description);
    }

    void OnClick(const ::covellite::core::params::Click & _Value)
    {
      DoClick(_Value.m_DocumentId, _Value.m_Tag, _Value.m_Type);
    }

  public:
    explicit Tested(const ::std::string & _ElementId)
    {
      using namespace ::covellite::core;
      using ::std::placeholders::_1;

      (*this)[Event::Error]
        .connect(::std::bind(&Tested::OnError, this, _1));

      (*this)[Event::Click][_ElementId]
        .connect(::std::bind(&Tested::OnClick, this, _1));
    }
  };
};

// Образец макроса для подстановки в класс EventHandler 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(EventHandler_test, Test_Function);

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_FALSE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_Subscribe_Document)
{
  android_app AndroidApp;
  const AppInfo_t Info{ &AndroidApp };

  ::mock::Rocket::Core::ElementDocument Document;

  Tested_t Example;

  auto * pListener = GetClickEventListener(Example);

  using namespace ::testing;

  EXPECT_CALL(Document, AddEventListener(Eq("click"), pListener, false))
    .Times(1);

  Example.Subscribe(&Document);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_Unsubscribe_Document)
{
  android_app AndroidApp;
  const AppInfo_t Info{ &AndroidApp };

  ::mock::Rocket::Core::ElementDocument Document;

  Tested_t Example;

  auto * pListener = GetClickEventListener(Example);

  using namespace ::testing;

  EXPECT_CALL(Document, RemoveEventListener(Eq("click"), pListener, false))
    .Times(1);

  Example.Unsubscribe(&Document);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_Subscribe_Contex)
{
  android_app AndroidApp;
  const AppInfo_t Info{ &AndroidApp };

  ::mock::Rocket::Core::Context Context;

  Tested_t Example;

  auto * pListener = GetClickEventListener(Example);

  using namespace ::testing;

  EXPECT_CALL(Context, AddEventListener(Eq("click"), pListener, false))
    .Times(1);

  Example.Subscribe(&Context);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_Unsubscribe_Contex)
{
  android_app AndroidApp;
  const AppInfo_t Info{ &AndroidApp };

  ::mock::Rocket::Core::Context Context;

  Tested_t Example;

  auto * pListener = GetClickEventListener(Example);

  using namespace ::testing;

  EXPECT_CALL(Context, RemoveEventListener(Eq("click"), pListener, false))
    .Times(1);

  Example.Unsubscribe(&Context);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_ElementEvents_TargetElementNullptr)
{
  android_app AndroidApp;
  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ "Id1710112240" };

  auto * pListener = GetClickEventListener(Example);

  ::mock::Rocket::Core::Event Event;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Event, GetTargetElement())
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_CALL(Example, DoClick(_, _, _))
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(1);

  pListener->ProcessEvent(Event);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_ElementEvents_OwnerDocument_Nullptr)
{
  android_app AndroidApp;
  const AppInfo_t Info{ &AndroidApp };

  ::mock::Rocket::Core::Element TargetElement;

  Tested Example{ "Id1710112300" };

  auto * pListener = GetClickEventListener(Example);

  ::mock::Rocket::Core::Event Event;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Event, GetTargetElement())
    .Times(1)
    .WillOnce(Return(&TargetElement));

  EXPECT_CALL(TargetElement, GetOwnerDocument())
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_CALL(Example, DoClick(_, _, _))
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(1);

  pListener->ProcessEvent(Event);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_ElementEvents_OnClick_Exception)
{
  android_app AndroidApp;
  const AppInfo_t Info{ &AndroidApp };

  ::mock::Rocket::Core::Element TargetElement;
  ::mock::Rocket::Core::ElementDocument OwnerDocument;
  const char * TargetElementId = "Id1710112259";
  const char * Error = "Error1710112258";

  Tested Example{ TargetElementId };

  auto * pListener = GetClickEventListener(Example);

  ::mock::Rocket::Core::Event Event;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Event, GetTargetElement())
    .Times(1)
    .WillOnce(Return(&TargetElement));

  EXPECT_CALL(TargetElement, GetOwnerDocument())
    .Times(1)
    .WillOnce(Return(&OwnerDocument));

  EXPECT_CALL(TargetElement, GetId())
    .Times(1)
    .WillOnce(Return(TargetElementId));

  EXPECT_CALL(OwnerDocument, GetId())
    .Times(1)
    .WillOnce(Return(""));

  EXPECT_CALL(TargetElement, GetTagName())
    .Times(1)
    .WillOnce(Return(""));

  EXPECT_CALL(TargetElement, GetAttribute(_, _))
    .Times(1)
    .WillOnce(Return(""));

  EXPECT_CALL(Example, DoClick(_, _, _))
    .Times(1)
    .WillOnce(Throw(::std::exception{ Error }));

  EXPECT_CALL(Example, DoError(Eq(Error)))
    .Times(1);

  pListener->ProcessEvent(Event);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_ElementEvents_OnClick)
{
  android_app AndroidApp;
  const AppInfo_t Info{ &AndroidApp };

  ::mock::Rocket::Core::Element TargetElement;
  ::mock::Rocket::Core::ElementDocument OwnerDocument;
  const char * TargetElementId = "Id1710112311";
  const char * OwnerDocumentId = "Id1710112312";
  const char * TagName = "Name1710112316";
  const char * Attribute = "Attribute1710112319";

  Tested Example{ TargetElementId };

  auto * pListener = GetClickEventListener(Example);

  ::mock::Rocket::Core::Event Event;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Event, GetTargetElement())
    .Times(1)
    .WillOnce(Return(&TargetElement));

  EXPECT_CALL(TargetElement, GetOwnerDocument())
    .Times(1)
    .WillOnce(Return(&OwnerDocument));

  EXPECT_CALL(TargetElement, GetId())
    .Times(1)
    .WillOnce(Return(TargetElementId));

  EXPECT_CALL(OwnerDocument, GetId())
    .Times(1)
    .WillOnce(Return(OwnerDocumentId));

  EXPECT_CALL(TargetElement, GetTagName())
    .Times(1)
    .WillOnce(Return(TagName));

  EXPECT_CALL(TargetElement, GetAttribute(Eq("type"), Eq("unknown")))
    .Times(1)
    .WillOnce(Return(Attribute));

  EXPECT_CALL(Example, DoClick(Eq(OwnerDocumentId),
    Eq(TagName),
    Eq(Attribute)))
    .Times(1);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  pListener->ProcessEvent(Event);
}
