
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Layer.
*/

#define Layer_test Layer_windows_test
#define Layer Layer_windows

// ������������ ������ Layer
#include "..\..\Covellite.Rocket\Layer.cpp"
#include "..\..\Covellite.Rocket\Layer.windows.cpp"

#include "Common_test.hpp"

// ������� ������� ��� ����������� � ����� Layer 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Layer_test, Test_Function);

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor)
{
  Window Window;
  ::covellite::rocket::IWindow & IWindow = Window;
  Document_t Document;

  const char * SourcePathToFile = "Path/To/����\\1710301242";
  const char * ExpectedPathToFile = "Path\\To\\����\\1710301242";

  using namespace ::testing;

  EXPECT_CALL(Window, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  using namespace ::alicorn::extension::std;
  Locale::Scope::SetDefault<Locale::Ascii128> Dummy;

  const Tested Example{ IWindow, Path_t{ SourcePathToFile } };
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title)
{
  Window Window;
  ::covellite::rocket::IWindow & IWindow = Window;
  Document_t Document;
  ::mock::Rocket::Core::Element Title;

  const char * SourcePathToFile = "Path/To/����\\1710301244";
  const char * ExpectedPathToFile = "Path\\To\\����\\1710301244";

  using namespace ::testing;

  EXPECT_CALL(Window, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Title));

  using namespace ::alicorn::extension::std;
  Locale::Scope::SetDefault<Locale::Ascii128> Dummy;

  const Tested Example{ IWindow, Path_t{ SourcePathToFile }, "" };
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Deprecated)
{
  Context_t Context;
  Document_t Document;

  const char * SourcePathToFile = "Path/To/����\\1710301242";
  const char * ExpectedPathToFile = "Path\\To\\����\\1710301242";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  using namespace ::alicorn::extension::std;
  Locale::Scope::SetDefault<Locale::Ascii128> Dummy;

  const Tested Example{ &Context, Path_t{ SourcePathToFile } };
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title_Deprecated)
{
  Context_t Context;
  Document_t Document;
  ::mock::Rocket::Core::Element Title;

  const char * SourcePathToFile = "Path/To/����\\1710301244";
  const char * ExpectedPathToFile = "Path\\To\\����\\1710301244";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Title));

  using namespace ::alicorn::extension::std;
  Locale::Scope::SetDefault<Locale::Ascii128> Dummy;

  const Tested Example{ &Context, Path_t{ SourcePathToFile }, "" };
}
