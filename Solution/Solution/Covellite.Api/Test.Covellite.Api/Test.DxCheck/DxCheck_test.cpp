
#include "stdafx.h"
#include <Platform/Windows.mock.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса DxCheck.
*/

// Расположение класса DxCheck
#include "..\..\Covellite.Api\Renderer\DxCheck.hpp"

// Общий тестовый класс класса DxCheck
class DxCheck_test :
  public ::testing::Test
{
protected:
  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }

protected:
  HRESULT Test(HRESULT _Value) { return _Value; }
};

// Образец макроса для подстановки в класс DxCheck 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(DxCheck_test, Test_Function);

// ************************************************************************** //
TEST_F(DxCheck_test, /*DISABLED_*/Test_Successfull)
{
  EXPECT_NO_THROW(DX_CHECK Test(S_OK));
}

// ************************************************************************** //
TEST_F(DxCheck_test, /*DISABLED_*/Test_Failed1)
{
  EXPECT_STDEXCEPTION(DX_CHECK Test(D3D10_ERROR_FILE_NOT_FOUND - 2),
    ".+[Dd]x[Cc]heck_test\\.cpp \\([0-9]+\\): Unknown error 0x88790000");
}

// ************************************************************************** //
TEST_F(DxCheck_test, /*DISABLED_*/Test_Failed2)
{
  EXPECT_STDEXCEPTION(DX_CHECK Test(D3D10_ERROR_FILE_NOT_FOUND - 3),
    ".+[Dd]x[Cc]heck_test\\.cpp \\([0-9]+\\): Unknown error 0x8878FFFF");
}
