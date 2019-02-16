
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Layer.
*/

// Расположение класса Layer
#include "..\..\Covellite.Gui\Layer.cpp"
#include "..\..\Covellite.Gui\Layer.windows.cpp"

#include "Common_test.hpp"

// Образец макроса для подстановки в класс Layer 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Layer_test, Test_Function);

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_NullptrDocument)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  using namespace ::testing;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_THROW(Tested(IWindow, ""), ::std::exception);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;

  {
    const char * PathToFile = "Path1710032323";

    using namespace ::testing;

    EXPECT_CALL(Window, LoadDocument(Eq(PathToFile)))
      .Times(1)
      .WillOnce(Return(&Document));

    const Tested Example{ IWindow, Path_t{ PathToFile } };

    InSequence Dummy;

    EXPECT_CALL(Document, RemoveReference())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Events)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;

  using namespace ::testing;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, "" };

  EXPECT_CALL(Example, DoMessage1())
    .Times(1);

  Window.m_Events[Tested::Message1]();
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title_NullptrDocument)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  using namespace ::testing;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_THROW(Tested(IWindow, "", ""), ::std::exception);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title_UnknownTitle)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  const char * PathToFile = "Path1710040012";
  Document_t Document;
  const char * TitleId = "Unknown1612201113";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(Eq(PathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(Eq(TitleId)))
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_CALL(Document, RemoveReference())
    .Times(1);

  EXPECT_STDEXCEPTION(Tested(IWindow, Path_t(PathToFile), TitleId),
    (".*Unexpected title id: " + ::std::string(TitleId)).c_str());
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  const char * PathToFile = "Path1710040013";
  Document_t Document;
  ::mock::Rocket::Core::Element Title;
  const char * TitleId = "TitleId1612201119";
  const char * DocumentTitle = "Title1612201121";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(Eq(PathToFile)))
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
    Tested Example{ IWindow, Path_t(PathToFile), TitleId };

    EXPECT_CALL(Document, RemoveReference())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title_Events)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;
  ::mock::Rocket::Core::Element Title;

  using namespace ::testing;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Title));

  Tested Example{ IWindow, "", "" };

  EXPECT_CALL(Example, DoMessage2())
    .Times(1);

  Window.m_Events[Tested::Message2]();
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Show)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };
  ITested_t & IExampe = Example;

  EXPECT_CALL(Document, Show())
    .Times(1);

  IExampe.Show();
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Hide)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };
  ITested_t & IExampe = Example;

  EXPECT_CALL(Document, Hide())
    .Times(1);

  IExampe.Hide();
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_GetId)
{
  Context_t Context;
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;
  const char * DocumentId = "Id1710102335";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetId())
    .Times(1)
    .WillOnce(Return(DocumentId));

  const auto Result = Example.GetId();
  EXPECT_EQ(DocumentId, Result);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_GetElement)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;
  ::mock::Rocket::Core::Element Element;
  const char * Id = "1701031206";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ IWindow, Path_t{} };

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
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;
  const ::mock::Rocket::Core::Vector2i Size = { 1701131210, 0 };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ IWindow, Path_t{} };

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
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;
  const ::mock::Rocket::Core::Vector2i Size = { 0, 1701131212 };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ IWindow, Path_t{} };

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
TEST_F(Layer_test, /*DISABLED_*/Test_EmployFontSize_UnexpectedDocumentTag)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;
  const ::mock::Rocket::Core::String TagName = "Tag1702091254";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  using namespace ::testing;

  EXPECT_CALL(Document, GetTagName())
    .Times(AtLeast(1))
    .WillOnce(Return(TagName));

  EXPECT_THROW(Example.EmployFontSize(0.0f), ::std::exception);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_EmployFontSize)
{
  Context_t Context;
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;

  using namespace ::testing;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

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

    const auto Result = Example.EmployFontSize(Percent);
    EXPECT_EQ(16.0, Result);
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

    const auto Result = Example.EmployFontSize(Percent);
    EXPECT_EQ(7.65f, Result);
  }
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Element_Focus)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;
  ::mock::Rocket::Core::Element Element;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, Focus())
    .Times(1);

  Example.GetElement("").SetFocus();
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Element_SetMeaning_Textarea)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  Document_t Document;
  ::mock::Rocket::Controls::ElementFormControl Element;
  const char * Tag = "textarea";
  const char * Type = "Type1902041652";
  const char * Value = u8"ValueЗначение1902041653";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, GetTagName())
    .Times(1)
    .WillOnce(Return(Tag));

  EXPECT_CALL(Element, GetAttribute(Eq("type"), _))
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Element, SetValue(Eq(Value)))
    .Times(1);

  using namespace ::alicorn::extension::std;

  Example.GetElement("").SetMeaning(string_cast<String, Locale::UTF8>(Value));
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Element_SetMeaning_Input_Text)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  Document_t Document;
  ::mock::Rocket::Controls::ElementFormControl Element;
  const char * Tag = "input";
  const char * Type = "text";
  const char * Value = u8"ValueЗначение1902041658";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, GetTagName())
    .Times(1)
    .WillOnce(Return(Tag));

  EXPECT_CALL(Element, GetAttribute(Eq("type"), _))
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Element, SetValue(Eq(Value)))
    .Times(1);

  using namespace ::alicorn::extension::std;

  Example.GetElement("").SetMeaning(string_cast<String, Locale::UTF8>(Value));
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Element_SetMeaning_Input_Range)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  Document_t Document;
  ::mock::Rocket::Controls::ElementFormControl Element;
  const char * Tag = "input";
  const char * Type = "range";
  const char * Value = u8"ValueЗначение1902041659";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, GetTagName())
    .Times(1)
    .WillOnce(Return(Tag));

  EXPECT_CALL(Element, GetAttribute(Eq("type"), _))
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Element, SetValue(Eq(Value)))
    .Times(1);

  using namespace ::alicorn::extension::std;

  Example.GetElement("").SetMeaning(string_cast<String, Locale::UTF8>(Value));
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Element_SetMeaning_InnerRml)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  Document_t Document;
  ::mock::Rocket::Core::Element Element;
  const char * Tag = "Tag1902041640";
  const char * Type = "Type1902041641";
  const char * Value = u8"Value1902041642Текст";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, GetTagName())
    .Times(1)
    .WillOnce(Return(Tag));

  EXPECT_CALL(Element, GetAttribute(Eq("type"), _))
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Element, SetInnerRML(Eq(Value)))
    .Times(1);

  using namespace ::alicorn::extension::std;

  Example.GetElement("").SetMeaning(string_cast<String, Locale::UTF8>(Value));
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Element_GetMeaning_Textarea)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  Document_t Document;
  ::mock::Rocket::Controls::ElementFormControl Element;
  const char * Tag = "textarea";
  const char * Type = "Type1902041715";
  const char * Value = u8"ValueТекст1902041716";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, GetTagName())
    .Times(1)
    .WillOnce(Return(Tag));

  EXPECT_CALL(Element, GetAttribute(Eq("type"), _))
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Element, GetValue())
    .Times(1)
    .WillOnce(Return(Value));

  using namespace ::alicorn::extension::std;

  const auto Result = Example.GetElement("").GetMeaning();
  EXPECT_EQ((string_cast<String, Locale::UTF8>(Value)), Result);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Element_GetMeaning_Input_Text)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  Document_t Document;
  ::mock::Rocket::Controls::ElementFormControl Element;
  const char * Tag = "input";
  const char * Type = "text";
  const char * Value = u8"ValueТекст1902041717";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, GetTagName())
    .Times(1)
    .WillOnce(Return(Tag));

  EXPECT_CALL(Element, GetAttribute(Eq("type"), _))
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Element, GetValue())
    .Times(1)
    .WillOnce(Return(Value));

  using namespace ::alicorn::extension::std;

  const auto Result = Example.GetElement("").GetMeaning();
  EXPECT_EQ((string_cast<String, Locale::UTF8>(Value)), Result);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Element_GetMeaning_Input_Range)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  Document_t Document;
  ::mock::Rocket::Controls::ElementFormControl Element;
  const char * Tag = "input";
  const char * Type = "range";
  const char * Value = u8"ValueТекст1902041718";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, GetTagName())
    .Times(1)
    .WillOnce(Return(Tag));

  EXPECT_CALL(Element, GetAttribute(Eq("type"), _))
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Element, GetValue())
    .Times(1)
    .WillOnce(Return(Value));

  using namespace ::alicorn::extension::std;

  const auto Result = Example.GetElement("").GetMeaning();
  EXPECT_EQ((string_cast<String, Locale::UTF8>(Value)), Result);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Element_GetMeaning_Select)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  Document_t Document;
  ::mock::Rocket::Controls::ElementFormControl Element;
  const char * Tag = "select";
  const char * Type = "Type1902041719";
  const char * Value = u8"ValueТекст1902041720";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, GetTagName())
    .Times(1)
    .WillOnce(Return(Tag));

  EXPECT_CALL(Element, GetAttribute(Eq("type"), _))
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Element, GetValue())
    .Times(1)
    .WillOnce(Return(Value));

  using namespace ::alicorn::extension::std;

  const auto Result = Example.GetElement("").GetMeaning();
  EXPECT_EQ((string_cast<String, Locale::UTF8>(Value)), Result);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Element_GetMeaning_InnerRml)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  Document_t Document;
  ::mock::Rocket::Core::Element Element;
  const char * Tag = "Tag1902041709";
  const char * Type = "Type1902041710";
  const char * Value = u8"ValueТекст1902041711";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, GetTagName())
    .Times(1)
    .WillOnce(Return(Tag));

  EXPECT_CALL(Element, GetAttribute(Eq("type"), _))
    .Times(1)
    .WillOnce(Return(Type));

  EXPECT_CALL(Element, GetInnerRML())
    .Times(1)
    .WillOnce(Return(Value));

  using namespace ::alicorn::extension::std;

  const auto Result = Example.GetElement("").GetMeaning();
  EXPECT_EQ((string_cast<String, Locale::UTF8>(Value)), Result);
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Element_SetClassStyle)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;

  Document_t Document;
  ::mock::Rocket::Core::Element Element;
  const char * Value = u8"ValueТекст1902041733";

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Window, LoadDocument(_))
    .Times(1)
    .WillOnce(Return(&Document));

  Tested Example{ IWindow, Path_t{} };

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Element));

  EXPECT_CALL(Element, SetAttribute(Eq("class"), Eq(Value)))
    .Times(1);

  using namespace ::alicorn::extension::std;

  Example.GetElement("").SetClassStyle(string_cast<String, Locale::UTF8>(Value));
}
