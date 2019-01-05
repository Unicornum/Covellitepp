
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
TEST_F(Fx_test, /*DISABLED_*/Test_Data)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"Data.expected.h",
    THIS_DIRECTORY / L"../../Covellite.Api/Renderer/fx/Data.h");
}

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_Input)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"Input.expected.fx",
    THIS_DIRECTORY / L"../../Covellite.Api/Renderer/fx/Input.fx");
}

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_Vertex)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"Vertex.expected.fx",
    THIS_DIRECTORY / L"../../Covellite.Api/Renderer/fx/Vertex.fx");
}

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_Pixel)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"Pixel.expected.fx",
    THIS_DIRECTORY / L"../../Covellite.Api/Renderer/fx/Pixel.fx");
}
