
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Application.
*/

#include <Covellite\App.mock.hpp>

// Расположение класса Application
#include "..\..\Covellite.App\Application.windows.cpp"

#include <Covellite\App\IWindow.hpp>

// Общий тестовый класс класса Application
class Application_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::app::Application;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<ATOM>::Set(1);
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<ATOM>::Clear();
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

namespace app
{

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_MakeWindow)
{
  class ExampleWindow :
    public ::covellite::app::IWindow
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD0(Constructor, ::mock::Id_t(void));
      MOCK_METHOD1(Destructor, void(::mock::Id_t));
    };

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

    auto & Window = Example.MakeWindow<ExampleWindow>();
    EXPECT_EQ(WindowId1, Window.m_Id);
    EXPECT_EQ(0, Window.m_IntValue);
    EXPECT_EQ(false, Window.m_BoolValue);
    ASSERT_EQ(Count, Example.m_Windows.size());
  }

  {
    Count++;
    const int IntValue = 1709261039;

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowId2));

    auto & Window = Example.MakeWindow<ExampleWindow>(IntValue);
    EXPECT_EQ(WindowId2, Window.m_Id);
    EXPECT_EQ(IntValue, Window.m_IntValue);
    EXPECT_EQ(false, Window.m_BoolValue);
    ASSERT_EQ(Count, Example.m_Windows.size());
  }

  {
    Count++;
    const int IntValue = 1709261040;

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowId3));

    auto & Window = Example.MakeWindow<ExampleWindow>(IntValue, true);
    EXPECT_EQ(WindowId3, Window.m_Id);
    EXPECT_EQ(IntValue, Window.m_IntValue);
    EXPECT_EQ(true, Window.m_BoolValue);
    ASSERT_EQ(Count, Example.m_Windows.size());
  }

  EXPECT_CALL(WindowProxy, Destructor(WindowId3))
    .Times(1);

  EXPECT_CALL(WindowProxy, Destructor(WindowId2))
    .Times(1);

  EXPECT_CALL(WindowProxy, Destructor(WindowId1))
    .Times(1);
}

} // namespace app

} // namespace covellite

  // ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_CallWindowDestructorOrder)
{
  class ExampleWindow :
    public ::covellite::app::IWindow
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD1(Constructor, ::mock::Id_t(::mock::Id_t));
      MOCK_METHOD1(Destructor, void(::mock::Id_t));
    };

  public:
    const ::mock::Id_t m_Id;

  public:
    ExampleWindow(void) :
      m_Id(Proxy::GetInstance()->Constructor(0))
    {

    }
    explicit ExampleWindow(const ExampleWindow & _Window) :
      m_Id(Proxy::GetInstance()->Constructor(_Window.m_Id))
    {

    }
    ~ExampleWindow(void) noexcept
    {
      Proxy::GetInstance()->Destructor(m_Id);
    }
  };

  class Tested :
    public Tested_t
  {
  public:
    explicit Tested(Continuous) :
      Tested_t(Continuous{})
    {
      MakeWindow<ExampleWindow>(
        MakeWindow<ExampleWindow>(
          MakeWindow<ExampleWindow>(
            MakeWindow<ExampleWindow>())));
    }

    explicit Tested(EventBased) :
      Tested_t(EventBased{})
    {
      MakeWindow<ExampleWindow>(
        MakeWindow<ExampleWindow>(
          MakeWindow<ExampleWindow>(
            MakeWindow<ExampleWindow>())));
    }
  };

  using WindowProxy_t = ExampleWindow::Proxy;
  WindowProxy_t WindowProxy;
  WindowProxy_t::GetInstance() = &WindowProxy;

  const ::mock::Id_t WindowId1 = 1710302125;
  const ::mock::Id_t WindowId2 = 1710302126;
  const ::mock::Id_t WindowId3 = 1710302127;
  const ::mock::Id_t WindowId4 = 1710302128;

  using namespace ::testing;

  {
    InSequence Dummy;

    EXPECT_CALL(WindowProxy, Constructor(0))
      .Times(1)
      .WillOnce(Return(WindowId1));

    EXPECT_CALL(WindowProxy, Constructor(WindowId1))
      .Times(1)
      .WillOnce(Return(WindowId2));

    EXPECT_CALL(WindowProxy, Constructor(WindowId2))
      .Times(1)
      .WillOnce(Return(WindowId3));

    EXPECT_CALL(WindowProxy, Constructor(WindowId3))
      .Times(1)
      .WillOnce(Return(WindowId4));

    EXPECT_CALL(WindowProxy, Destructor(_))
      .Times(0);

    Tested Example{ Tested_t::Continuous{} };

    EXPECT_CALL(WindowProxy, Destructor(WindowId4))
      .Times(1);

    EXPECT_CALL(WindowProxy, Destructor(WindowId3))
      .Times(1);

    EXPECT_CALL(WindowProxy, Destructor(WindowId2))
      .Times(1);

    EXPECT_CALL(WindowProxy, Destructor(WindowId1))
      .Times(1);
  }

  {
    InSequence Dummy;

    EXPECT_CALL(WindowProxy, Constructor(0))
      .Times(1)
      .WillOnce(Return(WindowId1));

    EXPECT_CALL(WindowProxy, Constructor(WindowId1))
      .Times(1)
      .WillOnce(Return(WindowId2));

    EXPECT_CALL(WindowProxy, Constructor(WindowId2))
      .Times(1)
      .WillOnce(Return(WindowId3));

    EXPECT_CALL(WindowProxy, Constructor(WindowId3))
      .Times(1)
      .WillOnce(Return(WindowId4));

    EXPECT_CALL(WindowProxy, Destructor(_))
      .Times(0);

    Tested Example{ Tested_t::EventBased{} };

    EXPECT_CALL(WindowProxy, Destructor(WindowId4))
      .Times(1);

    EXPECT_CALL(WindowProxy, Destructor(WindowId3))
      .Times(1);

    EXPECT_CALL(WindowProxy, Destructor(WindowId2))
      .Times(1);

    EXPECT_CALL(WindowProxy, Destructor(WindowId1))
      .Times(1);
  }
}
