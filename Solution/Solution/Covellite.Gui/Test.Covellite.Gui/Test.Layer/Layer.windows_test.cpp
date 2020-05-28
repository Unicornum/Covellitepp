
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Layer.
*/

#define Layer_test Layer_windows_test
#define Layer Layer_windows

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
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor)
{
  Window Window;
  ::covellite::gui::IWindow & IWindow = Window;
  Document_t Document;

  const char * SourcePathToFile = "Path/To/Файл\\1710301242";
  const char * ExpectedPathToFile = "Path\\To\\Файл\\1710301242";

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

  const char * SourcePathToFile = "Path/To/Файл\\1710301244";
  const char * ExpectedPathToFile = "Path\\To\\Файл\\1710301244";

  using namespace ::testing;

  EXPECT_CALL(Window, DoLoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Title));

  const Tested Example{ IWindow, Path_t{ SourcePathToFile }, "" };
}
