
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса EventHandler.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define EventHandler_test EventHandler_android_test
#define EventHandler EventHandler_android
#define ClickEventListener ClickEventListener_android

// Расположение класса Application
#include "..\..\Covellite.Core\EventHandler.android.cpp"
#include "..\..\Covellite.Core\EventHandler.cpp"
#include "..\..\Covellite.Core\ClickEventListener.cpp"

// Общий тестовый класс класса EventHandler
class EventHandler_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::core::EventHandler;
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
TEST_F(EventHandler_test, /*DISABLED_*/Test_Constructor_Nullptr_AndroidApp)
{
  EXPECT_THROW(Tested_t(), ::std::exception);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_Constructor_RealObject)
{
  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  {
    Tested_t Example;
    EXPECT_FALSE(AndroidApp.onAppCmd == nullptr);
    EXPECT_FALSE(AndroidApp.onInputEvent == nullptr);
    EXPECT_FALSE(AndroidApp.userData == nullptr);
  }

  EXPECT_TRUE(AndroidApp.onAppCmd == nullptr);
  EXPECT_TRUE(AndroidApp.onInputEvent == nullptr);
  EXPECT_TRUE(AndroidApp.userData == nullptr);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_Constructor_DummyObject)
{
  class Empty
  {
  public:
    static void CommandHandler(android_app *, int32_t) noexcept {}
    static int32_t InputHandler(android_app *, AInputEvent *) noexcept { return 0;}
  };

  android_app AndroidApp;
  AndroidApp.onAppCmd = Empty::CommandHandler;
  AndroidApp.onInputEvent = Empty::InputHandler;
  AndroidApp.userData = (void *)1710121255;

  const AppInfo_t Info{ &AndroidApp };

  {
    Tested_t Example{ Tested_t::Dummy{} };
    EXPECT_TRUE(AndroidApp.onAppCmd == Empty::CommandHandler);
    EXPECT_TRUE(AndroidApp.onInputEvent == Empty::InputHandler);
    EXPECT_TRUE(AndroidApp.userData == (void *)1710121255);
  }

  EXPECT_TRUE(AndroidApp.onAppCmd == Empty::CommandHandler);
  EXPECT_TRUE(AndroidApp.onInputEvent == Empty::InputHandler);
  EXPECT_TRUE(AndroidApp.userData == (void *)1710121255);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_CallCommandHandler_WithoutConstructor)
{
  ::android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  Tested_t Example;
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);
  ASSERT_TRUE(AndroidApp.userData != nullptr);

  auto OnAppCmd = AndroidApp.onAppCmd;

  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  OnAppCmd(nullptr, 1710091338); // Не должно падать
  OnAppCmd(&AndroidApp, APP_CMD_STOP); // Не должно падать
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_InputHandler_WithoutConstructor)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  ::AInputEvent InputEvent;

  const AppInfo_t Info{ &AndroidApp };

  Tested_t Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);
  ASSERT_TRUE(AndroidApp.userData != nullptr);

  auto OnInputEvent = AndroidApp.onInputEvent;

  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  using namespace ::testing;

  EXPECT_CALL(AEventProxy, GetType(_))
    .Times(0);

  {
    // Не должно падать
    const auto Result = OnInputEvent(nullptr, &InputEvent);
    EXPECT_EQ(0, Result);
  }

  {
    const auto Result = OnInputEvent(&AndroidApp, &InputEvent);
    EXPECT_EQ(0, Result);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnSystem_Exception)
{
  const ::std::string Error = "Error1611211454";

  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  using ::covellite::core::Event;

  static const ::std::map<int32_t, Event::System> Commands =
  {
    { APP_CMD_STOP, Event::Stop },
    { APP_CMD_PAUSE, Event::Pause },
    { APP_CMD_RESUME, Event::Resume },
    { APP_CMD_DESTROY, Event::Destroy },
  };

  for (const auto Command : Commands)
  {
    Tested Example{ Command.second };
    ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

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

    AndroidApp.onAppCmd(&AndroidApp, Command.first);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnWindow_Exception)
{
  const ::std::string Error = "Error1709241123";

  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  using ::covellite::core::Event;

  static const ::std::map<int32_t, Event::Window> Commands =
  {
    { APP_CMD_INIT_WINDOW, Event::Create },
    { APP_CMD_TERM_WINDOW, Event::Close },
    { APP_CMD_LOST_FOCUS, Event::LostFocus },
    { APP_CMD_GAINED_FOCUS, Event::GainedFocus },
    { APP_CMD_WINDOW_REDRAW_NEEDED, Event::Resize },
  };

  for (const auto Command : Commands)
  {
    Tested Example{ Command.second };
    ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

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

    AndroidApp.onAppCmd(&AndroidApp, Command.first);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnInput_Exception)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;
  const ::std::string Error = "Error1710221137";

  const AppInfo_t Info{ &AndroidApp };

  static const ::std::map<int32_t, Event::Input> Commands =
  {
    { AMOTION_EVENT_ACTION_MOVE, Event::Motion },
    { AMOTION_EVENT_ACTION_DOWN, Event::Motion },
    { AMOTION_EVENT_ACTION_DOWN, Event::Touch },
    { AMOTION_EVENT_ACTION_UP, Event::Release },
  };

  for (const auto Command : Commands)
  {
    Tested Example{ Command.second };
    ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(AEventProxy, GetType(&InputEvent))
      .Times(1)
      .WillOnce(Return(AINPUT_EVENT_TYPE_MOTION));

    EXPECT_CALL(AEventProxy, GetAction(&InputEvent))
      .Times(1)
      .WillOnce(Return(Command.first));

    EXPECT_CALL(Example, DoInput())
      .Times(1)
      .WillOnce(Throw(::std::exception(Error.c_str())));

    EXPECT_CALL(Example, DoSystem())
      .Times(0);

    EXPECT_CALL(Example, DoWindow())
      .Times(0);

    EXPECT_CALL(Example, DoError(Error))
      .Times(1);

    const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
    EXPECT_EQ(0, Result);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyPressed_Exception)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;
  const ::std::string Error = "Error1611201326";

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_KEY));

  EXPECT_CALL(AEventProxy, GetKeyAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(::covellite::core::Event::APP_CMD_KEY_PRESSED));

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(AtLeast(1))
    .WillRepeatedly(Return(1711011321));

  EXPECT_CALL(Example, DoKeyPressed(_))
    .Times(1)
    .WillOnce(Throw(::std::exception(Error.c_str())));

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoError(Error))
    .Times(1);

  const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyUp_Exception)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;
  const ::std::string Error = "Error1611201327";

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_KEY));

  EXPECT_CALL(AEventProxy, GetKeyAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(AKEY_EVENT_ACTION_UP));

  EXPECT_CALL(Example, DoKeyUp(_))
    .Times(1)
    .WillOnce(Throw(::std::exception(Error.c_str())));

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoError(Error))
    .Times(1);

  const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyDown_Exception)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;
  const ::std::string Error = "Error1711041305";

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_KEY));

  EXPECT_CALL(AEventProxy, GetKeyAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(AKEY_EVENT_ACTION_DOWN));

  EXPECT_CALL(Example, DoKeyDown(_))
    .Times(1)
    .WillOnce(Throw(::std::exception(Error.c_str())));

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoError(Error))
    .Times(1);

  const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnBack_Exception)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;
  const ::std::string Error = "Error1711041212";

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_KEY));

  EXPECT_CALL(AEventProxy, GetKeyAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(AKEY_EVENT_ACTION_UP));

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(1)
    .WillOnce(Return(AKEYCODE_BACK));

  EXPECT_CALL(Example, DoBack())
    .Times(1)
    .WillOnce(Throw(::std::exception(Error.c_str())));

  EXPECT_CALL(Example, DoKeyUp(_))
    .Times(0);

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoError(Error))
    .Times(1);

  const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_CommandHandler_UnknownEvent)
{
  const auto Command = 1709241142;

  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

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

  AndroidApp.onAppCmd(&AndroidApp, Command);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_InputHandler_UnknownEvent)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  {
    InSequence Dummy;

    EXPECT_CALL(AEventProxy, GetType(&InputEvent))
      .Times(1)
      .WillOnce(Return(1611201206));

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

    const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
    EXPECT_EQ(0, Result);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_InputHandler_UnknownKeyAction)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_KEY));

  EXPECT_CALL(AEventProxy, GetKeyAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(1611201300));

  EXPECT_CALL(AEventProxy, GetKeyCode(_))
    .Times(0);

  EXPECT_CALL(Example, DoKeyPressed(_))
    .Times(0);

  EXPECT_CALL(Example, DoKeyUp(_))
    .Times(0);

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

  const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnStop)
{
  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ ::covellite::core::Event::Stop };
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

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

  AndroidApp.onAppCmd(&AndroidApp, APP_CMD_STOP);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnPause)
{
  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ ::covellite::core::Event::Pause };
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

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

  AndroidApp.onAppCmd(&AndroidApp, APP_CMD_PAUSE);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnResume)
{
  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ ::covellite::core::Event::Resume };
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

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

  AndroidApp.onAppCmd(&AndroidApp, APP_CMD_RESUME);
}

namespace covellite
{

namespace core
{

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnDestroy)
{
  android_app AndroidApp;

  const AppInfo_t Info{ &AndroidApp };

  {
    Tested Example{ 1710111327 };
    ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

    EXPECT_FALSE(Example.OnSystem.empty());
    EXPECT_FALSE(Example.OnWindow.empty());
    EXPECT_FALSE(Example.OnInput.empty());
    EXPECT_FALSE(Example.OnFramework.empty());

    AndroidApp.onAppCmd(&AndroidApp, APP_CMD_DESTROY);
    EXPECT_TRUE(Example.OnSystem.empty());
    EXPECT_TRUE(Example.OnWindow.empty());
    EXPECT_TRUE(Example.OnInput.empty());
    EXPECT_TRUE(Example.OnFramework.empty());
  }

  {
    Tested Example{ "1710111328" };
    ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

    EXPECT_FALSE(Example.OnRocket.empty());

    AndroidApp.onAppCmd(&AndroidApp, APP_CMD_DESTROY);
    EXPECT_TRUE(Example.OnRocket.empty());
  }
}

} // namespace core

} // namespace covellite

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnCreate)
{
  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ ::covellite::core::Event::Create };
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

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

  AndroidApp.onAppCmd(&AndroidApp, APP_CMD_INIT_WINDOW);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnClose)
{
  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ ::covellite::core::Event::Close };
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

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

  AndroidApp.onAppCmd(&AndroidApp, APP_CMD_TERM_WINDOW);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnLostFocus)
{
  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ ::covellite::core::Event::LostFocus };
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

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

  AndroidApp.onAppCmd(&AndroidApp, APP_CMD_LOST_FOCUS);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnGainedFocus)
{
  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ ::covellite::core::Event::GainedFocus };
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

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

  AndroidApp.onAppCmd(&AndroidApp, APP_CMD_GAINED_FOCUS);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnResize)
{
  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ ::covellite::core::Event::Resize };
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

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

  AndroidApp.onAppCmd(&AndroidApp, APP_CMD_WINDOW_REDRAW_NEEDED);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnMotion)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;

  const AppInfo_t Info{ &AndroidApp };

  const float X = 1611201225.0f;
  const float Y = 1611201226.0f;

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_MOTION));

  EXPECT_CALL(AEventProxy, GetAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(AMOTION_EVENT_ACTION_MOVE));

  EXPECT_CALL(AEventProxy, GetX(&InputEvent, 0))
    .Times(1)
    .WillOnce(Return(X));

  EXPECT_CALL(AEventProxy, GetY(&InputEvent, 0))
    .Times(1)
    .WillOnce(Return(Y));

  EXPECT_CALL(Example, DoMotion(static_cast<int32_t>(X), static_cast<int32_t>(Y)))
    .Times(1);

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(1, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnTouch)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  ::AInputEvent InputEvent;

  const AppInfo_t Info{ &AndroidApp };

  const float X = 1711021259.0f;
  const float Y = 1711021300.0f;

  Tested Example{ ::covellite::core::Event::Touch };
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_MOTION));

  EXPECT_CALL(AEventProxy, GetAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(AMOTION_EVENT_ACTION_DOWN));

  EXPECT_CALL(AEventProxy, GetX(&InputEvent, 0))
    .Times(1)
    .WillOnce(Return(X));

  EXPECT_CALL(AEventProxy, GetY(&InputEvent, 0))
    .Times(1)
    .WillOnce(Return(Y));

  EXPECT_CALL(Example, DoMotion(static_cast<int32_t>(X), static_cast<int32_t>(Y)))
    .Times(1);

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

  AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnRelease)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  ::AInputEvent InputEvent;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ ::covellite::core::Event::Release };
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_MOTION));

  EXPECT_CALL(AEventProxy, GetAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(AMOTION_EVENT_ACTION_UP));

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

  AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyPressed_IgnoreKeyCode)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  for (int32_t KeyCode = 0; KeyCode < 32; KeyCode++)
  {
    if (KeyCode == 10) continue;

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(AEventProxy, GetType(&InputEvent))
      .Times(1)
      .WillOnce(Return(AINPUT_EVENT_TYPE_KEY));

    EXPECT_CALL(AEventProxy, GetKeyAction(&InputEvent))
      .Times(1)
      .WillOnce(Return(::covellite::core::Event::APP_CMD_KEY_PRESSED));

    EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
      .Times(AtLeast(1))
      .WillRepeatedly(Return(KeyCode));

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

    const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
    EXPECT_EQ(0, Result);
  }
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyPressed)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;
  const int32_t KeyCode = 1611201244;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_KEY));

  EXPECT_CALL(AEventProxy, GetKeyAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(::covellite::core::Event::APP_CMD_KEY_PRESSED));

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(AtLeast(1))
    .WillRepeatedly(Return(KeyCode));

  EXPECT_CALL(Example, DoKeyPressed(KeyCode))
    .Times(1);

  EXPECT_CALL(Example, DoSystem())
    .Times(0);

  EXPECT_CALL(Example, DoWindow())
    .Times(0);

  EXPECT_CALL(Example, DoFramework())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(1, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyUp)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;
  const int32_t KeyCode = 1611201251;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_KEY));

  EXPECT_CALL(AEventProxy, GetKeyAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(AKEY_EVENT_ACTION_UP));

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(AtLeast(1))
    .WillRepeatedly(Return(KeyCode));

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

  const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(1, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnKeyDown)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;
  const int32_t KeyCode = 1711041306;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_KEY));

  EXPECT_CALL(AEventProxy, GetKeyAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(AKEY_EVENT_ACTION_DOWN));

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(AtLeast(1))
    .WillRepeatedly(Return(KeyCode));

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
  
  const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(1, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_OnBack)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ::android_app AndroidApp;
  ::AInputEvent InputEvent;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(AINPUT_EVENT_TYPE_KEY));

  EXPECT_CALL(AEventProxy, GetKeyAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(AKEY_EVENT_ACTION_UP));

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(1)
    .WillOnce(Return(AKEYCODE_BACK));

  EXPECT_CALL(Example, DoBack())
    .Times(1);

  EXPECT_CALL(AEventProxy, GetKeyCode(&InputEvent))
    .Times(1)
    .WillOnce(Return(AKEYCODE_BACK));

  const auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(1, Result);
}

// ************************************************************************** //
TEST_F(EventHandler_test, /*DISABLED_*/Test_PostCommandDrawWindow)
{
  const ::std::string Error = "Error1709242316";
  android_app AndroidApp;

  const AppInfo_t Info{ &AndroidApp };

  Tested Example;
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoBeginDraw())
    .Times(1);

  EXPECT_CALL(Example, DoDraw())
    .Times(1);

  EXPECT_CALL(Example, DoEndDraw())
    .Times(1);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Example, DoBeginDraw())
    .Times(0);

  EXPECT_CALL(Example, DoDraw())
    .Times(0);

  EXPECT_CALL(Example, DoEndDraw())
    .Times(0);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Example, DoBeginDraw())
    .Times(1);

  EXPECT_CALL(Example, DoDraw())
    .Times(1);

  EXPECT_CALL(Example, DoEndDraw())
    .Times(1);

  EXPECT_CALL(Example, DoError(_))
    .Times(0);

  EXPECT_CALL(Example, DoBeginDraw())
    .Times(1);

  EXPECT_CALL(Example, DoDraw())
    .Times(1)
    .WillOnce(Throw(::std::exception{ Error.c_str() }));

  EXPECT_CALL(Example, DoError(Error))
    .Times(1);

  EXPECT_CALL(Example, DoEndDraw())
    .Times(1);

  AndroidApp.onAppCmd(&AndroidApp, APP_CMD_RESUME);

  Example.PostCommandDrawWindow();

  AndroidApp.onAppCmd(&AndroidApp, APP_CMD_PAUSE);

  Example.PostCommandDrawWindow();

  AndroidApp.onAppCmd(&AndroidApp, APP_CMD_RESUME);

  Example.PostCommandDrawWindow();

  Example.PostCommandDrawWindow();
}
