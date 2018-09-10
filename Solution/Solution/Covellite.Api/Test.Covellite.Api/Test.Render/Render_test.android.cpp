
#include "stdafx.h"
#include <unordered_map>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Render.
*/

#define Render Render_android
#define RenderImpl RenderImpl_android
#define IGraphicApi IGraphicApi_android

#include "../Mock/Eq.hpp"
#include "../Mock/RenderImpl.hpp"

namespace mock
{

namespace android
{

DEFINE_RENDER_IMPL(Vulkan);
DEFINE_RENDER_IMPL(OpenGLES);

} // namespace android

} // namespace mock

// Расположение класса Render
#include "../../Covellite.Api/Render/Render.cpp"
#include "../../Covellite.Api/Render/Render.android.cpp"

// Общий тестовый класс класса Render
class Render_android_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::render::Render;
  using Data_t = ::covellite::api::render::IRender::Data;
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

// Образец макроса для подстановки в класс Render 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Render_android_test, Test_Function);


// ************************************************************************** //
TEST_F(Render_android_test, /*DISABLED_*/Test_Constructor_Auto_Vulkan)
{
  using RenderImplProxy_t = ::mock::RenderImpl::Proxy;
  RenderImplProxy_t RenderImplProxy;
  RenderImplProxy_t::GetInstance() = &RenderImplProxy;

  const ::mock::Id_t Id = 1809031257;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RenderImplProxy, Constructor(_))
    .WillOnce(Return(Id));

  EXPECT_CALL(RenderImplProxy, SetClassName(Id, uT("Vulkan")))
    .Times(1);

  Tested_t Example{ uT("Auto"), Data_t{} };
}

// ************************************************************************** //
TEST_F(Render_android_test, /*DISABLED_*/Test_Constructor_Auto_OpenGLES)
{
  using RenderImplProxy_t = ::mock::RenderImpl::Proxy;
  RenderImplProxy_t RenderImplProxy;
  RenderImplProxy_t::GetInstance() = &RenderImplProxy;

  const ::mock::Id_t Id = 1809031258;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RenderImplProxy, Constructor(_))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Return(Id));

  EXPECT_CALL(RenderImplProxy, SetClassName(Id, uT("OpenGLES")))
    .Times(1);

  Tested_t Example{ uT("Auto"), Data_t{} };
}

// ************************************************************************** //
TEST_F(Render_android_test, /*DISABLED_*/Test_Constructor_Auto_Nothing)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  using RenderImplProxy_t = ::mock::RenderImpl::Proxy;
  RenderImplProxy_t RenderImplProxy;
  RenderImplProxy_t::GetInstance() = &RenderImplProxy;

  const ::std::unordered_map<String_t, const char *> Datas =
  {
    { uT("Vulkan"), "Error1809031434" },
    { uT("OpenGLES"), "Error1809031435" }
  };

  using namespace ::testing;

  InSequence Dummy;

  for (const auto & Data : Datas)
  {
    const auto ExpectedToLog = uT("Warning: Create graphics API ") +
      Data.first + uT(" error: ") + String_t::GetString(Data.second);

    EXPECT_CALL(RenderImplProxy, Constructor(_))
      .Times(1)
      .WillOnce(Throw(::std::exception{ Data.second }));

    EXPECT_CALL(LoggerProxy, ToLog(Eq(ExpectedToLog)))
      .Times(1);
  }

  EXPECT_THROW(Tested_t(uT("Auto"), Data_t{}), ::std::exception);
}
