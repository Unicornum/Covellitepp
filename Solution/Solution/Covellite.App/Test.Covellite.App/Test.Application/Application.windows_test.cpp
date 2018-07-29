
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

#define Application_test Application_windows_test
#define Application Application_windows

// Расположение класса Application
#include "..\..\Covellite.App\Application.windows.cpp"

#undef MessageBox

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

protected:
  class Tested :
    public Tested_t
  {
  public:
    explicit Tested(const Run_t & _Run = [](void) {}) : Tested_t(_Run) {}
  };

  class TestedMessage :
    public Tested
  {
    using Params_t = ::std::pair<WPARAM, LPARAM>;

  public:
    MOCK_METHOD2(CallSignal, void(WPARAM, LPARAM));

  public:
    operator LONG_PTR (void) { return reinterpret_cast<LONG_PTR>(&m_Events); }

  public:
    explicit TestedMessage(UINT _Message)
    {
      using namespace ::covellite::events;

      m_Events[Error.Exception].Connect([&]() {});

      m_Events[_Message].Connect([&](const Params_t & _Params)
      {
        CallSignal(_Params.first, _Params.second);
      });
    }
  };

  class TestedRun :
    public Tested_t
  {
  public:
    MOCK_CONST_METHOD0(DoStart, void(void));
    MOCK_CONST_METHOD0(DoUpdate, void(void));
    MOCK_CONST_METHOD0(DoStop, void(void));

  public:
    template<class T>
    explicit TestedRun(T _Type) :
      Tested_t(_Type)
    {
      using namespace ::covellite;

      m_Events[events::Application.Start].Connect([&]() { DoStart(); });
      m_Events[events::Application.Update].Connect([&]() { DoUpdate(); });
      m_Events[events::Application.Exit].Connect([&]() { DoStop(); });
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
  class Real :
    public Tested
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD0(Run, void(void));
    };

  public:
    Real(void) : Tested([&](void) { Proxy::GetInstance()->Run(); }) { }
  };

  using RealProxy_t = Real::Proxy;
  RealProxy_t Proxy;
  RealProxy_t::GetInstance() = &Proxy;

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
TEST_F(Application_test, /*DISABLED_*/Test_RegisterClass_Error)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  ZeroMemory(&WindowClass, sizeof(WindowClass));

  using namespace ::testing;

  EXPECT_CALL(Proxy, RegisterClassExW(_))
    .Times(1)
    .WillOnce(Return((ATOM)0));

  EXPECT_THROW(Tested{}, ::std::exception);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_RegisterClass)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const HMODULE hModule1 = (HMODULE)1710211422;
  const HMODULE hModule2 = (HMODULE)1710211429;
  const HCURSOR hCursor = (HCURSOR)1710281102;
  const HICON hIcon1 = (HICON)1710281125;
  const HICON hIcon2 = (HICON)1710281126;

  ZeroMemory(&WindowClass, sizeof(WindowClass));

  using namespace ::testing;

  InSequence Dummy;

  {
    EXPECT_CALL(Proxy, GetModuleHandleW(nullptr))
      .Times(1)
      .WillOnce(Return(hModule1));

    EXPECT_CALL(Proxy, LoadCursorW(NULL, (ULONG_PTR)IDC_ARROW))
      .Times(1)
      .WillOnce(Return(hCursor));

    EXPECT_CALL(Proxy, LoadIconW(hModule1, Eq(L"ApplicationIcon")))
      .Times(2)
      .WillOnce(Return(hIcon1))
      .WillOnce(Return(hIcon2));

    EXPECT_CALL(Proxy, RegisterClassExW(&WindowClass))
      .Times(1)
      .WillOnce(Return((ATOM)1));

    Tested Example;
    EXPECT_EQ(sizeof(WindowClass), WindowClass.cbSize);
    EXPECT_EQ(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, WindowClass.style);
    EXPECT_EQ(hModule1, WindowClass.hInstance);
    EXPECT_STREQ(::covellite::app::ClassName, WindowClass.lpszClassName);
    EXPECT_NE(nullptr, WindowClass.lpfnWndProc);
    EXPECT_EQ(0, WindowClass.cbClsExtra);
    EXPECT_EQ(0, WindowClass.cbWndExtra);
    EXPECT_EQ(hIcon1, WindowClass.hIcon);
    EXPECT_EQ(hIcon2, WindowClass.hIconSm);
    EXPECT_EQ(hCursor, WindowClass.hCursor);
    EXPECT_EQ(0, WindowClass.hbrBackground);
    EXPECT_EQ(0, WindowClass.lpszMenuName);

    EXPECT_CALL(Proxy, GetModuleHandleW(nullptr))
      .Times(1)
      .WillOnce(Return(hModule2));

    EXPECT_CALL(Proxy, UnregisterClassW(Eq(::covellite::app::ClassName), hModule2))
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_CallWndProc_EventsNullPtr)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const HWND hWnd = (HWND)1807031425;
  const UINT Message = 1807031426;
  const WPARAM wParam = 1807031427;
  const LPARAM lParam = 1807031428;
  const LRESULT lResult = 1807031429;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Proxy, GetWindowLongPtrW(hWnd, GWLP_USERDATA))
    .Times(1)
    .WillOnce(Return((LONG_PTR)nullptr));

  EXPECT_CALL(Proxy, DefWindowProcW(hWnd, Message, wParam, lParam))
    .Times(1)
    .WillOnce(Return(lResult));

  const auto Result = WindowClass.lpfnWndProc(hWnd, Message, wParam, lParam);
  EXPECT_EQ(lResult, Result);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_CallWndProc_Success)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const HWND hWnd = (HWND)1807031436;
  const UINT Message = 1807031437;
  const WPARAM wParam = 1807031438;
  const LPARAM lParam = 1807031439;

  TestedMessage Example{ Message };
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Proxy, GetWindowLongPtrW(hWnd, GWLP_USERDATA))
    .Times(1)
    .WillOnce(Return((LONG_PTR)Example));

  EXPECT_CALL(Example, CallSignal(wParam, lParam))
    .Times(1);

  EXPECT_CALL(Proxy, DefWindowProcW(hWnd, Message, wParam, lParam))
    .Times(0);

  const auto Result = WindowClass.lpfnWndProc(hWnd, Message, wParam, lParam);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_CallWndProc_Fail)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const HWND hWnd = (HWND)1807031507;
  const UINT Message = 1807031508;
  const WPARAM wParam = 1807031509;
  const LPARAM lParam = 1807031510;
  const LRESULT lResult = 1807031511;

  TestedMessage Example{ Message };
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Proxy, GetWindowLongPtrW(hWnd, GWLP_USERDATA))
    .Times(1)
    .WillOnce(Return((LONG_PTR)Example));

  EXPECT_CALL(Example, CallSignal(wParam, lParam))
    .Times(1)
    .WillOnce(Throw(::std::exception{}));

  EXPECT_CALL(Proxy, DefWindowProcW(hWnd, Message, wParam, lParam))
    .Times(1)
    .WillOnce(Return(lResult));

  const auto Result = WindowClass.lpfnWndProc(hWnd, Message, wParam, lParam);
  EXPECT_EQ(lResult, Result);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Run_DrawingMode_Continuous)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  using namespace ::testing;

  const TestedRun Example{ Tested_t::Continuous{} };

  InSequence Dummy;

  EXPECT_CALL(Example, DoStart())
    .Times(1);

  // 1 итерация цикла
  {
    EXPECT_CALL(Proxy, PeekMessage(0, 0, 0, PM_REMOVE))
      .Times(1)
      .WillOnce(Return(FALSE));

    EXPECT_CALL(Example, DoUpdate())
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

    EXPECT_CALL(Example, DoUpdate())
      .Times(1);

    EXPECT_CALL(Proxy, Sleep(0))
      .Times(1);
  }

  // 3 итерация цикла - потеря фокуса ввода
  {
    EXPECT_CALL(Proxy, PeekMessage(0, 0, 0, PM_REMOVE))
      .Times(1)
      .WillOnce(Return(TRUE));

    MSG MessageLostFocus = { 0 };
    MessageLostFocus.message = WM_KILLFOCUS;

    EXPECT_CALL(Proxy, BuildMessage())
      .Times(1)
      .WillOnce(Return(MessageLostFocus));

    EXPECT_CALL(Proxy, TranslateMessage(MessageLostFocus))
      .Times(1);

    EXPECT_CALL(Proxy, DispatchMessageW(MessageLostFocus))
      .Times(1);

    EXPECT_CALL(Example, DoUpdate())
      .Times(1);

    EXPECT_CALL(Proxy, Sleep(0))
      .Times(1);
  }

  // 4 итерация цикла
  {
    EXPECT_CALL(Proxy, GetMessageW(0, 0, 0))
      .Times(1)
      .WillOnce(Return(TRUE));

    MSG Message;
    Message.message = 1807031950;
    Message.hwnd = (HWND)1807031951;
    Message.lParam = 1807031952;
    Message.wParam = 1807031953;
    Message.pt = { 1807031954, 1807031955 };
    Message.time = 1807031956;

    EXPECT_CALL(Proxy, BuildMessage())
      .Times(1)
      .WillOnce(Return(Message));

    EXPECT_CALL(Proxy, TranslateMessage(Message))
      .Times(1);

    EXPECT_CALL(Proxy, DispatchMessageW(Message))
      .Times(1);

    EXPECT_CALL(Example, DoUpdate())
      .Times(1);

    EXPECT_CALL(Proxy, Sleep(0))
      .Times(1);
  }

  // 5 итерация цикла - возврат фокуса ввода
  {
    EXPECT_CALL(Proxy, GetMessageW(0, 0, 0))
      .Times(1)
      .WillOnce(Return(TRUE));

    MSG MessageReturnFocus = { 0 };
    MessageReturnFocus.message = WM_SETFOCUS;

    EXPECT_CALL(Proxy, BuildMessage())
      .Times(1)
      .WillOnce(Return(MessageReturnFocus));

    EXPECT_CALL(Proxy, TranslateMessage(MessageReturnFocus))
      .Times(1);

    EXPECT_CALL(Proxy, DispatchMessageW(MessageReturnFocus))
      .Times(1);

    EXPECT_CALL(Example, DoUpdate())
      .Times(1);

    EXPECT_CALL(Proxy, Sleep(0))
      .Times(1);
  }

  // 6 итерация цикла
  {
    EXPECT_CALL(Proxy, PeekMessage(0, 0, 0, PM_REMOVE))
      .Times(1)
      .WillOnce(Return(TRUE));

    MSG MessageQuit = { 0 };
    MessageQuit.message = WM_QUIT;

    EXPECT_CALL(Proxy, BuildMessage())
      .Times(1)
      .WillOnce(Return(MessageQuit));

    EXPECT_CALL(Proxy, TranslateMessage(_))
      .Times(0);

    EXPECT_CALL(Proxy, DispatchMessageW(_))
      .Times(0);

    EXPECT_CALL(Example, DoUpdate())
      .Times(0);

    EXPECT_CALL(Proxy, Sleep(_))
      .Times(0);
  }

  EXPECT_CALL(Example, DoStop())
    .Times(0);

  Example.Run();
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Run_DrawingMode_EventBased)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  using namespace ::testing;

  const TestedRun Example{ Tested_t::EventBased{} };

  InSequence Dummy;

  EXPECT_CALL(Example, DoStart())
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

    EXPECT_CALL(Example, DoUpdate())
      .Times(1);
  }

  // 2 итерация цикла
  {
    EXPECT_CALL(Proxy, GetMessageW(0, 0, 0))
      .Times(1)
      .WillOnce(Return(TRUE));

    MSG Message;
    Message.message = 1807031941;
    Message.hwnd = (HWND)1807031942;
    Message.lParam = 1807031943;
    Message.wParam = 1807031944;
    Message.pt = { 1807031945, 1807031946 };
    Message.time = 1807031947;

    EXPECT_CALL(Proxy, BuildMessage())
      .Times(1)
      .WillOnce(Return(Message));

    EXPECT_CALL(Proxy, TranslateMessage(Message))
      .Times(1);

    EXPECT_CALL(Proxy, DispatchMessageW(Message))
      .Times(1);

    EXPECT_CALL(Example, DoUpdate())
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

    EXPECT_CALL(Proxy, TranslateMessage(_))
      .Times(0);

    EXPECT_CALL(Proxy, DispatchMessageW(_))
      .Times(0);

    EXPECT_CALL(Example, DoUpdate())
      .Times(0);
  }

  EXPECT_CALL(Example, DoStop())
    .Times(0);

  Example.Run();
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Run_DrawingMode_EventBased_GetMessage_Error)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  using namespace ::testing;

  const TestedRun Example{ Tested_t::EventBased{} };

  EXPECT_CALL(Proxy, GetMessageW(0, 0, 0))
    .Times(1)
    .WillOnce(Return(-1));

  EXPECT_THROW(Example.Run(), ::std::exception);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_OnExit)
{
  {
    using WindowsProxy_t = ::mock::WindowsProxy;
    WindowsProxy_t WindowsProxy;
    WindowsProxy_t::GetInstance() = &WindowsProxy;

    Tested_t Example{ Tested_t::EventBased{} };
    ::covellite::events::Events Events = Example;

    Events[::covellite::events::Error.Exception].Connect([]() {});

    using namespace ::testing;

    EXPECT_CALL(WindowsProxy, PostQuitMessage(0))
      .Times(1);

    Events[::covellite::events::Application.Exit]();
  }

  {
    using WindowsProxy_t = ::mock::WindowsProxy;
    WindowsProxy_t WindowsProxy;
    WindowsProxy_t::GetInstance() = &WindowsProxy;

    Tested_t Example{ Tested_t::Continuous{} };
    ::covellite::events::Events Events = Example;

    using namespace ::testing;

    EXPECT_CALL(WindowsProxy, PostQuitMessage(0))
      .Times(1);

    Events[::covellite::events::Application.Exit]();
  }
}
