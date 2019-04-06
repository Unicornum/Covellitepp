
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

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define Application_test Application_android_test
#define Application Application_android

// Расположение класса Application
#include "..\..\Covellite.App\Application.android.cpp"

#include <Covellite\App\IWindow.hpp>

// Общий тестовый класс класса Application
class Application_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::app::Application;
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
    MOCK_METHOD1(DoSystemMessage, void(int32_t));
    MOCK_METHOD1(DoInputMessage, void(AInputEvent *));
    MOCK_METHOD0(DoStart, void(void));
    MOCK_METHOD0(DoStop, void(void));

  public:
    explicit Tested(const Run_t & _Run = [](void) {}) : 
      Tested_t(_Run) 
    {
      using namespace ::covellite;

      m_Events[events::Application.Start]
        .Connect([&]() { DoStart(); });
      m_Events[events::Application.Exit]
        .Connect([&]() { DoStop(); });
    }
    explicit Tested(int32_t _Message) :
      Tested_t([](void) {})
    {
      m_Events[_Message]
        .Connect([=]() { DoSystemMessage(_Message); });
    }
    Tested(int32_t _Message, AInputEvent *) :
      Tested_t([](void) {})
    {
      m_Events[_Message]
        .Connect([&](AInputEvent * _pEvent) { DoInputMessage(_pEvent); });
    }
  };

  class TestedRun :
    public Tested_t
  {
  public:
    using Commands_t = ::std::vector<int32_t>;

  public:
    MOCK_METHOD1(DoPostCommand, bool (bool));
    MOCK_METHOD0(DoUpdate, void(void));

  public:
    bool PostCommand(bool _IsWaitMessage) override
    {
      auto & AndroidApp = AppInfo_t::Get<android_app>();

      if (AndroidApp.StepToDestroyRequest > 0)
      {
        AndroidApp.StepToDestroyRequest--;

        if (AndroidApp.StepToDestroyRequest == 0)
        {
          AndroidApp.destroyRequested = 1;
        }
      }

      if (!m_Commands.empty())
      {
        if (m_CommandIndex >= m_Commands.size()) m_CommandIndex = 0;
        m_Events[m_Commands[m_CommandIndex]]();
        m_CommandIndex++;
      }

      return DoPostCommand(_IsWaitMessage);
    }

  private:
    const Commands_t m_Commands;
    size_t m_CommandIndex = 0;

  public:
    TestedRun(Continuous, const Commands_t & _Commands) :
      Tested_t(Continuous{}),
      m_Commands(_Commands)
    {
      using namespace ::covellite;

      m_Events[events::Application.Update].Connect([&]() { DoUpdate(); });
    }
    explicit TestedRun(EventBased) :
      Tested_t(EventBased{})
    {
      using namespace ::covellite;

      m_Events[events::Application.Update].Connect([&]() { DoUpdate(); });
    }
  };
};

// Образец макроса для подстановки в класс Application 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Application_test, Test_Function);

#pragma warning(push)
#pragma warning(disable: 6011)

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Main)
{
  using AndroidLogProxy_t = ::mock::AndroidLogProxy;
  AndroidLogProxy_t AndroidLogProxy;
  AndroidLogProxy_t::GetInstance() = &AndroidLogProxy;

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
    static android_app & GetApp(void)
    {
      static android_app App;
      return App;
    }

  public:
    Real(void) : 
      Tested([&](void)
        {
          Proxy_t::GetInstance()->Run();

          using AppInfo_t = ::alicorn::system::platform::AppInfo;

          EXPECT_EQ(&GetApp(), &AppInfo_t::Get<android_app>());
          EXPECT_EQ(GetApp().activity, &AppInfo_t::Get<ANativeActivity>());
          EXPECT_EQ(GetApp().activity->vm, &AppInfo_t::Get<JavaVM>());
          EXPECT_EQ(GetApp().activity, AppInfo_t::Get<ANativeActivity *>());
          EXPECT_EQ(GetApp().window, AppInfo_t::Get<ANativeWindow *>());
          EXPECT_EQ(GetApp().config, AppInfo_t::Get<AConfiguration *>());
        })
    {
    }
  };

  JavaVM VM;
  ANativeActivity Activity;
  Activity.vm = &VM;

  ANativeWindow Window;
  AConfiguration Configuration;

  Real::GetApp().activity = &Activity;
  Real::GetApp().window = &Window;
  Real::GetApp().config = &Configuration;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AndroidLogProxy, Print(ANDROID_LOG_INFO, Eq("Covellite++"),
    Eq("Application::Main(): start program.")))
    .Times(1);

  EXPECT_CALL(Proxy, Run())
    .Times(1);

  EXPECT_CALL(AndroidLogProxy, Print(ANDROID_LOG_INFO, Eq("Covellite++"),
    Eq("Application::Main(): finish program.")))
    .Times(1);

  Tested_t::Main<Real>(&Real::GetApp());
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Main_StdException)
{
  using AndroidLogProxy_t = ::mock::AndroidLogProxy;
  AndroidLogProxy_t AndroidLogProxy;
  AndroidLogProxy_t::GetInstance() = &AndroidLogProxy;

  JavaVM VM;
  ANativeActivity Activity;
  Activity.vm = &VM;

  ANativeWindow Window;
  AConfiguration Configuration;

  ::android_app AndroidApp;
  AndroidApp.activity = &Activity;
  AndroidApp.window = &Window;
  AndroidApp.config = &Configuration;

  class Real :
    public Tested
  {
  public:
    static const char * GetError(void) 
    {
      static const char Error[] = "Error1710151328";
      return Error;
    }

  public:
    Real(void) : Tested([&](void) { throw ::std::exception{ GetError() }; }) {}
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AndroidLogProxy, Print(ANDROID_LOG_INFO, Eq("Covellite++"),
    Eq("Application::Main(): start program.")))
    .Times(1);

  EXPECT_CALL(AndroidLogProxy, Print(ANDROID_LOG_ERROR, Eq("Covellite++"),
    Eq("Application::Main(): error [%s].")))
    .Times(1);

  EXPECT_CALL(AndroidLogProxy, Arg(Eq(Real::GetError())))
    .Times(1);

  EXPECT_CALL(AndroidLogProxy, Print(ANDROID_LOG_INFO, Eq("Covellite++"),
    Eq("Application::Main(): finish program.")))
    .Times(1);

  Tested_t::Main<Real>(&AndroidApp);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Main_UnknownException)
{
  using AndroidLogProxy_t = ::mock::AndroidLogProxy;
  AndroidLogProxy_t AndroidLogProxy;
  AndroidLogProxy_t::GetInstance() = &AndroidLogProxy;

  JavaVM VM;
  ANativeActivity Activity;
  Activity.vm = &VM;

  ANativeWindow Window;
  AConfiguration Configuration;

  ::android_app AndroidApp;
  AndroidApp.activity = &Activity;
  AndroidApp.window = &Window;
  AndroidApp.config = &Configuration;

  class Real :
    public Tested
  {
  public:
    Real(void) : Tested([&](void) { throw 1; }) {}
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AndroidLogProxy, Print(ANDROID_LOG_INFO, Eq("Covellite++"),
    Eq("Application::Main(): start program.")))
    .Times(1);

  EXPECT_CALL(AndroidLogProxy, Print(ANDROID_LOG_ERROR, Eq("Covellite++"),
    Eq("Application::Main(): error [unknown].")))
    .Times(1);

  EXPECT_CALL(AndroidLogProxy, Print(ANDROID_LOG_INFO, Eq("Covellite++"),
    Eq("Application::Main(): finish program.")))
    .Times(1);

  Tested_t::Main<Real>(&AndroidApp);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Constructor_Nullptr_AndroidApp)
{
  EXPECT_THROW(Tested{}, ::std::exception);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_CommandHandlers)
{
  android_app AndroidApp;
  AndroidApp.onAppCmd = nullptr;
  AndroidApp.onInputEvent = nullptr;
  AndroidApp.userData = nullptr;

  const AppInfo_t Info{ &AndroidApp };

  {
    Tested Example;
    EXPECT_FALSE(AndroidApp.onAppCmd == nullptr);
    EXPECT_FALSE(AndroidApp.onInputEvent == nullptr);
    EXPECT_FALSE(AndroidApp.userData == nullptr);
  }

  EXPECT_TRUE(AndroidApp.onAppCmd == nullptr);
  EXPECT_TRUE(AndroidApp.onInputEvent == nullptr);
  EXPECT_TRUE(AndroidApp.userData == nullptr);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_OnAppCmd)
{
  const int32_t Message = 1807051418;

  android_app AndroidApp = { 0 };
  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ Message };
  ASSERT_TRUE(AndroidApp.onAppCmd != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Example, DoSystemMessage(_))
    .Times(0);

  AndroidApp.onAppCmd(nullptr, 1807051426);

  EXPECT_CALL(Example, DoSystemMessage(Message))
    .Times(1);

  AndroidApp.onAppCmd(&AndroidApp, Message);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_GenerateApplicationEvents)
{
  android_app AndroidApp = { 0 };
  const AppInfo_t Info{ &AndroidApp };

  using namespace ::testing;

  {
    Tested Example;
    ::covellite::events::Events Events = Example;

    EXPECT_CALL(Example, DoStart())
      .Times(1);

    Events[(int32_t)APP_CMD_INIT_WINDOW]();
  }

  {
    Tested Example;
    ::covellite::events::Events Events = Example;

    EXPECT_CALL(Example, DoStop())
      .Times(1);

    Events[(int32_t)APP_CMD_STOP]();
  }
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_OnInputEvent_Nullptr)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  AInputEvent InputEvent;
  android_app AndroidApp = { 0 };
  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ 0, &InputEvent };
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(_))
    .Times(0);

  EXPECT_CALL(AEventProxy, GetAction(_))
    .Times(0);

  EXPECT_CALL(Example, DoInputMessage(_))
    .Times(0);

  auto Result = AndroidApp.onInputEvent(nullptr, &InputEvent);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_OnInputEvent_Success)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  const int32_t EventType = 1;
  const int32_t ActionType = 3;
  const auto Message = (EventType << 8) | ActionType;

  AInputEvent InputEvent;
  android_app AndroidApp = { 0 };
  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ Message, &InputEvent };
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(EventType));

  EXPECT_CALL(AEventProxy, GetAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(ActionType));

  EXPECT_CALL(Example, DoInputMessage(&InputEvent))
    .Times(1);

  auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(1, Result);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_OnInputEvent_Exception)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  const int32_t EventType = 2;
  const int32_t ActionType = 5;
  const auto Message = (EventType << 8) | ActionType;

  AInputEvent InputEvent;
  android_app AndroidApp = { 0 };
  const AppInfo_t Info{ &AndroidApp };

  Tested Example{ Message, &InputEvent };
  ASSERT_TRUE(AndroidApp.onInputEvent != nullptr);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(AEventProxy, GetType(&InputEvent))
    .Times(1)
    .WillOnce(Return(EventType));

  EXPECT_CALL(AEventProxy, GetAction(&InputEvent))
    .Times(1)
    .WillOnce(Return(ActionType));

  EXPECT_CALL(Example, DoInputMessage(&InputEvent))
    .Times(1)
    .WillOnce(Throw(::std::exception{}));

  auto Result = AndroidApp.onInputEvent(&AndroidApp, &InputEvent);
  EXPECT_EQ(0, Result);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_GetCommandLine)
{
  android_app AndroidApp = { 0 };
  const AppInfo_t Info{ &AndroidApp };

  const Tested Example;

  const auto Result = Example.GetCommandLine();
  EXPECT_EQ("", Result);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Run_DrawingMode_Continuous)
{
  TestedRun::Commands_t Commands =
  {
    0, 0, 0, 0, APP_CMD_LOST_FOCUS, 0, 0, APP_CMD_GAINED_FOCUS, 0, 0
  };

  android_app App;
  App.destroyRequested = 0;
  App.StepToDestroyRequest = Commands.size();

  const AppInfo_t Info{ &App };

  using namespace ::testing;

  TestedRun Example{ Tested_t::Continuous{}, Commands };

  InSequence Dummy;

  EXPECT_CALL(Example, DoPostCommand(false))
    .WillOnce(Return(true))
    .WillOnce(Return(true))
    .WillOnce(Return(false));

  EXPECT_CALL(Example, DoUpdate())
    .Times(1);

  EXPECT_CALL(Example, DoPostCommand(false))
    .WillOnce(Return(true))
    .WillOnce(Return(false));

  EXPECT_CALL(Example, DoUpdate())
    .Times(1);

  EXPECT_CALL(Example, DoPostCommand(true))
    .WillOnce(Return(true));

  EXPECT_CALL(Example, DoUpdate())
    .Times(1);

  EXPECT_CALL(Example, DoPostCommand(true))
    .WillOnce(Return(true));

  EXPECT_CALL(Example, DoUpdate())
    .Times(1);

  EXPECT_CALL(Example, DoPostCommand(true))
    .WillOnce(Return(true));

  EXPECT_CALL(Example, DoUpdate())
    .Times(1);

  EXPECT_CALL(Example, DoPostCommand(false))
    .WillOnce(Return(true))
    .WillOnce(Return(false));

  EXPECT_CALL(Example, DoUpdate())
    .Times(1);

  Example.Run();
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Run_DrawingMode_EventBased)
{
  android_app App;
  App.destroyRequested = 0;
  App.StepToDestroyRequest = 2;

  const AppInfo_t Info{ &App };

  using namespace ::testing;

  TestedRun Example{ Tested_t::EventBased{} };

  InSequence Dummy;

  for (size_t i = 0; i < App.StepToDestroyRequest; i++)
  {
    EXPECT_CALL(Example, DoPostCommand(true))
      .Times(1);

    EXPECT_CALL(Example, DoUpdate())
      .Times(1);
  }

  Example.Run();
}

namespace covellite
{

namespace app
{

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_OnTermWindow)
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

  public:
    ExampleWindow(void) :
      m_Id(Proxy::GetInstance()->Constructor())
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

  android_app AndroidApp = { 0 };
  const AppInfo_t Info{ &AndroidApp };

  using namespace ::testing;

  const ::std::vector<::mock::Id_t> WindowIds = 
  {
    1810081517,
    1810081518,
    1810081519
  };

  Tested_t Example{ [](void) {} };

  using namespace ::testing;

  InSequence Dummy;

  for (int i = 0; i < WindowIds.size(); i++)
  {
    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowIds[i]));

    auto & Window = Example.MakeWindow<ExampleWindow>();
    EXPECT_EQ(WindowIds[i], Window.m_Id);
    ASSERT_EQ(i + 1, Example.m_Windows.size());
  }

  EXPECT_CALL(WindowProxy, Destructor(WindowIds[2]))
    .Times(1);

  EXPECT_CALL(WindowProxy, Destructor(WindowIds[1]))
    .Times(1);

  EXPECT_CALL(WindowProxy, Destructor(WindowIds[0]))
    .Times(1);

  ::covellite::events::Events Events = Example;
  Events[(int32_t)APP_CMD_TERM_WINDOW]();

  {
    // Это чтобы убедиться, что окна были разрушены по событию, а не 
    // в деструкторе.

    EXPECT_CALL(WindowProxy, Constructor())
      .Times(1)
      .WillOnce(Return(0));

    Example.MakeWindow<ExampleWindow>();

    EXPECT_CALL(WindowProxy, Destructor(0))
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_OnExit)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  ANativeActivity NativeActivity;

  android_app App;
  App.activity = &NativeActivity;

  const AppInfo_t Info{ &App };

  {
    Tested_t Example{ Tested_t::Continuous{} };

    using namespace ::testing;

    EXPECT_CALL(AEventProxy, Finish(&NativeActivity))
      .Times(1);

    ::covellite::events::Events Events = Example;
    Events[::covellite::events::Application.Exit]();
  }

  {
    Tested_t Example{ Tested_t::EventBased{} };

    using namespace ::testing;

    EXPECT_CALL(AEventProxy, Finish(&NativeActivity))
      .Times(1);

    ::covellite::events::Events Events = Example;
    Events[::covellite::events::Application.Exit]();
  }
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_PostCommand)
{
  using AEventProxy_t = ::mock::AEventProxy;
  AEventProxy_t AEventProxy;
  AEventProxy_t::GetInstance() = &AEventProxy;

  android_app App;

  android_poll_source Source;
  Source.process = Process;

  const AppInfo_t Info{ &App };

  Tested Example;

  using namespace ::testing;

  {
    InSequence Dummy;

    AEventProxy.m_pOutData = &Source;

    EXPECT_CALL(AEventProxy, PollAll(-1, nullptr, nullptr))
      .Times(1)
      .WillOnce(Return(1612251127));

    EXPECT_CALL(AEventProxy, Process(&App, &Source))
      .Times(1);

    const auto Result = Example.PostCommand(true);
    EXPECT_TRUE(Result);
  }

  {
    InSequence Dummy;

    AEventProxy.m_pOutData = &Source;

    EXPECT_CALL(AEventProxy, PollAll(-1, nullptr, nullptr))
      .Times(1)
      .WillOnce(Return(-1));

    EXPECT_CALL(AEventProxy, Process(_, _))
      .Times(0);

    const auto Result = Example.PostCommand(true);
    EXPECT_FALSE(Result);
  }

  {
    InSequence Dummy;

    AEventProxy.m_pOutData = nullptr;

    EXPECT_CALL(AEventProxy, PollAll(0, nullptr, nullptr))
      .Times(1)
      .WillOnce(Return(1612251127));

    EXPECT_CALL(AEventProxy, Process(_, _))
      .Times(0);

    const auto Result = Example.PostCommand(false);
    EXPECT_FALSE(Result);
  }

  {
    InSequence Dummy;

    AEventProxy.m_pOutData = nullptr;

    EXPECT_CALL(AEventProxy, PollAll(0, nullptr, nullptr))
      .Times(1)
      .WillOnce(Return(-1));

    EXPECT_CALL(AEventProxy, Process(_, _))
      .Times(0);

    const auto Result = Example.PostCommand(false);
    EXPECT_FALSE(Result);
  }
}

} // namespace app

} // namespace covellite

#pragma warning(pop)
