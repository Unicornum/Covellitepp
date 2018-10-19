
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

#define Renderer Renderer_windows
#define RendererImpl RendererImpl_windows
#define IGraphicApi IGraphicApi_windows

#include "../Mock/Eq.hpp"
#include "../Mock/RendererImpl.hpp"

namespace mock
{

namespace windows
{

DEFINE_RENDER_IMPL(DirectX12);
DEFINE_RENDER_IMPL(DirectX11);
DEFINE_RENDER_IMPL(Vulkan);
DEFINE_RENDER_IMPL(OpenGL);

} // namespace windows

} // namespace mock

// Расположение класса Renderer
#include "../../Covellite.Api/Renderer/Renderer.cpp"
#include "../../Covellite.Api/Renderer/Renderer.windows.cpp"

// Общий тестовый класс класса Renderer
class Renderer_windows_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::Renderer;
  using Data_t = ::covellite::api::renderer::IRenderer::Data;
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

// Образец макроса для подстановки в класс Renderer 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Renderer_windows_test, Test_Function);

// ************************************************************************** //
TEST_F(Renderer_windows_test, /*DISABLED_*/Test_Constructor_Auto_DirectX12)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1809031219;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("DirectX12")))
    .Times(1);

  Tested_t Example{ uT("Auto"), Data_t{} };
}

// ************************************************************************** //
TEST_F(Renderer_windows_test, /*DISABLED_*/Test_Constructor_Auto_DirectX11)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1809031242;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("DirectX11")))
    .Times(1);

  Tested_t Example{ uT("Auto"), Data_t{} };
}

// ************************************************************************** //
TEST_F(Renderer_windows_test, /*DISABLED_*/Test_Constructor_Auto_Vulkan)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1809031244;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("Vulkan")))
    .Times(1);

  Tested_t Example{ uT("Auto"), Data_t{} };
}

// ************************************************************************** //
TEST_F(Renderer_windows_test, /*DISABLED_*/Test_Constructor_Auto_OpenGL)
{
  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::mock::Id_t Id = 1809031245;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(RendererImplProxy, Constructor(_))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Throw(::std::exception{}))
    .WillOnce(Return(Id));

  EXPECT_CALL(RendererImplProxy, SetClassName(Id, uT("OpenGL")))
    .Times(1);

  Tested_t Example{ uT("Auto"), Data_t{} };
}

// ************************************************************************** //
TEST_F(Renderer_windows_test, /*DISABLED_*/Test_Constructor_Auto_Nothing)
{
  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

  using RendererImplProxy_t = ::mock::RendererImpl::Proxy;
  RendererImplProxy_t RendererImplProxy;
  RendererImplProxy_t::GetInstance() = &RendererImplProxy;

  const ::std::unordered_map<String_t, const char *> Datas =
  {
    { uT("DirectX12"), "Error1809031406" },
    { uT("DirectX11"), "Error1809031412" },
    { uT("Vulkan"), "Error1809031413" },
    { uT("OpenGL"), "Error1809031414" }
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
