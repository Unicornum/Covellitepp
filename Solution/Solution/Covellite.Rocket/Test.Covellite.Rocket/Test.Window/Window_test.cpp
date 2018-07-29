
#include "stdafx.h"
#include <alicorn\boost\string-cast.hpp>

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Window.
*/

// ������������ ������ Window
#include <Covellite/Rocket/Window.hpp>

#include <Covellite/Rocket/ClickEventListener.hpp>
#include "../Mock/ClickEventListener.inl"
#include "../../Covellite.Rocket/Window.cpp"

#include <Covellite/Os/Events.hpp>
#include <Covellite/Api/Events.hpp>

// ����� �������� ����� ������ Window
class Window_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::rocket::Window;
  using ITested_t = ::covellite::rocket::IWindow;
  using WindowOs_t = ::mock::covellite::os::Window;
  using IWindowApi_t = ::covellite::api::IWindow;
  using WindowApi_t = ::mock::covellite::api::Window<int>;
  using RenderInterfacePtr_t = 
    ::std::shared_ptr<::mock::Rocket::Core::RenderInterface>;
  using Vector_t = ::mock::Rocket::Core::Vector2i;
  using String_t = ::mock::Rocket::Core::String;
  using Path_t = ::boost::filesystem::path;
  using Events_t = ::covellite::events::Events;
  using Context_t = ::mock::Rocket::Core::Context;
  using Document_t = ::mock::Rocket::Core::ElementDocument;

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
    using namespace ::alicorn::extension::std;

    ::testing::DefaultValue<Tested_t::ClickEventListenerPtr_t>::Set(m_pClickEventListener);
    ::testing::DefaultValue<RenderInterfacePtr_t>::Set(m_pRenderInterface);
    ::testing::DefaultValue<String_t>::Set("DefaultString");
    ::testing::DefaultValue<String>::Set(string_cast<String>(m_PathToFontsDirectory));
    ::testing::DefaultValue<IWindowApi_t::Rect>::Set({ 0, 0, 1, 1});
    ::testing::DefaultValue<Context_t *>::Set(&m_Context);
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
    using namespace ::alicorn::extension::std;

    ::testing::DefaultValue<Tested_t::ClickEventListenerPtr_t>::Clear();
    ::testing::DefaultValue<RenderInterfacePtr_t>::Clear();
    ::testing::DefaultValue<String_t>::Clear();
    ::testing::DefaultValue<String>::Clear();
    ::testing::DefaultValue<IWindowApi_t::Rect>::Clear();
    ::testing::DefaultValue<Context_t *>::Clear();
  }

protected:
  const Path_t m_PathToFontsDirectory = THIS_DIRECTORY / L"fonts";

private:
  const Events_t m_Events;
  Tested_t::ClickEventListenerPtr_t m_pClickEventListener =
    ::std::make_shared<Tested_t::ClickEventListener>(m_Events);
  RenderInterfacePtr_t m_pRenderInterface =
    ::std::make_shared<::mock::covellite::api::RenderOpenGL>(0);
  Context_t m_Context;
};

// ������� ������� ��� ����������� � ����� Window 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Window_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor_CreateContext_Fail)
{
  using RocketProxy_t = ::mock::Rocket::Core::Proxy;
  RocketProxy_t RocketProxy;
  RocketProxy_t::GetInstance() = &RocketProxy;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  EXPECT_CALL(RocketProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_STDEXCEPTION(Tested_t{ IWindowApi }, ".+Create context failed.");
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor)
{
  using ClickEventListenerProxy_t = ::covellite::rocket::mock::ClickEventListenerProxy;
  ClickEventListenerProxy_t ClickEventListenerProxy;
  ClickEventListenerProxy_t::GetInstance() = &ClickEventListenerProxy;

  using StringTranslatorProxy_t = ::mock::covellite::rocket::StringTranslator::Proxy;
  StringTranslatorProxy_t StringTranslatorProxy;
  StringTranslatorProxy_t::GetInstance() = &StringTranslatorProxy;

  using WindowApiProxy_t = ::mock::covellite::api::Window<int>::Proxy;
  WindowApiProxy_t WindowApiProxy;
  WindowApiProxy_t::GetInstance() = &WindowApiProxy;

  using InterfacesProxy_t = ::mock::covellite::InterfacesProxy;
  InterfacesProxy_t InterfacesProxy;
  InterfacesProxy_t::GetInstance() = &InterfacesProxy;

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
  const int StatusBarHeight = 1612202238;
  const int Width = 1612202303;
  const int Height = 1612202304;
  const Vector_t ContextSize(Width, Height - StatusBarHeight);

  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  EXPECT_CALL(StringTranslatorProxy, Constructor())
    .Times(2)
    .WillRepeatedly(Return(StringTranslatorId));

  InSequence Dummy;

  EXPECT_CALL(WindowApiProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(WindowApiId));

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(InterfacesProxy, RenderConstructor(_))
    .Times(1)
    .WillOnce(Return(IRenderId));

  RenderInterfacePtr_t pRenderInterface =
    ::std::make_shared<::mock::covellite::api::RenderOpenGL>(0);

  auto pClickEventListener =
    ::std::make_shared<Tested_t::ClickEventListener>(IWindowApi);

  EXPECT_CALL(ClickEventListenerProxy, Make(_))
    .Times(1)
    .WillOnce(Return(pClickEventListener));

  auto pStringTranslator =
    ::std::make_shared<::mock::covellite::rocket::StringTranslator>();

  ::mock::covellite::rocket::Initializer::Data Data =
  {
    pRenderInterface,
    pStringTranslator
  };

  EXPECT_CALL(WindowApiProxy, MakeRenderInterface(WindowApiId))
    .Times(1)
    .WillOnce(Return(pRenderInterface));

  EXPECT_CALL(InitializerProxy, Constructor(Data))
    .Times(1);

  EXPECT_CALL(WindowApiProxy, GetClientRect(WindowApiId))
    .Times(1)
    .WillOnce(Return(IWindowApi_t::Rect{ 0, StatusBarHeight, Width, Height }));

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

  EXPECT_CALL(Context, 
    AddEventListener(Eq("click"), pClickEventListener.get(), false))
    .Times(1);

  EXPECT_CALL(FontDatabaseProxy, LoadFontFace(_))
    .Times(AtLeast(1));

  const Tested_t Example{ IWindowApi };

  EXPECT_CALL(Context,
    RemoveEventListener(Eq("click"), pClickEventListener.get(), false))
    .Times(1);
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

  ::mock::Rocket::Core::Context Context;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

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

    Tested_t Example{ IWindowApi };
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

    Tested_t Example{ IWindowApi };
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor_LoadFontFace)
{
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

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  const auto AppRootPath = THIS_DIRECTORY;

  using namespace ::testing;

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

  Tested_t Example{ IWindowApi };
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_DefaultDestructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
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

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  EXPECT_CALL(EventHandlerProxy, DummyConstructor())
    .Times(1)
    .WillOnce(Return(EventHandlerId));

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  {
    Tested_t Example{ IWindowApi };

    InSequence Dummy;

    EXPECT_CALL(EventHandlerProxy, Unsubscribe(EventHandlerId, &Context))
      .Times(1);

    EXPECT_CALL(Context, RemoveReference())
      .Times(1);
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetEvents)
{
  class Proxy
  {
  public:
    MOCK_METHOD0(DoEvent, void(void));
  };

  Proxy oProxy;
  static const int Id = 1807231902;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  Events_t SourceEvents = IWindowApi;
  SourceEvents[Id].Connect([&]() { oProxy.DoEvent(); });
  SourceEvents[::covellite::events::Error.Exception].Connect([]() {});

  const Tested_t Example{ IWindowApi };

  using namespace ::testing;

  EXPECT_CALL(oProxy, DoEvent())
    .Times(1);

  Events_t ExampleEvents = Example;
  ExampleEvents[Id]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_LoadDocument)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  Context_t Context;
  const char * PathToFile = "Path1807231921";
  Document_t Document;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };
  ITested_t & IExample = Example;

  EXPECT_CALL(Context, LoadDocument(Eq(PathToFile)))
    .WillOnce(Return(nullptr))
    .WillOnce(Return(&Document));

  auto pDocument = IExample.LoadDocument(PathToFile);
  EXPECT_EQ(nullptr, pDocument);

  pDocument = IExample.LoadDocument(PathToFile);
  EXPECT_EQ(&Document, pDocument);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Set)
{
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

  const ::mock::Id_t StringTranslatorId = 1710081426;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  EXPECT_CALL(StringTranslatorProxy, Constructor())
    .Times(1)
    .WillOnce(Return(StringTranslatorId));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(StringTranslatorProxy, Set(StringTranslatorId, Bank))
    .Times(1);

  Example.Set(Bank);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_PushLayer)
{
  using LayersProxy_t = ::mock::covellite::rocket::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  using LayerProxy_t = ::mock::covellite::rocket::Layer::Proxy;
  LayerProxy_t LayerProxy;
  LayerProxy_t::GetInstance() = &LayerProxy;

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
    explicit Layer(ITested_t & _Window) :
      ::mock::covellite::rocket::Layer(&_Window, GetPathToFile())
    {
    }
  };

  using namespace ::testing;

  InSequence Dummy;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(LayersProxy, Constructor())
    .Times(1)
    .WillOnce(Return(LayersId));

  Tested_t Example{ IWindowApi };
  ITested_t & IExample = Example;

  EXPECT_CALL(LayerProxy, Constructor(&IExample, Eq(Layer::GetPathToFile())))
    .Times(1)
    .WillOnce(Return(LayerId));

  EXPECT_CALL(LayersProxy, Push(LayersId, LayerId))
    .Times(1);

  Example.PushLayer<Layer>();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Back_ExistsLayer)
{
  using LayersProxy_t = ::mock::covellite::rocket::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  using WindowCoreProxy_t = ::mock::covellite::core::Window::Proxy;
  WindowCoreProxy_t WindowCoreProxy;
  WindowCoreProxy_t::GetInstance() = &WindowCoreProxy;

  using namespace ::testing;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  InSequence Dummy;

  Tested_t Example{ IWindowApi };

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
  using LayersProxy_t = ::mock::covellite::rocket::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  using WindowCoreProxy_t = ::mock::covellite::core::Window::Proxy;
  WindowCoreProxy_t WindowCoreProxy;
  WindowCoreProxy_t::GetInstance() = &WindowCoreProxy;

  const ::mock::Id_t WindowOsId = 1710021307;

  using namespace ::testing;

  EXPECT_CALL(WindowCoreProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowOsId));

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  Tested_t Example{ IWindowApi };

  InSequence Dummy;

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
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  ::mock::Rocket::Core::Context Context;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(Context, Update())
    .Times(1);

  EXPECT_CALL(Context, Render())
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[Drawing.Do]();
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

  const int Width = 1612211039;
  const int Height = 1612211040;
  const int StatusBarHeight = 1612211053;
  const Vector_t ContextSize(Width, Height - StatusBarHeight);

  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(WindowApiProxy, GetClientRect(_))
    .Times(1)
    .WillOnce(Return(IWindowApi_t::Rect{ 0, StatusBarHeight, Width, Height }));

  EXPECT_CALL(Context, SetDimensions(ContextSize))
    .Times(1);

  using namespace ::covellite;

  events::Events Events = IWindowApi;
  Events[events::Window.Resize]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnMotion)
{
  using WindowApiProxy_t = ::mock::covellite::api::Window<int>::Proxy;
  WindowApiProxy_t WindowApiProxy;
  WindowApiProxy_t::GetInstance() = &WindowApiProxy;

  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  const int StatusBarHeight = 1612211053;
  const int X = 1711012105;
  const int Y = 1711012106;

  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(WindowApiProxy, GetClientRect(_))
    .Times(1)
    .WillOnce(Return(IWindowApi_t::Rect{ 0, StatusBarHeight, 0, 0 }));

  EXPECT_CALL(Context, ProcessMouseMove(X, Y - StatusBarHeight, 0))
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[Cursor.Motion](Cursor_t::Position{ X, Y });
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnTouch)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(Context, ProcessMouseButtonDown(0, 0))
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[Cursor.Touch]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnRelease)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(Context, ProcessMouseButtonUp(0, 0))
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[Cursor.Release]();
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

  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  InSequence Dummy;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  using namespace ::covellite::core;

  EXPECT_CALL(LayersProxy, Pop(_))
    .Times(1)
    .WillOnce(Return(true));

  EXPECT_CALL(WindowCoreProxy, Exit(_))
    .Times(0);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[::covellite::events::Key.Back]();
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

  ::mock::Rocket::Core::Context Context;

  using namespace ::testing;

  EXPECT_CALL(WindowCoreProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowOsId));

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  InSequence Dummy;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  using namespace ::covellite::core;

  EXPECT_CALL(LayersProxy, Pop(_))
    .Times(1)
    .WillOnce(Return(false));

  EXPECT_CALL(WindowCoreProxy, Exit(WindowOsId))
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[::covellite::events::Key.Back]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnKeyDown)
{
  using SystemToRocketKeyCodeProxy_t = ::covellite::rocket::SystemToRocketKeyCodeProxy;
  SystemToRocketKeyCodeProxy_t SystemToRocketKeyCodeProxy;
  SystemToRocketKeyCodeProxy_t::GetInstance() = &SystemToRocketKeyCodeProxy;

  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  const auto KeyCode = 1711012116;
  const auto RocketKeyCode = 
    (::mock::Rocket::Core::Input::KeyIdentifier)1711012117;

  ::mock::Rocket::Core::Context Context;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  using namespace ::covellite::core;
  
  EXPECT_CALL(SystemToRocketKeyCodeProxy, Convert(KeyCode))
    .Times(1)
    .WillOnce(Return(RocketKeyCode));

  EXPECT_CALL(Context, ProcessKeyDown(RocketKeyCode, 0))
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[::covellite::events::Key.Down](KeyCode);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnKeyPressed)
{
  using RocketCoreProxy_t = ::mock::Rocket::Core::Proxy;
  RocketCoreProxy_t RocketCoreProxy;
  RocketCoreProxy_t::GetInstance() = &RocketCoreProxy;

  const auto KeyCode = 1711012121;

  using namespace ::testing;

  ::mock::Rocket::Core::Context Context;

  InSequence Dummy;

  const WindowOs_t WindowOs;
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(RocketCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(Context, ProcessTextInput(
    static_cast<::mock::Rocket::Core::word>(KeyCode)))
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[::covellite::events::Key.Pressed](KeyCode);
}
