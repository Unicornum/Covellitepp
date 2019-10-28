
#pragma once

// ************************************************************************** //
TEST_F(Common_test, /*DISABLED_*/Test_StructSizeAlign16Bytes)
{
  EXPECT_EQ(0, sizeof(::Camera) % 16);
  EXPECT_EQ(0, sizeof(::Fog) % 16);
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
TEST_F(Common_test, /*DISABLED_*/Test_Material)
{
  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Material"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make({ }));
  EXPECT_EQ(nullptr, Render);
}


////////////////////////////////////////////////////////////////////////////////


// ************************************************************************** //
TEST_F(Common_test, /*DISABLED_*/Test_Light_Points_LimitCount_deprecated)
{
  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itLightCreator = IExample.GetCreators().find(uT("Light"));
  ASSERT_NE(IExample.GetCreators().end(), itLightCreator);

  auto pLight = Component_t::Make(
    {
      { uT("kind"), uT("Point") },
    });

  auto LightRender = itLightCreator->second(pLight);
  ASSERT_NE(nullptr, LightRender);

  for (size_t i = 0; i < COVELLITE_MAX_LIGHT_POINT_SCENE_COUNT; i++)
  {
    LightRender();
  }

  // Лишний источник света - не должно упасть.
  LightRender();
}

// ************************************************************************** //
TEST_F(Common_test, /*DISABLED_*/Test_StructSizeAlign16Bytes_deprecated)
{
  EXPECT_EQ(0, sizeof(::Matrices) % 16);
  EXPECT_EQ(0, sizeof(::SceneLights) % 16);
}
