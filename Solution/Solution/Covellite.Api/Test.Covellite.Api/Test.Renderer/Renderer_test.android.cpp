
#include "stdafx.h"
#include <unordered_map>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Renderer.
*/

#define Renderer Renderer_android
#define Renderer_deprecated Renderer_deprecated_android
#define RendererImpl RendererImpl_android
#define IGraphicApi IGraphicApi_android

// Расположение класса Renderer
#include "../../Covellite.Api/Renderer/Renderer.cpp"
#include "../../Covellite.Api/Renderer/Renderer.android.cpp"

#include <Covellite.Gui/Covellite.Gui/Renderer.cpp>

#include "../Mock/Equal.hpp"
#include "../Mock/RendererImpl.hpp"

namespace mock
{

namespace android
{

DEFINE_RENDER_IMPL(Vulkan);
DEFINE_RENDER_IMPL(OpenGLES);

} // namespace android

} // namespace mock

// Общий тестовый класс класса Renderer
class Renderer_android_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::Renderer;
  using Data_t = ::covellite::api::renderer::Renderer::Data;
  using String_t = ::alicorn::extension::std::String;
  using Renders_t = ::covellite::api::Component::Renders;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<String_t>::Set(uT("DefaultString"));
    ::testing::DefaultValue<const Renders_t::Creators_t &>::Set(m_EmptyCreators);
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<String_t>::Clear();
    ::testing::DefaultValue<const Renders_t::Creators_t &>::Clear();
  }

private:
  Renders_t::Creators_t m_EmptyCreators;
};

// Образец макроса для подстановки в класс Renderer 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Renderer_android_test, Test_Function);


// ************************************************************************** //
TEST_F(Renderer_android_test, /*DISABLED_*/Test_Constructor_Auto_Vulkan)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1809031257;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("Vulkan")))
    .Times(1);

  Tested_t Example{ uT("Auto"), Data_t{} };
}

// ************************************************************************** //
TEST_F(Renderer_android_test, /*DISABLED_*/Test_Constructor_Auto_OpenGLES)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1809031258;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("OpenGLES")))
    .Times(1);

  Tested_t Example{ uT("Auto"), Data_t{} };
}

// ************************************************************************** //
TEST_F(Renderer_android_test, /*DISABLED_*/Test_Constructor_Auto_Nothing)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

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

    EXPECT_CALL(RendererImplProxy, Constructor(_))
      .Times(1)
      .WillOnce(Throw(::std::exception{ Data.second }));

    EXPECT_CALL(LoggerProxy, ToLog(Eq(ExpectedToLog)))
      .Times(1);
  }

  EXPECT_THROW(Tested_t(uT("Auto"), Data_t{}), ::std::exception);
}
