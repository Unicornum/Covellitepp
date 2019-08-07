
#pragma once

// ************************************************************************** //
TEST_F(Updater_test, /*DISABLED_*/Test_Updater_NoFunction)
{
  Tested_t oExample{ Data_t{} };
  ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Updater"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make({}));
  ASSERT_NE(nullptr, Render);

  EXPECT_NO_THROW(Render());
}

// ************************************************************************** //
TEST_F(Updater_test, /*DISABLED_*/Test_Updater)
{
  using Time_t = ::std::chrono::microseconds;

  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Update, void(float));
  };

  Proxy theProxy;
  Proxy::GetInstance() = &theProxy;

  using SystemClockProxy_t = ::mock::std::chrono::system_clock::Proxy;
  SystemClockProxy_t SystemClockProxy;
  SystemClockProxy_t::GetInstance() = &SystemClockProxy;

  const auto BeginTime = 1907232116;

  const ::covellite::api::Updater_t Updater = [&](const float _Time)
  {
    theProxy.Update(_Time);
  };

  const auto pComponent = Component_t::Make(
    {
      { uT("function"), Updater },
    });

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(SystemClockProxy, Now())
    .Times(1)
    .WillOnce(Return(Time_t{ BeginTime }));

  Tested_t oExample{ Data_t{} };
  ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Updater"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(theProxy, Update(0.0f))
    .Times(1);

  Render();

  EXPECT_CALL(SystemClockProxy, Now())
    .Times(1)
    .WillOnce(Return(Time_t{ BeginTime + 1000000 }));

  IExample.PresentFrame();

  EXPECT_CALL(theProxy, Update(1.0f))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(Updater_test, /*DISABLED_*/Test_Updater_ReplaceFunction)
{
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Update, void(int));
  };

  Proxy theProxy;
  Proxy::GetInstance() = &theProxy;

  const auto pComponent = Component_t::Make({ });

  const ::covellite::api::Updater_t Updater2 = [&](const float)
  {
    theProxy.Update(2);
  };

  const ::covellite::api::Updater_t Updater1 = [&](const float)
  {
    pComponent->SetValue(uT("function"), Updater2);

    theProxy.Update(1);
  };

  const ::covellite::api::Updater_t Updater0 = [&](const float)
  {
    theProxy.Update(0);
  };

  Tested_t oExample{ Data_t{} };
  ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Updater"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(theProxy, Update(_))
    .Times(0);

  Render();

  pComponent->SetValue(uT("function"), Updater0);

  EXPECT_CALL(theProxy, Update(0))
    .Times(1);

  Render();

  pComponent->SetValue(uT("function"), Updater1);

  EXPECT_CALL(theProxy, Update(1))
    .Times(1);

  Render();

  // Новый updater устанавливается внутри предыдущего.

  EXPECT_CALL(theProxy, Update(2))
    .Times(1);

  Render();
}
