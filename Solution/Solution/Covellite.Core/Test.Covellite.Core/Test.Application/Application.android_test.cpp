
#include "stdafx.h"
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

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define Application_test Application_android_test
#define Application Application_android

// Расположение класса Application
#include "..\..\Covellite.Core\Application.android.cpp"

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
    using Commands_t = ::std::vector<int32_t>;

  public:
    MOCK_CONST_METHOD1(DoPostCommand, bool (bool));

  public:
    bool PostCommand(bool _IsWaitMessage) const override
    {
      if (!m_Commands.empty())
      {
        if (m_CommandIndex >= m_Commands.size()) m_CommandIndex = 0;
        m_pEvents->DoCommand(m_Commands[m_CommandIndex], 
          ::covellite::core::params::Empty{});
        m_CommandIndex++;
      }

      return DoPostCommand(_IsWaitMessage);
    }

  private:
    const Commands_t m_Commands;
    mutable size_t m_CommandIndex = 0;

  public:
    TestedRun(Continuous, const Commands_t & _Commands) :
      Tested_t(Continuous{}),
      m_Commands(_Commands)
    {

    }
    explicit TestedRun(EventBased) :
      Tested_t(EventBased{})
    {

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
TEST_F(Application_test, /*DISABLED_*/Test_GetCommandLine)
{
  const Tested Example;

  const auto Result = Example.GetCommandLine();
  EXPECT_EQ("", Result);
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Run_DrawingMode_Continuous)
{
  using EventHandlerProxy_t = ::mock::covellite::core::EventHandler::Proxy;
  EventHandlerProxy_t EventHandlerProxy;
  EventHandlerProxy_t::GetInstance() = &EventHandlerProxy;

  using ::covellite::core::Event;

  TestedRun::Commands_t Commands =
  {
    0, 0, 0, 0, Event::LostFocus, 0, 0, Event::GainedFocus, 0, 0
  };

  android_app App;
  App.destroyRequested = 0;
  App.StepToDestroyRequest = 4;

  const AppInfo_t Info{ &App };

  const ::mock::Id_t EventHandlerId = 1710092232;

  using namespace ::testing;

  EXPECT_CALL(EventHandlerProxy, Constructor())
    .Times(1)
    .WillOnce(Return(EventHandlerId));

  TestedRun Example{ Tested_t::Continuous{}, Commands };

  InSequence Dummy;

  EXPECT_CALL(Example, DoPostCommand(false))
    .WillOnce(Return(true))
    .WillOnce(Return(true))
    .WillOnce(Return(false));

  EXPECT_CALL(EventHandlerProxy, PostCommandDrawWindow(EventHandlerId))
    .Times(1);

  EXPECT_CALL(Example, DoPostCommand(false))
    .WillOnce(Return(true))
    .WillOnce(Return(false));

  EXPECT_CALL(EventHandlerProxy, PostCommandDrawWindow(EventHandlerId))
    .Times(1);

  EXPECT_CALL(Example, DoPostCommand(true))
    .WillOnce(Return(true))
    .WillOnce(Return(true))
    .WillOnce(Return(false));

  EXPECT_CALL(EventHandlerProxy, PostCommandDrawWindow(EventHandlerId))
    .Times(1);

  EXPECT_CALL(Example, DoPostCommand(false))
    .WillOnce(Return(true))
    .WillOnce(Return(false));

  EXPECT_CALL(EventHandlerProxy, PostCommandDrawWindow(EventHandlerId))
    .Times(1);

  Example.Run();
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_Run_DrawingMode_EventBased)
{
  using EventHandlerProxy_t = ::mock::covellite::core::EventHandler::Proxy;
  EventHandlerProxy_t EventHandlerProxy;
  EventHandlerProxy_t::GetInstance() = &EventHandlerProxy;

  android_app App;
  App.destroyRequested = 0;
  App.StepToDestroyRequest = 2;

  const AppInfo_t Info{ &App };

  const ::mock::Id_t EventHandlerId = 1710092236;

  using namespace ::testing;

  EXPECT_CALL(EventHandlerProxy, Constructor())
    .Times(1)
    .WillOnce(Return(EventHandlerId));

  TestedRun Example{ Tested_t::EventBased{} };

  InSequence Dummy;

  for (size_t i = 0; i < App.StepToDestroyRequest; i++)
  {
    EXPECT_CALL(Example, DoPostCommand(true))
      .Times(1);

    EXPECT_CALL(EventHandlerProxy, PostCommandDrawWindow(EventHandlerId))
      .Times(1);
  }

  Example.Run();
}

// ************************************************************************** //
TEST_F(Application_test, /*DISABLED_*/Test_OnStop)
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

    DoCommand(Example, ::covellite::core::Event::Stop);
  }

  {
    Tested_t Example{ Tested_t::EventBased{} };

    using namespace ::testing;

    EXPECT_CALL(AEventProxy, Finish(&NativeActivity))
      .Times(1);

    DoCommand(Example, ::covellite::core::Event::Stop);
  }
}

namespace covellite
{

namespace core
{

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

} // namespace core

} // namespace covellite

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

  android_app App;

  const AppInfo_t Info{ &App };

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
