
#include "stdafx.h"
#include <alicorn\boost\string-cast.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Window.
*/

// Расположение класса Window
#include <Covellite\Rocket\Window.hpp>

// Общий тестовый класс класса Window
class Window_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::rocket::Window;
  using ITested_t = ::covellite::core::IWindow;
  using WindowOs_t = ::mock::covellite::os::Window;
  using IWindowApiPtr_t = ::std::shared_ptr<::covellite::api::IWindow>;
  using WindowApi_t = ::mock::covellite::api::Window<int>;
  using RenderInterfacePtr_t = 
    ::std::shared_ptr<::mock::Rocket::Core::RenderInterface>;
  using Vector_t = ::mock::Rocket::Core::Vector2i;
  using String_t = ::mock::Rocket::Core::String;
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  using Path_t = ::boost::filesystem::path;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    using namespace ::alicorn::extension::std;

    ::testing::DefaultValue<RenderInterfacePtr_t>::Set(m_pRenderInterface);
    ::testing::DefaultValue<String_t>::Set("DefaultString");
    ::testing::DefaultValue<String>::Set(string_cast<String>(m_PathToFontsDirectory));
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    using namespace ::alicorn::extension::std;

    ::testing::DefaultValue<RenderInterfacePtr_t>::Clear();
    ::testing::DefaultValue<String_t>::Clear();
    ::testing::DefaultValue<String>::Clear();
  }

protected:
  const Path_t m_PathToFontsDirectory = THIS_DIRECTORY / L"fonts";

private:
  RenderInterfacePtr_t m_pRenderInterface =
    ::std::make_shared<::mock::covellite::api::RenderOpenGL>(0);
};

// Образец макроса для подстановки в класс Window 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Window_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor_CreateContext_Fail)
{
  using RocketProxy_t = ::mock::Rocket::Core::Proxy;
  RocketProxy_t RocketProxy;
  RocketProxy_t::GetInstance() = &RocketProxy;

  android_app App;

  const AppInfo_t Info{ &App };

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  using namespace ::testing;

  EXPECT_CALL(RocketProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_STDEXCEPTION(Tested_t{ pWindowApi }, ".+Create context failed.");
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor)
{
  using StringTranslatorProxy_t = 
    ::mock::covellite::rocket::StringTranslator::Proxy;
  StringTranslatorProxy_t StringTranslatorProxy;
  StringTranslatorProxy_t::GetInstance() = &StringTranslatorProxy;

  using WindowApiProxy_t = ::mock::covellite::api::Window<int>::Proxy;
  WindowApiProxy_t WindowApiProxy;
  WindowApiProxy_t::GetInstance() = &WindowApiProxy;

  using InterfacesProxy_t = ::mock::covellite::InterfacesProxy;
  InterfacesProxy_t InterfacesProxy;
  InterfacesProxy_t::GetInstance() = &InterfacesProxy;

  using WindowCoreProxy_t = ::mock::covellite::core::Window::Proxy;
  WindowCoreProxy_t WindowCoreProxy;
  WindowCoreProxy_t::GetInstance() = &WindowCoreProxy;

  using InitializerProxy_t = ::mock::covellite::rocket::Initializer::Proxy;
  InitializerProxy_t InitializerProxy;
  InitializerProxy_t::GetInstance() = &InitializerProxy;

  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using RocketControlsProxy_t = ::mock::Rocket::Controls::Proxy;
  RocketControlsProxy_t RocketControlsProxy;
  RocketControlsProxy_t::GetInstance() = &RocketControlsProxy;

  using RocketDebuggerProxy_t = ::mock::Rocket::Debugger::Proxy;
  RocketDebuggerProxy_t RocketDebuggerProxy;
  RocketDebuggerProxy_t::GetInstance() = &RocketDebuggerProxy;

  using FontDatabaseProxy_t = ::mock::Rocket::Core::FontDatabase::Proxy;
  FontDatabaseProxy_t FontDatabaseProxy;
  FontDatabaseProxy_t::GetInstance() = &FontDatabaseProxy;

  const ::mock::Id_t StringTranslatorId = 1612212313;
  const ::mock::Id_t WindowApiId = 1612202302;
  const ::mock::Id_t IRenderId = 1709291347;
  const ::mock::Id_t WindowCoreId = 1612202237;
  const int StatusBarHeight = 1612202238;
  const int Width = 1612202303;
  const int Height = 1612202304;
  const Vector_t ContextSize(Width, Height - StatusBarHeight);

  android_app App;

  const AppInfo_t Info{ &App };

  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  EXPECT_CALL(StringTranslatorProxy, Constructor())
    .Times(2)
    .WillRepeatedly(Return(StringTranslatorId));

  EXPECT_CALL(WindowCoreProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowCoreId));

  InSequence Dummy;

  EXPECT_CALL(WindowApiProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(WindowApiId));

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  IWindowApiPtr_t pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  EXPECT_CALL(InterfacesProxy, RenderConstructor(_))
    .Times(1)
    .WillOnce(Return(IRenderId));

  RenderInterfacePtr_t pRenderInterface =
    ::std::make_shared<::mock::covellite::api::RenderOpenGL>(0);

  auto pStringTranslator =
    ::std::make_shared<::mock::covellite::rocket::StringTranslator>();

  ::mock::covellite::rocket::Initializer::Data Data =
  {
    pRenderInterface,
    pStringTranslator
  };

  EXPECT_CALL(WindowCoreProxy, GetStatusBarHeight(WindowCoreId))
    .Times(1)
    .WillOnce(Return(StatusBarHeight));

  EXPECT_CALL(WindowApiProxy, MakeRenderInterface(WindowApiId))
    .Times(1)
    .WillOnce(Return(pRenderInterface));

  EXPECT_CALL(InitializerProxy, Constructor(Data))
    .Times(1);

  EXPECT_CALL(WindowApiProxy, GetHeight(WindowApiId))
    .Times(1)
    .WillOnce(Return(Height));

  EXPECT_CALL(WindowApiProxy, GetWidth(WindowApiId))
    .Times(1)
    .WillOnce(Return(Width));

  EXPECT_CALL(RocketCoreProxy, CreateContext(Eq("main"), ContextSize, nullptr))
    .Times(1)
    .WillOnce(Return(&Context));

  EXPECT_CALL(RocketControlsProxy, Initialise())
    .Times(1);

  if (::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION)
  {
    EXPECT_CALL(RocketDebuggerProxy, SetContext(_))
      .Times(1);
  }

  EXPECT_CALL(FontDatabaseProxy, LoadFontFace(_))
    .Times(AtLeast(1));

  Tested_t Example{ pWindowApi };
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor_DebuggerInitialise)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using RocketDebuggerProxy_t = ::mock::Rocket::Debugger::Proxy;
  RocketDebuggerProxy_t RocketDebuggerProxy;
  RocketDebuggerProxy_t::GetInstance() = &RocketDebuggerProxy;

  android_app App;

  const AppInfo_t Info{ &App };

  ::mock::Rocket::Core::Context Context;

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  using namespace ::testing;

  {
    InSequence Dummy;

    EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
      .Times(1)
      .WillOnce(Return(&Context));

    if (::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION)
    {
      EXPECT_CALL(RocketDebuggerProxy, SetContext(&Context))
        .Times(1)
        .WillOnce(Return(false));

      EXPECT_CALL(RocketDebuggerProxy, Initialise(&Context))
        .Times(1);
    }
    else
    {
      EXPECT_CALL(RocketDebuggerProxy, SetContext(_))
        .Times(0);

      EXPECT_CALL(RocketDebuggerProxy, Initialise(_))
        .Times(0);
    }

    Tested_t Example{ pWindowApi };
  }

  {
    InSequence Dummy;

    EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
      .Times(1)
      .WillOnce(Return(&Context));

    if (::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION)
    {
      EXPECT_CALL(RocketDebuggerProxy, SetContext(&Context))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(RocketDebuggerProxy, Initialise(_))
        .Times(0);
    }
    else
    {
      EXPECT_CALL(RocketDebuggerProxy, SetContext(_))
        .Times(0);

      EXPECT_CALL(RocketDebuggerProxy, Initialise(_))
        .Times(0);
    }

    Tested_t Example{ pWindowApi };
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor_LoadFontFace)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using SectionImplProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SectionImplProxy_t SectionImplProxy;
  SectionImplProxy_t::GetInstance() = &SectionImplProxy;

  using CurrentModuleProxy_t = 
    ::mock::alicorn::system::application::CurrentModule::Proxy;
  CurrentModuleProxy_t CurrentModuleProxy;
  CurrentModuleProxy_t::GetInstance() = &CurrentModuleProxy;

  using FontDatabaseProxy_t = ::mock::Rocket::Core::FontDatabase::Proxy;
  FontDatabaseProxy_t FontDatabaseProxy;
  FontDatabaseProxy_t::GetInstance() = &FontDatabaseProxy;

  ::mock::Rocket::Core::Context Context;

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  const auto AppRootPath = THIS_DIRECTORY;

  using namespace ::testing;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  InSequence Dummy;

  using namespace ::alicorn::extension::std;

  EXPECT_CALL(SectionImplProxy, GetValue(_, uT("PathToFontsDirectory")))
    .Times(1)
    .WillOnce(Return(string_cast<String>(m_PathToFontsDirectory)));

  for (const auto FontFile : { "font1.ttf", "font2.ttf", "font3.ttf" })
  {
    const auto PathToFont = (m_PathToFontsDirectory / FontFile).string();

    EXPECT_CALL(FontDatabaseProxy, LoadFontFace(Eq(PathToFont.c_str())))
      .Times(1);
  }

  Tested_t Example{ pWindowApi };
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Destructor)
{
  using EventHandlerProxy_t = ::mock::covellite::core::EventHandler::Proxy;
  EventHandlerProxy_t EventHandlerProxy;
  EventHandlerProxy_t::GetInstance() = &EventHandlerProxy;

  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  const ::mock::Id_t EventHandlerId = 1710121146;
  ::mock::Rocket::Core::Context Context;

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  IWindowApiPtr_t pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  using namespace ::testing;

  EXPECT_CALL(EventHandlerProxy, DummyConstructor())
    .Times(1)
    .WillOnce(Return(EventHandlerId));

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  {
    Tested_t Example{ pWindowApi };

    InSequence Dummy;

    EXPECT_CALL(EventHandlerProxy, Unsubscribe(EventHandlerId, &Context))
      .Times(1);

    EXPECT_CALL(Context, RemoveReference())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Subscribe)
{
  using EventHandlerProxy_t = ::mock::covellite::core::EventHandler::Proxy;
  EventHandlerProxy_t EventHandlerProxy;
  EventHandlerProxy_t::GetInstance() = &EventHandlerProxy;

  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  const ::mock::Id_t EventHandlerId = 1710121150;
  ::mock::Rocket::Core::Context Context;

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  IWindowApiPtr_t pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  using namespace ::testing;

  EXPECT_CALL(EventHandlerProxy, DummyConstructor())
    .Times(1)
    .WillOnce(Return(0));

  EXPECT_CALL(EventHandlerProxy, Constructor())
    .Times(1)
    .WillOnce(Return(EventHandlerId));

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  {
    Tested_t Example{ pWindowApi };

    auto pEvents = ::std::make_shared<::mock::covellite::core::EventHandler>();

    InSequence Dummy;

    EXPECT_CALL(EventHandlerProxy, Subscribe(EventHandlerId, &Context))
      .Times(1);

    Example.Subscribe(pEvents);

    EXPECT_CALL(EventHandlerProxy, Unsubscribe(EventHandlerId, &Context))
      .Times(1);

    EXPECT_CALL(Context, RemoveReference())
      .Times(1);
  }
}

// ************************************************************************** //
// cppcheck-suppress syntaxError
TEST_F(Window_test, /*DISABLED_*/Test_Set)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using StringTranslatorProxy_t =
    ::mock::covellite::rocket::StringTranslator::Proxy;
  StringTranslatorProxy_t StringTranslatorProxy;
  StringTranslatorProxy_t::GetInstance() = &StringTranslatorProxy;

  using Bank_t = ::std::map<::std::string, ::std::string>;

  const Bank_t Bank =
  {
    { "1", "2" },
    { "11", "22" },
    { "111", "222" },
  };

  ::mock::Rocket::Core::Context Context;
  const ::mock::Id_t StringTranslatorId = 1710081426;

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  IWindowApiPtr_t pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  using namespace ::testing;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  EXPECT_CALL(StringTranslatorProxy, Constructor())
    .Times(1)
    .WillOnce(Return(StringTranslatorId));

  Tested_t Example{ pWindowApi };

  EXPECT_CALL(StringTranslatorProxy, Set(StringTranslatorId, Bank))
    .Times(1);

  Example.Set(Bank);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_AddLayer)
{
  using WindowCoreProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowCoreProxy_t WindowCoreProxy;
  WindowCoreProxy_t::GetInstance() = &WindowCoreProxy;

  using EventHandlerProxy_t = ::mock::covellite::core::EventHandler::Proxy;
  EventHandlerProxy_t EventHandlerProxy;
  EventHandlerProxy_t::GetInstance() = &EventHandlerProxy;

  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using LayersProxy_t = ::mock::covellite::rocket::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  using LayerProxy_t = ::mock::covellite::rocket::Layer::Proxy;
  LayerProxy_t LayerProxy;
  LayerProxy_t::GetInstance() = &LayerProxy;

  const ::mock::Id_t EventHandlerId = 1710121212;
  const ::mock::Id_t WindowOsId = 1612211010;
  ::mock::Rocket::Core::ElementDocument Document;
  const ::mock::Id_t LayersId = 1710021315;
  const ::mock::Id_t LayerId = 1612211020;

  class Layer :
    public ::mock::covellite::rocket::Layer
  {
  public:
    static const char * GetPathToFile(void)
    {
      static const char * PathToFile = "Path1612211000";
      return PathToFile;
    }

  public:
    explicit Layer(Context_t * _pContext) :
      ::mock::covellite::rocket::Layer(_pContext, GetPathToFile())
    {
    }
  };

  android_app App;

  const AppInfo_t Info{ &App };

  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(WindowCoreProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowOsId));

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  EXPECT_CALL(LayersProxy, Constructor())
    .Times(1)
    .WillOnce(Return(LayersId));

  EXPECT_CALL(EventHandlerProxy, DummyConstructor())
    .Times(1)
    .WillOnce(Return(EventHandlerId));

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  EXPECT_CALL(LayerProxy, Constructor(&Context, Eq(Layer::GetPathToFile())))
    .Times(1)
    .WillOnce(Return(LayerId));

  EXPECT_CALL(LayersProxy, Push(LayersId, LayerId))
    .Times(1);

  EXPECT_CALL(LayerProxy, Subscribe(LayerId, EventHandlerId))
    .Times(1);

  auto pResult = Example.AddLayer<Layer>();
  EXPECT_EQ(LayerId, pResult->m_Id);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Back_ExistsLayer)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using LayersProxy_t = ::mock::covellite::rocket::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  using WindowCoreProxy_t = ::mock::covellite::core::Window::Proxy;
  WindowCoreProxy_t WindowCoreProxy;
  WindowCoreProxy_t::GetInstance() = &WindowCoreProxy;

  ::android_app App;

  const AppInfo_t Info{ &App };

  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  InSequence Dummy;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  EXPECT_CALL(LayersProxy, Pop(_))
    .Times(1)
    .WillOnce(Return(true));

  EXPECT_CALL(WindowCoreProxy, Exit(_))
    .Times(0);

  Example.Back();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Back_NotExistsLayer)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using LayersProxy_t = ::mock::covellite::rocket::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  using WindowCoreProxy_t = ::mock::covellite::core::Window::Proxy;
  WindowCoreProxy_t WindowCoreProxy;
  WindowCoreProxy_t::GetInstance() = &WindowCoreProxy;

  const ::mock::Id_t WindowOsId = 1710021307;

  ::android_app App;

  const AppInfo_t Info{ &App };

  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  EXPECT_CALL(WindowCoreProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowOsId));

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  InSequence Dummy;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  EXPECT_CALL(LayersProxy, Pop(_))
    .Times(1)
    .WillOnce(Return(false));

  EXPECT_CALL(WindowCoreProxy, Exit(WindowOsId))
    .Times(1);

  Example.Back();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnDrawWindow)
{
  class Window :
    public ::covellite::core::IWindow
  {
  public:
    class Proxy :
      public ::alicorn::extension::testing::Proxy<Proxy>
    {
    public:
      MOCK_METHOD0(DoDrawing, void(void));
    };

  public:
    void Subscribe(const EventHandlerPtr_t & _pEvents) override
    {
      using namespace ::covellite::core;

      (*_pEvents)[Event::Drawing]
        .connect([](const Params &) { Proxy::GetInstance()->DoDrawing(); });
    }
  };

  using WindowProxy_t = Window::Proxy;
  WindowProxy_t WindowProxy;
  WindowProxy_t::GetInstance() = &WindowProxy;

  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  android_app App;

  const AppInfo_t Info{ &App };

  ::mock::Rocket::Core::Context Context;
  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  Window FirstSubscriber;
  FirstSubscriber.Subscribe(pEventHandler);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  ITested_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  EXPECT_CALL(WindowProxy, DoDrawing())
    .Times(1);

  EXPECT_CALL(Context, Update())
    .Times(1);

  EXPECT_CALL(Context, Render())
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::Drawing, params::Empty{});
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnResize)
{
  using WindowApiProxy_t = ::mock::covellite::api::Window<int>::Proxy;
  WindowApiProxy_t WindowApiProxy;
  WindowApiProxy_t::GetInstance() = &WindowApiProxy;

  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using WindowCoreProxy_t = ::mock::covellite::core::Window::Proxy;
  WindowCoreProxy_t WindowCoreProxy;
  WindowCoreProxy_t::GetInstance() = &WindowCoreProxy;

  const ::mock::Id_t WindowApiId = 1612211034;
  const int Width = 1612211039;
  const int Height = 1612211040;
  const int StatusBarHeight = 1612211053;
  const Vector_t ContextSize(Width, Height - StatusBarHeight);

  android_app App;

  const AppInfo_t Info{ &App };

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();
  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  EXPECT_CALL(WindowApiProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(WindowApiId));

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  EXPECT_CALL(WindowCoreProxy, GetStatusBarHeight(_))
    .Times(1)
    .WillOnce(Return(StatusBarHeight));

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  ITested_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  EXPECT_CALL(WindowApiProxy, GetWidth(WindowApiId))
    .WillRepeatedly(Return(Width));

  EXPECT_CALL(WindowApiProxy, GetHeight(WindowApiId))
    .WillRepeatedly(Return(Height));

  EXPECT_CALL(Context, SetDimensions(ContextSize))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::Resize, params::Empty{});
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnMotion)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using WindowCoreProxy_t = ::mock::covellite::core::Window::Proxy;
  WindowCoreProxy_t WindowCoreProxy;
  WindowCoreProxy_t::GetInstance() = &WindowCoreProxy;

  const int StatusBarHeight = 1612211053;
  const int X = 1711012105;
  const int Y = 1711012106;

  android_app App;

  const AppInfo_t Info{ &App };

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();
  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  EXPECT_CALL(WindowCoreProxy, GetStatusBarHeight(_))
    .Times(1)
    .WillOnce(Return(StatusBarHeight));

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  ITested_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  EXPECT_CALL(Context, ProcessMouseMove(X, Y - StatusBarHeight, 0))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::Motion, params::Motion{ X, Y });
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnTouch)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  android_app App;

  const AppInfo_t Info{ &App };

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();
  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  ITested_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  EXPECT_CALL(Context, ProcessMouseButtonDown(0, 0))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::Touch, params::Empty{});
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnRelease)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  android_app App;

  const AppInfo_t Info{ &App };

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();
  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  ITested_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  EXPECT_CALL(Context, ProcessMouseButtonUp(0, 0))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::Release, params::Empty{});
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnBack_ExistLayers)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using LayersProxy_t = ::mock::covellite::rocket::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  using WindowCoreProxy_t = ::mock::covellite::core::Window::Proxy;
  WindowCoreProxy_t WindowCoreProxy;
  WindowCoreProxy_t::GetInstance() = &WindowCoreProxy;

  ::android_app App;

  const AppInfo_t Info{ &App };

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();
  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  EXPECT_CALL(WindowCoreProxy, Constructor())
    .Times(1)
    .WillOnce(Return(1710021307));

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  InSequence Dummy;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  ITested_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  using namespace ::covellite::core;

  EXPECT_CALL(LayersProxy, Pop(_))
    .Times(1)
    .WillOnce(Return(true));

  EXPECT_CALL(WindowCoreProxy, Exit(_))
    .Times(0);

  App.destroyRequested = 0;

  pEventHandler->DoCommand(Event::Back, params::Empty{});
  EXPECT_EQ(0, App.destroyRequested);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnBack_EmptyLayers)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using LayersProxy_t = ::mock::covellite::rocket::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  using WindowCoreProxy_t = ::mock::covellite::core::Window::Proxy;
  WindowCoreProxy_t WindowCoreProxy;
  WindowCoreProxy_t::GetInstance() = &WindowCoreProxy;

  const ::mock::Id_t WindowOsId = 1710021308;

  ::android_app App;

  const AppInfo_t Info{ &App };

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();
  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  EXPECT_CALL(WindowCoreProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowOsId));

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  InSequence Dummy;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  ITested_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  using namespace ::covellite::core;

  EXPECT_CALL(LayersProxy, Pop(_))
    .Times(1)
    .WillOnce(Return(false));

  EXPECT_CALL(WindowCoreProxy, Exit(WindowOsId))
    .Times(1);

  pEventHandler->DoCommand(Event::Back, params::Empty{});
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnKeyDown)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  using ParamsProxy_t = ::mock::covellite::core::ParamsProxy;
  ParamsProxy_t ParamsProxy;
  ParamsProxy_t::GetInstance() = &ParamsProxy;

  const auto KeyCode = 1711012116;
  const auto RocketKeyCode = 
    (::mock::Rocket::Core::Input::KeyIdentifier)1711012117;

  android_app App;

  const AppInfo_t Info{ &App };

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();
  ::mock::Rocket::Core::Context Context;

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  ITested_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  using namespace ::covellite::core;
  
  EXPECT_CALL(ParamsProxy, ToRocketKeyCode(KeyCode))
    .Times(1)
    .WillOnce(Return(RocketKeyCode));

  EXPECT_CALL(Context, ProcessKeyDown(RocketKeyCode, 0))
    .Times(1);

  pEventHandler->DoCommand(Event::KeyDown, params::KeyCode{ KeyCode });
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnKeyPressed)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  auto KeyCode = 1711012121;

  android_app App;

  const AppInfo_t Info{ &App };

  using namespace ::testing;

  ::mock::Rocket::Core::Context Context;
  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

  InSequence Dummy;

  auto pWindowOs = ::std::make_shared<WindowOs_t>();
  auto pWindowApi = ::std::make_shared<WindowApi_t>(pWindowOs);

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ pWindowApi };

  ITested_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  EXPECT_CALL(Context, ProcessTextInput(
    static_cast<::mock::Rocket::Core::word>(KeyCode)))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::KeyPressed, params::KeyPressed{ KeyCode });
}
