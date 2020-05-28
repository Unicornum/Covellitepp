
#include "stdafx.h"
#include <alicorn/boost/string-cast.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса Window.
*/

// Расположение класса Window
#include <Covellite/Gui/Window.hpp>

#include <Covellite/Gui/EventListener.hpp>
#include "../Mock/EventListener.inl"
#include "../../Covellite.Gui/Window.cpp"

#include <Covellite/App.mock.hpp>
#include <Covellite/Os/Events.hpp>
#include <Covellite/Api/Events.hpp>
#include <Covellite/Covellite.Api.lib.hpp>

// Общий тестовый класс класса Window
class Window_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::Window;
  using ITested_t = ::covellite::gui::IWindow;
  using WindowOs_t = ::mock::covellite::os::Window;
  using IWindowApi_t = ::covellite::api::IWindow;
  using WindowApi_t = ::mock::covellite::api::Window;
  using RenderInterfacePtr_t = 
    ::std::shared_ptr<::mock::CovelliteGui::Core::RenderInterface>;
  using Vector_t = ::mock::CovelliteGui::Core::Vector2i;
  using String_t = ::mock::CovelliteGui::Core::String;
  using Path_t = ::boost::filesystem::path;
  using Events_t = ::covellite::events::Events;
  using Context_t = ::mock::CovelliteGui::Core::Context;
  using Document_t = ::mock::CovelliteGui::Core::ElementDocument;
  using Renders_t = ::covellite::api::Component::Renders;
  using RendersPtr_t = ::std::shared_ptr<Renders_t>;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    using namespace ::alicorn::extension::std;

    ::testing::DefaultValue<Tested_t::EventListenerPtr_t>::Set(m_pEventListener);
    ::testing::DefaultValue<RenderInterfacePtr_t>::Set(m_pRenderInterface);
    ::testing::DefaultValue<String_t>::Set("DefaultString");
    ::testing::DefaultValue<String>::Set(string_cast<String>(m_PathToFontsDirectory));
    ::testing::DefaultValue<WindowOs_t::Rect>::Set({ 0, 0, 1, 1});
    ::testing::DefaultValue<Context_t *>::Set(&m_Context);
    ::testing::DefaultValue<RendersPtr_t>::Set(m_pRenders);
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    using namespace ::alicorn::extension::std;

    ::testing::DefaultValue<Tested_t::EventListenerPtr_t>::Clear();
    ::testing::DefaultValue<RenderInterfacePtr_t>::Clear();
    ::testing::DefaultValue<String_t>::Clear();
    ::testing::DefaultValue<String>::Clear();
    ::testing::DefaultValue<WindowOs_t::Rect>::Clear();
    ::testing::DefaultValue<Context_t *>::Clear();
    ::testing::DefaultValue<RendersPtr_t>::Clear();
  }

protected:
  const Path_t m_PathToFontsDirectory = THIS_DIRECTORY / L"fonts";

private:
  const Events_t m_Events;
  Tested_t::EventListenerPtr_t m_pEventListener =
    ::std::make_shared<Tested_t::EventListener>(m_Events);
  RenderInterfacePtr_t m_pRenderInterface =
    ::std::make_shared<::mock::covellite::api::RenderOpenGL>(0);
  Context_t m_Context;
  RendersPtr_t m_pRenders = 
    ::std::make_shared<Renders_t>(Renders_t::Creators_t{});

protected:
  class SupportWindow
  {
  public:
    MOCK_METHOD0(OnExit, void(void));

  private:
    Events_t m_Events;

  public:
    explicit SupportWindow(const IWindowApi_t & _WindowApi) :
      m_Events(_WindowApi)
    {
      m_Events[::covellite::events::Application.Exit].Connect([&](void) 
      { 
        OnExit(); 
      });
    }
  };

protected:
  using App_t = ::mock::covellite::app::Application;
  App_t m_App{ App_t::EventBased{} };
};

// Образец макроса для подстановки в класс Window 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Window_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_AppIWindow)
{
  EXPECT_TRUE((::std::is_convertible<Tested_t &,
    ::covellite::app::IWindow &>::value));
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor_CreateContext_Fail)
{
  using CovelliteGuiProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiProxy_t CovelliteGuiProxy;
  CovelliteGuiProxy_t::GetInstance() = &CovelliteGuiProxy;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  EXPECT_CALL(CovelliteGuiProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(nullptr));

  EXPECT_STDEXCEPTION(Tested_t{ IWindowApi }, ".+Create context failed.");
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor)
{
  using RendererProxy_t = ::mock::covellite::gui::Renderer::Proxy;
  RendererProxy_t RendererProxy;
  RendererProxy_t::GetInstance() = &RendererProxy;

  using EventListenerProxy_t = ::covellite::gui::mock::EventListenerProxy;
  EventListenerProxy_t EventListenerProxy;
  EventListenerProxy_t::GetInstance() = &EventListenerProxy;

  using StringTranslatorProxy_t = ::mock::covellite::gui::StringTranslator::Proxy;
  StringTranslatorProxy_t StringTranslatorProxy;
  StringTranslatorProxy_t::GetInstance() = &StringTranslatorProxy;

  using WindowApiProxy_t = ::mock::covellite::api::Window::Proxy;
  WindowApiProxy_t WindowApiProxy;
  WindowApiProxy_t::GetInstance() = &WindowApiProxy;

  using InitializerProxy_t = ::mock::covellite::gui::Initializer::Proxy;
  InitializerProxy_t InitializerProxy;
  InitializerProxy_t::GetInstance() = &InitializerProxy;

  using CovelliteGuiCoreProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiCoreProxy_t CovelliteGuiCoreProxy;
  CovelliteGuiCoreProxy_t::GetInstance() = &CovelliteGuiCoreProxy;

  using CovelliteGuiControlsProxy_t = ::mock::CovelliteGui::Controls::Proxy;
  CovelliteGuiControlsProxy_t CovelliteGuiControlsProxy;
  CovelliteGuiControlsProxy_t::GetInstance() = &CovelliteGuiControlsProxy;

  using CovelliteGuiDebuggerProxy_t = ::mock::CovelliteGui::Debugger::Proxy;
  CovelliteGuiDebuggerProxy_t CovelliteGuiDebuggerProxy;
  CovelliteGuiDebuggerProxy_t::GetInstance() = &CovelliteGuiDebuggerProxy;

  using FontDatabaseProxy_t = ::mock::CovelliteGui::Core::FontDatabase::Proxy;
  FontDatabaseProxy_t FontDatabaseProxy;
  FontDatabaseProxy_t::GetInstance() = &FontDatabaseProxy;

  const ::mock::Id_t RendererId = 1811191437;
  const auto pRenders = ::std::make_shared<Renders_t>(Renders_t::Creators_t{});
  const ::mock::Id_t StringTranslatorId = 1612212313;
  const ::mock::Id_t WindowApiId = 1612202302;
  const int StatusBarHeight = 1612202238;
  const int Width = 1612202303;
  const int Height = 1612202304;
  const Vector_t ContextSize(Width, Height - StatusBarHeight);

  ::mock::CovelliteGui::Core::Context Context;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererProxy, Constructor(pRenders))
    .Times(1)
    .WillOnce(Return(RendererId));

  RenderInterfacePtr_t pRenderInterface =
    ::std::make_shared<::mock::covellite::gui::Renderer>(pRenders);

  EXPECT_CALL(StringTranslatorProxy, Constructor())
    .Times(1)
    .WillRepeatedly(Return(StringTranslatorId));

  auto pStringTranslator =
    ::std::make_shared<::mock::covellite::gui::StringTranslator>();

  ::mock::covellite::gui::Initializer::Data Data =
  {
    pRenderInterface,
    pStringTranslator
  };

  EXPECT_CALL(WindowApiProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(WindowApiId));

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(WindowApiProxy, GetRenders(WindowApiId))
    .Times(1)
    .WillOnce(Return(pRenders));

  EXPECT_CALL(RendererProxy, Constructor(pRenders))
    .Times(1)
    .WillOnce(Return(RendererId));

  auto pEventListener =
    ::std::make_shared<Tested_t::EventListener>(IWindowApi);

  EXPECT_CALL(EventListenerProxy, Make(_))
    .Times(1)
    .WillOnce(Return(pEventListener));

  EXPECT_CALL(StringTranslatorProxy, Constructor())
    .Times(1)
    .WillRepeatedly(Return(StringTranslatorId));

  EXPECT_CALL(InitializerProxy, Constructor(Data))
    .Times(1);

  EXPECT_CALL(WindowApiProxy, GetClientRect(WindowApiId))
    .Times(1)
    .WillOnce(Return(WindowOs_t::Rect{ 0, StatusBarHeight, Width, Height }));

  EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(Eq("main"), ContextSize, nullptr))
    .Times(1)
    .WillOnce(Return(&Context));

  EXPECT_CALL(CovelliteGuiControlsProxy, Initialise())
    .Times(1);

  if (::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION)
  {
    EXPECT_CALL(CovelliteGuiDebuggerProxy, SetContext(_))
      .Times(1);
  }

  EXPECT_CALL(Context, 
    AddEventListener(Eq("click"), pEventListener.get(), false))
    .Times(1);

  EXPECT_CALL(Context,
    AddEventListener(Eq("mousedown"), pEventListener.get(), false))
    .Times(1);

  EXPECT_CALL(Context,
    AddEventListener(Eq("change"), pEventListener.get(), false))
    .Times(1);

  EXPECT_CALL(FontDatabaseProxy, LoadFontFace(_))
    .Times(AtLeast(1));

  const Tested_t Example{ IWindowApi };

  EXPECT_CALL(Context,
    RemoveEventListener(Eq("click"), pEventListener.get(), false))
    .Times(1);

  EXPECT_CALL(Context,
    RemoveEventListener(Eq("mousedown"), pEventListener.get(), false))
    .Times(1);

  EXPECT_CALL(Context,
    RemoveEventListener(Eq("change"), pEventListener.get(), false))
    .Times(1);

# ifdef COVELLITE_GUI_ROCKET

  EXPECT_CALL(Context, RemoveReference())
    .Times(1);

# elif defined COVELLITE_GUI_RMLUI

  const CovelliteGui::Core::String Name = "Name2005271214";

  EXPECT_CALL(Context, GetName())
    .Times(1)
    .WillOnce(Return(Name));

  EXPECT_CALL(CovelliteGuiCoreProxy, RemoveContext(Name))
    .Times(1);

# else
#   error "Need COVELLITE_GUI_ROCKET or COVELLITE_GUI_RMLUI."
# endif

}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor_DebuggerInitialise)
{
  using CovelliteGuiCoreProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiCoreProxy_t CovelliteGuiCoreProxy;
  CovelliteGuiCoreProxy_t::GetInstance() = &CovelliteGuiCoreProxy;

  using CovelliteGuiDebuggerProxy_t = ::mock::CovelliteGui::Debugger::Proxy;
  CovelliteGuiDebuggerProxy_t CovelliteGuiDebuggerProxy;
  CovelliteGuiDebuggerProxy_t::GetInstance() = &CovelliteGuiDebuggerProxy;

  ::mock::CovelliteGui::Core::Context Context;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  {
    InSequence Dummy;

    EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(_, _, _))
      .Times(1)
      .WillOnce(Return(&Context));

    if (::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION)
    {
      EXPECT_CALL(CovelliteGuiDebuggerProxy, SetContext(&Context))
        .Times(1)
        .WillOnce(Return(false));

      EXPECT_CALL(CovelliteGuiDebuggerProxy, Initialise(&Context))
        .Times(1);
    }
    else
    {
      EXPECT_CALL(CovelliteGuiDebuggerProxy, SetContext(_))
        .Times(0);

      EXPECT_CALL(CovelliteGuiDebuggerProxy, Initialise(_))
        .Times(0);
    }

    Tested_t Example{ IWindowApi };
  }

  {
    InSequence Dummy;

    EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(_, _, _))
      .Times(1)
      .WillOnce(Return(&Context));

    if (::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION)
    {
      EXPECT_CALL(CovelliteGuiDebuggerProxy, SetContext(&Context))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(CovelliteGuiDebuggerProxy, Initialise(_))
        .Times(0);
    }
    else
    {
      EXPECT_CALL(CovelliteGuiDebuggerProxy, SetContext(_))
        .Times(0);

      EXPECT_CALL(CovelliteGuiDebuggerProxy, Initialise(_))
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

  using FontDatabaseProxy_t = ::mock::CovelliteGui::Core::FontDatabase::Proxy;
  FontDatabaseProxy_t FontDatabaseProxy;
  FontDatabaseProxy_t::GetInstance() = &FontDatabaseProxy;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

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
TEST_F(Window_test, /*DISABLED_*/Test_GetEvents)
{
  class Proxy
  {
  public:
    MOCK_METHOD0(DoEvent, void(void));
  };

  Proxy oProxy;
  static const int Id = 1807231902;

  const WindowOs_t WindowOs{ m_App };
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
  using CovelliteGuiCoreProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiCoreProxy_t CovelliteGuiCoreProxy;
  CovelliteGuiCoreProxy_t::GetInstance() = &CovelliteGuiCoreProxy;

  Context_t Context;
  const char * PathToFile = "Path1807231921";
  Document_t Document;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };
  ITested_t & IExample = Example;

  EXPECT_CALL(Context, LoadDocument(Eq(PathToFile)))
    .WillOnce(Return(nullptr))
    .WillOnce(Return(&Document));

  auto pDocument = IExample.LoadDocument(PathToFile);
  EXPECT_EQ(nullptr, pDocument.get());

  pDocument = IExample.LoadDocument(PathToFile);
  EXPECT_EQ(&Document, pDocument.get());

# ifdef COVELLITE_GUI_ROCKET

  EXPECT_CALL(Document, RemoveReference())
    .Times(1);

# elif defined COVELLITE_GUI_RMLUI

  EXPECT_CALL(Document, Close())
    .Times(1);

# else
#   error "Need COVELLITE_GUI_ROCKET or COVELLITE_GUI_RMLUI."
# endif
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Set)
{
  using StringTranslatorProxy_t =
    ::mock::covellite::gui::StringTranslator::Proxy;
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

  const WindowOs_t WindowOs{ m_App };
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
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  using LayersProxy_t = ::mock::covellite::gui::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  using LayerProxy_t = ::mock::covellite::gui::Layer::Proxy;
  LayerProxy_t LayerProxy;
  LayerProxy_t::GetInstance() = &LayerProxy;

  const ::mock::Id_t LayersId = 1710021315;
  const ::mock::Id_t LayerId = 1612211020;

  class Layer :
    public ::mock::covellite::gui::Layer
  {
  public:
    static const char * GetPathToFile(void)
    {
      static const char * PathToFile = "Path1612211000";
      return PathToFile;
    }

  public:
    explicit Layer(ITested_t & _Window) :
      ::mock::covellite::gui::Layer(_Window, GetPathToFile())
    {
    }
  };

  using namespace ::testing;

  InSequence Dummy;

  const WindowOs_t WindowOs{ m_App };
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

  EXPECT_CALL(LoggerProxy, ToLog(uT("Info: Push layer [Layer]")))
    .Times(1);

  Example.PushLayer<Layer>();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Back_ExistsLayer)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  using LayersProxy_t = ::mock::covellite::gui::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  SupportWindow Support{ IWindowApi };

  using namespace ::testing;

  InSequence Dummy;

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(LoggerProxy, ToLog(uT("Info: Pop layer")))
    .Times(1);

  EXPECT_CALL(LayersProxy, Pop(_))
    .Times(1)
    .WillOnce(Return(true));

  EXPECT_CALL(Support, OnExit())
    .Times(0);

  Example.Back();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Back_NotExistsLayer)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  using LayersProxy_t = ::mock::covellite::gui::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  SupportWindow Support{ IWindowApi };
  Tested_t Example{ IWindowApi };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(LoggerProxy, ToLog(uT("Info: Pop layer")))
    .Times(1);

  EXPECT_CALL(LayersProxy, Pop(_))
    .Times(1)
    .WillOnce(Return(false));

  EXPECT_CALL(Support, OnExit())
    .Times(1);

  Example.Back();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnDrawWindow)
{
  using RendererProxy_t = ::mock::covellite::gui::Renderer::Proxy;
  RendererProxy_t RendererProxy;
  RendererProxy_t::GetInstance() = &RendererProxy;

  using CovelliteGuiCoreProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiCoreProxy_t CovelliteGuiCoreProxy;
  CovelliteGuiCoreProxy_t::GetInstance() = &CovelliteGuiCoreProxy;

  ::mock::CovelliteGui::Core::Context Context;
  const ::mock::Id_t RendererId = 1811191434;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RendererId));

  EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(Context, Update())
    .Times(1);

  EXPECT_CALL(Context, Render())
    .Times(1);

  EXPECT_CALL(RendererProxy, RenderScene(RendererId))
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[Drawing.Do]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnResize)
{
  using WindowApiProxy_t = WindowApi_t::Proxy;
  WindowApiProxy_t WindowApiProxy;
  WindowApiProxy_t::GetInstance() = &WindowApiProxy;

  using CovelliteGuiCoreProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiCoreProxy_t CovelliteGuiCoreProxy;
  CovelliteGuiCoreProxy_t::GetInstance() = &CovelliteGuiCoreProxy;

  const int Width = 1612211039;
  const int Height = 1612211040;
  const int StatusBarHeight = 1612211053;
  const Vector_t ContextSize(Width, Height - StatusBarHeight);

  ::mock::CovelliteGui::Core::Context Context;

  using namespace ::testing;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(WindowApiProxy, GetClientRect(_))
    .Times(1)
    .WillOnce(Return(WindowOs_t::Rect{ 0, StatusBarHeight, Width, Height }));

  EXPECT_CALL(Context, SetDimensions(ContextSize))
    .Times(1);

  using namespace ::covellite;

  events::Events Events = IWindowApi;
  Events[events::Window.Resize]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnMotion)
{
  using WindowApiProxy_t = WindowApi_t::Proxy;
  WindowApiProxy_t WindowApiProxy;
  WindowApiProxy_t::GetInstance() = &WindowApiProxy;

  using CovelliteGuiCoreProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiCoreProxy_t CovelliteGuiCoreProxy;
  CovelliteGuiCoreProxy_t::GetInstance() = &CovelliteGuiCoreProxy;

  const int StatusBarHeight = 1612211053;
  const int X = 1711012105;
  const int Y = 1711012106;

  ::mock::CovelliteGui::Core::Context Context;

  using namespace ::testing;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(WindowApiProxy, GetClientRect(_))
    .Times(1)
    .WillOnce(Return(WindowOs_t::Rect{ 0, StatusBarHeight, 0, 0 }));

  EXPECT_CALL(Context, ProcessMouseMove(X, Y - StatusBarHeight, 0))
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[Cursor.Motion](Cursor_t::Position{ X, Y });
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnTouch)
{
  using CovelliteGuiCoreProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiCoreProxy_t CovelliteGuiCoreProxy;
  CovelliteGuiCoreProxy_t::GetInstance() = &CovelliteGuiCoreProxy;

  ::mock::CovelliteGui::Core::Context Context;

  using namespace ::testing;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(_, _, _))
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
  using CovelliteGuiCoreProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiCoreProxy_t CovelliteGuiCoreProxy;
  CovelliteGuiCoreProxy_t::GetInstance() = &CovelliteGuiCoreProxy;

  ::mock::CovelliteGui::Core::Context Context;

  using namespace ::testing;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(_, _, _))
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
  using LayersProxy_t = ::mock::covellite::gui::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;
  SupportWindow Support{ IWindowApi };

  Tested_t Example{ IWindowApi };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(LayersProxy, Pop(_))
    .Times(1)
    .WillOnce(Return(true));

  EXPECT_CALL(Support, OnExit())
    .Times(0);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[::covellite::events::Key.Back]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnBack_EmptyLayers)
{
  using LayersProxy_t = ::mock::covellite::gui::Layers::Proxy;
  LayersProxy_t LayersProxy;
  LayersProxy_t::GetInstance() = &LayersProxy;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;
  SupportWindow Support{ IWindowApi };

  Tested_t Example{ IWindowApi };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(LayersProxy, Pop(_))
    .Times(1)
    .WillOnce(Return(false));

  EXPECT_CALL(Support, OnExit())
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[::covellite::events::Key.Back]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnKeyDown)
{
  using SystemToGuiKeyCodeProxy_t = ::covellite::gui::SystemToGuiKeyCodeProxy;
  SystemToGuiKeyCodeProxy_t SystemToGuiKeyCodeProxy;
  SystemToGuiKeyCodeProxy_t::GetInstance() = &SystemToGuiKeyCodeProxy;

  using CovelliteGuiCoreProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiCoreProxy_t CovelliteGuiCoreProxy;
  CovelliteGuiCoreProxy_t::GetInstance() = &CovelliteGuiCoreProxy;

  const auto KeyCode = 1711012116;
  const auto CovelliteGuiKeyCode = 
    (::mock::CovelliteGui::Core::Input::KeyIdentifier)1711012117;

  ::mock::CovelliteGui::Core::Context Context;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(SystemToGuiKeyCodeProxy, Convert(KeyCode))
    .Times(1)
    .WillOnce(Return(CovelliteGuiKeyCode));

  EXPECT_CALL(Context, ProcessKeyDown(CovelliteGuiKeyCode, 0))
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[::covellite::events::Key.Down](KeyCode);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnKeyPressed_ControlCodes)
{
  using CovelliteGuiCoreProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiCoreProxy_t CovelliteGuiCoreProxy;
  CovelliteGuiCoreProxy_t::GetInstance() = &CovelliteGuiCoreProxy;

  using namespace ::testing;

  ::mock::CovelliteGui::Core::Context Context;

  InSequence Dummy;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  using namespace ::covellite::events;

  Events Events = IWindowApi;

  for (int i = 0; i < 0x20; i++)
  {
    EXPECT_CALL(Context, ProcessTextInput(_))
      .Times(0);

    Events[::covellite::events::Key.Pressed](i);
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_OnKeyPressed)
{
  using CovelliteGuiCoreProxy_t = ::mock::CovelliteGui::Core::Proxy;
  CovelliteGuiCoreProxy_t CovelliteGuiCoreProxy;
  CovelliteGuiCoreProxy_t::GetInstance() = &CovelliteGuiCoreProxy;

  const auto KeyCode = static_cast< CovelliteGuiUnicode_t>(17110);

  using namespace ::testing;

  ::mock::CovelliteGui::Core::Context Context;

  InSequence Dummy;

  const WindowOs_t WindowOs{ m_App };
  const WindowApi_t WindowApi{ WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(CovelliteGuiCoreProxy, CreateContext(_, _, _))
    .Times(1)
    .WillOnce(Return(&Context));

  Tested_t Example{ IWindowApi };

  EXPECT_CALL(Context, ProcessTextInput(KeyCode))
    .Times(1);

  using namespace ::covellite::events;

  Events Events = IWindowApi;
  Events[::covellite::events::Key.Pressed](static_cast<int>(KeyCode));
}
