
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
#include "..\..\Covellite.Gui\Layer.cpp"
#include "..\..\Covellite.Gui\Layer.windows.cpp"

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
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;

  const char * SourcePathToFile = "Path/To/����\\1710301242";
  const char * ExpectedPathToFile = "Path\\To\\����\\1710301242";

  using namespace ::testing;

  EXPECT_CALL(Window, DoLoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ IWindow, Path_t{ SourcePathToFile } };
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;
  ::mock::CovelliteGui::Core::Element Title;

  const char * SourcePathToFile = "Path/To/����\\1710301244";
  const char * ExpectedPathToFile = "Path\\To\\����\\1710301244";

  using namespace ::testing;

  EXPECT_CALL(Window, DoLoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Title));

  const Tested Example{ IWindow, Path_t{ SourcePathToFile }, "" };
}
