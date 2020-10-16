
#pragma once

// ************************************************************************** //
TEST_F(Common_test, /*DISABLED_*/Test_StructSizeAlign16Bytes)
{
  EXPECT_EQ(0, sizeof(::Camera) % 16);
  EXPECT_EQ(0, sizeof(::Object) % 16);
}

// ************************************************************************** //
TEST_F(Common_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(Common_test, /*DISABLED_*/Test_RegisterIntoFactory)
{
  using namespace ::alicorn::modules::patterns;

  const Data_t oData;
  const ::covellite::api::renderer::SettingsData & Data = oData;

  auto pExample = factory::make_unique<ITested_t>(ImplClassName, Data);
  EXPECT_NO_THROW(dynamic_cast<Tested_t &>(*pExample));
}

// ************************************************************************** //
TEST_F(Common_test, /*DISABLED_*/Test_State_AlphaTest)
{
  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  {
    const auto pState = Component_t::Make(
      {
        { uT("kind"), uT("AlphaTest") },
      });

    EXPECT_EQ(nullptr, itCreator->second(pState));
  }

  {
    const auto Value = 1904271913.0f;

    const auto pState = Component_t::Make(
      {
        { uT("kind"), uT("AlphaTest") },
        { uT("discard"), Value },
      });

    EXPECT_EQ(nullptr, itCreator->second(pState));
  }
}

// ************************************************************************** //
TEST_F(Common_test, /*DISABLED_*/Test_Transform_UnknownKind)
{
  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Unknow1908061947") },
    });

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(Common_test, /*DISABLED_*/Test_Buffer_Constant_User_EmptyMapper)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  // Тип буфера выводится из переданного mapper'a
  //EXPECT_THROW(itCreator->second(Component_t::Make({ })), ::std::exception);

  const auto pComponent1 = Component_t::Make(
    {
      { uT("mapper"), BufferMapper_t{ } },
    });

  EXPECT_THROW(itCreator->second(pComponent1), ::std::exception);

  const auto pComponent2 = Component_t::Make(
    {
      { uT("mapper"), BufferMapper_t{ nullptr } },
    });

  EXPECT_THROW(itCreator->second(pComponent2), ::std::exception);
}

// ************************************************************************** //
TEST_F(Common_test, /*DISABLED_*/Test_Buffer_Constant_User_EmptySize)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("mapper"), BufferMapper_t{ [](void *) { return false; } } },
    });

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}
