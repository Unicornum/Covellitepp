
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Fx.
*/

// ����� �������� ����� ������ Fx
class Fx_test :
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
};

// ������� ������� ��� ����������� � ����� Fx 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Fx_test, Test_Function);

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_Render)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"Render.expected.fx", 
    THIS_DIRECTORY / L"../../Covellite.Api/Render/fx/Render.fx");
}
