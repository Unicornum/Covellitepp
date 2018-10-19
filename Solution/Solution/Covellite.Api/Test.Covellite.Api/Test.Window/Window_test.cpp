
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Window.
*/

#include <Covellite/Os.mock.hpp>
#include <Covellite/App/Events.hpp>
#include <Covellite/Os/Events.hpp>
#include <Covellite/Api/Events.hpp>

#include "../Mock/Renderer.hpp"
#include "../Mock/Eq.hpp"

// Расположение класса Window
#include "../../Covellite.Api/Window.cpp"

// Общий тестовый класс класса Window
class Window_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::Window;
  using Render_t = ::mock::covellite::api::renderer::Renderer;
  using IWindowApi_t = ::covellite::api::IWindow;
  using IWindowCore_t = ::covellite::core::IWindow;
  using WindowOs_t = ::mock::covellite::os::Window;
  using String_t = ::alicorn::extension::std::String;
  using Rect_t = ::covellite::os::IWindow::Rect;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<Rect_t>::Set(Rect_t{ 0, 0, 0, 0 });
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<Rect_t>::Clear();
  }
};

// Образец макроса для подстановки в класс Window 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Window_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_AppIWindow)
{
  EXPECT_TRUE((::std::is_convertible<Tested_t &,
    ::covellite::app::IWindow &>::value));
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Dummy)
{
  // Исключительно для того, чтобы создался объект настроек.

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  WindowOs_t WindowOs;
  Tested_t Example{ WindowOs };
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RenderProxy_t = Render_t::Proxy;
  RenderProxy_t RenderProxy;
  RenderProxy_t::GetInstance() = &RenderProxy;

  const ::mock::Id_t WindowOsId = 1710311123;
  const ::mock::Id_t WindowSectionId = 1806101824;
  const ::mock::Id_t BackgroundColorSectionId = 1808231108;
  const auto ApiName = uT("Api1808211209");

  for (const auto IsFullScreen : { true, false })
  {
    ::covellite::api::renderer::IRenderer::Data Data;
    Data.Handle = (HWND)1808221257;
    Data.Top = 1808271300;
    Data.BkColor = { 0.1f, 0.2f, 0.3f, 0.4f };
    Data.IsFullScreen = IsFullScreen;

    using namespace ::testing;

    EXPECT_CALL(WindowOsProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowOsId));

    WindowOs_t WindowOs;

    InSequence Dummy;

    EXPECT_CALL(WindowOsProxy, GetHandle(WindowOsId))
      .Times(1)
      .WillOnce(Return(Data.Handle));

    EXPECT_CALL(WindowOsProxy, GetClientRect(WindowOsId))
      .Times(1)
      .WillOnce(Return(Rect_t{ 0, Data.Top, 0, 0 }));

    EXPECT_CALL(SettingsProxy, GetChildSectionImpl(_, uT("Window")))
      .Times(1);

    EXPECT_CALL(SettingsProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowSectionId));

    EXPECT_CALL(SettingsProxy, GetValue(WindowSectionId, uT("IsFullScreen")))
      .Times(1)
      .WillOnce(Return(IsFullScreen ? uT("true") : uT("false")));

    EXPECT_CALL(SettingsProxy,
      GetChildSectionImpl(WindowSectionId, uT("BackgroundColor")))
      .Times(1);

    EXPECT_CALL(SettingsProxy, Constructor())
      .Times(1)
      .WillOnce(Return(BackgroundColorSectionId));

    EXPECT_CALL(SettingsProxy, GetValue(BackgroundColorSectionId, uT("R")))
      .Times(1)
      .WillOnce(Return(uT("25.5")));

    EXPECT_CALL(SettingsProxy, GetValue(BackgroundColorSectionId, uT("G")))
      .Times(1)
      .WillOnce(Return(uT("51")));

    EXPECT_CALL(SettingsProxy, GetValue(BackgroundColorSectionId, uT("B")))
      .Times(1)
      .WillOnce(Return(uT("76.5")));

    EXPECT_CALL(SettingsProxy, GetValue(BackgroundColorSectionId, uT("A")))
      .Times(1)
      .WillOnce(Return(uT("102")));

    EXPECT_CALL(SettingsProxy, GetValue(WindowSectionId, uT("GraphicsApi")))
      .Times(1)
      .WillOnce(Return(ApiName));

    EXPECT_CALL(RenderProxy, Constructor(ApiName, Eq(Data)))
      .Times(1);

    const Tested_t Example{ WindowOs };
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_DoApplicationUpdate)
{
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD0(DoDrawing, void(void));
  };

  Proxy Proxy;
  Proxy::GetInstance() = &Proxy;

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RenderProxy_t = ::mock::covellite::api::renderer::Renderer::Proxy;
  RenderProxy_t RenderProxy;
  RenderProxy_t::GetInstance() = &RenderProxy;

  const ::mock::Id_t RenderId = 1808221312;
  WindowOs_t WindowOs;

  using namespace ::covellite::events;

  Events Events = WindowOs;

  Events[Drawing.Do].Connect([&]() { Proxy.DoDrawing(); });

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  EXPECT_CALL(RenderProxy, Constructor(_, _))
    .Times(1)
    .WillOnce(Return(RenderId));

  const Tested_t Example{ WindowOs };

  InSequence Dummy;

  EXPECT_CALL(RenderProxy, ClearWindow(RenderId))
    .Times(1);

  EXPECT_CALL(Proxy, DoDrawing())
    .Times(1);

  EXPECT_CALL(RenderProxy, Present(RenderId))
    .Times(1);

  Events[Application.Update]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_DoWindowResize)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RenderProxy_t = ::mock::covellite::api::renderer::Renderer::Proxy;
  RenderProxy_t RenderProxy;
  RenderProxy_t::GetInstance() = &RenderProxy;

  const ::mock::Id_t RenderId = 1808221411;
  const auto Width = 1808221412;
  const auto Height = 1808221413;
  WindowOs_t WindowOs;

  using namespace ::covellite;

  events::Events Events = WindowOs;

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  EXPECT_CALL(RenderProxy, Constructor(_, _))
    .Times(1)
    .WillOnce(Return(RenderId));

  const Tested_t Example{ WindowOs };

  InSequence Dummy;

  EXPECT_CALL(WindowOsProxy, GetClientRect(_))
    .Times(1)
    .WillOnce(Return(Rect_t{ 0, 0, Width, Height }));

  EXPECT_CALL(RenderProxy, ResizeWindow(RenderId, Width, Height))
    .Times(1);

  Events[events::Window.Resize]();
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Events)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  const auto Message = 1807201322;

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  WindowOs_t WindowOs;
  ::covellite::events::Events Events = WindowOs;

  const Tested_t Example{ WindowOs };
  const IWindowApi_t & IExample = Example;

  auto Result = IExample.operator ::covellite::events::Events();

  bool IsChanged = false;
  Result[Message].Connect([&]() { IsChanged = true; });
  EXPECT_FALSE(IsChanged);

  Events[Message]();
  EXPECT_TRUE(IsChanged);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetClientRect)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  const ::mock::Id_t WindowOsId = 1807201251;
  const Rect_t Rect = { 1807201252, 1807201253, 1807201254, 1807201255 };

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  EXPECT_CALL(WindowOsProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowOsId));

  WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };
  const IWindowApi_t & IExample = Example;

  EXPECT_CALL(WindowOsProxy, GetClientRect(WindowOsId))
    .Times(1)
    .WillOnce(Return(Rect));

  const auto Result = IExample.GetClientRect();
  EXPECT_EQ(Rect.Left, Result.Left);
  EXPECT_EQ(Rect.Top, Result.Top);
  EXPECT_EQ(Rect.Width, Result.Width);
  EXPECT_EQ(Rect.Height, Result.Height);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetRenderInterface)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RenderProxy_t = Render_t::Proxy;
  RenderProxy_t RenderProxy;
  RenderProxy_t::GetInstance() = &RenderProxy;

  const ::mock::Id_t RenderId = 1808202112;

  using namespace ::testing;

  auto pWindow = ::std::make_shared<WindowOs_t>();

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  EXPECT_CALL(RenderProxy, Constructor(_, _))
    .Times(1)
    .WillOnce(Return(RenderId));

  WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };
  const IWindowApi_t & IExample = Example;

  const auto pResult = IExample.GetRenderInterface();
  EXPECT_EQ(RenderId, dynamic_cast<const Render_t &>(*pResult).m_Id);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor_Deprecated)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RenderProxy_t = Render_t::Proxy;
  RenderProxy_t RenderProxy;
  RenderProxy_t::GetInstance() = &RenderProxy;

  const ::mock::Id_t WindowOsId = 1808211304;
  const ::mock::Id_t WindowSectionId = 1808211305;
  const ::mock::Id_t BackgroundColorSectionId = 1808241138;
  const auto ApiName = uT("Api1808211306");

  for (const auto IsFullScreen : { true, false })
  {
    ::covellite::api::renderer::IRenderer::Data Data;
    Data.Handle = (HWND)1808241139;
    Data.Top = 1808271313;
    Data.BkColor = { 0.1f, 0.2f, 0.3f, 0.4f };
    Data.IsFullScreen = IsFullScreen;

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(WindowOsProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowOsId));

    EXPECT_CALL(WindowOsProxy, GetHandle(WindowOsId))
      .Times(1)
      .WillOnce(Return(Data.Handle));

    EXPECT_CALL(WindowOsProxy, GetClientRect(WindowOsId))
      .Times(1)
      .WillOnce(Return(Rect_t{ 0, Data.Top, 0, 0 }));

    EXPECT_CALL(SettingsProxy, GetChildSectionImpl(_, uT("Window")))
      .Times(1);

    EXPECT_CALL(SettingsProxy, Constructor())
      .Times(1)
      .WillOnce(Return(WindowSectionId));

    EXPECT_CALL(SettingsProxy, GetValue(WindowSectionId, uT("IsFullScreen")))
      .Times(1)
      .WillOnce(Return(IsFullScreen ? uT("true") : uT("false")));

    EXPECT_CALL(SettingsProxy,
      GetChildSectionImpl(WindowSectionId, uT("BackgroundColor")))
      .Times(1);

    EXPECT_CALL(SettingsProxy, Constructor())
      .Times(1)
      .WillOnce(Return(BackgroundColorSectionId));

    EXPECT_CALL(SettingsProxy, GetValue(BackgroundColorSectionId, uT("R")))
      .Times(1)
      .WillOnce(Return(uT("25.5")));

    EXPECT_CALL(SettingsProxy, GetValue(BackgroundColorSectionId, uT("G")))
      .Times(1)
      .WillOnce(Return(uT("51")));

    EXPECT_CALL(SettingsProxy, GetValue(BackgroundColorSectionId, uT("B")))
      .Times(1)
      .WillOnce(Return(uT("76.5")));

    EXPECT_CALL(SettingsProxy, GetValue(BackgroundColorSectionId, uT("A")))
      .Times(1)
      .WillOnce(Return(uT("102")));

    EXPECT_CALL(SettingsProxy, GetValue(WindowSectionId, uT("GraphicsApi")))
      .Times(1)
      .WillOnce(Return(ApiName));

    EXPECT_CALL(RenderProxy, Constructor(ApiName, Eq(Data)))
      .Times(1);

    const Tested_t Example{ ::std::make_shared<WindowOs_t>() };
  }
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetWidth_Deprecated)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  const ::mock::Id_t WindowOsId = 1808221243;
  const ::covellite::os::IWindow::Rect Rect = { 0, 0, 1808221242, 0 };

  using namespace ::testing;

  EXPECT_CALL(WindowOsProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowOsId));

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };
  const IWindowApi_t & IExample = Example;

  EXPECT_CALL(WindowOsProxy, GetClientRect(WindowOsId))
    .Times(1)
    .WillOnce(Return(Rect));

  const auto Result = IExample.GetWidth();
  EXPECT_EQ(Rect.Width, Result);
}

// ************************************************************************** //
TEST_F(Window_test,/* DISABLED_*/Test_GetHeight_Deprecated)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  const ::mock::Id_t WindowOsId = 1808221243;
  const ::covellite::os::IWindow::Rect Rect = { 0, 0, 0, 1808221244 };

  using namespace ::testing;

  EXPECT_CALL(WindowOsProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowOsId));

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };
  const IWindowApi_t & IExample = Example;

  EXPECT_CALL(WindowOsProxy, GetClientRect(WindowOsId))
    .Times(1)
    .WillOnce(Return(Rect));

  const auto Result = IExample.GetHeight();
  EXPECT_EQ(Rect.Height, Result);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_MakeRenderInterface_Deprecated)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RenderProxy_t = Render_t::Proxy;
  RenderProxy_t RenderProxy;
  RenderProxy_t::GetInstance() = &RenderProxy;

  const ::mock::Id_t RenderId = 1808211442;

  using namespace ::testing;

  auto pWindow = ::std::make_shared<WindowOs_t>();

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  EXPECT_CALL(RenderProxy, Constructor(_, _))
    .Times(1)
    .WillOnce(Return(RenderId));

  WindowOs_t WindowOs;
  const Tested_t Example{ WindowOs };
  const IWindowApi_t & IExample = Example;

  const auto pResult = IExample.MakeRenderInterface();
  EXPECT_EQ(RenderId, dynamic_cast<const Render_t &>(*pResult).m_Id);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_DoStartDrawing_Deprecated)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RenderProxy_t = ::mock::covellite::api::renderer::Renderer::Proxy;
  RenderProxy_t RenderProxy;
  RenderProxy_t::GetInstance() = &RenderProxy;

  const ::mock::Id_t WindowSectionId = 1808231107;
  const ::mock::Id_t RenderId = 1808221312;
  auto pWindow = ::std::make_shared<WindowOs_t>();
  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  EXPECT_CALL(RenderProxy, Constructor(_, _))
    .Times(1)
    .WillOnce(Return(RenderId));

  Tested_t Example{ pWindow };
  IWindowCore_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  EXPECT_CALL(RenderProxy, ClearWindow(RenderId))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::StartDrawing, params::Empty{});
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_DoFinishDrawing_Deprecated)
{
  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RenderProxy_t = ::mock::covellite::api::renderer::Renderer::Proxy;
  RenderProxy_t RenderProxy;
  RenderProxy_t::GetInstance() = &RenderProxy;

  const ::mock::Id_t RenderId = 1808221312;
  auto pWindow = ::std::make_shared<WindowOs_t>();
  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  EXPECT_CALL(RenderProxy, Constructor(_, _))
    .Times(1)
    .WillOnce(Return(RenderId));

  Tested_t Example{ pWindow };
  IWindowCore_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  EXPECT_CALL(RenderProxy, Present(RenderId))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::FinishDrawing, params::Empty{});
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_DoResize_Deprecated)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RenderProxy_t = ::mock::covellite::api::renderer::Renderer::Proxy;
  RenderProxy_t RenderProxy;
  RenderProxy_t::GetInstance() = &RenderProxy;

  const ::mock::Id_t RenderId = 1808221423;
  const auto Width = 1808221424;
  const auto Height = 1808221425;
  auto pWindow = ::std::make_shared<WindowOs_t>();
  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("0")));

  EXPECT_CALL(RenderProxy, Constructor(_, _))
    .Times(1)
    .WillOnce(Return(RenderId));

  Tested_t Example{ pWindow };
  IWindowCore_t & IExample = Example;
  IExample.Subscribe(pEventHandler);

  InSequence Dummy;

  EXPECT_CALL(WindowOsProxy, GetClientRect(_))
    .Times(1)
    .WillOnce(Return(Rect_t{ 0, 0, Width, Height }));

  EXPECT_CALL(RenderProxy, ResizeWindow(RenderId, Width, Height))
    .Times(1);

  using namespace ::covellite::core;

  pEventHandler->DoCommand(Event::Resize, params::Empty{});
}
