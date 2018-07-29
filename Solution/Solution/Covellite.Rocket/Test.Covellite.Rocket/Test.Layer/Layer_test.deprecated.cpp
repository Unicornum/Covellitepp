
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Layer.
*/

#define Layer Layer_tested_deprecated
#define Layer_test Layer_test_deprecated

// Расположение класса Layer
#include "..\..\Covellite.Rocket\Layer.cpp"
#include "..\..\Covellite.Rocket\Layer.windows.cpp"

// Общий тестовый класс класса Layer
class Layer_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::rocket::Layer;
  using ITested_t = ::covellite::rocket::ILayer;
  using Context_t = ::mock::Rocket::Core::Context;
  using Document_t = ::mock::Rocket::Core::ElementDocument;
  using String_t = ::mock::Rocket::Core::String;
  using Path_t = ::boost::filesystem::path;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<String_t>::Set("DefaultString");
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<String_t>::Clear();
  }

protected:
  class Tested :
    public Tested_t
  {
  public:
    void Subscribe(const EventHandlerPtr_t &) override {}

  public:
    using Tested_t::Tested_t;
  };
};

// Образец макроса для подстановки в класс Layer 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Layer_test, Test_Function);

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_LoadDocument_Nullptr)
{
  Context_t Context;
  const char * PathToFile = "Path1710032313";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(Eq(PathToFile)))
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_STDEXCEPTION(Tested( &Context, Path_t(PathToFile) ),
    (::std::string(".*Document null pointer: \"") + PathToFile + "\"").c_str());
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor)
{
  const ::mock::Id_t EventHandlerId = 1710121239;
  Context_t Context;
  Document_t Document;

  {
    const char * PathToFile = "Path1710032323";

    using namespace ::testing;

    EXPECT_CALL(Context, LoadDocument(Eq(PathToFile)))
      .Times(1)
      .WillOnce(Return(&Document));

    const Tested Example{ &Context, Path_t{ PathToFile } };

    InSequence Dummy;

    EXPECT_CALL(Document, RemoveReference())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title_Nullptr)
{
  Context_t Context;
  const char * PathToFile = "Path1710040009";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(Eq(PathToFile)))
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_STDEXCEPTION(Tested(&Context, Path_t(PathToFile), ""),
    (::std::string(".*Document null pointer: \"") + PathToFile + "\"").c_str());
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title_UnknownTitle)
{
  Context_t Context;
  const char * PathToFile = "Path1710040012";
  Document_t Document;
  const char * TitleId = "Unknown1612201113";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(Eq(PathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(Eq(TitleId)))
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_CALL(Document, RemoveReference())
    .Times(1);

  EXPECT_STDEXCEPTION(Tested(&Context, Path_t(PathToFile), TitleId),
    (".*Unexpected title id: " + ::std::string(TitleId)).c_str());
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title)
{
  const ::mock::Id_t EventHandlerId = 1710121244;
  Context_t Context;
  const char * PathToFile = "Path1710040013";
  Document_t Document;
  ::mock::Rocket::Core::Element Title;
  const char * TitleId = "TitleId1612201119";
  const char * DocumentTitle = "Title1612201121";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(Eq(PathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(Eq(TitleId)))
    .Times(1)
    .WillOnce(Return(&Title));

  EXPECT_CALL(Document, GetTitle())
    .Times(1)
    .WillOnce(Return(DocumentTitle));

  EXPECT_CALL(Title, SetInnerRML(Eq(DocumentTitle)))
    .Times(1);

  {
    Tested Example{ &Context, Path_t(PathToFile), TitleId };

    EXPECT_CALL(Document, RemoveReference())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Show)
{
  Context_t Context;
  Document_t Document;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ &Context, Path_t{} };
  ITested_t & IExampe = Example;

  EXPECT_CALL(Document, Show())
    .Times(1);

  IExampe.Show();
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Hide)
{
  Context_t Context;
  Document_t Document;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ &Context, Path_t{} };
  ITested_t & IExampe = Example;

  EXPECT_CALL(Document, Hide())
    .Times(1);

  IExampe.Hide();
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_GetElement)
{
  Context_t Context;
  Document_t Document;
  ::mock::Rocket::Core::Element Element;
  const char * Id = "1701031206";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ &Context, Path_t{} };

  EXPECT_CALL(Document, GetElementById(Eq(Id)))
    .Times(2)
    .WillOnce(Return(nullptr))
    .WillOnce(Return(&Element));

  EXPECT_THROW(Example.GetElement(Id), ::std::exception);

  Example.GetElement(Id);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_GetWidth)
{
  Context_t Context;
  Document_t Document;
  const ::mock::Rocket::Core::Vector2i Size = { 1701131210, 0 };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ &Context, Path_t{} };

  EXPECT_CALL(Document, GetContext())
    .Times(2)
    .WillOnce(Return(nullptr))
    .WillOnce(Return(&Context));

  EXPECT_CALL(Context, GetDimensions())
    .Times(1)
    .WillOnce(Return(Size));

  EXPECT_THROW(Example.GetWidth(), ::std::exception);

  const auto Result = Example.GetWidth();
  EXPECT_EQ(Size.x, Result);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_GetHeight)
{
  Context_t Context;
  Document_t Document;
  const ::mock::Rocket::Core::Vector2i Size = { 0, 1701131212 };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ &Context, Path_t{} };

  EXPECT_CALL(Document, GetContext())
    .Times(2)
    .WillOnce(Return(nullptr))
    .WillOnce(Return(&Context));

  EXPECT_CALL(Context, GetDimensions())
    .Times(1)
    .WillOnce(Return(Size));

  EXPECT_THROW(Example.GetWidth(), ::std::exception);

  const auto Result = Example.GetHeight();
  EXPECT_EQ(Size.y, Result);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_SetFontSize_InvalidDocumentTagName)
{
  Context_t Context;
  Document_t Document;
  const ::mock::Rocket::Core::String TagName = "Tag1702091254";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ &Context, Path_t{} };

  using namespace ::testing;

  EXPECT_CALL(Document, GetTagName())
    .Times(AtLeast(1))
    .WillOnce(Return(TagName));

  EXPECT_THROW(Example.SetFontSize(), ::std::exception);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_SetFontSize)
{
  Context_t Context;
  Document_t Document;

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ &Context, Path_t{} };

  EXPECT_CALL(Document, GetTagName())
    .WillRepeatedly(Return("body"));

  EXPECT_CALL(Document, GetContext())
    .WillRepeatedly(Return(&Context));

  {
    InSequence Dummy;

    const double Percent = 2.0;
    const ::mock::Rocket::Core::Vector2i Size = { 0, 800 };

    EXPECT_CALL(Context, GetDimensions())
      .Times(2)
      .WillRepeatedly(Return(Size));

    EXPECT_CALL(Document, SetAttribute(Eq("style"), Eq("font-size: 16px;")))
      .Times(1);

    Example.SetFontSize(Percent);
  }

  {
    InSequence Dummy;

    const double Percent = 1.0;
    const ::mock::Rocket::Core::Vector2i Size = { 765, 0 };

    EXPECT_CALL(Context, GetDimensions())
      .Times(2)
      .WillRepeatedly(Return(Size));

    EXPECT_CALL(Document, SetAttribute(Eq("style"), Eq("font-size: 7.65px;")))
      .Times(1);

    Example.SetFontSize(Percent);
  }
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_GetId)
{
  Context_t Context;
  Document_t Document;
  const char * DocumentId = "Id1710102335";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ &Context, Path_t{} };

  EXPECT_CALL(Document, GetId())
    .Times(1)
    .WillOnce(Return(DocumentId));

  const auto Result = Example.GetId();
  EXPECT_EQ(DocumentId, Result);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_SetText_TextElement)
{
  Context_t Context;
  Document_t Document;
  ::mock::Rocket::Core::Element Element;
  const char * Text = "Text1701031215";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ &Context, Path_t{} };

  using namespace ::testing;

  {
    InSequence Dummy;

    EXPECT_CALL(Document, GetElementById(_))
      .Times(1)
      .WillOnce(Return(&Element));

    EXPECT_CALL(Element, GetTagName())
      .Times(1)
      .WillOnce(Return("textarea"));

    EXPECT_CALL(Element, GetAttribute(Eq("type"), Eq("unknown")))
      .Times(1)
      .WillOnce(Return("Type1701121404"));

    EXPECT_CALL(Element, SetAttribute(Eq("value"), Eq(Text)))
      .Times(1);

    Example.GetElement("").SetText(Text);
  }

  {
    InSequence Dummy;

    EXPECT_CALL(Document, GetElementById(_))
      .Times(1)
      .WillOnce(Return(&Element));

    EXPECT_CALL(Element, GetTagName())
      .Times(1)
      .WillOnce(Return("input"));

    EXPECT_CALL(Element, GetAttribute(Eq("type"), Eq("unknown")))
      .Times(1)
      .WillOnce(Return("text"));

    EXPECT_CALL(Element, SetAttribute(Eq("value"), Eq(Text)))
      .Times(1);

    Example.GetElement("").SetText(Text);
  }
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_SetText_AnotherElement)
{
  Context_t Context;
  Document_t Document;
  ::mock::Rocket::Core::Element Element;
  const char * Text = "Text1701121406";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ &Context, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, GetTagName())
    .Times(1)
    .WillOnce(Return("Tag1701121407"));

  EXPECT_CALL(Element, GetAttribute(Eq("type"), Eq("unknown")))
    .Times(1)
    .WillOnce(Return("Type1701121408"));

  EXPECT_CALL(Element, SetInnerRML(Eq(Text)))
    .Times(1);

  Example.GetElement("").SetText(Text);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_GetText_TextElement)
{
  Context_t Context;
  Document_t Document;
  ::mock::Rocket::Core::Element Element;
  const char * Text = "Text1701121412";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ &Context, Path_t{} };

  using namespace ::testing;

  {
    InSequence Dummy;

    EXPECT_CALL(Document, GetElementById(_))
      .Times(1)
      .WillOnce(Return(&Element));

    EXPECT_CALL(Element, GetTagName())
      .Times(1)
      .WillOnce(Return("textarea"));

    EXPECT_CALL(Element, GetAttribute(Eq("type"), Eq("unknown")))
      .Times(1)
      .WillOnce(Return("Type1701121411"));

    EXPECT_CALL(Element, GetAttribute(Eq("value"), Eq("unknown")))
      .Times(1)
      .WillOnce(Return(Text));

    const auto Result = Example.GetElement("").GetText();
    EXPECT_EQ(Text, Result);
  }

  {
    InSequence Dummy;

    EXPECT_CALL(Document, GetElementById(_))
      .Times(1)
      .WillOnce(Return(&Element));

    EXPECT_CALL(Element, GetTagName())
      .Times(1)
      .WillOnce(Return("input"));

    EXPECT_CALL(Element, GetAttribute(Eq("type"), Eq("unknown")))
      .Times(1)
      .WillOnce(Return("text"));

    EXPECT_CALL(Element, GetAttribute(Eq("value"), Eq("unknown")))
      .Times(1)
      .WillOnce(Return(Text));

    const auto Result = Example.GetElement("").GetText();
    EXPECT_EQ(Text, Result);
  }
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_GetText_AnotherElement)
{
  Context_t Context;
  Document_t Document;
  ::mock::Rocket::Core::Element Element;
  const char * Text = "Text1701121413";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ &Context, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, GetTagName())
    .Times(1)
    .WillOnce(Return("Tag1701121415"));

  EXPECT_CALL(Element, GetAttribute(Eq("type"), Eq("unknown")))
    .Times(1)
    .WillOnce(Return("Type1701121414"));

  EXPECT_CALL(Element, GetInnerRML())
    .Times(1)
    .WillOnce(Return(Text));

  const auto Result = Example.GetElement("").GetText();
  EXPECT_EQ(Text, Result);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_SetStyle)
{
  Context_t Context;
  Document_t Document;
  ::mock::Rocket::Core::Element Element;
  const char * Text = "Text1701131203";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ &Context, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, SetAttribute(Eq("style"), Eq(Text)))
    .Times(1);

  Example.GetElement("").SetStyle(Text);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Focus)
{
  Context_t Context;
  Document_t Document;
  ::mock::Rocket::Core::Element Element;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Context, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ &Context, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, Focus())
    .Times(1);

  Example.GetElement("").SetFocus();
}
