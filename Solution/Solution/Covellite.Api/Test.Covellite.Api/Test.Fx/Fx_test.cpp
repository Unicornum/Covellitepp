
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
TEST_F(Fx_test, /*DISABLED_*/Test_Pixel)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"Pixel.expected.fx",
    THIS_DIRECTORY / L"../../Covellite.Api/Renderer/fx/Pixel.fx");
}

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_PixelInput)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"PixelInput.expected.fx",
    THIS_DIRECTORY / L"../../Covellite.Api/Renderer/fx/PixelInput.fx");
}

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_Vertex)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"Vertex.expected.fx",
    THIS_DIRECTORY / L"../../Covellite.Api/Renderer/fx/Vertex.fx");
}

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_VertexInput)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"VertexInput.expected.fx",
    THIS_DIRECTORY / L"../../Covellite.Api/Renderer/fx/VertexInput.fx");
}
