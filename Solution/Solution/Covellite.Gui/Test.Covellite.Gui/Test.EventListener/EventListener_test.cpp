
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn/google/test/example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  ����� ������ EventListener.
*/

#define EventListener EventListener_tested

// ������������ ������ EventListener
#include <Covellite/Gui/EventListener.hpp>

#include "../../Covellite.Gui/EventListener.cpp"
#include <Covellite/Gui/Events.hpp>


// ����� �������� ����� ������ EventListener
class EventListener_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::Window::EventListener;
# undef EventListener

  using ITested_t = ::mock::CovelliteGui::Core::EventListener;
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
    MOCK_METHOD0(DoChange, void(void));
    MOCK_METHOD1(DoError, void(::std::string));
  };
};

// ������� ������� ��� ����������� � ����� EventListener 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(EventListener_test, Test_Function);

// ************************************************************************** //
TEST_F(EventListener_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(EventListener_test, /*DISABLED_*/Test_ComparisonEventHash)
{
  using namespace ::covellite::events;

  ::std::hash<Gui::Document::Element> Hash;

  {
    const ::std::vector<Gui::Document::Element> Ids =
    {
      Click.DocumentId("Unknown").ElementId("Unknown"),
      Click.DocumentId("1807280808").ElementId("Unknown"),
      Click.DocumentId("Unknown").ElementId("1807280809"),
      Change.DocumentId("Unknown").ElementId("Unknown"),
      Change.DocumentId("1807280808").ElementId("Unknown"),
      Change.DocumentId("Unknown").ElementId("1807280809"),
    };

    for (size_t i = 0; i != Ids.size(); i++)
    {
      for (size_t j = 0; j != Ids.size(); j++)
      {
        if (i == j) continue;

        EXPECT_TRUE(Hash(Ids[i]) != Hash(Ids[j]));
      }
    }
  }

  {
    const ::std::vector<Gui::Document::Element> Ids =
    {
      Click.DocumentId("Unknown").ElementId("Unknown"),
      Click.DocumentId("Unknown").ElementId("Unknown"),
    };

    for (size_t i = 0; i != Ids.size(); i++)
    {
      for (size_t j = 0; j != Ids.size(); j++)
      {
        EXPECT_TRUE(Hash(Ids[i]) == Hash(Ids[j]));
      }
    }
  }

  {
    const ::std::vector<Gui::Document::Element> Ids =
    {
      Change.DocumentId("Unknown").ElementId("Unknown"),
      Change.DocumentId("Unknown").ElementId("Unknown"),
    };

    for (size_t i = 0; i != Ids.size(); i++)
    {
      for (size_t j = 0; j != Ids.size(); j++)
      {
        EXPECT_TRUE(Hash(Ids[i]) == Hash(Ids[j]));
      }
    }
  }
}

// ************************************************************************** //
TEST_F(EventListener_test, /*DISABLED_*/Test_ProcessEvent_TargetElementNullptr)
{
  using namespace ::covellite::events;

  Handler Handler;
  Events_t Events;
  Events[Error.Exception]
    .Connect([&](const Error_t::Description & _Desc) { Handler.DoError(_Desc); });
  Events[Click.DocumentId("Unknown").ElementId("Unknown")]
    .Connect([&](void) { Handler.DoClick(); });

  auto pExample = Tested_t::Make(Events);
  ITested_t & IExample = *pExample;

  ::mock::CovelliteGui::Core::Event Event;

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
TEST_F(EventListener_test, /*DISABLED_*/Test_ProcessEvent_OwnerDocument_Nullptr)
{
  using namespace ::covellite::events;

  Handler Handler;
  Events_t Events;
  Events[Error.Exception]
    .Connect([&](const Error_t::Description & _Desc) { Handler.DoError(_Desc); });
  Events[Click.DocumentId("Unknown").ElementId("Unknown")]
    .Connect([&](void) { Handler.DoClick(); });

  auto pExample = Tested_t::Make(Events);
  ITested_t & IExample = *pExample;

  ::mock::CovelliteGui::Core::Element TargetElement;
  ::mock::CovelliteGui::Core::Event Event;

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
TEST_F(EventListener_test, /*DISABLED_*/Test_ProcessEvent_OnClick_Exception)
{
  ::mock::CovelliteGui::Core::Element TargetElement;
  ::mock::CovelliteGui::Core::ElementDocument OwnerDocument;
  const char * Type = "click";
  const char * DocumentId = "Id1807271203";
  const char * TargetElementId = "Id1710112259";
  const char * ErrorDescription = "Error1710112258";

  using namespace ::covellite::events;

  Handler Handler;
  Events_t Events;
  Events[Error.Exception]
    .Connect([&](const Error_t::Description & _Desc) { Handler.DoError(_Desc); });
  Events[Click.DocumentId(DocumentId).ElementId(TargetElementId)]
    .Connect([&](void) { Handler.DoClick(); });

  auto pExample = Tested_t::Make(Events);
  ITested_t & IExample = *pExample;

  ::mock::CovelliteGui::Core::Event Event;

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
    .WillOnce(Return(DocumentId));

  EXPECT_CALL(Event, GetType())
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Handler, DoClick())
    .Times(1)
    .WillOnce(Throw(::std::exception{ ErrorDescription }));

  EXPECT_CALL(Handler, DoError(Eq(ErrorDescription)))
    .Times(1);

  IExample.ProcessEvent(Event);
}

// ************************************************************************** //
TEST_F(EventListener_test, /*DISABLED_*/Test_ProcessEvent_OnChange_Exception)
{
  ::mock::CovelliteGui::Core::Element TargetElement;
  ::mock::CovelliteGui::Core::ElementDocument OwnerDocument;
  const char * Type = "change";
  const char * DocumentId = "Id1807271203";
  const char * TargetElementId = "Id1710112259";
  const char * ErrorDescription = "Error1710112258";

  using namespace ::covellite::events;

  Handler Handler;
  Events_t Events;
  Events[Error.Exception]
    .Connect([&](const Error_t::Description & _Desc) { Handler.DoError(_Desc); });
  Events[Change.DocumentId(DocumentId).ElementId(TargetElementId)]
    .Connect([&](void) { Handler.DoChange(); });

  auto pExample = Tested_t::Make(Events);
  ITested_t & IExample = *pExample;

  ::mock::CovelliteGui::Core::Event Event;

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
    .WillOnce(Return(DocumentId));

  EXPECT_CALL(Event, GetType())
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Handler, DoChange())
    .Times(1)
    .WillOnce(Throw(::std::exception{ ErrorDescription }));

  EXPECT_CALL(Handler, DoError(Eq(ErrorDescription)))
    .Times(1);

  IExample.ProcessEvent(Event);
}

// ************************************************************************** //
TEST_F(EventListener_test, /*DISABLED_*/Test_ProcessEvent_OnClick)
{
  ::mock::CovelliteGui::Core::Element TargetElement;
  ::mock::CovelliteGui::Core::ElementDocument OwnerDocument;
  const char * Type = "click";
  const char * TargetElementId = "Id1710112311";
  const char * OwnerDocumentId = "Id1710112312";

  using namespace ::covellite::events;

  Handler Handler;
  Events_t Events;
  Events[Error.Exception]
    .Connect([&](const Error_t::Description & _Desc) { Handler.DoError(_Desc); });
  Events[Click.DocumentId(OwnerDocumentId).ElementId(TargetElementId)]
    .Connect([&](void) { Handler.DoClick(); });

  auto pExample = Tested_t::Make(Events);
  ITested_t & IExample = *pExample;

  ::mock::CovelliteGui::Core::Event Event;

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

  EXPECT_CALL(Event, GetType())
    .Times(1)
    .WillOnce(Return(Type));  

  EXPECT_CALL(Handler, DoClick())
    .Times(1);

  EXPECT_CALL(Handler, DoError(_))
    .Times(0);

  IExample.ProcessEvent(Event);
}

// ************************************************************************** //
TEST_F(EventListener_test, /*DISABLED_*/Test_ProcessEvent_OnChamge)
{
  ::mock::CovelliteGui::Core::Element TargetElement;
  ::mock::CovelliteGui::Core::ElementDocument OwnerDocument;
  const char * Type = "change";
  const char * TargetElementId = "Id1710112311";
  const char * OwnerDocumentId = "Id1710112312";

  using namespace ::covellite::events;

  Handler Handler;
  Events_t Events;
  Events[Error.Exception]
    .Connect([&](const Error_t::Description & _Desc) { Handler.DoError(_Desc); });
  Events[Change.DocumentId(OwnerDocumentId).ElementId(TargetElementId)]
    .Connect([&](void) { Handler.DoChange(); });

  auto pExample = Tested_t::Make(Events);
  ITested_t & IExample = *pExample;

  ::mock::CovelliteGui::Core::Event Event;

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

  EXPECT_CALL(Event, GetType())
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Handler, DoChange())
    .Times(1);

  EXPECT_CALL(Handler, DoError(_))
    .Times(0);

  IExample.ProcessEvent(Event);
}
