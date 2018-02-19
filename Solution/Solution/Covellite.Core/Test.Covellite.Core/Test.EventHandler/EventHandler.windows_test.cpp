
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса EventHandler.
*/

#define EventHandler_test EventHandler_windows_test
#define EventHandler EventHandler_windows
#define ClickEventListener ClickEventListener_windows

// Расположение класса Application
#include "..\..\Covellite.Core\EventHandler.windows.cpp"
#include "..\..\Covellite.Core\EventHandler.cpp"
#include "..\..\Covellite.Core\ClickEventListener.cpp"

// Общий тестовый класс класса EventHandler
class EventHandler_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::EventHandler;

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
    MOCK_METHOD0(DoSystem, void(void));
    MOCK_METHOD0(DoWindow, void(void));
    MOCK_METHOD0(DoInput, void(void));
    MOCK_METHOD0(DoFramework, void(void));

    MOCK_METHOD0(DoDraw, void(void));
    MOCK_METHOD0(DoBeginDraw, void(void));
    MOCK_METHOD0(DoEndDraw, void(void));

    MOCK_METHOD2(DoMotion, void(int32_t, int32_t));
    MOCK_METHOD1(DoKeyPressed, void(int32_t));
    MOCK_METHOD1(DoKeyUp, void(int32_t));
    MOCK_METHOD1(DoKeyDown, void(int32_t));
    MOCK_METHOD0(DoBack, void(void));

    MOCK_METHOD1(DoError, void(::std::string));

    MOCK_METHOD3(DoClick, void(::std::string, ::std::string, ::std::string));

  private:
    void OnMotion(const ::covellite::core::params::Motion & _Params)
    {
      DoMotion(_Params.X, _Params.Y);
    }

    void OnKeyPressed(const ::covellite::core::params::KeyPressed & _Params)
    {
      DoKeyPressed(_Params.UnicodeKeyCode);
    }

    void OnKeyUp(const ::covellite::core::params::KeyCode & _Params)
    {
      DoKeyUp(_Params.AsciiKeyCode);
    }

    void OnKeyDown(const ::covellite::core::params::KeyCode & _Params)
    {
      DoKeyDown(_Params.AsciiKeyCode);
    }

    void OnError(const ::covellite::core::params::Error & _Value)
    {
      DoError(_Value.Description);
    }

    void OnClick(const ::covellite::core::params::Click & _Value)
    {
      DoClick(_Value.m_DocumentId, _Value.m_Tag, _Value.m_Type);
    }

  public:
    explicit Tested(Command_t _Command = 0)
    {
      using namespace ::covellite::core;

      const auto SystemCommand = static_cast<Event::System>(_Command);
      const auto WindowCommand = static_cast<Event::Window>(_Command);
      const auto InputCommand = static_cast<Event::Input>(_Command);
      const auto FrameworkCommand = static_cast<Event::Framework>(_Command);

      using ::std::placeholders::_1;

      (*this)[SystemCommand]
        .connect(::std::bind(&Tested::DoSystem, this),
        ::boost::signals2::at_front);
      (*this)[WindowCommand]
        .connect(::std::bind(&Tested::DoWindow, this),
        ::boost::signals2::at_front);
      (*this)[InputCommand]
        .connect(::std::bind(&Tested::DoInput, this),
        ::boost::signals2::at_front);
      (*this)[FrameworkCommand]
        .connect(::std::bind(&Tested::DoFramework, this));

      (*this)[Event::Drawing]
        .connect(::std::bind(&Tested::DoDraw, this));
      (*this)[Event::StartDrawing]
        .connect(::std::bind(&Tested::DoBeginDraw, this));
      (*this)[Event::FinishDrawing]
        .connect(::std::bind(&Tested::DoEndDraw, this));

      (*this)[Event::Motion]
        .connect(::std::bind(&Tested::OnMotion, this, _1));
      (*this)[Event::KeyPressed]
        .connect(::std::bind(&Tested::OnKeyPressed, this, _1));
      (*this)[Event::KeyUp]
        .connect(::std::bind(&Tested::OnKeyUp, this, _1));
      (*this)[Event::KeyDown]
        .connect(::std::bind(&Tested::OnKeyDown, this, _1));
      (*this)[Event::Back]
        .connect(::std::bind(&Tested::DoBack, this));

      (*this)[Event::Error]
        .connect(::std::bind(&Tested::OnError, this, _1));
    }

    explicit Tested(const ::std::string & _ElementId)
    {
      using namespace ::covellite::core;
      using ::std::placeholders::_1;

      (*this)[Event::Error]
        .connect(::std::bind(&Tested::OnError, this, _1));

      (*this)[Event::Click][_ElementId]
        .connect(::std::bind(&Tested::OnClick, this, _1));
    }
  };
};

// Образец макроса для подстановки в класс EventHandler 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(EventHandler_test, Test_Function);

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_DummyObject)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  EXPECT_EQ(nullptr, pEvents);

  {
    const Tested_t Example{ Tested_t::Dummy{} };
    EXPECT_EQ(nullptr, pEvents);

    using namespace ::testing;

    EXPECT_CALL(Proxy, UnregisterClassW(_, _))
      .Times(0);
  }

  EXPECT_EQ(nullptr, pEvents);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_MainEvents_Init_Reset)
{
  EXPECT_EQ(nullptr, pEvents);

  {
    Tested_t Example;
    EXPECT_EQ(&Example, pEvents);
  }

  EXPECT_EQ(nullptr, pEvents);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_Constructor_Destructor)
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

    Tested_t Example;
    EXPECT_EQ(sizeof(WindowClass), WindowClass.cbSize);
    EXPECT_EQ(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, WindowClass.style);
    EXPECT_EQ(hModule1, WindowClass.hInstance);
    EXPECT_STREQ(::covellite::core::ClassName, WindowClass.lpszClassName);
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

    EXPECT_CALL(Proxy, UnregisterClassW(Eq(::covellite::core::ClassName), hModule2))
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_Constructor_RegisterClassEx_Error)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  ZeroMemory(&WindowClass, sizeof(WindowClass));

  using namespace ::testing;

  EXPECT_CALL(Proxy, RegisterClassExW(_))
    .Times(1)
    .WillOnce(Return((ATOM)0));

  EXPECT_THROW(Tested_t{}, ::std::exception);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_CallWndProc_MainEventsNullptr)
{
  ZeroMemory(&WindowClass, sizeof(WindowClass));

  Tested_t Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  pEvents = nullptr;
  WindowClass.lpfnWndProc(NULL, 0, NULL, NULL); // Не должно падать
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnSystem_Exception)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const ::std::string Error = "Erro1710212116";
  using ::covellite::core::Event;
  const HWND hWnd = (HWND)1710212120;
  const WPARAM wParam = (WPARAM)1710212121;
  const LPARAM lParam = (LPARAM)1710212122;
  const LRESULT lResult = (LRESULT)1710212123;

  static const ::std::map<UINT, Event::System> Commands =
  {
    { WM_CLOSE, Event::Stop },
  };

  for (const auto Command : Commands)
  {
    Tested Example{ Command.second };
    ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(Example, DoSystem())
      .Times(1)
      .WillOnce(Throw(::std::exception(Error.c_str())));

    EXPECT_CALL(Example, DoWindow())
      .Times(0);

    EXPECT_CALL(Example, DoInput())
      .Times(0);

    EXPECT_CALL(Example, DoFramework())
      .Times(0);

    EXPECT_CALL(Example, DoError(Error))
      .Times(1);

    EXPECT_CALL(Proxy, DefWindowProcW(hWnd, Command.first, wParam, lParam))
      .Times(1)
      .WillOnce(Return(lResult));

    const auto Result = 
      WindowClass.lpfnWndProc(hWnd, Command.first, wParam, lParam);
    EXPECT_EQ(lResult, Result);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnWindow_Exception)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const ::std::string Error = "Error1710212144";
  using ::covellite::core::Event;
  const HWND hWnd = (HWND)1710212145;
  const WPARAM wParam = (WPARAM)1710212146;
  const LPARAM lParam = (LPARAM)1710212147;
  const LRESULT lResult = (LRESULT)1710212148;

  static const ::std::map<int32_t, Event::Window> Commands =
  {
    { WM_KILLFOCUS, Event::LostFocus },
    { WM_SETFOCUS, Event::GainedFocus },
    { WM_SIZE, Event::Resize },
  };

  for (const auto Command : Commands)
  {
    Tested Example{ Command.second };
    ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(Example, DoSystem())
      .Times(0);

    EXPECT_CALL(Example, DoWindow())
      .Times(1)
      .WillOnce(Throw(::std::exception(Error.c_str())));

    EXPECT_CALL(Example, DoInput())
      .Times(0);

    EXPECT_CALL(Example, DoFramework())
      .Times(0);

    EXPECT_CALL(Example, DoError(Error))
      .Times(1);

    EXPECT_CALL(Proxy, DefWindowProcW(hWnd, Command.first, wParam, lParam))
      .Times(1)
      .WillOnce(Return(lResult));

    const auto Result =
      WindowClass.lpfnWndProc(hWnd, Command.first, wParam, lParam);
    EXPECT_EQ(lResult, Result);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnInput_Exception)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const ::std::string Error = "Error1710221121";
  const HWND hWnd = (HWND)1710221122;
  const WPARAM wParam = (WPARAM)1710221123;
  const LPARAM lParam = (LPARAM)1710221124;
  const LRESULT lResult = (LRESULT)1710221125;

  using ::covellite::core::Event;

  static const ::std::map<int32_t, Event::Input> Commands =
  {
    { WM_LBUTTONDOWN, Event::Touch },
    { WM_LBUTTONUP, Event::Release },
  };

  for (const auto Command : Commands)
  {
    Tested Example{ Command.second };
    ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(Example, DoSystem())
      .Times(0);

    EXPECT_CALL(Example, DoWindow())
      .Times(0);

    EXPECT_CALL(Example, DoInput())
      .Times(1)
      .WillOnce(Throw(::std::exception(Error.c_str())));

    EXPECT_CALL(Example, DoFramework())
      .Times(0);

    EXPECT_CALL(Example, DoError(Error))
      .Times(1);

    EXPECT_CALL(Proxy, DefWindowProcW(hWnd, Command.first, wParam, lParam))
      .Times(1)
      .WillOnce(Return(lResult));

    const auto Result =
      WindowClass.lpfnWndProc(hWnd, Command.first, wParam, lParam);
    EXPECT_EQ(lResult, Result);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnMotion_Exception)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const ::std::string Error = "Error1710212150";
  const HWND hWnd = (HWND)1710212151;
  const WPARAM wParam = (WPARAM)1710212152;
  const LPARAM lParam = (LPARAM)1710212153;
  const LRESULT lResult = (LRESULT)1710212154;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoMotion(_, _))
    .Times(1)
    .WillOnce(Throw(::std::exception(Error.c_str())));

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoError(Error))
    .Times(1);

  EXPECT_CALL(Proxy, DefWindowProcW(hWnd, WM_MOUSEMOVE, wParam, lParam))
    .Times(1)
    .WillOnce(Return(lResult));

  const auto Result = 
    WindowClass.lpfnWndProc(hWnd, WM_MOUSEMOVE, wParam, lParam);
  EXPECT_EQ(lResult, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyPressed_Exception)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const ::std::string Error = "Error1710212203";
  const HWND hWnd = (HWND)1710212204;
  const WPARAM wParam = (WPARAM)1710212205;
  const LPARAM lParam = (LPARAM)1710212206;
  const LRESULT lResult = (LRESULT)1710212207;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoKeyPressed(_))
    .Times(1)
    .WillOnce(Throw(::std::exception(Error.c_str())));

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoError(Error))
    .Times(1);

  EXPECT_CALL(Proxy, DefWindowProcW(hWnd, WM_CHAR, wParam, lParam))
    .Times(1)
    .WillOnce(Return(lResult));

  const auto Result = WindowClass.lpfnWndProc(hWnd, WM_CHAR, wParam, lParam);
  EXPECT_EQ(lResult, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyUp_Exception)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const ::std::string Error = "Error1710212208";
  const HWND hWnd = (HWND)1710212209;
  const WPARAM wParam = (WPARAM)1710212210;
  const LPARAM lParam = (LPARAM)1710212211;
  const LRESULT lResult = (LRESULT)1710212212;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoKeyUp(_))
    .Times(1)
    .WillOnce(Throw(::std::exception(Error.c_str())));

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoError(Error))
    .Times(1);

  EXPECT_CALL(Proxy, DefWindowProcW(hWnd, WM_KEYUP, wParam, lParam))
    .Times(1)
    .WillOnce(Return(lResult));

  const auto Result = WindowClass.lpfnWndProc(hWnd, WM_KEYUP, wParam, lParam);
  EXPECT_EQ(lResult, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyDown_Exception)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const ::std::string Error = "Error1711041322";
  const HWND hWnd = (HWND)1711041323;
  const WPARAM wParam = (WPARAM)1711041324;
  const LPARAM lParam = (LPARAM)1711041325;
  const LRESULT lResult = (LRESULT)1711041326;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoKeyDown(_))
    .Times(1)
    .WillOnce(Throw(::std::exception(Error.c_str())));

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoError(Error))
    .Times(1);

  EXPECT_CALL(Proxy, DefWindowProcW(hWnd, WM_KEYDOWN, wParam, lParam))
    .Times(1)
    .WillOnce(Return(lResult));
    
  const auto Result = WindowClass.lpfnWndProc(hWnd, WM_KEYDOWN, wParam, lParam);
  EXPECT_EQ(lResult, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnBack_Exception)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const ::std::string Error = "Error1711041233";
  const HWND hWnd = (HWND)1711041234;
  const LPARAM lParam = (LPARAM)1711041235;
  const LRESULT lResult = (LRESULT)1711041236;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoBack())
    .Times(1)
    .WillOnce(Throw(::std::exception(Error.c_str())));

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoError(Error))
    .Times(1);

  EXPECT_CALL(Proxy, DefWindowProcW(hWnd, WM_SYSKEYUP, VK_LEFT, lParam))
    .Times(1)
    .WillOnce(Return(lResult));

  const auto Result = WindowClass.lpfnWndProc(hWnd, WM_SYSKEYUP, VK_LEFT, lParam);
  EXPECT_EQ(lResult, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_SysKeyUp_UnknownKey)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const HWND hWnd = (HWND)1711041240;
  const WPARAM wParam = (WPARAM)1711041241;
  const LPARAM lParam = (LPARAM)1711041242;
  const LRESULT lResult = (LRESULT)1711041243;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoBack())
    .Times(0);

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoInput())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(hWnd, WM_SYSKEYUP, wParam, lParam))
    .Times(1)
    .WillOnce(Return(lResult));

  const auto Result = WindowClass.lpfnWndProc(hWnd, WM_SYSKEYUP, wParam, lParam);
  EXPECT_EQ(lResult, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_CommandHandler_UnknownEvent)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const UINT Command = WM_USER + 1710212218;
  const HWND hWnd = (HWND)1710212214;
  const WPARAM wParam = (WPARAM)1710212215;
  const LPARAM lParam = (LPARAM)1710212216;
  const LRESULT lResult = (LRESULT)1710212217;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoInput())
    .Times(0);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(hWnd, Command, wParam, lParam))
    .Times(1)
    .WillOnce(Return(lResult));

  const auto Result = WindowClass.lpfnWndProc(hWnd, Command, wParam, lParam);
  EXPECT_EQ(lResult, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnStop)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  Tested Example{ ::covellite::core::Event::Stop };
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoSystem())
    .Times(1);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoInput())
    .Times(0);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
    .Times(0);

  const auto Result = WindowClass.lpfnWndProc(NULL, WM_CLOSE, 0, 0);
  EXPECT_EQ(0, Result);
}

namespace covellite
{

namespace core
{

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnDestroy)
{
  using namespace ::covellite::core;

  {
    Tested Example{ 1710212221 };
    ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

    EXPECT_FALSE(Example.OnSystem.empty());
    EXPECT_FALSE(Example.OnWindow.empty());
    EXPECT_FALSE(Example.OnInput.empty());
    EXPECT_FALSE(Example.OnFramework.empty());

    WindowClass.lpfnWndProc(NULL, WM_DESTROY, 0, 0);

    EXPECT_FALSE(Example.OnSystem.empty());
    EXPECT_FALSE(Example.OnWindow.empty());
    EXPECT_FALSE(Example.OnInput.empty());
    EXPECT_FALSE(Example.OnFramework.empty());

    Example.DoCommand(Event::Destroy, params::Empty{});
    EXPECT_TRUE(Example.OnSystem.empty());
    EXPECT_TRUE(Example.OnWindow.empty());
    EXPECT_TRUE(Example.OnInput.empty());
    EXPECT_TRUE(Example.OnFramework.empty());
  }

  {
    Tested Example{ "1710212222" };
    ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

    EXPECT_FALSE(Example.OnRocket.empty());

    WindowClass.lpfnWndProc(NULL, WM_DESTROY, 0, 0);

    EXPECT_FALSE(Example.OnRocket.empty());

    Example.DoCommand(Event::Destroy, params::Empty{});
    EXPECT_TRUE(Example.OnRocket.empty());
  }
}

} // namespace core

} // namespace covellite

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnCreate)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const HWND hWnd = (HWND)1710221242;
  const WPARAM wParam = (WPARAM)1710221243;
  const LPARAM lParam = (LPARAM)1710221244;
  const LRESULT lResult = (LRESULT)1710221245;

  Tested Example{ ::covellite::core::Event::Create };
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoInput())
    .Times(0);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(hWnd, WM_CREATE, wParam, lParam))
    .Times(1)
    .WillOnce(Return(lResult));

  const auto Result = WindowClass.lpfnWndProc(hWnd, WM_CREATE, wParam, lParam);
  EXPECT_EQ(lResult, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnLostFocus)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  Tested Example{ ::covellite::core::Event::LostFocus };
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(1);

  EXPECT_CALL(Example, DoInput())
    .Times(0);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
    .Times(0);

  const auto Result = WindowClass.lpfnWndProc(NULL, WM_KILLFOCUS, 0, 0);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnGainedFocus)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  Tested Example{ ::covellite::core::Event::GainedFocus };
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(1);

  EXPECT_CALL(Example, DoInput())
    .Times(0);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
    .Times(0);

  const auto Result = WindowClass.lpfnWndProc(NULL, WM_SETFOCUS, 0, 0);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnResize)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  Tested Example{ ::covellite::core::Event::Resize };
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(1);

  EXPECT_CALL(Example, DoInput())
    .Times(0);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
    .Times(0);

  const auto Result = WindowClass.lpfnWndProc(NULL, WM_SIZE, 0, 0);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnMotion)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const int32_t X = 1234;
  const int32_t Y = 5678;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoMotion(X, Y))
    .Times(1);

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
    .Times(0);

  const auto Result = 
    WindowClass.lpfnWndProc(NULL, WM_MOUSEMOVE, 0, MAKELPARAM(X, Y));
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnTouch)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  Tested Example{ ::covellite::core::Event::Touch };
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoInput())
    .Times(1);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
    .Times(0);

  const auto Result = WindowClass.lpfnWndProc(NULL, WM_LBUTTONDOWN, 0, 0);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnRelease)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  Tested Example{ ::covellite::core::Event::Release };
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoInput())
    .Times(1);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
    .Times(0);

  const auto Result = WindowClass.lpfnWndProc(NULL, WM_LBUTTONUP, 0, 0);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyPressed_IgnoreKeyCode)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const auto ReturnCode = 1711011306;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  for (int32_t KeyCode = 0; KeyCode < 32; KeyCode++)
  {
    if (KeyCode == 10 || KeyCode == 13) continue;

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(Example, DoKeyPressed(_))
      .Times(0);

    EXPECT_CALL(Example, DoSystem())
      .Times(0);

    EXPECT_CALL(Example, DoWindow())
      .Times(0);

    EXPECT_CALL(Example, DoFramework())
      .Times(0);

    EXPECT_CALL(Example, DoError(_))
      .Times(0);

    EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
      .Times(1)
      .WillOnce(Return(ReturnCode));

    const auto Result = WindowClass.lpfnWndProc(NULL, WM_CHAR, KeyCode, 0);
    EXPECT_EQ(ReturnCode, Result);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyPressed)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const ::std::map<int32_t, int32_t> KeyCodes =
  {
    { 1710212237, 1710212237 },
    { 13, 10},
  };

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  for (const auto KeyCode : KeyCodes)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(Example, DoKeyPressed(KeyCode.second))
      .Times(1);

    EXPECT_CALL(Example, DoSystem())
      .Times(0);

    EXPECT_CALL(Example, DoWindow())
      .Times(0);

    EXPECT_CALL(Example, DoFramework())
      .Times(0);

    EXPECT_CALL(Example, DoError(_))
      .Times(0);

    EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
      .Times(0);

    const auto Result = WindowClass.lpfnWndProc(NULL, WM_CHAR, KeyCode.first, 0);
    EXPECT_EQ(0, Result);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyUp)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const int32_t KeyCode = 1710212247;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoKeyUp(KeyCode))
    .Times(1);

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
    .Times(0);

  const auto Result = WindowClass.lpfnWndProc(NULL, WM_KEYUP, KeyCode, 0);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyDown)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  const int32_t KeyCode = 1711041327;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoKeyDown(KeyCode))
    .Times(1);

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
    .Times(0);

  const auto Result = WindowClass.lpfnWndProc(NULL, WM_KEYDOWN, KeyCode, 0);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnBack)
{
  using Proxy_t = ::mock::WindowsProxy;
  Proxy_t Proxy;
  Proxy_t::GetInstance() = &Proxy;

  Tested Example;
  ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

  using namespace ::testing;

  EXPECT_CALL(Example, DoBack())
    .Times(1);

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Proxy, DefWindowProcW(_, _, _, _))
    .Times(0);

  const auto Result = WindowClass.lpfnWndProc(NULL, WM_SYSKEYUP, VK_LEFT, 0);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_PostCommandDrawWindow)
{
  {
    Tested Example;
    ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(Example, DoBeginDraw())
      .Times(1);

    EXPECT_CALL(Example, DoDraw())
      .Times(1);

    EXPECT_CALL(Example, DoEndDraw())
      .Times(1);

    Example.PostCommandDrawWindow();
  }

  {
    const ::std::string Error = "Error1710212250";

    Tested Example;
    ASSERT_NE(nullptr, WindowClass.lpfnWndProc);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(Example, DoBeginDraw())
      .Times(1);

    EXPECT_CALL(Example, DoDraw())
      .Times(1)
      .WillOnce(Throw(::std::exception{ Error.c_str() }));

    EXPECT_CALL(Example, DoError(Error))
      .Times(1);

    EXPECT_CALL(Example, DoEndDraw())
      .Times(1);

    Example.PostCommandDrawWindow();
  }
}
