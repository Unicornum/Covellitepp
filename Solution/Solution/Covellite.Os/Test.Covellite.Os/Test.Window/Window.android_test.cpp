
#include "stdafx.h"
#include <Covellite/Events.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Window.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_PLAT_ANDROID
#define BOOST_PLAT_ANDROID 1
#define Window_test Window_android_test
#define Window Window_android
#define Configuration Configuration_android

// Расположение класса Window
#include "..\..\Covellite.Os\Window.android.cpp"
#include "..\..\Covellite.Os\Window.cpp"
#include "..\..\Covellite.Os\Configuration.android.cpp"
#include "..\..\Covellite.Os\Configuration.cpp"

// Общий тестовый класс класса Window
class Window_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::os::Window;
  using ITested_t = ::covellite::os::IWindow;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  using String_t = ::alicorn::extension::std::String;
  using Application_t = ::mock::covellite::app::Application;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }

protected:
  class Application :
    public Application_t
  {
  public:
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
TEST_F(Window_test, /*DISABLED_*/Test_GetHandle)
{
  ANativeWindow Window;

  android_app App;
  App.window = &Window;

  const AppInfo_t Info{ &App };

  const Application Application;
  const Tested_t Example{ Application };
  const ITested_t & IExample = Example;

  const auto Result = IExample.GetHandle();
  EXPECT_EQ(&Window, ::covellite::any_cast<ANativeWindow *>(Result));
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Resize)
{
  ANativeWindow Window;

  android_app App;
  App.window = &Window;

  const AppInfo_t Info{ &App };

  Application Application;
  Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(Application, OnResize())
    .Times(1);

  ::covellite::events::Events Events = Example;
  Events[(int32_t)APP_CMD_WINDOW_REDRAW_NEEDED]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Motion)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  const auto X = 7081012;
  const auto Y = 7081013;
  const int32_t Message = 
    // cppcheck-suppress integerOverflow
    (AINPUT_EVENT_TYPE_MOTION << 8) | AMOTION_EVENT_ACTION_MOVE;

  AInputEvent InputEvent;
  ANativeWindow Window;

  android_app App;
  App.window = &Window;

  const AppInfo_t Info{ &App };

  Application Application;
  Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, GetX(&InputEvent, 0))
    .Times(1)
    .WillOnce(Return((float)X));

  EXPECT_CALL(AEventProxy, GetY(&InputEvent, 0))
    .Times(1)
    .WillOnce(Return((float)Y));

  EXPECT_CALL(Application, OnMotion(X, Y))
    .Times(1);

  ::covellite::events::Events Events = Example;
  Events[Message](&InputEvent);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Touch)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  const auto X = 7081047;
  const auto Y = 7081048;
  const int32_t Message = 
    // cppcheck-suppress integerOverflow
    (AINPUT_EVENT_TYPE_MOTION << 8) | AMOTION_EVENT_ACTION_DOWN;

  AInputEvent InputEvent;
  ANativeWindow Window;

  android_app App;
  App.window = &Window;

  const AppInfo_t Info{ &App };

  Application Application;
  Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, GetX(&InputEvent, 0))
    .Times(1)
    .WillOnce(Return((float)X));

  EXPECT_CALL(AEventProxy, GetY(&InputEvent, 0))
    .Times(1)
    .WillOnce(Return((float)Y));

  InSequence Dummy;

  EXPECT_CALL(Application, OnMotion(X, Y))
    .Times(1);

  EXPECT_CALL(Application, OnTouch())
    .Times(1);

  ::covellite::events::Events Events = Example;
  Events[Message](&InputEvent);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Release)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  const auto X = 7081050;
  const auto Y = 7081051;
  const int32_t Message =
    // cppcheck-suppress integerOverflow
    (AINPUT_EVENT_TYPE_MOTION << 8) | AMOTION_EVENT_ACTION_UP;

  AInputEvent InputEvent;
  ANativeWindow Window;

  android_app App;
  App.window = &Window;

  const AppInfo_t Info{ &App };

  Application Application;
  Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, GetX(&InputEvent, 0))
    .Times(1)
    .WillOnce(Return((float)X));

  EXPECT_CALL(AEventProxy, GetY(&InputEvent, 0))
    .Times(1)
    .WillOnce(Return((float)Y));

  InSequence Dummy;

  EXPECT_CALL(Application, OnMotion(X, Y))
    .Times(1);

  EXPECT_CALL(Application, OnRelease())
    .Times(1);

  ::covellite::events::Events Events = Example;
  Events[Message](&InputEvent);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Pressed)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  const auto Code = 1807081055;
  // cppcheck-suppress integerOverflow
  const int32_t Message = (AINPUT_EVENT_TYPE_KEY << 8) | 
    ::covellite::events::Key_t::APP_CMD_KEY_PRESSED;

  AInputEvent InputEvent;
  ANativeWindow Window;

  android_app App;
  App.window = &Window;

  const AppInfo_t Info{ &App };

  Application Application;
  Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(1)
    .WillOnce(Return(Code));

  EXPECT_CALL(Application, OnPressed(Code))
    .Times(1);

  ::covellite::events::Events Events = Example;
  Events[Message](&InputEvent);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Down)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  const auto Code = 1807081110;
  const int32_t Message =
    // cppcheck-suppress integerOverflow
    (AINPUT_EVENT_TYPE_KEY << 8) | AKEY_EVENT_ACTION_DOWN;

  AInputEvent InputEvent;
  ANativeWindow Window;

  android_app App;
  App.window = &Window;

  const AppInfo_t Info{ &App };

  Application Application;
  Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(1)
    .WillOnce(Return(Code));

  EXPECT_CALL(Application, OnDown(Code))
    .Times(1);

  ::covellite::events::Events Events = Example;
  Events[Message](&InputEvent);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Up)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  const auto Code = 1807081112;
  const int32_t Message =
    // cppcheck-suppress integerOverflow
    (AINPUT_EVENT_TYPE_KEY << 8) | AKEY_EVENT_ACTION_UP;

  AInputEvent InputEvent;
  ANativeWindow Window;

  android_app App;
  App.window = &Window;

  const AppInfo_t Info{ &App };

  Application Application;
  Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(1)
    .WillOnce(Return(Code));

  EXPECT_CALL(Application, OnUp(Code))
    .Times(1);

  EXPECT_CALL(Application, OnBack())
    .Times(0);

  ::covellite::events::Events Events = Example;
  Events[Message](&InputEvent);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Back)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  const auto Code = AKEYCODE_BACK;
  const int32_t Message =
    // cppcheck-suppress integerOverflow
    (AINPUT_EVENT_TYPE_KEY << 8) | AKEY_EVENT_ACTION_UP;

  AInputEvent InputEvent;
  ANativeWindow Window;

  android_app App;
  App.window = &Window;

  const AppInfo_t Info{ &App };

  Application Application;
  Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(1)
    .WillOnce(Return(Code));

  EXPECT_CALL(Application, OnBack())
    .Times(1);

  EXPECT_CALL(Application, OnMenu())
    .Times(0);

  EXPECT_CALL(Application, OnUp(_))
    .Times(0);

  ::covellite::events::Events Events = Example;
  Events[Message](&InputEvent);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Menu)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  const auto Code = AKEYCODE_MENU;
  const int32_t Message =
    // cppcheck-suppress integerOverflow
    (AINPUT_EVENT_TYPE_KEY << 8) | AKEY_EVENT_ACTION_UP;

  AInputEvent InputEvent;
  ANativeWindow Window;

  android_app App;
  App.window = &Window;

  const AppInfo_t Info{ &App };

  Application Application;
  Tested_t Example{ Application };

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(1)
    .WillOnce(Return(Code));

  EXPECT_CALL(Application, OnMenu())
    .Times(1);

  EXPECT_CALL(Application, OnBack())
    .Times(0);

  EXPECT_CALL(Application, OnUp(_))
    .Times(0);

  ::covellite::events::Events Events = Example;
  Events[Message](&InputEvent);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetClientRect)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  using EnvironmentProxy_t = ::alicorn::system::platform::proxy::Environment;
  EnvironmentProxy_t EnvironmentProxy;
  EnvironmentProxy_t::GetInstance() = &EnvironmentProxy;

  const int32_t Top = 1808221157;
  const int32_t Width = 1808221158;
  const int32_t Height = 1808221159;
  const ::mock::Id_t EnvironmentId = 1808221200;

  ANativeWindow Window;

  android_app App;
  App.window = &Window;
  auto * pWindow = &Window;

  const AppInfo_t Info{ &App };

  const Application Application;
  const Tested_t Example{ Application };
  const ITested_t & IExample = Example;

  using namespace ::testing;

  EXPECT_CALL(EnvironmentProxy, Constructor())
    .Times(1)
    .WillOnce(Return(EnvironmentId));

  EXPECT_CALL(EnvironmentProxy, GetStatusBarHeight(EnvironmentId))
    .Times(1)
    .WillOnce(Return(Top));

  EXPECT_CALL(AEventProxy, GetWidth(pWindow))
    .Times(1)
    .WillOnce(Return(Width));

  EXPECT_CALL(AEventProxy, GetHeight(pWindow))
    .Times(1)
    .WillOnce(Return(Height));

  const auto Result = IExample.GetClientRect();
  EXPECT_EQ(0, Result.Left);
  EXPECT_EQ(Top, Result.Top);
  EXPECT_EQ(Width, Result.Width);
  EXPECT_EQ(Height, Result.Height);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetSystemLanguage)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ANativeWindow Window;
  AConfiguration Config;

  android_app App;
  App.window = &Window;
  App.config = &Config;

  const AppInfo_t Info{ &App };

  const Application Application;
  const Tested_t Example{ Application };
  const ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  const ::std::map<::std::string, String_t> Languages =
  {
    { "en", uT("English") },
    { "en", uT("English") },
    { "ru", uT("Russian") },
    { "Unknown", uT("Default") },
  };

  for (const auto & Language : Languages)
  {
    EXPECT_CALL(AEventProxy, GetLanguage(&Config))
      .Times(1)
      .WillOnce(Return(Language.first));

    const auto Result = IExample.GetConfiguration().GetSystemLanguage();
    EXPECT_EQ(Language.second, Result);
  }
}
