
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ IGraphicApi.
*/

// ������������ ������ IGraphicApi
#include "..\..\Covellite.Api\Renderer\IGraphicApi.hpp"

// ����� �������� ����� ������ IGraphicApi
class IGraphicApi_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::IGraphicApi;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� IGraphicApi 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(IGraphicApi_test, Test_Function);

// ************************************************************************** //
TEST_F(IGraphicApi_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

TEST_F(IGraphicApi_test, /*DISABLED_*/Test_ITexture_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t::ITexture>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t::ITexture>::value);
}

TEST_F(IGraphicApi_test, /*DISABLED_*/Test_IGeometry_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t::IGeometry>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t::IGeometry>::value);
}
