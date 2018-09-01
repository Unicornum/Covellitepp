
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Events.
*/

// Расположение класса Events
#include <Covellite/Events/IEvents.hpp>
#include <Covellite/Events/Events.hpp>

class IApplication :
  public ::covellite::events::IEvents
{
public:
  MOCK_CONST_METHOD0(Call1, void(void));
  MOCK_CONST_METHOD1(Call1, void(const ::std::string &));
  MOCK_CONST_METHOD0(Call2, void(void));
  MOCK_CONST_METHOD1(ProcessException, void(const ::std::string &));
};

class Application :
  public IApplication
{
public:
  enum Id
  {
    One = 0,
    Two,
  };

public:
  operator ::covellite::events::Events (void) const override
  {
    return m_Events;
  }

private:
  ::covellite::events::Events m_Events;

public:
  Application(void);
};

class Window :
  public ::covellite::events::IEvents
{
public:
  enum Id
  {
    One = 0,
    Two,
  };

public:
  MOCK_METHOD0(Call1, void(void));
  MOCK_METHOD1(Call1, void(const ::std::string &));
  MOCK_METHOD0(Call2, void(void));

public:
  operator ::covellite::events::Events (void) const override
  {
    return m_Events;
  }

private:
  ::covellite::events::Events m_Events;

public:
  explicit Window(const IApplication &);
};

class Window2 :
  public ::covellite::events::IEvents
{
public:
  enum Id
  {
    One = 0,
    Two,
  };

public:
  operator ::covellite::events::Events (void) const override
  {
    return m_Events;
  }

private:
  ::covellite::events::Events m_Events;

public:
  explicit Window2(const IApplication &);
};

#include <Covellite/Events.hpp>

Application::Application(void)
{
  using namespace ::covellite::events;

  m_Events[Application::One].Connect([&]() { Call1(); });
  m_Events[Application::One].Connect([&](const ::std::string & _Value) { Call1(_Value); });
  m_Events[Application::Two].Connect([&]() { Call2(); });
  m_Events[Error.Exception].Connect([&](const Error_t::Description & _Value) { ProcessException(_Value); });
}

Window::Window(const IApplication & _Application) :
  m_Events(_Application)
{
  using namespace ::covellite::events;

  m_Events[Window::One].Connect([&]() { Call1(); });
  m_Events[Window::One].Connect([&](const ::std::string & _Value) { Call1(_Value); });
  m_Events[Window::Two].Connect([&]() { Call2(); });
}

Window2::Window2(const IApplication & _Application) :
  m_Events(_Application)
{
  using namespace ::covellite::events;

  m_Events[Application::One].Connect([&]() { _Application.Call1(); });
  m_Events[Application::One].Connect([&](const ::std::string & _Value) { _Application.Call1(_Value); });
  m_Events[Application::Two].Connect([&]() { _Application.Call2(); });
}

// Общий тестовый класс класса Events
class Events_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::events::Events;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс Events 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Events_test, Test_Function);

// ************************************************************************** //
TEST_F(Events_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Events_test, /*DISABLED_*/Test_Using_BaseObject_One)
{
  const ::std::string Value = "Value1806171326";

  Application Example;
  Window Window{ Example };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, Call1())
    .Times(1);

  EXPECT_CALL(Example, Call1(Value))
    .Times(1);

  EXPECT_CALL(Example, Call2())
    .Times(0);

  EXPECT_CALL(Window, Call1())
    .Times(0);

  EXPECT_CALL(Window, Call1(_))
    .Times(0);

  EXPECT_CALL(Window, Call2())
    .Times(0);

  EXPECT_CALL(Example, ProcessException(_))
    .Times(0);

  Tested_t Events = Example;
  const auto Result = Events[Application::One](Value);
  EXPECT_TRUE(Result);
}

// ************************************************************************** //
TEST_F(Events_test, /*DISABLED_*/Test_Using_InheritedObject_One)
{
  const ::std::string Value = "Value1806171349";

  Application Example;
  Window Window{ Example };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, Call1())
    .Times(0);

  EXPECT_CALL(Example, Call1(_))
    .Times(0);

  EXPECT_CALL(Example, Call2())
    .Times(0);

  EXPECT_CALL(Window, Call1())
    .Times(1);

  EXPECT_CALL(Window, Call1(Value))
    .Times(1);

  EXPECT_CALL(Window, Call2())
    .Times(0);

  EXPECT_CALL(Example, ProcessException(_))
    .Times(0);

  Tested_t Events = Window;
  const auto Result = Events[Window::One](Value);
  EXPECT_TRUE(Result);
}

// ************************************************************************** //
TEST_F(Events_test, /*DISABLED_*/Test_Using_BaseObject_Two)
{
  Application Example;
  Window Window{ Example };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, Call1())
    .Times(0);

  EXPECT_CALL(Example, Call1(_))
    .Times(0);

  EXPECT_CALL(Example, Call2())
    .Times(1);

  EXPECT_CALL(Window, Call1())
    .Times(0);

  EXPECT_CALL(Window, Call1(_))
    .Times(0);

  EXPECT_CALL(Window, Call2())
    .Times(0);

  EXPECT_CALL(Example, ProcessException(_))
    .Times(0);

  Tested_t Events = Example;
  const auto Result = Events[Application::Two]();
  EXPECT_TRUE(Result);
}

// ************************************************************************** //
TEST_F(Events_test, /*DISABLED_*/Test_Using_InheritedObject_Two)
{
  Application Example;
  Window Window{ Example };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, Call1())
    .Times(0);

  EXPECT_CALL(Example, Call1(_))
    .Times(0);

  EXPECT_CALL(Example, Call2())
    .Times(0);

  EXPECT_CALL(Window, Call1())
    .Times(0);

  EXPECT_CALL(Window, Call1(_))
    .Times(0);

  EXPECT_CALL(Window, Call2())
    .Times(1);

  EXPECT_CALL(Example, ProcessException(_))
    .Times(0);

  Tested_t Events = Window;
  const auto Result = Events[Window::Two]();
  EXPECT_TRUE(Result);
}

// ************************************************************************** //
TEST_F(Events_test, /*DISABLED_*/Test_ProcessException)
{
  const ::std::string Value = "Value1806171356";

  Application Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, Call2())
    .Times(1)
    .WillOnce(Throw(::std::exception{ Value.c_str() }));

  EXPECT_CALL(Example, ProcessException(Value))
    .Times(1);

  Tested_t Events = Example;
  const auto Result = Events[Application::Two]();
  EXPECT_FALSE(Result);
}

// ************************************************************************** //
TEST_F(Events_test, /*DISABLED_*/Test_InvalidParameter)
{
  Application Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, Call1())
    .Times(1);

  EXPECT_CALL(Example, ProcessException(_))
    .Times(1);

  Tested_t Events = Example;
  const auto Result = Events[Application::One](1806181339);
  EXPECT_FALSE(Result);
}

// ************************************************************************** //
TEST_F(Events_test, /*DISABLED_*/Test_DisconnectSlots)
{
  Application Example;
  Tested_t Events = Example;

  using namespace ::testing;

  InSequence Dummy;

  {
    Window2 Window{ Example };

    EXPECT_CALL(Example, Call1())
      .Times(2);

    Events[Application::One](::std::string{});

    EXPECT_CALL(Example, Call2())
      .Times(2);

    Events[Application::Two]();
  }

  EXPECT_CALL(Example, Call1())
    .Times(1);

  Events[Application::One](::std::string{});

  EXPECT_CALL(Example, Call2())
    .Times(1);

  const auto Result = Events[Application::Two]();
  EXPECT_TRUE(Result);
}

// ************************************************************************** //
TEST_F(Events_test, /*DISABLED_*/Test_ActivateEmptySlot)
{
  Application Example;
  Window2 Window{ Example };
  Tested_t Events = Window;

  const auto Result = Events[Window2::Two]();
  EXPECT_FALSE(Result);
}
