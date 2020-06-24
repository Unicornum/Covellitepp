
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса Window.
*/

// Расположение класса Window
#include <Covellite/Expanse/Window.hpp>

#include <Covellite/Api/Events.hpp>
#include <Covellite/Covellite.Events.lib.hpp>
#include <Covellite/Covellite.Api.lib.hpp>
#pragma comment(lib, "opengl32.lib")

// Общий тестовый класс класса Window
class Window_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::Window;
  using IGameScene_t = ::covellite::expanse::IGameScene;
  using I3DScene_t = ::covellite::expanse::I3DScene;
  using IWindowApi_t = ::covellite::api::IWindow;
  using WindowApi_t = ::mock::covellite::api::Window;
  using Events_t = ::covellite::events::Events;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) noexcept override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) noexcept override
  {
  }

protected:
  using App_t = ::mock::covellite::app::Application;
  using WindowOs_t = ::mock::covellite::os::Window;
  const App_t m_App{ App_t::EventBased{} };
  const WindowOs_t m_WindowOs{ m_App };
};

ALICORN_DISABLE_GTEST_WARNINGS

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
TEST_F(Window_test, /*DISABLED_*/Test_Constructor)
{
  using Renders_t = ::covellite::api::Component::Renders;

  using WindowApiProxy_t = ::mock::covellite::api::Window::Proxy;
  WindowApiProxy_t WindowApiProxy;

  using C3DSceneProxy_t = ::mock::covellite::expanse::C3DScene::Proxy;
  C3DSceneProxy_t C3DSceneProxy;

  using GameSceneProxy_t = ::mock::covellite::expanse::GameScene::Proxy;
  GameSceneProxy_t GameSceneProxy;

  constexpr ::mock::Id_t WindowApiId = 2006201332;
  constexpr ::mock::Id_t C3DSceneId = 2006201329;
  const auto pRenders = ::std::make_shared<Renders_t>(Renders_t::Creators_t{});

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(WindowApiProxy, Constructor(_))
    .Times(1)
    .WillOnce(Return(WindowApiId));

  const WindowApi_t WindowApi{ m_WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(C3DSceneProxy, Constructor())
    .Times(1)
    .WillOnce(Return(C3DSceneId));

  EXPECT_CALL(WindowApiProxy, GetRenders(WindowApiId))
    .Times(1)
    .WillOnce(Return(pRenders));

  EXPECT_CALL(GameSceneProxy, Constructor(pRenders, C3DSceneId))
    .Times(1);

  const Tested_t Example{ IWindowApi };
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Events)
{
  class Proxy
  {
  public:
    MOCK_METHOD0(DoEvent, void(void));
  };

  Proxy oProxy;
  constexpr int Id = 2006201239;

  const WindowApi_t WindowApi{ m_WindowOs };
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
TEST_F(Window_test, /*DISABLED_*/Test_CreateObject)
{
  using GameSceneProxy_t = ::mock::covellite::expanse::GameScene::Proxy;
  GameSceneProxy_t GameSceneProxy;

  constexpr ::mock::Id_t GameSceneId = 2006201347;
  constexpr ::covellite::expanse::ObjectId_t ObjectId = 2006201351;

  const ::covellite::expanse::GameObject_t GameObject =
  {
    ::covellite::api::Component::Make({}),
    ::covellite::api::Component::Make({}),
    ::covellite::api::Component::Make({}),
  };

  const WindowApi_t WindowApi{ m_WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GameSceneProxy, Constructor(_, _))
    .Times(1)
    .WillOnce(Return(GameSceneId));

  Tested_t Example{ IWindowApi };
  IGameScene_t & IExample = Example;

  EXPECT_CALL(GameSceneProxy, CreateObject(GameSceneId, GameObject))
    .Times(1)
    .WillOnce(Return(ObjectId));

  const auto Result = IExample.CreateObject(GameObject);
  EXPECT_EQ(Result, ObjectId);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_RemoveObject)
{
  using GameSceneProxy_t = ::mock::covellite::expanse::GameScene::Proxy;
  GameSceneProxy_t GameSceneProxy;

  constexpr ::mock::Id_t GameSceneId = 2006201401;
  constexpr ::covellite::expanse::ObjectId_t ObjectId = 2006201402;

  const WindowApi_t WindowApi{ m_WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GameSceneProxy, Constructor(_, _))
    .Times(1)
    .WillOnce(Return(GameSceneId));

  Tested_t Example{ IWindowApi };
  IGameScene_t & IExample = Example;

  EXPECT_CALL(GameSceneProxy, RemoveObject(GameSceneId, ObjectId))
    .Times(1);

  IExample.RemoveObject(ObjectId);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_Add)
{
  using C3DSceneProxy_t = ::mock::covellite::expanse::C3DScene::Proxy;
  C3DSceneProxy_t C3DSceneProxy;

  constexpr ::mock::Id_t C3DSceneId = 2006201539;

  const WindowApi_t WindowApi{ m_WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(C3DSceneProxy, Constructor())
    .Times(1)
    .WillOnce(Return(C3DSceneId));

  Tested_t Example{ IWindowApi };
  I3DScene_t & IExample = Example;

  EXPECT_CALL(C3DSceneProxy, Add(C3DSceneId, 0, 2006201540, 0))
    .Times(1);

  IExample.Add(0, 2006201540);

  EXPECT_CALL(C3DSceneProxy, Add(C3DSceneId, 1, 2006201541, 2006201542))
    .Times(1);

  IExample.Add(1, 2006201541, 2006201542);
}

// ************************************************************************** //
TEST_F(Window_test, /*DISABLED_*/Test_RedrawWindow)
{
  using C3DSceneProxy_t = ::mock::covellite::expanse::C3DScene::Proxy;
  C3DSceneProxy_t C3DSceneProxy;

  using GameSceneProxy_t = ::mock::covellite::expanse::GameScene::Proxy;
  GameSceneProxy_t GameSceneProxy;

  constexpr ::mock::Id_t C3DSceneId = 2006201549;
  constexpr ::mock::Id_t GameSceneId = 2006201550;

  using namespace ::testing;

  InSequence Dummy;

  const WindowApi_t WindowApi{ m_WindowOs };
  const IWindowApi_t & IWindowApi = WindowApi;

  EXPECT_CALL(C3DSceneProxy, Constructor())
    .Times(1)
    .WillOnce(Return(C3DSceneId));

  EXPECT_CALL(GameSceneProxy, Constructor(_, _))
    .Times(1)
    .WillOnce(Return(GameSceneId));

  const Tested_t Example{ IWindowApi };


  EXPECT_CALL(GameSceneProxy, Update(GameSceneId))
    .Times(1);

  EXPECT_CALL(C3DSceneProxy, Render(C3DSceneId))
    .Times(1);

  Events_t ExampleEvents = Example;
  ExampleEvents[::covellite::events::Drawing.Do]();
}

ALICORN_RESTORE_WARNINGS
