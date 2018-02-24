
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса IWindow.
*/

#include <Covellite\Core.mock.hpp>

// Расположение класса IWindow
#include <Covellite\Core\IWindow.hpp>

// Общий тестовый класс класса IWindow
class IWindow_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::IWindow;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс IWindow 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(IWindow_test, Test_Function);

// ************************************************************************** //
TEST_F(IWindow_test, /*DISABLED_*/Test_VirtualDestructor)
{
  class Tested :
    public Tested_t
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD0(Destructor, void(void));
    };

  public:
    void Subscribe(const EventHandlerPtr_t &) override {}

  public:
    ~Tested(void) noexcept
    {
      Proxy::GetInstance()->Destructor();
    }
  };

  // Почему не работает ::std::make_shared<>()???
  Tested_t * pExample = new Tested;

  using Proxy_t = Tested::Proxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  using namespace ::testing;

  EXPECT_CALL(Proxy, Destructor())
    .Times(1);

  delete pExample;
}
