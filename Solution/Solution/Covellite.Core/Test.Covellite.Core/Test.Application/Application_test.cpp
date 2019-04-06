
#include "stdafx.h"
#include <Platform\Windows.mock.hpp>
#include <Platform\Android.mock.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Application.
*/

#include <Covellite\Core.mock.hpp>

// Расположение класса Application
#include "..\..\Covellite.Core\Application.windows.cpp"

#include <Covellite\Core\IWindow.hpp>
#include <Covellite\Core\Event.hpp>

// Общий тестовый класс класса Application
class Application_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::Application;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс Application 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Application_test, Test_Function);

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

namespace covellite
{

namespace core
{

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_MakeWindow)
{
  class ExampleWindow :
    public ::covellite::core::IWindow
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD0(Constructor, ::mock::Id_t(void));
      MOCK_METHOD1(Destructor, void(::mock::Id_t));
      MOCK_METHOD2(Subscribe, void(::mock::Id_t, EventHandlerPtr_t));
    };

  public:
    void Subscribe(const EventHandlerPtr_t & _pEvents) override
    {
      Proxy::GetInstance()->Subscribe(m_Id, _pEvents);
    }

  public:
    const ::mock::Id_t m_Id;
    const int m_IntValue;
    const bool m_BoolValue;

  public:
    explicit ExampleWindow(int _IntValue = 0, bool _BoolValue = false) :
      m_Id(Proxy::GetInstance()->Constructor()),
      m_IntValue(_IntValue),
      m_BoolValue(_BoolValue)
    {

    }
    ~ExampleWindow(void) noexcept
    { 
      Proxy::GetInstance()->Destructor(m_Id); 
    }
  };

  using WindowProxy_t = ExampleWindow::Proxy;
  WindowProxy_t WindowProxy;
  WindowProxy_t::GetInstance() = &WindowProxy;

  const ::mock::Id_t WindowId1 = 1709261037;
  const ::mock::Id_t WindowId2 = 1709261038;
  const ::mock::Id_t WindowId3 = 1710302034;

  Tested_t Example{ [](void) {} };

  int Count = 0;

  using namespace ::testing;

  InSequence Dummy;

  {
    Count++;

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowId1));

    EXPECT_CALL(WindowProxy, Subscribe(WindowId1, Example.m_pEvents))
      .Times(1);

    auto pWindow = Example.MakeWindow<ExampleWindow>();
    EXPECT_EQ(WindowId1, pWindow->m_Id);
    EXPECT_EQ(0, pWindow->m_IntValue);
    EXPECT_EQ(false, pWindow->m_BoolValue);
    ASSERT_EQ(Count, Example.m_Windows.size());
  }

  {
    Count++;
    const int IntValue = 1709261039;

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowId2));

    EXPECT_CALL(WindowProxy, Subscribe(WindowId2, Example.m_pEvents))
      .Times(1);

    auto pWindow = Example.MakeWindow<ExampleWindow>(IntValue);
    EXPECT_EQ(WindowId2, pWindow->m_Id);
    EXPECT_EQ(IntValue, pWindow->m_IntValue);
    EXPECT_EQ(false, pWindow->m_BoolValue);
    ASSERT_EQ(Count, Example.m_Windows.size());
  }

  {
    Count++;
    const int IntValue = 1709261040;

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowId3));

    EXPECT_CALL(WindowProxy, Subscribe(WindowId3, Example.m_pEvents))
      .Times(1);

    auto pWindow = Example.MakeWindow<ExampleWindow>(IntValue, true);
    EXPECT_EQ(WindowId3, pWindow->m_Id);
    EXPECT_EQ(IntValue, pWindow->m_IntValue);
    EXPECT_EQ(true, pWindow->m_BoolValue);
    ASSERT_EQ(Count, Example.m_Windows.size());
  }

  EXPECT_CALL(WindowProxy, Destructor(WindowId3))
    .Times(1);

  EXPECT_CALL(WindowProxy, Destructor(WindowId2))
    .Times(1);

  EXPECT_CALL(WindowProxy, Destructor(WindowId1))
    .Times(1);
}

} // namespace core

} // namespace covellite
