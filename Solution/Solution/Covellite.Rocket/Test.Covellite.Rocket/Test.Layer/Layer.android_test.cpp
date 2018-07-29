
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Layer.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define Layer_test Layer_android_test
#define Layer Layer_android

// Расположение класса Layer
#include "..\..\Covellite.Rocket\Layer.cpp"
#include "..\..\Covellite.Rocket\Layer.android.cpp"

#include "Common_test.hpp"

// Образец макроса для подстановки в класс Layer 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Layer_test, Test_Function);

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor)
{
  Window Window;
  ::covellite::rocket::IWindow & IWindow = Window;
  Document_t Document;

  const char * SourcePathToFile = "Path\\To\\File/1710301246";
  const char * ExpectedPathToFile = "Path/To/File/1710301246";

  using namespace ::testing;

  EXPECT_CALL(Window, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ IWindow, Path_t{ SourcePathToFile } };
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title)
{
  Window Window;
  ::covellite::rocket::IWindow & IWindow = Window;
  Document_t Document;
  ::mock::Rocket::Core::Element Title;

  const char * SourcePathToFile = "Path\\To\\File/1710301248";
  const char * ExpectedPathToFile = "Path/To/File/1710301248";

  using namespace ::testing;

  EXPECT_CALL(Window, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Title));

  const Tested Example{ IWindow, Path_t{ SourcePathToFile }, "" };
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Deprecated)
{
  Context_t Context;
  Document_t Document;

  const char * SourcePathToFile = "Path\\To\\File/1710301246";
  const char * ExpectedPathToFile = "Path/To/File/1710301246";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ &Context, Path_t{ SourcePathToFile } };
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title_Deprecated)
{
  Context_t Context;
  Document_t Document;
  ::mock::Rocket::Core::Element Title;

  const char * SourcePathToFile = "Path\\To\\File/1710301248";
  const char * ExpectedPathToFile = "Path/To/File/1710301248";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Title));

  const Tested Example{ &Context, Path_t{ SourcePathToFile }, "" };
}
