
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса ApplicationEntryPoint.
*/

#include <Covellite\App.mock.hpp>

#undef APPLICATION_ENTRY_POINT

// Расположение класса ApplicationEntryPoint
#include <Covellite\App\ApplicationEntryPoint.hpp>

namespace covellite
{

// Общий тестовый класс класса ApplicationEntryPoint
class ApplicationEntryPoint_test :
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
  class Application { };
  APPLICATION_ENTRY_POINT(Application);
};

// Образец макроса для подстановки в класс ApplicationEntryPoint 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(ApplicationEntryPoint_test, Test_Function);

// ************************************************************************** //
TEST_F(ApplicationEntryPoint_test, /*DISABLED_*/Test_WinMain)
{
  using ApplicationProxy_t = ::mock::covellite::app::Application::Proxy;
  ApplicationProxy_t ApplicationProxy;
  ApplicationProxy_t::GetInstance() = &ApplicationProxy;

  char Params[] = "Params1710191038";

  using namespace ::testing;

  EXPECT_CALL(ApplicationProxy, Main(Eq(typeid(Application).name()), Params))
    .Times(1);

#pragma warning(push)
#pragma warning(disable: 6387)

  const auto Result = WinMain(0, 0, Params, 0);
  EXPECT_EQ(0, Result);

#pragma warning(pop)
}

} // namespace covellite
