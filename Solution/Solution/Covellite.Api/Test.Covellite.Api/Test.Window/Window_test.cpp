
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Window.
*/

#include <Covellite\Os.mock.hpp>
#include <Covellite.Api\Test.Covellite.Api\Mock\ApiImpl.hpp>

// Расположение класса Window
#include <Covellite.Api\Covellite.Api\Window.cpp>

// Общий тестовый класс класса Window
class Window_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::Window<::mock::ApiImpl>;
  using IWindowCore_t = ::covellite::core::IWindow;
  using IWindowApi_t = ::covellite::api::IWindow;
  using WindowOs_t = ::mock::covellite::os::Window;
  using RenderInterface_t = ::mock::Rocket::Core::RenderInterface;
  using String_t = ::alicorn::extension::std::String;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<String_t>::Set(uT("DefaultString"));
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<String_t>::Clear();
  }
};

// Образец макроса для подстановки в класс Window 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Window_test, Test_Function);

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Constructor)
{
  using WindowOsProxy_t = ::mock::covellite::os::Window::Proxy;
  WindowOsProxy_t WindowOsProxy;
  WindowOsProxy_t::GetInstance() = &WindowOsProxy;

  using ApiImplProxy_t = ::mock::ApiImpl::Proxy;
  ApiImplProxy_t ApiImplProxy;
  ApiImplProxy_t::GetInstance() = &ApiImplProxy;

  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  const ::mock::Id_t WindowOsId = 1710311123;
  const ::mock::Id_t WindowApiId = 1710311117;
  const auto Version = uT("Version1710311118");

  using namespace ::testing;

  EXPECT_CALL(WindowOsProxy, Constructor())
    .Times(1)
    .WillOnce(Return(WindowOsId));

  EXPECT_CALL(ApiImplProxy, Constructor(WindowOsId))
    .Times(1)
    .WillOnce(Return(WindowApiId));

  EXPECT_CALL(ApiImplProxy, GetUsingApi(WindowApiId))
    .Times(1)
    .WillOnce(Return(Version));

  EXPECT_CALL(LoggerProxy, ToLog(uT("Info: Using graphics API: ") + Version))
    .Times(1);

  Tested_t Example{ ::std::make_shared<WindowOs_t>() };
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Subscribe)
{
  using ApiImplProxy_t = ::mock::ApiImpl::Proxy;
  ApiImplProxy_t ApiImplProxy;
  ApiImplProxy_t::GetInstance() = &ApiImplProxy;

  using EventHandlerProxy_t = ::mock::covellite::core::EventHandler::Proxy;
  EventHandlerProxy_t EventHandlerProxy;
  EventHandlerProxy_t::GetInstance() = &EventHandlerProxy;

  const ::mock::Id_t WindowApiId = 1710161316;
  const ::mock::Id_t EventHandlerId = 1710161320;

  using namespace ::testing;

  auto pWindow = ::std::make_shared<WindowOs_t>();

  EXPECT_CALL(ApiImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(WindowApiId));

  Tested_t Example{ pWindow };
  IWindowCore_t & IExample = Example;

  EXPECT_CALL(EventHandlerProxy, Constructor())
    .Times(1)
    .WillOnce(Return(EventHandlerId));

  auto pEventHandler =
    ::std::make_shared<::mock::covellite::core::EventHandler>();

  EXPECT_CALL(ApiImplProxy, Subscribe(WindowApiId, EventHandlerId))
    .Times(1);

  IExample.Subscribe(pEventHandler);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetUsingApi)
{
  const Tested_t Example{ ::std::make_shared<WindowOs_t>() };
  const IWindowApi_t & IExample = Example;

  EXPECT_THROW(IExample.GetUsingApi(), ::std::exception);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetWidth)
{
  using ApiImplProxy_t = ::mock::ApiImpl::Proxy;
  ApiImplProxy_t ApiImplProxy;
  ApiImplProxy_t::GetInstance() = &ApiImplProxy;

  const ::mock::Id_t WindowApiId = 1710161333;
  const int32_t Width = 1710161331;

  using namespace ::testing;

  auto pWindow = ::std::make_shared<WindowOs_t>();

  EXPECT_CALL(ApiImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(WindowApiId));

  const Tested_t Example{ pWindow };
  const IWindowApi_t & IExample = Example;

  EXPECT_CALL(ApiImplProxy, GetWidth(WindowApiId))
    .Times(1)
    .WillOnce(Return(Width));

  const auto Result = IExample.GetWidth();
  EXPECT_EQ(Width, Result);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_GetHeight)
{
  using ApiImplProxy_t = ::mock::ApiImpl::Proxy;
  ApiImplProxy_t ApiImplProxy;
  ApiImplProxy_t::GetInstance() = &ApiImplProxy;

  const ::mock::Id_t WindowApiId = 1710161335;
  const int32_t Height = 1710161336;

  using namespace ::testing;

  auto pWindow = ::std::make_shared<WindowOs_t>();

  EXPECT_CALL(ApiImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(WindowApiId));

  const Tested_t Example{ pWindow };
  const IWindowApi_t & IExample = Example;

  EXPECT_CALL(ApiImplProxy, GetHeight(WindowApiId))
    .Times(1)
    .WillOnce(Return(Height));

  const auto Result = IExample.GetHeight();
  EXPECT_EQ(Height, Result);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_MakeRenderInterface)
{
  using ApiImplProxy_t = ::mock::ApiImpl::Proxy;
  ApiImplProxy_t ApiImplProxy;
  ApiImplProxy_t::GetInstance() = &ApiImplProxy;

  const ::mock::Id_t WindowApiId = 1710161341;
  auto pRenderInterface = ::std::make_shared<RenderInterface_t>(1710161342);

  using namespace ::testing;

  auto pWindow = ::std::make_shared<WindowOs_t>();

  EXPECT_CALL(ApiImplProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(WindowApiId));

  const Tested_t Example{ pWindow };
  const IWindowApi_t & IExample = Example;

  EXPECT_CALL(ApiImplProxy, MakeRenderInterface(WindowApiId))
    .Times(1)
    .WillOnce(Return(pRenderInterface));

  const auto pResult = IExample.MakeRenderInterface();
  EXPECT_EQ(pRenderInterface, pResult);
}
