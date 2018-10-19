
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса IGraphicApi.
*/

// Расположение класса IGraphicApi
#include "..\..\Covellite.Api\Renderer\IGraphicApi.hpp"

// Общий тестовый класс класса IGraphicApi
class IGraphicApi_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::IGraphicApi;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс IGraphicApi 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
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
