
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ ClickEventListener.
*/

#define ClickEventListener ClickEventListener_tested

// ������������ ������ ClickEventListener
#include <Covellite/Rocket/ClickEventListener.hpp>

#include "../../Covellite.Rocket/ClickEventListener.cpp"
#include <Covellite/Gui/Events.hpp>

// ����� �������� ����� ������ ClickEventListener
class ClickEventListener_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::rocket::Window::ClickEventListener;
  using ITested_t = ::mock::Rocket::Core::EventListener;
  using Events_t = ::covellite::events::Events;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }

protected:
  class Handler
  {
  public:
    MOCK_METHOD0(DoClick, void(void));
    MOCK_METHOD1(DoError, void(::std::string));
  };
};

// ������� ������� ��� ����������� � ����� ClickEventListener 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(ClickEventListener_test, Test_Function);

// ************************************************************************** //
TEST_F(ClickEventListener_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(ClickEventListener_test, /*DISABLED_*/Test_Click_Hash)
{
  using namespace ::covellite::events;

  const auto Id1 = Click.DocumentId("Unknown").ElementId("Unknown");
  const auto Id2 = Click.DocumentId("Unknown").ElementId("Unknown");
  const auto Id3 = Click.DocumentId("1807280808").ElementId("Unknown");
  const auto Id4 = Click.DocumentId("Unknown").ElementId("1807280809");

  ::std::hash<Click_t::Document::Element> Hash;

  EXPECT_TRUE(Hash(Id1) == Hash(Id2));
  EXPECT_TRUE(Hash(Id1) != Hash(Id3));
  EXPECT_TRUE(Hash(Id1) != Hash(Id4));
  EXPECT_TRUE(Hash(Id2) != Hash(Id3));
  EXPECT_TRUE(Hash(Id2) != Hash(Id4));
  EXPECT_TRUE(Hash(Id3) != Hash(Id4));
}

// ************************************************************************** //
TEST_F(ClickEventListener_test, /*DISABLED_*/Test_ProcessEvent_TargetElementNullptr)
{
  using namespace ::covellite::events;

  Handler Handler;
  Events_t Events;
  Events[Error.Exception]
    .Connect([&](const Error_t::Description & _Desc) 
    { Handler.DoError(_Desc); });
  Events[Click.DocumentId("Unknown").ElementId("Unknown")]
    .Connect([&](void)
    { Handler.DoClick(); });

  auto pExample = Tested_t::Make(Events);
  ITested_t & IExample = *pExample;

  ::mock::Rocket::Core::Event Event;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Event, GetTargetElement())
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_CALL(Handler, DoClick())
    .Times(0);

  EXPECT_CALL(Handler, DoError(_))
    .Times(1);

  IExample.ProcessEvent(Event);
}

// ************************************************************************** //
TEST_F(ClickEventListener_test, /*DISABLED_*/Test_ProcessEvent_OwnerDocument_Nullptr)
{
  using namespace ::covellite::events;

  Handler Handler;
  Events_t Events;
  Events[Error.Exception]
    .Connect([&](const Error_t::Description & _Desc)
  { Handler.DoError(_Desc); });
  Events[Click.DocumentId("Unknown").ElementId("Unknown")]
    .Connect([&](void)
  { Handler.DoClick(); });

  auto pExample = Tested_t::Make(Events);
  ITested_t & IExample = *pExample;

  ::mock::Rocket::Core::Element TargetElement;
  ::mock::Rocket::Core::Event Event;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Event, GetTargetElement())
    .Times(1)
    .WillOnce(Return(&TargetElement));

  EXPECT_CALL(TargetElement, GetOwnerDocument())
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_CALL(Handler, DoClick())
    .Times(0);

  EXPECT_CALL(Handler, DoError(_))
    .Times(1);

  IExample.ProcessEvent(Event);
}

// ************************************************************************** //
TEST_F(ClickEventListener_test, /*DISABLED_*/Test_ProcessEvent_OnClick_Exception)
{
  ::mock::Rocket::Core::Element TargetElement;
  ::mock::Rocket::Core::ElementDocument OwnerDocument;
  const char * DocumentId = "Id1807271203";
  const char * TargetElementId = "Id1710112259";
  const char * ErrorDescription = "Error1710112258";

  using namespace ::covellite::events;

  Handler Handler;
  Events_t Events;
  Events[Error.Exception]
    .Connect([&](const Error_t::Description & _Desc)
  { Handler.DoError(_Desc); });
  Events[Click.DocumentId(DocumentId).ElementId(TargetElementId)]
    .Connect([&](void)
  { Handler.DoClick(); });

  auto pExample = Tested_t::Make(Events);
  ITested_t & IExample = *pExample;

  ::mock::Rocket::Core::Event Event;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Event, GetTargetElement())
    .Times(1)
    .WillOnce(Return(&TargetElement));

  EXPECT_CALL(TargetElement, GetOwnerDocument())
    .Times(1)
    .WillOnce(Return(&OwnerDocument));

  EXPECT_CALL(OwnerDocument, GetId())
    .Times(1)
    .WillOnce(Return(DocumentId));

  EXPECT_CALL(TargetElement, GetId())
    .Times(1)
    .WillOnce(Return(TargetElementId));

  EXPECT_CALL(Handler, DoClick())
    .Times(1)
    .WillOnce(Throw(::std::exception{ ErrorDescription }));

  EXPECT_CALL(Handler, DoError(Eq(ErrorDescription)))
    .Times(1);

  IExample.ProcessEvent(Event);
}

// ************************************************************************** //
TEST_F(ClickEventListener_test, /*DISABLED_*/Test_ProcessEvent_OnClick)
{
  ::mock::Rocket::Core::Element TargetElement;
  ::mock::Rocket::Core::ElementDocument OwnerDocument;
  const char * TargetElementId = "Id1710112311";
  const char * OwnerDocumentId = "Id1710112312";

  using namespace ::covellite::events;

  Handler Handler;
  Events_t Events;
  Events[Error.Exception]
    .Connect([&](const Error_t::Description & _Desc)
  { Handler.DoError(_Desc); });
  Events[Click.DocumentId(OwnerDocumentId).ElementId(TargetElementId)]
    .Connect([&](void)
  { Handler.DoClick(); });

  auto pExample = Tested_t::Make(Events);
  ITested_t & IExample = *pExample;

  ::mock::Rocket::Core::Event Event;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Event, GetTargetElement())
    .Times(1)
    .WillOnce(Return(&TargetElement));

  EXPECT_CALL(TargetElement, GetOwnerDocument())
    .Times(1)
    .WillOnce(Return(&OwnerDocument));

  EXPECT_CALL(OwnerDocument, GetId())
    .Times(1)
    .WillOnce(Return(OwnerDocumentId));

  EXPECT_CALL(TargetElement, GetId())
    .Times(1)
    .WillOnce(Return(TargetElementId));
  
  EXPECT_CALL(Handler, DoClick())
    .Times(1);

  EXPECT_CALL(Handler, DoError(_))
    .Times(0);

  IExample.ProcessEvent(Event);
}
