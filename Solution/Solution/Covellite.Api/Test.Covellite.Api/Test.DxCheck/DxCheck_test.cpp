
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ DxCheck.
*/

// ������������ ������ DxCheck
#include "..\..\Covellite.Api\Render\DxCheck.hpp"

// ����� �������� ����� ������ DxCheck
class DxCheck_test :
  public ::testing::Test
{
protected:
  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }

protected:
  HRESULT Test(HRESULT _Value) { return _Value; }
};

// ������� ������� ��� ����������� � ����� DxCheck 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(DxCheck_test, Test_Function);

// ************************************************************************** //
TEST_F(DxCheck_test, /*DISABLED_*/Test_Successfull)
{
  EXPECT_NO_THROW(DX_CHECK Test(S_OK));
}

// ************************************************************************** //
TEST_F(DxCheck_test, /*DISABLED_*/Test_Failed1)
{
  EXPECT_STDEXCEPTION(DX_CHECK Test(DXGI_ERROR_INVALID_CALL),
    ".+dxcheck_test\\.cpp \\([0-9]+\\): Unknown error 0x887A0001");
}

// ************************************************************************** //
TEST_F(DxCheck_test, /*DISABLED_*/Test_Failed2)
{
  EXPECT_STDEXCEPTION(DX_CHECK Test(DXGI_ERROR_ACCESS_DENIED),
    ".+dxcheck_test\\.cpp \\([0-9]+\\): Unknown error 0x887A002B");
}
