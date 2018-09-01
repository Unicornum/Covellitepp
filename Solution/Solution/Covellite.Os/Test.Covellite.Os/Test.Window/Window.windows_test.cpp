
#include "stdafx.h"
#include <Covellite/App/Settings.mock.hpp>
#include <Covellite/Events.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Window.
*/

#define Window_test Window_windows_test
#define Window Window_windows
#define Configuration Configuration_windows

// Расположение класса Window
#include "..\..\Covellite.Os\Window.windows.cpp"
#include "..\..\Covellite.Os\Window.cpp"
#include "..\..\Covellite.Os\Configuration.windows.cpp"
#include "..\..\Covellite.Os\Configuration.cpp"

// Общий тестовый класс класса Window
class Window_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::os::Window;
  using ITested_t = ::covellite::os::IWindow;
  using String_t = ::alicorn::extension::std::String;
  using Application_t = ::mock::covellite::app::Application;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<String_t>::Set(uT("0"));
    ::testing::DefaultValue<RECT>::Set({ 0, 0, 0, 0 });
    ::testing::DefaultValue<BOOL>::Set(TRUE);
    ::testing::DefaultValue<HWND>::Set((HWND)1807061251);
    ::testing::DefaultValue<LONG_PTR>::Set((LONG_PTR)1807191107);
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<String_t>::Clear();
    ::testing::DefaultValue<RECT>::Clear();
    ::testing::DefaultValue<BOOL>::Clear();
    ::testing::DefaultValue<HWND>::Clear();
    ::testing::DefaultValue<LONG_PTR>::Clear();
  }

protected:
  class Application :
    public Application_t
  {
  public:
    MOCK_METHOD0(OnExit, void(void));
    MOCK_METHOD0(OnResize, void(void));
    MOCK_METHOD2(OnMotion, void(int, int));
    MOCK_METHOD0(OnTouch, void(void));
    MOCK_METHOD0(OnRelease, void(void));
    MOCK_METHOD1(OnPressed, void(int));
    MOCK_METHOD1(OnDown, void(int));
    MOCK_METHOD1(OnUp, void(int));
    MOCK_METHOD0(OnBack, void(void));
    MOCK_METHOD0(OnMenu, void(void));

  public:
    Application(void) :
      Application_t(Application_t::EventBased{})
    {
      using namespace ::covellite;

      m_Events[events::Application.Exit]
        .Connect([&]() { OnExit(); });

      m_Events[events::Window.Resize]
        .Connect([&]() { OnResize(); });

      using Position_t = events::Cursor_t::Position;

      m_Events[events::Cursor.Motion]
        .Connect([&](const Position_t & _Pos) { OnMotion(_Pos.X, _Pos.Y); });
      m_Events[events::Cursor.Touch]
        .Connect([&]() { OnTouch(); });
      m_Events[events::Cursor.Release]
        .Connect([&]() { OnRelease(); });

      using KeyCode_t = events::Key_t::Code;

      m_Events[events::Key.Pressed]
        .Connect([&](const KeyCode_t & _Code) { OnPressed(_Code); });
      m_Events[events::Key.Down]
        .Connect([&](const KeyCode_t & _Code) { OnDown(_Code); });
      m_Events[events::Key.Up]
        .Connect([&](const KeyCode_t & _Code) { OnUp(_Code); });
      m_Events[events::Key.Back]
        .Connect([&]() { OnBack(); });
      m_Events[events::Key.Menu]
        .Connect([&]() { OnMenu(); });
    }
  };
};

// Образец макроса для подстановки в класс Window 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Window_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_WindowMode_Resized)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using InfoProxy_t = ::mock::alicorn::system::version::Info::Proxy;
  InfoProxy_t InfoProxy;
  InfoProxy_t::GetInstance() = &InfoProxy;

  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const ::mock::Id_t InfoId = 1711032335;
  const auto Title = uT("Title1711032336");
  const ::mock::Id_t RootSectionId = 1711091118;
  const ::mock::Id_t MainSectionId = 1711091120;
  const ::mock::Id_t WindowSectionId = 1711091058;
  const ::mock::Id_t SizeSectionId = 1711091059;
  const auto ClientWidth = 1711091107;
  const auto ClientHeight = 1711091108;
  const HWND hWnd = (HWND)1710240002;
  const HMODULE hModule = (HMODULE)1710240005;
  const auto WindowFlags = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPED | 
    WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
  const auto WindowFlagsEx =
    WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
  const auto WindowWidth = 1711091129;
  const auto WindowHeight = 1711091130;
  const RECT WindowRect = { 1134, 1135, 1134 + WindowWidth, 1135 + WindowHeight };
  const auto ScreenWidth = 1711091224;
  const auto ScreenHeight = 1711091234;
  const auto X = (ScreenWidth - WindowWidth) / 2;
  const auto Y = (ScreenHeight - WindowHeight) / 2;

  using namespace ::testing;

  EXPECT_CALL(InfoProxy, Constructor())
    .Times(1)
    .WillOnce(Return(InfoId));

  EXPECT_CALL(InfoProxy, GetValue(InfoId, uT("ApplicationName")))
    .Times(1)
    .WillOnce(Return(Title));

  EXPECT_CALL(SettingsProxy, Constructor())
    //.WillOnce(Return(RootSectionId))
    //.WillOnce(Return(MainSectionId))
    .WillOnce(Return(WindowSectionId))
    .WillOnce(Return(SizeSectionId));

  //EXPECT_CALL(SettingsProxy, GetChildSectionImpl(RootSectionId, _))
  //  .Times(1);

  EXPECT_CALL(SettingsProxy, GetChildSectionImpl(/*MainSectionId*/_, uT("Window")))
    .Times(1);

  EXPECT_CALL(SettingsProxy, GetChildSectionImpl(WindowSectionId, uT("Size")))
    .Times(1);

  using ::boost::lexical_cast;

  EXPECT_CALL(SettingsProxy, GetValue(WindowSectionId, uT("IsFullScreen")))
    .Times(1)
    .WillOnce(Return(uT("false")));

  EXPECT_CALL(SettingsProxy, GetValue(WindowSectionId, uT("IsResized")))
    .Times(1)
    .WillOnce(Return(uT("true")));

  EXPECT_CALL(SettingsProxy, GetValue(SizeSectionId, uT("Width")))
    .Times(1)
    .WillOnce(Return(lexical_cast<String_t>(ClientWidth)));

  EXPECT_CALL(SettingsProxy, GetValue(SizeSectionId, uT("Height")))
    .Times(1)
    .WillOnce(Return(lexical_cast<String_t>(ClientHeight)));

  EXPECT_CALL(WindowsProxy, GetModuleHandleW(nullptr))
    .Times(1)
    .WillOnce(Return(hModule));

  EXPECT_CALL(WindowsProxy, GetSystemMetrics(SM_CXSCREEN))
    .Times(1)
    .WillOnce(Return(ScreenWidth));

  EXPECT_CALL(WindowsProxy, GetSystemMetrics(SM_CYSCREEN))
    .Times(1)
    .WillOnce(Return(ScreenHeight));

  {
    using namespace ::alicorn::extension::std;

    InSequence Dummy;

    EXPECT_CALL(WindowsProxy, SetClientRect(0, 0, ClientWidth, ClientHeight))
      .Times(1);

    EXPECT_CALL(WindowsProxy, GetWindowRect())
      .Times(1)
      .WillOnce(Return(WindowRect));

    EXPECT_CALL(WindowsProxy, 
      AdjustWindowRectEx(WindowFlags, FALSE, WindowFlagsEx))
      .Times(1)
      .WillOnce(Return(TRUE));

    EXPECT_CALL(WindowsProxy, CreateWindowExW1(WindowFlagsEx, 
      Eq(::covellite::app::ClassName), Eq(string_cast<::std::wstring>(Title)),
      WindowFlags, X, Y, WindowWidth, WindowHeight, nullptr, nullptr))
      .Times(1);

    EXPECT_CALL(WindowsProxy, CreateWindowExW2(hModule, nullptr))
      .Times(1)
      .WillOnce(Return(hWnd));

    EXPECT_CALL(WindowsProxy, ShowWindow(hWnd, SW_SHOW))
      .Times(1)
      .WillOnce(Return(TRUE));

    const Application_t Application{ Application_t::EventBased{} };
    const Tested_t Example{ Application };
    const ITested_t & IExample = Example;
    EXPECT_EQ(hWnd, IExample.GetHandle());

    EXPECT_CALL(WindowsProxy, DestroyWindow(hWnd))
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_WindowMode_NoResized)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using InfoProxy_t = ::mock::alicorn::system::version::Info::Proxy;
  InfoProxy_t InfoProxy;
  InfoProxy_t::GetInstance() = &InfoProxy;

  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const ::mock::Id_t InfoId = 1711032335;
  const auto Title = uT("Title1711032336");
  const ::mock::Id_t RootSectionId = 1711091118;
  const ::mock::Id_t MainSectionId = 1711091120;
  const ::mock::Id_t WindowSectionId = 1711091058;
  const ::mock::Id_t SizeSectionId = 1711091059;
  const auto ClientWidth = 1711091107;
  const auto ClientHeight = 1711091108;
  const HWND hWnd = (HWND)1710240002;
  const HMODULE hModule = (HMODULE)1710240005;
  const auto WindowFlags = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPED |
    WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
  const auto WindowFlagsEx =
    WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
  const auto WindowWidth = 1711091129;
  const auto WindowHeight = 1711091130;
  const RECT WindowRect = { 1134, 1135, 1134 + WindowWidth, 1135 + WindowHeight };
  const auto ScreenWidth = 1711091224;
  const auto ScreenHeight = 1711091234;
  const auto X = (ScreenWidth - WindowWidth) / 2;
  const auto Y = (ScreenHeight - WindowHeight) / 2;

  using namespace ::testing;

  EXPECT_CALL(InfoProxy, Constructor())
    .Times(1)
    .WillOnce(Return(InfoId));

  EXPECT_CALL(InfoProxy, GetValue(InfoId, uT("ApplicationName")))
    .Times(1)
    .WillOnce(Return(Title));

  EXPECT_CALL(SettingsProxy, Constructor())
    .WillOnce(Return(WindowSectionId))
    .WillOnce(Return(SizeSectionId));

  EXPECT_CALL(SettingsProxy, GetChildSectionImpl(_, uT("Window")))
    .Times(1);

  EXPECT_CALL(SettingsProxy, GetChildSectionImpl(WindowSectionId, uT("Size")))
    .Times(1);

  using ::boost::lexical_cast;

  EXPECT_CALL(SettingsProxy, GetValue(WindowSectionId, uT("IsFullScreen")))
    .Times(1)
    .WillOnce(Return(uT("false")));

  EXPECT_CALL(SettingsProxy, GetValue(WindowSectionId, uT("IsResized")))
    .Times(1)
    .WillOnce(Return(uT("false")));

  EXPECT_CALL(SettingsProxy, GetValue(SizeSectionId, uT("Width")))
    .Times(1)
    .WillOnce(Return(lexical_cast<String_t>(ClientWidth)));

  EXPECT_CALL(SettingsProxy, GetValue(SizeSectionId, uT("Height")))
    .Times(1)
    .WillOnce(Return(lexical_cast<String_t>(ClientHeight)));

  EXPECT_CALL(WindowsProxy, GetModuleHandleW(nullptr))
    .Times(1)
    .WillOnce(Return(hModule));

  EXPECT_CALL(WindowsProxy, GetSystemMetrics(SM_CXSCREEN))
    .Times(1)
    .WillOnce(Return(ScreenWidth));

  EXPECT_CALL(WindowsProxy, GetSystemMetrics(SM_CYSCREEN))
    .Times(1)
    .WillOnce(Return(ScreenHeight));

  {
    using namespace ::alicorn::extension::std;

    InSequence Dummy;

    EXPECT_CALL(WindowsProxy, SetClientRect(0, 0, ClientWidth, ClientHeight))
      .Times(1);

    EXPECT_CALL(WindowsProxy, GetWindowRect())
      .Times(1)
      .WillOnce(Return(WindowRect));

    EXPECT_CALL(WindowsProxy,
      AdjustWindowRectEx(WindowFlags, FALSE, WindowFlagsEx))
      .Times(1)
      .WillOnce(Return(TRUE));

    EXPECT_CALL(WindowsProxy, CreateWindowExW1(WindowFlagsEx,
      Eq(::covellite::app::ClassName), Eq(string_cast<::std::wstring>(Title)),
      WindowFlags, X, Y, WindowWidth, WindowHeight, nullptr, nullptr))
      .Times(1);

    EXPECT_CALL(WindowsProxy, CreateWindowExW2(hModule, nullptr))
      .Times(1)
      .WillOnce(Return(hWnd));

    EXPECT_CALL(WindowsProxy, ShowWindow(hWnd, SW_SHOW))
      .Times(1)
      .WillOnce(Return(TRUE));

    const Application_t Application{ Application_t::EventBased{} };
    const Tested_t Example{ Application };
    const ITested_t & IExample = Example;
    EXPECT_EQ(hWnd, IExample.GetHandle());

    EXPECT_CALL(WindowsProxy, DestroyWindow(hWnd))
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_BoundWindowSize)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const auto ScreenWidth = 1711091235;
  const auto ScreenHeight = 1711091236;
  const RECT WindowRect = { 0, 0, ScreenWidth + 10, ScreenHeight + 10 };

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, GetSystemMetrics(SM_CXSCREEN))
    .Times(1)
    .WillOnce(Return(ScreenWidth));

  EXPECT_CALL(WindowsProxy, GetSystemMetrics(SM_CYSCREEN))
    .Times(1)
    .WillOnce(Return(ScreenHeight));

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, GetValue(_, uT("IsFullScreen")))
    .Times(1)
    .WillOnce(Return(uT("false")));

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillRepeatedly(Return(uT("0")));

  EXPECT_CALL(WindowsProxy, GetWindowRect())
    .Times(1)
    .WillOnce(Return(WindowRect));

  EXPECT_CALL(WindowsProxy, CreateWindowExW1(_, _, _, _, 
    0, 0, ScreenWidth, ScreenHeight, _, _))
    .Times(1);

  EXPECT_CALL(WindowsProxy, CreateWindowExW2(_, _))
    .Times(1)
    .WillOnce(Return((HWND)1711091232));

  const Application_t Application{ Application_t::EventBased{} };
  const Tested_t Example{ Application };
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_AdjustWindowRectEx_Failed)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, GetValue(_, uT("IsFullScreen")))
    .Times(1)
    .WillOnce(Return(uT("false")));

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillRepeatedly(Return(uT("0")));

  EXPECT_CALL(WindowsProxy, AdjustWindowRectEx(_, _, _))
    .Times(1)
    .WillOnce(Return(FALSE));

  EXPECT_CALL(WindowsProxy, CreateWindowExW2(_, _))
    .Times(0);

  const Application_t Application{ Application_t::EventBased{} };
  EXPECT_THROW(Tested_t{ Application }, ::std::exception);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_FullScreenMode)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using InfoProxy_t = ::mock::alicorn::system::version::Info::Proxy;
  InfoProxy_t InfoProxy;
  InfoProxy_t::GetInstance() = &InfoProxy;

  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const ::mock::Id_t InfoId = 1711091251;
  const auto Title = uT("Title1711091252");
  const ::mock::Id_t MainSectionId = 1711091308;
  const ::mock::Id_t WindowSectionId = 1711091255;
  const HWND hWnd = (HWND)1711091257;
  const HMODULE hModule = (HMODULE)1711091258;
  const auto WindowFlags = WS_POPUP;
  const auto WindowFlagsEx = 0;
  const auto ScreenWidth = 1711091259;
  const auto ScreenHeight = 1711091300;

  using namespace ::testing;

  EXPECT_CALL(InfoProxy, Constructor())
    .Times(1)
    .WillOnce(Return(InfoId));

  EXPECT_CALL(InfoProxy, GetValue(InfoId, uT("ApplicationName")))
    .Times(1)
    .WillOnce(Return(Title));

  EXPECT_CALL(SettingsProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowSectionId));

  EXPECT_CALL(SettingsProxy, GetChildSectionImpl(_, uT("Window")))
    .Times(1);

  EXPECT_CALL(SettingsProxy, GetValue(WindowSectionId, uT("IsFullScreen")))
    .Times(1)
    .WillOnce(Return(uT("true")));

  EXPECT_CALL(WindowsProxy, GetModuleHandleW(nullptr))
    .Times(1)
    .WillOnce(Return(hModule));

  EXPECT_CALL(WindowsProxy, GetSystemMetrics(SM_CXSCREEN))
    .Times(1)
    .WillOnce(Return(ScreenWidth));

  EXPECT_CALL(WindowsProxy, GetSystemMetrics(SM_CYSCREEN))
    .Times(1)
    .WillOnce(Return(ScreenHeight));

  {
    using namespace ::alicorn::extension::std;

    InSequence Dummy;

    EXPECT_CALL(WindowsProxy, CreateWindowExW1(WindowFlagsEx,
      Eq(::covellite::app::ClassName), Eq(string_cast<::std::wstring>(Title)),
      WindowFlags, 0, 0, ScreenWidth, ScreenHeight, nullptr, nullptr))
      .Times(1);

    EXPECT_CALL(WindowsProxy, CreateWindowExW2(hModule, nullptr))
      .Times(1)
      .WillOnce(Return(hWnd));

    EXPECT_CALL(WindowsProxy, ShowWindow(hWnd, SW_SHOW))
      .Times(1)
      .WillOnce(Return(TRUE));

    const Application_t Application{ Application_t::EventBased{} };
    const Tested_t Example{ Application };
    const ITested_t & IExample = Example;
    EXPECT_EQ(hWnd, IExample.GetHandle());

    EXPECT_CALL(WindowsProxy, DestroyWindow(hWnd))
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_CreateWindowFailed)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, CreateWindowExW2(_, _))
    .Times(1)
    .WillOnce(Return((HWND)NULL));

  const Application_t Application{ Application_t::EventBased{} };
  EXPECT_THROW(Tested_t{ Application }, ::std::exception);
}

namespace covellite
{

namespace os
{

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_SetWindowLongPtr)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const HWND hWnd = (HWND)1807191040;
  const LONG_PTR OldLongPtr = (LONG_PTR)1807191049;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(WindowsProxy, CreateWindowExW2(_, _))
    .Times(1)
    .WillOnce(Return(hWnd));

  EXPECT_CALL(WindowsProxy, SetWindowLongPtrW(hWnd, GWLP_USERDATA))
    .Times(1)
    .WillOnce(Return(OldLongPtr));

  const Application_t Application{ Application_t::EventBased{} };
  Tested_t Example{ Application };
  EXPECT_EQ(WindowsProxy.m_NewLongPtr, (LONG_PTR)&Example.m_Events);

  EXPECT_CALL(WindowsProxy, DestroyWindow(hWnd))
    .Times(1);
}

} // namespace os

} // namespace covellite

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_CloseWindow)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const UINT Message = WM_CLOSE;

  Application Application;
  ::covellite::events::Events Events = Application;

  Events[::covellite::events::Error.Exception].Connect([]() {});

  const Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(Application, OnExit())
    .Times(1);

  Events[Message]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Resize)
{
  const UINT Message = WM_SIZE;

  Application Application;
  const Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(Application, OnResize())
    .Times(1);

  ::covellite::events::Events Events = Application;
  Events[Message]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Motion)
{
  const UINT Message = WM_MOUSEMOVE;
  const int32_t X = 1234;
  const int32_t Y = 5678;

  Application Application;
  const Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(Application, OnMotion(X, Y))
    .Times(1);

  ::covellite::events::Events Events = Application;
  Events[Message](::std::pair<WPARAM, LPARAM>{ 1807071427, MAKELPARAM(X, Y) });
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Touch)
{
  const UINT Message = WM_LBUTTONDOWN;

  Application Application;
  const Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(Application, OnTouch())
    .Times(1);

  ::covellite::events::Events Events = Application;
  Events[Message]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Release)
{
  const UINT Message = WM_LBUTTONUP;

  Application Application;
  const Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(Application, OnRelease())
    .Times(1);

  ::covellite::events::Events Events = Application;
  Events[Message]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Pressed)
{
  const UINT Message = WM_CHAR;
  const int32_t KeyCode = 1807071510;

  Application Application;
  const Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(Application, OnPressed(KeyCode))
    .Times(1);

  ::covellite::events::Events Events = Application;
  Events[Message](::std::pair<WPARAM, LPARAM>{ KeyCode, 1807071511 });
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Pressed_Enter)
{
  const UINT Message = WM_CHAR;
  const int32_t KeyCode = VK_RETURN;

  Application Application;
  const Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(Application, OnPressed(0x0A))
    .Times(1);

  ::covellite::events::Events Events = Application;
  Events[Message](::std::pair<WPARAM, LPARAM>{ KeyCode, 1807071512 });
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Pressed_Controls)
{
  const UINT Message = WM_CHAR;

  for (int32_t KeyCode = 0; KeyCode < VK_SPACE; KeyCode++)
  {
    if (KeyCode == VK_RETURN) continue;

    Application Application;
    const Tested_t Example{ Application };

    using namespace ::testing;

    EXPECT_CALL(Application, OnPressed(_))
      .Times(0);

    ::covellite::events::Events Events = Application;
    Events[Message](::std::pair<WPARAM, LPARAM>{ KeyCode, 1807071513 });
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Down)
{
  const UINT Message = WM_KEYDOWN;
  const int32_t KeyCode = 1807071428;

  Application Application;
  const Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(Application, OnDown(KeyCode))
    .Times(1);

  ::covellite::events::Events Events = Application;
  Events[Message](::std::pair<WPARAM, LPARAM>{ KeyCode, 1807071426 });
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Up)
{
  const UINT Message = WM_KEYUP;
  const int32_t KeyCode = 1807071429;

  Application Application;
  const Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(Application, OnUp(KeyCode))
    .Times(1);

  ::covellite::events::Events Events = Application;
  Events[Message](::std::pair<WPARAM, LPARAM>{ KeyCode, 1807071430 });
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Back)
{
  const UINT Message = WM_SYSKEYUP;
  const int32_t KeyCode = VK_LEFT;

  Application Application;
  const Tested_t Example{ Application };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Application, OnBack())
    .Times(1);

  ::covellite::events::Events Events = Application;
  Events[Message](::std::pair<WPARAM, LPARAM>{ KeyCode, 1807071431 });

  EXPECT_CALL(Application, OnBack())
    .Times(0);

  Events[Message](::std::pair<WPARAM, LPARAM>{ 1807071432, 1807071431 });
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Menu)
{
  const UINT Message = WM_SYSKEYUP;
  const int32_t KeyCode = VK_SPACE;

  Application Application;
  const Tested_t Example{ Application };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Application, OnMenu())
    .Times(1);

  ::covellite::events::Events Events = Application;
  Events[Message](::std::pair<WPARAM, LPARAM>{ KeyCode, 1807311240 });

  EXPECT_CALL(Application, OnMenu())
    .Times(0);

  Events[Message](::std::pair<WPARAM, LPARAM>{ 1807311241, 1807311242 });
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetClientRect)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  const auto hWnd = (HWND)1808221137;
  const int Left = 8221142;
  const int Width = 1808221143;
  const int Top = 8221144;
  const int Height = 1808221145;
  const RECT ClientRect = { Left, Top, Left + Width, Top + Height };

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, CreateWindowExW2(_, _))
    .Times(1)
    .WillOnce(Return(hWnd));

  const Application_t Application{ Application_t::EventBased{} };
  const Tested_t Example{ Application };
  const ITested_t & IExample = Example;

  EXPECT_CALL(WindowsProxy, BuildClientRect())
    .WillRepeatedly(Return(ClientRect));

  EXPECT_CALL(WindowsProxy, GetClientRect(hWnd))
    .Times(2)
    .WillOnce(Return(FALSE))
    .WillOnce(Return(TRUE));

  EXPECT_THROW(IExample.GetClientRect(), ::std::exception);
  const auto Result = IExample.GetClientRect();
  EXPECT_EQ(0, Result.Left);
  EXPECT_EQ(0, Result.Top);
  EXPECT_EQ(Width, Result.Width);
  EXPECT_EQ(Height, Result.Height);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetSystemLanguage)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, CreateWindowExW2(_, _))
    .Times(1)
    .WillOnce(Return((HWND)1710240026));

  const Application_t Application{ Application_t::EventBased{} };
  const Tested_t Example{ Application };
  const ITested_t & IExample = Example;

  InSequence Dummy;

  const ::std::map<::std::wstring, String_t> Languages =
  {
    { L"en-EN", uT("English") },
    { L"en-US", uT("English") },
    { L"ru-RU", uT("Russian") },
    { L"Unknown", uT("Default") },
  };

  for (const auto & Language : Languages)
  {
    EXPECT_CALL(WindowsProxy, GetLocaleName())
      .Times(1)
      .WillOnce(Return(Language.first));

    EXPECT_CALL(WindowsProxy, GetUserDefaultLocaleName(LOCALE_NAME_MAX_LENGTH))
      .Times(1)
      .WillOnce(Return(1710241049));

    const auto Result = IExample.GetConfiguration().GetSystemLanguage();
    EXPECT_EQ(Language.second, Result);
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetSystemLanguage_Error)
{
  using WindowsProxy_t = ::mock::WindowsProxy;
  WindowsProxy_t WindowsProxy;
  WindowsProxy_t::GetInstance() = &WindowsProxy;

  using namespace ::testing;

  EXPECT_CALL(WindowsProxy, CreateWindowExW2(_, _))
    .Times(1)
    .WillOnce(Return((HWND)1710241058));

  const Application_t Application{ Application_t::EventBased{} };
  const Tested_t Example{ Application };
  const ITested_t & IExample = Example;

  EXPECT_CALL(WindowsProxy, GetLocaleName())
    .Times(1)
    .WillOnce(Return(L""));

  EXPECT_CALL(WindowsProxy, GetUserDefaultLocaleName(LOCALE_NAME_MAX_LENGTH))
    .Times(1)
    .WillOnce(Return(0));

  const auto Result = IExample.GetConfiguration().GetSystemLanguage();
  EXPECT_EQ(uT("Default"), Result);
}
