
#include "stdafx.h"
#include <alicorn/std/vector.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Renders.
*/

// Расположение класса Renders
#include <Covellite/Api/Renders.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Vertex.hpp>

// Общий тестовый класс класса Renders
class Renders_test :
  public ::testing::Test
{
protected:
  using Component_t = ::covellite::api::Component;
  using Tested_t = ::covellite::api::Component::Renders;
  using String_t = ::alicorn::extension::std::String;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }

protected:
  class TestCaller
  {
  public:
    MOCK_METHOD1(Creator, void(Component_t::Id_t));
    MOCK_METHOD1(Render, void(Component_t::Id_t));
  };

  bool IsExistsAnyTypeParameter(const Component_t & _Component, const String_t & _Name)
  {
    try
    {
      _Component[_Name];
      return true;
    }
    catch (const ::std::exception &) {}

    return false;
  };
};

// Образец макроса для подстановки в класс Renders 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Renders_test, Test_Function);

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_FALSE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_UnknownType)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  Tested_t Example{ {} };

  const auto LogMessage = uT("Warning: ") +
    uT("Creating a component of an unknown type: Position [id: Id2006111517].");

  using namespace ::testing;

  EXPECT_CALL(LoggerProxy, ToLog(LogMessage))
    .Times(1);

  const auto Result = Example.Obtain(Component_t::Make(
    { 
      { uT("type"), uT("Position") },
      { uT("id"), uT("Id2006111517") },
   }));
  EXPECT_NO_THROW(Result());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateEmptyRender)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  const auto Creator = [&](const Component_t::ComponentPtr_t &) -> Tested_t::Render_t
  {
    return nullptr;
  };

  Tested_t Example{
  {
    { uT("Texture"), Creator },
  } };

  const auto LogMessage = uT("Warning: ") +
    uT("Received an empty render: [id: Id2006111532, type: Texture].");

  using namespace ::testing;

  EXPECT_CALL(LoggerProxy, ToLog(LogMessage))
    .Times(1);

  const auto Result = Example.Obtain(Component_t::Make(
    {
      { uT("type"), uT("Texture") },
      { uT("id"), uT("Id2006111532") },
    }));
  EXPECT_NO_THROW(Result());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateException)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  const auto Creator = [&](const Component_t::ComponentPtr_t &) -> Tested_t::Render_t
  {
    throw ::std::exception{ "Exception2006111545" };
  };

  Tested_t Example{
  {
    { uT("Shader"), Creator },
  } };

  const auto LogMessage = uT("Error: ") +
    uT("Create render fail: Exception2006111545 [id: Id2006111546, type: Shader].");

  using namespace ::testing;

  EXPECT_CALL(LoggerProxy, ToLog(LogMessage))
    .Times(1);

  const auto Result = Example.Obtain(Component_t::Make(
    {
      { uT("type"), uT("Shader") },
      { uT("id"), uT("Id2006111546") },
   }));
  EXPECT_NO_THROW(Result());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateNewRender)
{
  TestCaller Caller;

  auto TextureCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(uT("Texture"));

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  auto MeshCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(uT("Mesh"));

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{
    {
      { uT("Texture"), TextureCreator },
      { uT("Mesh"), MeshCreator },
    } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(1);

  auto Result = Example.Obtain(Component_t::Make(
    {
      { uT("type"), uT("Texture") },
      { uT("id"), uT("TextureId") },
    }));

  EXPECT_CALL(Caller, Render(uT("TextureId")))
    .Times(1);

  Result();

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  Result = Example.Obtain(Component_t::Make(
    {
      { uT("type"), uT("Mesh") },
      { uT("id"), uT("MeshId") },
    }));

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  Result();
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_ExistsRender)
{
  TestCaller Caller;

  auto Creator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(_pComponent->Type);

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{
    {
      { uT("Texture"), Creator },
      { uT("Mesh"), Creator },
    } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(1);

  auto Result = Example.Obtain(Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }));

  EXPECT_CALL(Caller, Render(uT("TextureId")))
    .Times(1);

  Result();

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(0);

  Result = Example.Obtain(Component_t::Make({
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
        }));

  EXPECT_CALL(Caller, Render(uT("TextureId")))
    .Times(1);

  Result();

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  Result = Example.Obtain(Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }));

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  Result();

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(0);

  Result = Example.Obtain(Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }));

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  Result();

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(0);

  Result = Example.Obtain(Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }));

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  Result();
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Remove)
{
  TestCaller Caller;

  auto Creator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(_pComponent->Type);

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{
    {
      { uT("Texture"), Creator },
      { uT("Mesh"), Creator },
    } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(1);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
    }));

  EXPECT_CALL(Caller, Creator(_))
    .Times(0);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  Example.Remove(Component_t::Make(
    {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
    }));

  Example.Remove(Component_t::Make(
    {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
    }));

  EXPECT_CALL(Caller, Creator(_))
    .Times(0);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  Example.Remove(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  Example.Remove(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  Example.Remove( // Лишний раз - умышленно
      Component_t::Make(
        {
          { uT("type"), uT("Texture") },
          { uT("id"), uT("TextureId") },
        }));

  EXPECT_CALL(Caller, Creator(uT("Texture")))
    .Times(1);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId") },
    }));

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
    }));
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_ComplexUsing)
{
  TestCaller Caller;

  auto Creator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(_pComponent->Id);

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{
    {
      { uT("Texture"), Creator },
      { uT("Mesh"), Creator },
    } };

  Tested_t::Renders_t Result;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("TextureId1")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("TextureId2")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("MeshId")))
    .Times(1);

  using namespace ::alicorn::extension::std;

  Result += Tested_t::Renders_t
  {
    Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Shader") },
        { uT("id"), uT("ShaderId") },
    })),
    Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId1") },
    })),
    Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId2") },
    })),
    Example.Obtain(Component_t::Make(
    {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
    }))
  };

  EXPECT_CALL(Caller, Creator(uT("TextureId3")))
    .Times(1);

  Result += Tested_t::Renders_t
  {
    Example.Obtain(Component_t::Make(
      {
        { uT("type"), uT("Shader") },
        { uT("id"), uT("ShaderId") },
      })),
    Example.Obtain(Component_t::Make(
      {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId2") },
      })),
    Example.Obtain(Component_t::Make(
      {
        { uT("type"), uT("Texture") },
        { uT("id"), uT("TextureId3") },
      })),
    Example.Obtain(Component_t::Make(
      {
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
      })),
  };

  EXPECT_CALL(Caller, Render(uT("TextureId1")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("TextureId2")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("TextureId2")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("TextureId3")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  for (auto & Render : Result) Render();
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_UnknownType_deprecated)
{
  Tested_t Example{ {} };

  const auto Result = Example.Obtain(
    {
      Component_t::Make({ { uT("type"), uT("Position") } }),
      Component_t::Make({ { uT("type"), uT("FakeTexture") } }),
      Component_t::Make({ { uT("type"), uT("Mesh") } }),
    });
  EXPECT_TRUE(Result.empty());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateNullptr_deprecated)
{
  TestCaller Caller;

  auto Creator =
    [&](const Component_t::ComponentPtr_t &) -> Tested_t::Render_t
  {
    Caller.Creator(uT("FakeTexture"));

    return nullptr;
  };

  Tested_t Example{
  {
    { uT("FakeTexture"), Creator },
  } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("FakeTexture")))
    .Times(2);

  const auto Result = Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
    });
  EXPECT_TRUE(Result.empty());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateException_deprecated)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  const auto LogMessage = uT("Error: ") +
    uT("Create render fail: Exception1812151850 [id: TextureId, type: FakeTexture].");

  TestCaller Caller;

  auto Creator = [&](const Component_t::ComponentPtr_t &) -> Tested_t::Render_t
  {
    Caller.Creator(uT("FakeTexture"));

    throw ::std::exception{ "Exception1812151850" };
  };

  Tested_t Example{
  {
    { uT("FakeTexture"), Creator },
  } };

  using namespace ::testing;

  InSequence Dummy;

  for (size_t i = 0; i < 2; i++)
  {
    EXPECT_CALL(Caller, Creator(uT("FakeTexture")))
      .Times(1);

    EXPECT_CALL(LoggerProxy, ToLog(LogMessage))
      .Times(1);
  }

  const auto Result = Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
    });
  EXPECT_TRUE(Result.empty());
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_CreateNewRenders_deprecated)
{
  TestCaller Caller;

  auto TextureCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(uT("FakeTexture"));

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  auto MeshCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(uT("Mesh"));

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{ 
    {
      { uT("FakeTexture"), TextureCreator },
      { uT("Mesh"), MeshCreator },
    } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("FakeTexture")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  const auto Result = Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

  EXPECT_CALL(Caller, Render(uT("TextureId")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  for (auto & Render : Result) Render();
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_ExistsRenders_deprecated)
{
  TestCaller Caller;

  auto Creator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(_pComponent->Type);

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{
    {
      { uT("FakeTexture"), Creator },
      { uT("Mesh"), Creator },
    } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("FakeTexture")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  const auto Result = Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

  EXPECT_CALL(Caller, Render(uT("TextureId")))
    .Times(2);

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(3);

  for (auto & Render : Result) Render();
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_EmptyRenders_deprecated)
{
  TestCaller Caller;

  auto TextureCreator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(_pComponent->Type);

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  auto MeshCreator =
    [&](const Component_t::ComponentPtr_t &) -> Tested_t::Render_t
  {
    return nullptr;
  };

  Tested_t Example{
    {
      { uT("FakeTexture"), TextureCreator },
      { uT("Mesh"), MeshCreator },
    } };

  using namespace ::testing;

  // Первый вызов - создание.
  {
    InSequence Dummy;

    EXPECT_CALL(Caller, Creator(uT("FakeTexture")))
      .Times(1);

    EXPECT_CALL(Caller, Creator(_))
      .Times(0);

    const auto Result = Example.Obtain(
      {
        Component_t::Make({
          { uT("type"), uT("FakeTexture") },
          { uT("id"), uT("TextureId") },
          }),
        Component_t::Make({
          { uT("type"), uT("FakeTexture") },
          { uT("id"), uT("TextureId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
      });
    EXPECT_EQ(2, Result.size());

    EXPECT_CALL(Caller, Render(uT("TextureId")))
      .Times(2);

    EXPECT_CALL(Caller, Render(_))
      .Times(0);

    for (auto & Render : Result) Render();
  }

  // Второй вызов - получение существующих.
  {
    InSequence Dummy;

    EXPECT_CALL(Caller, Creator(_))
      .Times(0);

    const auto Result = Example.Obtain(
      {
        Component_t::Make({
          { uT("type"), uT("FakeTexture") },
          { uT("id"), uT("TextureId") },
          }),
        Component_t::Make({
          { uT("type"), uT("FakeTexture") },
          { uT("id"), uT("TextureId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
        Component_t::Make({
          { uT("type"), uT("Mesh") },
          { uT("id"), uT("MeshId") },
          }),
      });
    EXPECT_EQ(2, Result.size());

    EXPECT_CALL(Caller, Render(uT("TextureId")))
      .Times(2);

    EXPECT_CALL(Caller, Render(_))
      .Times(0);

    for (auto & Render : Result) Render();
  }
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_BuildService_deprecated)
{
  using Service_t = ::std::vector<Component_t::ComponentPtr_t>;

  Tested_t Example{ {} };

  const auto pData1 = Component_t::Make({
    { uT("type"), uT("Data") },
    });
  const auto pData2 = Component_t::Make({
    { uT("type"), uT("Data") },
    });
  const auto pData3 = Component_t::Make({
    { uT("type"), uT("Data") },
    });
  const auto pData4 = Component_t::Make({
    { uT("type"), uT("Data") },
    });
  const auto pMainComponent1 = Component_t::Make({
    { uT("type"), uT("Unknown") },
    });
  const auto pMainComponent2 = Component_t::Make({
    { uT("type"), uT("Unknown") },
    });
  const auto pMainComponent3 = Component_t::Make({
    { uT("type"), uT("Unknown") },
    });

  const Service_t ExpectedService1 =
  {
    pData1,
  };

  const Service_t ExpectedService2 =
  {
    pData2,
    pData3,
    pData4,
  };

  Example.Obtain(
    {
      pData1,
      pMainComponent1,
      pData2,
      pData3,
      pData4,
      pMainComponent2,
      pMainComponent3
    });

  const Service_t Result1 = (*pMainComponent1)[uT("service")];
  EXPECT_EQ(ExpectedService1, Result1);

  const Service_t Result2 = (*pMainComponent2)[uT("service")];
  EXPECT_EQ(ExpectedService2, Result2);

  EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent3, uT("service")));
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_TextureContent_deprecated)
{
  using Data_t = ::std::vector<uint8_t>;

  const Data_t Expected =
  {
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
  };

  Tested_t Example{ {} };

  {
    const auto pData = Component_t::Make({
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Texture") },
      { uT("data"), Expected.data() },
      { uT("width"), 3 },
      { uT("height"), 5 },
      });
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Texture") },
      });

    Example.Obtain(
      {
        pData,
        pMainComponent,
      });

    const Data_t Result = (*pData)[uT("content")];
    EXPECT_EQ(Expected, Result);
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
  }

  {
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Texture") },
      { uT("data"), Expected.data() },
      { uT("width"), 3 },
      { uT("height"), 5 },
      });

    Example.Obtain(Tested_t::Object_t
      {
        pMainComponent,
      });

    const Data_t Result = (*pMainComponent)[uT("content")];
    EXPECT_EQ(Expected, Result);
  }

  {
    const auto pData = Component_t::Make({
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Texture") },
      });
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Texture") },
      });

    Example.Obtain(
      {
        pData,
        pMainComponent,
      });

    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pData, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pData, uT("count")));
  }

  {
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Texture") },
      });

    Example.Obtain(Tested_t::Object_t
      {
        pMainComponent,
      });

    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
  }
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_ShaderContent_deprecated)
{
  using Data_t = ::std::vector<uint8_t>;

  const Data_t Expected =
  {
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
    0x00, 0x01, 0x02, 0x03,   0x04, 0x05, 0x06, 0x07,   0x08, 0x09, 0x10, 0x11,
  };

  Tested_t Example{ {} };

  {
    const auto pData = Component_t::Make({
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Shader") },
      { uT("data"), Expected.data() },
      { uT("count"), Expected.size() },
      });
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Shader") },
      });

    Example.Obtain(
      {
        pData,
        pMainComponent,
      });

    const Data_t Result = (*pData)[uT("content")];
    EXPECT_EQ(Expected, Result);
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
  }

  {
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Shader") },
      { uT("data"), Expected.data() },
      { uT("count"), Expected.size() },
      });

    Example.Obtain(Tested_t::Object_t
      {
        pMainComponent,
      });

    const Data_t Result = (*pMainComponent)[uT("content")];
    EXPECT_EQ(Expected, Result);
  }

  {
    const auto pData = Component_t::Make({
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Shader") },
      });
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Shader") },
      });

    Example.Obtain(
      {
        pData,
        pMainComponent,
      });

    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pData, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pData, uT("count")));
  }

  {
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Shader") },
      });

    Example.Obtain(Tested_t::Object_t
      {
        pMainComponent,
      });

    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
  }
}

namespace covellite
{

namespace api
{

inline bool operator==(
  const ::covellite::api::Vertex & _Left,
  const ::covellite::api::Vertex & _Right)
{
  if (_Left.px != _Right.px) return false;
  if (_Left.py != _Right.py) return false;
  if (_Left.pz != _Right.pz) return false;
  if (_Left.pw != _Right.pw) return false;
  if (_Left.tu != _Right.tu) return false;
  if (_Left.tv != _Right.tv) return false;
  if (_Left.ex != _Right.ex) return false;
  if (_Left.ey != _Right.ey) return false;
  if (_Left.ez != _Right.ez) return false;
  if (_Left.ew != _Right.ew) return false;
  return true;
}

} // namespace api

} // namespace covellite

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_VertexBufferContent_deprecated)
{
  using Data_t = ::std::vector<::covellite::api::Vertex>;

  const Data_t Expected =
  {
    { 0.0f, 0.1f, 0.2f, 0.3f,   0.4f, 0.5f,   0.6f, 0.7f, 0.8f, 0.9f },
    { 0.0f, 0.1f, 0.2f, 0.3f,   0.4f, 0.5f,   0.6f, 0.7f, 0.8f, 0.9f },
    { 0.0f, 0.1f, 0.2f, 0.3f,   0.4f, 0.5f,   0.6f, 0.7f, 0.8f, 0.9f },
    { 0.0f, 0.1f, 0.2f, 0.3f,   0.4f, 0.5f,   0.6f, 0.7f, 0.8f, 0.9f },
    { 0.0f, 0.1f, 0.2f, 0.3f,   0.4f, 0.5f,   0.6f, 0.7f, 0.8f, 0.9f },
    { 0.0f, 0.1f, 0.2f, 0.3f,   0.4f, 0.5f,   0.6f, 0.7f, 0.8f, 0.9f },
  };

  Tested_t Example{ {} };

  {
    const auto pData = Component_t::Make({
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Buffer") },
      { uT("data"), Expected.data() },
      { uT("count"), Expected.size() },
      });
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Buffer") },
      });

    Example.Obtain(
      {
        pData,
        pMainComponent,
      });

    const Data_t Result = (*pData)[uT("content")];
    EXPECT_EQ(Expected, Result);
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
  }

  {
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Buffer") },
      { uT("data"), Expected.data() },
      { uT("count"), Expected.size() },
      });

    Example.Obtain(Tested_t::Object_t
      {
        pMainComponent,
      });

    const Data_t Result = (*pMainComponent)[uT("content")];
    EXPECT_EQ(Expected, Result);
  }

  {
    const auto pData = Component_t::Make({
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Buffer") },
      });
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Buffer") },
      });

    Example.Obtain(
      {
        pData,
        pMainComponent,
      });

    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pData, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pData, uT("count")));
  }

  {
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Buffer") },
      });

    Example.Obtain(Tested_t::Object_t
      {
        pMainComponent,
      });

    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
  }
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Obtain_IndexBufferContent_deprecated)
{
  using Data_t = ::std::vector<int>;

  const Data_t Expected =
  {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
  };

  Tested_t Example{ {} };

  {
    const auto pData = Component_t::Make({
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Buffer") },
      { uT("data"), Expected.data() },
      { uT("count"), Expected.size() },
      });
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Present") },
      });

    Example.Obtain(
      {
        pData,
        pMainComponent,
      });

    const Data_t Result = (*pData)[uT("content")];
    EXPECT_EQ(Expected, Result);
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
  }

  {
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Present") },
      { uT("data"), Expected.data() },
      { uT("count"), Expected.size() },
      });

    Example.Obtain(Tested_t::Object_t
      {
        pMainComponent,
      });

    const Data_t Result = (*pMainComponent)[uT("content")];
    EXPECT_EQ(Expected, Result);
  }

  {
    const auto pData = Component_t::Make({
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Buffer") },
      });
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Present") },
      });

    Example.Obtain(
      {
        pData,
        pMainComponent,
      });

    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pData, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pData, uT("count")));
  }

  {
    const auto pMainComponent = Component_t::Make({
      { uT("type"), uT("Present") },
      });

    Example.Obtain(Tested_t::Object_t
      {
        pMainComponent,
      });

    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("content")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("data")));
    EXPECT_FALSE(IsExistsAnyTypeParameter(*pMainComponent, uT("count")));
  }
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_Remove_deprecated)
{
  TestCaller Caller;

  auto Creator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(_pComponent->Type);

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{
    {
      { uT("FakeTexture"), Creator },
      { uT("Mesh"), Creator },
    } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("FakeTexture")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

  EXPECT_CALL(Caller, Creator(_))
    .Times(0);

  Example.Obtain(Tested_t::Object_t
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
    });

  Example.Remove(
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

  EXPECT_CALL(Caller, Creator(_))
    .Times(0);

  Example.Obtain(Tested_t::Object_t
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
    });

  Example.Remove(Tested_t::Object_t
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
    });

  Example.Remove(Tested_t::Object_t
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
    });

  Example.Remove(Tested_t::Object_t // Лишний раз - умышленно
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
    });

  EXPECT_CALL(Caller, Creator(uT("FakeTexture")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("Mesh")))
    .Times(1);

  Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });
}

// ************************************************************************** //
TEST_F(Renders_test, /*DISABLED_*/Test_ComplexUsing_deprecated)
{
  TestCaller Caller;

  auto Creator =
    [&](const Component_t::ComponentPtr_t & _pComponent) -> Tested_t::Render_t
  {
    Caller.Creator(_pComponent->Id);

    return [&, _pComponent]() { Caller.Render(_pComponent->Id); };
  };

  Tested_t Example{
    {
      { uT("FakeTexture"), Creator },
      { uT("Mesh"), Creator },
    } };

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(Caller, Creator(uT("TextureId1")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("TextureId2")))
    .Times(1);

  EXPECT_CALL(Caller, Creator(uT("MeshId")))
    .Times(1);

  auto Result = Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("FakeShader") },
        { uT("id"), uT("ShaderId") },
        }),
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId1") },
        }),
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId2") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

  EXPECT_CALL(Caller, Creator(uT("TextureId3")))
    .Times(1);

  using namespace ::alicorn::extension::std;

  Result += Example.Obtain(
    {
      Component_t::Make({
        { uT("type"), uT("FakeShader") },
        { uT("id"), uT("ShaderId") },
        }),
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId2") },
        }),
      Component_t::Make({
        { uT("type"), uT("FakeTexture") },
        { uT("id"), uT("TextureId3") },
        }),
      Component_t::Make({
        { uT("type"), uT("Mesh") },
        { uT("id"), uT("MeshId") },
        }),
    });

  EXPECT_CALL(Caller, Render(uT("TextureId1")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("TextureId2")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("TextureId2")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("TextureId3")))
    .Times(1);

  EXPECT_CALL(Caller, Render(uT("MeshId")))
    .Times(1);

  for (auto & Render : Result) Render();
}
