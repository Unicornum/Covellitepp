
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Application.
*/

#include <Covellite\Core.mock.hpp>

#define Application_test Application_windows_test
#define Application Application_windows

// Расположение класса Application
#include "..\..\Covellite.Core\Application.windows.cpp"

#undef MessageBox

#include <Covellite\Core\IWindow.hpp>
#include <Covellite\Core\Event.hpp>

// Общий тестовый класс класса Application
class Application_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::Application;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }

protected:
  void DoCommand(const Tested_t & _Example, int32_t _Command)
  {
    _Example.m_pEvents->DoCommand(_Command, ::covellite::core::params::Empty{});
  }

  class Tested :
    public Tested_t
  {
  public:
    explicit Tested(const Run_t & _Run = [](void) {}) : Tested_t(_Run) {}
  };

  class TestedRun :
    public Tested_t
  {
  public:
    MOCK_CONST_METHOD0(DoCreate, void(void));
    MOCK_CONST_METHOD0(DoClose, void(void));
    MOCK_CONST_METHOD0(DoDestroy, void(void));

  public:
    template<class T>
    explicit TestedRun(T _Type) :
      Tested_t(_Type)
    {
      using namespace ::covellite::core;

      (*m_pEvents)[Event::Create]
        .connect(::std::bind(&TestedRun::DoCreate, this));
      (*m_pEvents)[Event::Close]
        .connect(::std::bind(&TestedRun::DoClose, this));
      (*m_pEvents)[Event::Destroy]
        .connect(::std::bind(&TestedRun::DoDestroy, this));
    }
  };
};

// Образец макроса для подстановки в класс Application 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Application_test, Test_Function);

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Main)
{
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD0(Run, void(void));
  };

  using Proxy_t = Proxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  class Real :
    public Tested
  {
  public:
    Real(void) : Tested([&](void) { Proxy_t::GetInstance()->Run(); }) { }
  };

  using namespace ::testing;

  EXPECT_CALL(Proxy, Run())
    .Times(1);

  Tested_t::Main<Real>("");
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Main_StdException)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  class Real :
    public Tested
  {
  public:
    static const char * GetError(void)
    {
      static const char Error[] = "Error1710192145";
      return Error;
    }

  public:
    Real(void) : Tested([&](void) { throw ::std::exception{ GetError() }; }) { }
  };

  using namespace ::testing;

  EXPECT_CALL(Proxy, MessageBoxA(NULL, 
    Eq((::std::string{ "Exception: " } + Real::GetError() + ".").c_str()), 
    Eq("Covellite++"), MB_OK))
    .Times(1);

  Tested_t::Main<Real>("");
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Main_UnknowException)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  class Real :
    public Tested
  {
  public:
    Real(void) : Tested([&](void) { throw 1; }) { }
  };

  using namespace ::testing;

  EXPECT_CALL(Proxy,
    MessageBoxA(NULL, Eq("Exception: unknown."), Eq("Covellite++"), MB_OK))
    .Times(1);

  Tested_t::Main<Real>("");
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_GetCommandLine)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  char CommandLine[] = "CommandLine1711041148";

  const Tested Example;

  using namespace ::testing;

  EXPECT_CALL(Proxy, GetCommandLineA())
    .WillOnce(Return(nullptr))
    .WillOnce(Return(CommandLine));

  const auto Empty = Example.GetCommandLine();
  EXPECT_EQ("", Empty);

  const auto Result = Example.GetCommandLine();
  EXPECT_EQ(CommandLine, Result);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Run_DrawingMode_Continuous)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  using EventHandlerProxy_t = ::mock::covellite::core::EventHandler::Proxy;
  EventHandlerProxy_t EventHandlerProxy;
  EventHandlerProxy_t::GetInstance() = &EventHandlerProxy;

  const ::mock::Id_t EventHandlerId = 1710231250;

  android_app App;

  const AppInfo_t Info{ &App };

  using namespace ::testing;

  EXPECT_CALL(EventHandlerProxy, Constructor())
    .Times(1)
    .WillOnce(Return(EventHandlerId));

  const TestedRun Example{ Tested_t::Continuous{} };

  InSequence Dummy;

  EXPECT_CALL(Example, DoCreate())
    .Times(1);

  // 1 итерация цикла
  {
    EXPECT_CALL(Proxy, PeekMessage(0, 0, 0, PM_REMOVE))
      .Times(1)
      .WillOnce(Return(FALSE));

    EXPECT_CALL(EventHandlerProxy, PostCommandDrawWindow(EventHandlerId))
      .Times(1);

    EXPECT_CALL(Proxy, Sleep(0))
      .Times(1);
  }

  // 2 итерация цикла
  {
    EXPECT_CALL(Proxy, PeekMessage(0, 0, 0, PM_REMOVE))
      .Times(1)
      .WillOnce(Return(TRUE));

    MSG Message;
    Message.message = 1710231325;
    Message.hwnd = (HWND)1710231326;
    Message.lParam = 1710231327;
    Message.wParam = 1710231328;
    Message.pt = { 1710231329, 1710231331 };
    Message.time = 1710231330;

    EXPECT_CALL(Proxy, BuildMessage())
      .Times(1)
      .WillOnce(Return(Message));

    EXPECT_CALL(Proxy, TranslateMessage(Message))
      .Times(1);

    EXPECT_CALL(Proxy, DispatchMessageW(Message))
      .Times(1);

    EXPECT_CALL(EventHandlerProxy, PostCommandDrawWindow(EventHandlerId))
      .Times(1);

    EXPECT_CALL(Proxy, Sleep(0))
      .Times(1);
  }

  // 3 итерация цикла
  {
    EXPECT_CALL(Proxy, PeekMessage(0, 0, 0, PM_REMOVE))
      .Times(1)
      .WillOnce(Return(TRUE));

    MSG MessageQuit = { 0 };
    MessageQuit.message = WM_QUIT;

    EXPECT_CALL(Proxy, BuildMessage())
      .Times(1)
      .WillOnce(Return(MessageQuit));
  }

  EXPECT_CALL(Example, DoClose())
    .Times(1);

  EXPECT_CALL(Example, DoDestroy())
    .Times(1);

  Example.Run();
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Run_DrawingMode_EventBased)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  using EventHandlerProxy_t = ::mock::covellite::core::EventHandler::Proxy;
  EventHandlerProxy_t EventHandlerProxy;
  EventHandlerProxy_t::GetInstance() = &EventHandlerProxy;

  const ::mock::Id_t EventHandlerId = 1710231250;

  android_app App;

  const AppInfo_t Info{ &App };

  using namespace ::testing;

  EXPECT_CALL(EventHandlerProxy, Constructor())
    .Times(1)
    .WillOnce(Return(EventHandlerId));

  const TestedRun Example{ Tested_t::EventBased{} };

  InSequence Dummy;

  EXPECT_CALL(Example, DoCreate())
    .Times(1);

  // 1 итерация цикла
  {
    EXPECT_CALL(Proxy, GetMessageW(0, 0, 0))
      .Times(1)
      .WillOnce(Return(FALSE));

    MSG Message;
    Message.message = 1710231346;
    Message.hwnd = (HWND)1710231347;
    Message.lParam = 1710231348;
    Message.wParam = 1710231349;
    Message.pt = { 1710231350, 1710231351 };
    Message.time = 1710231352;

    EXPECT_CALL(Proxy, BuildMessage())
      .Times(1)
      .WillOnce(Return(Message));

    EXPECT_CALL(Proxy, TranslateMessage(Message))
      .Times(1);

    EXPECT_CALL(Proxy, DispatchMessageW(Message))
      .Times(1);

    EXPECT_CALL(EventHandlerProxy, PostCommandDrawWindow(EventHandlerId))
      .Times(1);

    EXPECT_CALL(Proxy, Sleep(0))
      .Times(1);
  }

  // 2 итерация цикла
  {
    EXPECT_CALL(Proxy, GetMessageW(0, 0, 0))
      .Times(1)
      .WillOnce(Return(TRUE));

    MSG Message;
    Message.message = 1710231346;
    Message.hwnd = (HWND)1710231347;
    Message.lParam = 1710231348;
    Message.wParam = 1710231349;
    Message.pt = { 1710231350, 1710231351 };
    Message.time = 1710231352;

    EXPECT_CALL(Proxy, BuildMessage())
      .Times(1)
      .WillOnce(Return(Message));

    EXPECT_CALL(Proxy, TranslateMessage(Message))
      .Times(1);

    EXPECT_CALL(Proxy, DispatchMessageW(Message))
      .Times(1);

    EXPECT_CALL(EventHandlerProxy, PostCommandDrawWindow(EventHandlerId))
      .Times(1);

    EXPECT_CALL(Proxy, Sleep(0))
      .Times(1);
  }

  // 3 итерация цикла
  {
    EXPECT_CALL(Proxy, GetMessageW(0, 0, 0))
      .Times(1)
      .WillOnce(Return(0));

    MSG MessageQuit = { 0 };
    MessageQuit.message = WM_QUIT;

    EXPECT_CALL(Proxy, BuildMessage())
      .Times(1)
      .WillOnce(Return(MessageQuit));
  }

  EXPECT_CALL(Example, DoClose())
    .Times(1);

  EXPECT_CALL(Example, DoDestroy())
    .Times(1);

  Example.Run();
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Run_DrawingMode_EventBased_GetMessage_Error)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  android_app App;

  const AppInfo_t Info{ &App };

  using namespace ::testing;

  const TestedRun Example{ Tested_t::EventBased{} };

  EXPECT_CALL(Proxy, GetMessageW(0, 0, 0))
    .Times(1)
    .WillOnce(Return(-1));

  EXPECT_THROW(Example.Run(), ::std::exception);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_OnStop)
{
  {
    using Proxy_t = ::mock::WindowsProxy;
    Proxy_t Proxy;
    Proxy_t::GetInstance() = &Proxy;

    Tested_t Example{ Tested_t::Continuous{} };

    using namespace ::testing;

    EXPECT_CALL(Proxy, PostQuitMessage(0))
      .Times(1);

    DoCommand(Example, ::covellite::core::Event::Stop);
  }

  {
    using Proxy_t = ::mock::WindowsProxy;
    Proxy_t Proxy;
    Proxy_t::GetInstance() = &Proxy;

    Tested_t Example{ Tested_t::EventBased{} };

    using namespace ::testing;

    EXPECT_CALL(Proxy, PostQuitMessage(0))
      .Times(1);

    DoCommand(Example, ::covellite::core::Event::Stop);
  }
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_CallDestructorOrder_OnClose)
{
  class ExampleWindow :
    public ::covellite::core::IWindow
  {
    using WindowPtr_t = ::std::shared_ptr<ExampleWindow>;

  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD0(Constructor, ::mock::Id_t(void));
      MOCK_METHOD1(Destructor, void(::mock::Id_t));
    };

  public:
    void Subscribe(const EventHandlerPtr_t &) override { }

  public:
    const ::mock::Id_t m_Id;
    WindowPtr_t m_pWindow;

  public:
    explicit ExampleWindow(const WindowPtr_t & _pWindow, bool _IsSave = true) :
      m_Id(Proxy::GetInstance()->Constructor()),
      m_pWindow((_IsSave) ? _pWindow : nullptr)
    {

    }
    ~ExampleWindow(void)
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
            MakeWindow<ExampleWindow>(nullptr)), false));
    }

    explicit Tested(EventBased) :
      Tested_t(EventBased{})
    {
      MakeWindow<ExampleWindow>(
        MakeWindow<ExampleWindow>(
          MakeWindow<ExampleWindow>(
            MakeWindow<ExampleWindow>(nullptr)), false));
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

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowId1));

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowId2));

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowId3));

    EXPECT_CALL(WindowProxy, Constructor())
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

    DoCommand(Example, ::covellite::core::Event::Close);

    EXPECT_CALL(WindowProxy, Destructor(_))
      .Times(0);
  }

  {
    InSequence Dummy;

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowId1));

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowId2));

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowId3));

    EXPECT_CALL(WindowProxy, Constructor())
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

    DoCommand(Example, ::covellite::core::Event::Close);

    EXPECT_CALL(WindowProxy, Destructor(_))
      .Times(0);
  }
}
