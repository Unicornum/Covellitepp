
#include "stdafx.h"
#include <alicorn/patterns/factory.hpp>
#include <Platform/Windows.mock.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Window.
*/

#include <Covellite/App.mock.hpp>
#include <Covellite/Os.mock.hpp>
#include <Covellite/App/Events.hpp>
#include <Covellite/Os/Events.hpp>
#include <Covellite/Api/Events.hpp>
#include <Covellite/Api/Component.inl>

// Расположение класса Window
#include "../../Covellite.Api/Window.cpp"

namespace covellite
{

namespace api
{

/*static*/ auto Window::GetRenderers(void) -> ::std::vector<String_t>
{
  return {};
}

/*static*/ auto Window::MakeImpl(const String_t & _Api,
  const renderer::SettingsData & _Data) -> IGraphicApiPtr_t
{
  using namespace ::alicorn::modules::patterns;

  return factory::make_unique<renderer::IGraphicApi>(_Api, _Data);
}

} // namespace api

} // namespace covellite

#include "../Mock/Equal.hpp"
#include "../Mock/RendererImpl.hpp"

namespace mock
{

namespace common
{

DEFINE_RENDER_IMPL(Dummy);

} // namespace common

} // namespace mock

// Общий тестовый класс класса Window
class Window_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::Window;
  using String_t = ::alicorn::extension::std::String;
  using IWindowApi_t = ::covellite::api::IWindow;
  using WindowOs_t = ::mock::covellite::os::Window;
  using Rect_t = ::covellite::os::IWindow::Rect;
  using Data_t = ::covellite::api::renderer::SettingsData;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<String_t>::Set(uT("DefaultString"));
    ::testing::DefaultValue<Rect_t>::Set(Rect_t{ 0, 0, 0, 0 });
    ::testing::DefaultValue<::covellite::Any_t>::Set((HWND)0);
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<String_t>::Clear();
    ::testing::DefaultValue<Rect_t>::Clear();
    ::testing::DefaultValue<::covellite::Any_t>::Clear();
  }

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
    .WillRepeatedly(Return(uT("Dummy")));

  WindowOs_t WindowOs{ m_App };
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

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t WindowOsId = 1710311123;
  const ::mock::Id_t WindowSectionId = 1806101824;

  for (const auto IsFullScreen : { true, false })
  {
    Data_t Data;
    Data.Handle = (HWND)1808221257;
    Data.Top = 1808271300;
    Data.IsFullScreen = IsFullScreen;

    using namespace ::testing;

    EXPECT_CALL(WindowOsProxy, Constructor(_))
      .Times(1)
      .WillOnce(Return(WindowOsId));

    WindowOs_t WindowOs{ m_App };

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

    EXPECT_CALL(SettingsProxy, GetValue(WindowSectionId, uT("GraphicsApi")))
      .Times(1)
      .WillOnce(Return(uT("Dummy")));

    EXPECT_CALL(RendererImplProxy, Constructor(Eq(Data)))
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

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RenderId = 1808221312;
  WindowOs_t WindowOs{ m_App };

  using namespace ::covellite::events;

  Events Events = WindowOs;

  Events[Drawing.Do].Connect([&]() { Proxy.DoDrawing(); });

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("Dummy")));

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RenderId));

  const Tested_t Example{ WindowOs };

  InSequence Dummy;

  EXPECT_CALL(Proxy, DoDrawing())
    .Times(1);

  EXPECT_CALL(RendererImplProxy, PresentFrame(RenderId))
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

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t RenderId = 1808221411;
  const auto Width = 1808221412;
  const auto Height = 1808221413;
  WindowOs_t WindowOs{ m_App };

  using namespace ::covellite;

  events::Events Events = WindowOs;

  using namespace ::testing;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("Dummy")));

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(RenderId));

  const Tested_t Example{ WindowOs };

  InSequence Dummy;

  EXPECT_CALL(WindowOsProxy, GetClientRect(_))
    .Times(1)
    .WillOnce(Return(Rect_t{ 0, 0, Width, Height }));

  EXPECT_CALL(RendererImplProxy, ResizeWindow(RenderId, Width, Height))
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
    .WillRepeatedly(Return(uT("Dummy")));

  WindowOs_t WindowOs{ m_App };
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
    .WillRepeatedly(Return(uT("Dummy")));

  EXPECT_CALL(WindowOsProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(WindowOsId));

  WindowOs_t WindowOs{ m_App };
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
TEST_F(Window_test, /*DISABLED_*/Test_GetRenders)
{
  using Render_t = ::std::function<void(void)>;
  using ComponentPtr_t = ::std::shared_ptr<::covellite::api::Component>;
  using Creator_t = ::std::function<Render_t(const ComponentPtr_t &)>;
  using Creators_t = ::std::map<String_t, Creator_t>;

  using SettingsProxy_t = ::mock::alicorn::modules::settings::SectionImplProxy;
  SettingsProxy_t SettingsProxy;
  SettingsProxy_t::GetInstance() = &SettingsProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t ImplId = 1905031704;

  const Creator_t Creator = [](const ComponentPtr_t &) { return [](void) {}; };

  const Creators_t Creators1 =
  {
    { uT("Type1"), Creator },
  };

  const Creators_t Creators2 =
  {
    { uT("Type2"), Creator },
  };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SettingsProxy, GetValue(_, _))
    .WillOnce(Return(uT("false")))
    .WillRepeatedly(Return(uT("Dummy")));

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(ImplId));

  WindowOs_t WindowOs{ m_App };
  const Tested_t Example{ WindowOs };
  const IWindowApi_t & IExample = Example;

  EXPECT_CALL(RendererImplProxy, GetCreators(ImplId))
    .Times(1)
    .WillOnce(ReturnRef(Creators1));

  const auto pResult1 = IExample.GetRenders();
  const auto Result1 = pResult1->Obtain(
    { 
      ::covellite::api::Component::Make({ { uT("type"), uT("Type1") } }),
    });

  EXPECT_CALL(RendererImplProxy, GetCreators(ImplId))
    .Times(1)
    .WillOnce(ReturnRef(Creators2));

  const auto pResult2 = IExample.GetRenders();
  const auto Result2 = pResult2->Obtain(
    { 
      ::covellite::api::Component::Make({ { uT("type"), uT("Type2") } }),
    });

  EXPECT_NE(pResult1, pResult2);
  EXPECT_EQ(1, Result1.size());
  EXPECT_EQ(1, Result2.size());
}
