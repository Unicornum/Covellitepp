
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса GameScene.
*/

// Расположение класса GameScene
#include "../../Covellite.Expanse/GameScene.impl.hpp"
#include "../../Covellite.Expanse/IProduce3DObject.impl.hpp"
#include <Covellite/Api/Renders.hpp>
#include <Covellite/Covellite.Api.lib.hpp>
#pragma comment(lib, "opengl32.lib")

// Общий тестовый класс класса GameScene
class GameScene_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::expanse::GameScene;
  using ObjectId_t = ::covellite::expanse::ObjectId_t;
  using C3DObject_t = ::covellite::expanse::C3DObject_t;
  using Component_t = ::covellite::api::Component;
  using ComponentPtr_t = ::covellite::api::Component::ComponentPtr_t;
  using Renders_t = ::covellite::api::Component::Renders;
  using String_t = ::alicorn::extension::std::String;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) noexcept override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) noexcept override
  {
  }

protected:
  class Produce3DObject :
    public ::covellite::expanse::IProduce3DObject
  {
  public:
    MOCK_METHOD1(DoAdd, void(const ObjectId_t));
    MOCK_METHOD1(DoRemove, void(const ObjectId_t));

  public:
    void Add(const ObjectId_t _Id, const C3DObject_t & _Renders) override
    {
      DoAdd(_Id);

      for (const auto & Render : _Renders) Render();
    }

    void Remove(const ObjectId_t _Id) override
    {
      DoRemove(_Id);
    }
  };
};

ALICORN_DISABLE_GTEST_WARNINGS

// Образец макроса для подстановки в класс GameScene 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(GameScene_test, Test_Function);

// ************************************************************************** //
TEST_F(GameScene_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(GameScene_test, /*DISABLED_*/Test_ProduceObjects_UniqueId)
{
  const auto pRenders = ::std::make_shared<Renders_t>(Renders_t::Creators_t{ });

  Produce3DObject oProduce3DObject;

  Tested_t Example{ pRenders, oProduce3DObject };

  auto Id = Example.CreateObject({ Component_t::Make({}) });
  EXPECT_EQ(0, Id);

  Id = Example.CreateObject({ Component_t::Make({}) });
  EXPECT_EQ(1, Id);

  Id = Example.CreateObject({ Component_t::Make({}) });
  EXPECT_EQ(2, Id);

  Id = Example.CreateObject({ Component_t::Make({}) });
  EXPECT_EQ(3, Id);

  Example.RemoveObject(1);
  Example.RemoveObject(0);
  Example.RemoveObject(0);
  Example.RemoveObject(2006191950);

  Id = Example.CreateObject({ Component_t::Make({}) });
  EXPECT_EQ(1, Id);

  Id = Example.CreateObject({ Component_t::Make({}) });
  EXPECT_EQ(0, Id);

  Id = Example.CreateObject({ Component_t::Make({}) });
  EXPECT_EQ(4, Id);
}

// ************************************************************************** //
TEST_F(GameScene_test, /*DISABLED_*/Test_ProduceObjects)
{
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(CreateCamera, void(String_t));
    MOCK_METHOD1(CreateState, void(String_t));
    MOCK_METHOD1(CreateShader, void(String_t));
    MOCK_METHOD1(CreateUpdater, void(String_t));
    MOCK_METHOD1(Render, void(String_t));
  };

  Proxy oProxy;

  const auto CameraCreator = [&](const ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    oProxy.CreateCamera(_pComponent->Id);

    return [&, _pComponent]() { oProxy.Render(_pComponent->Id); };
  };

  const auto StateCreator = [&](const ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    oProxy.CreateState(_pComponent->Id);

    return [&, _pComponent]() { oProxy.Render(_pComponent->Id); };
  };

  const auto ShaderCreator = [&](const ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    oProxy.CreateShader(_pComponent->Id);

    return [&, _pComponent]() { oProxy.Render(_pComponent->Id); };
  };

  const auto UpdaterCreator = [&](const ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    oProxy.CreateUpdater(_pComponent->Id);

    return [&, _pComponent]() { oProxy.Render(_pComponent->Id); };
  };

  const auto pRenders = ::std::make_shared<Renders_t>(Renders_t::Creators_t
    {
      { uT("Camera"), CameraCreator },
      { uT("State"), StateCreator },
      { uT("Shader"), ShaderCreator },
      { uT("Updater"), UpdaterCreator },
    });

  Produce3DObject oProduce3DObject;

  Tested_t Example{ pRenders, oProduce3DObject };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(oProxy, CreateCamera(uT("2006192004")))
    .Times(1);

  EXPECT_CALL(oProduce3DObject, DoAdd(0))
    .Times(1);

  EXPECT_CALL(oProxy, Render(uT("2006192004")))
    .Times(1);

  Example.CreateObject(
    {
      Component_t::Make(
        {
          { uT("id"), uT("2006192004") },
          { uT("type"), uT("Camera") },
        }),
    });

  EXPECT_CALL(oProxy, CreateCamera(uT("2006192005")))
    .Times(1);

  EXPECT_CALL(oProxy, CreateState(uT("2006192006")))
    .Times(1);

  EXPECT_CALL(oProxy, CreateShader(uT("2006192007")))
    .Times(1);

  EXPECT_CALL(oProxy, CreateUpdater(uT("2006192053")))
    .Times(1);

  EXPECT_CALL(oProduce3DObject, DoAdd(1))
    .Times(1);

  EXPECT_CALL(oProxy, Render(uT("2006192005")))
    .Times(1);

  EXPECT_CALL(oProxy, Render(uT("2006192006")))
    .Times(1);

  EXPECT_CALL(oProxy, Render(uT("2006192007")))
    .Times(1);

  EXPECT_CALL(oProxy, Render(uT("2006192053")))
    .Times(0);

  Example.CreateObject(
    {
      Component_t::Make(
        {
          { uT("id"), uT("2006192005") },
          { uT("type"), uT("Camera") },
        }),
      Component_t::Make(
        {
          { uT("id"), uT("2006192006") },
          { uT("type"), uT("State") },
        }),
      Component_t::Make(
        {
          { uT("id"), uT("2006192007") },
          { uT("type"), uT("Shader") },
        }),
      Component_t::Make(
        {
          { uT("id"), uT("2006192053") },
          { uT("type"), uT("Updater") },
        }),
    });

  EXPECT_CALL(oProduce3DObject, DoRemove(0))
    .Times(1);

  Example.RemoveObject(0);

  EXPECT_CALL(oProxy, CreateCamera(uT("2006192004")))
    .Times(1);

  pRenders->Obtain(Component_t::Make(
      {
        { uT("id"), uT("2006192004") },
        { uT("type"), uT("Camera") },
      }));

  EXPECT_CALL(oProduce3DObject, DoRemove(1))
    .Times(1);

  Example.RemoveObject(1);

  EXPECT_CALL(oProxy, CreateCamera(uT("2006192005")))
    .Times(1);

  pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), uT("2006192005") },
      { uT("type"), uT("Camera") },
    }));

  EXPECT_CALL(oProxy, CreateState(uT("2006192006")))
    .Times(1);

  pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), uT("2006192006") },
      { uT("type"), uT("State") },
    }));

  EXPECT_CALL(oProxy, CreateShader(uT("2006192007")))
    .Times(1);

  pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), uT("2006192007") },
      { uT("type"), uT("Shader") },
    }));
}

// ************************************************************************** //
TEST_F(GameScene_test, /*DISABLED_*/Test_Update)
{
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(CreateCamera, void(String_t));
    MOCK_METHOD1(CreateUpdater, void(String_t));
    MOCK_METHOD1(Render, void(String_t));
  };

  Proxy oProxy;

  const auto CameraCreator = [&](const ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    oProxy.CreateCamera(_pComponent->Id);

    return [&, _pComponent]() { oProxy.Render(_pComponent->Id); };
  };

  const auto UpdaterCreator = [&](const ComponentPtr_t & _pComponent) -> Renders_t::Render_t
  {
    oProxy.CreateUpdater(_pComponent->Id);

    return [&, _pComponent]() { oProxy.Render(_pComponent->Id); };
  };

  const auto pRenders = ::std::make_shared<Renders_t>(Renders_t::Creators_t
    {
      { uT("Camera"), CameraCreator },
      { uT("Updater"), UpdaterCreator },
    });

  Produce3DObject oProduce3DObject;

  Tested_t Example{ pRenders, oProduce3DObject };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(oProxy, CreateUpdater(uT("2006192058")))
    .Times(1);

  EXPECT_CALL(oProduce3DObject, DoAdd(0))
    .Times(1);

  EXPECT_CALL(oProxy, Render(_))
    .Times(0);

  Example.CreateObject(
    {
      Component_t::Make(
        {
          { uT("id"), uT("2006192058") },
          { uT("type"), uT("Updater") },
        }),
    });

  EXPECT_CALL(oProxy, CreateCamera(uT("2006192059")))
    .Times(1);

  EXPECT_CALL(oProxy, CreateUpdater(uT("2006192100")))
    .Times(1);

  EXPECT_CALL(oProduce3DObject, DoAdd(1))
    .Times(1);

  EXPECT_CALL(oProxy, Render(uT("2006192059")))
    .Times(1);

  EXPECT_CALL(oProxy, Render(_))
    .Times(0);

  Example.CreateObject(
    {
      Component_t::Make(
        {
          { uT("id"), uT("2006192059") },
          { uT("type"), uT("Camera") },
        }),
      Component_t::Make(
        {
          { uT("id"), uT("2006192100") },
          { uT("type"), uT("Updater") },
        }),
    });

  EXPECT_CALL(oProxy, Render(uT("2006192058")))
    .Times(1);

  EXPECT_CALL(oProxy, Render(uT("2006192100")))
    .Times(1);

  Example.Update();

  EXPECT_CALL(oProduce3DObject, DoRemove(0))
    .Times(1);

  Example.RemoveObject(0);

  EXPECT_CALL(oProxy, CreateUpdater(uT("2006192058")))
    .Times(1);

  pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), uT("2006192058") },
      { uT("type"), uT("Updater") },
    }));

  EXPECT_CALL(oProxy, Render(uT("2006192100")))
    .Times(1);

  Example.Update();

  EXPECT_CALL(oProduce3DObject, DoRemove(1))
    .Times(1);

  Example.RemoveObject(1);

  EXPECT_CALL(oProxy, CreateCamera(uT("2006192059")))
    .Times(1);

  pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), uT("2006192059") },
      { uT("type"), uT("Camera") },
    }));

  EXPECT_CALL(oProxy, CreateUpdater(uT("2006192100")))
    .Times(1);

  pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), uT("2006192100") },
      { uT("type"), uT("Updater") },
    }));
}

ALICORN_RESTORE_WARNINGS
