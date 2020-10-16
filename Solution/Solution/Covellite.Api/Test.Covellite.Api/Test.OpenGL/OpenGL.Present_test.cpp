
#include "stdafx.h"
#include "../Mock/OpenGL.hpp"

// Примеры макросов библиотеки Google Test
#include <alicorn/google/test/example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn/google/mock/example.hpp>

/**
* \file
*  Тесты класса OpenGL.
*/

#define GraphicApi GraphicApi_OpenGL
#define OpenGLCommon OpenGLCommon_OpenGL
#define OpenGLCommonShader OpenGLCommonShader_OpenGL

// Расположение класса OpenGL
#include "../../Covellite.Api/Renderer/OpenGL.cpp"
#include "../../Covellite.Api/Renderer/GraphicApi.cpp"
#include "../../Covellite.Api/Renderer/GraphicApi.CapturingServiceComponent.cpp"
#include "../../Covellite.Api/Renderer/OpenGLCommon.cpp"
#include "../../Covellite.Api/Renderer/OpenGLCommon.Texture.cpp"
#include "../../Covellite.Api/Renderer/OpenGLCommonShader.cpp"

// Общий тестовый класс класса OpenGL
class OpenGL_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::api::renderer::OpenGL;
  using ITested_t = ::covellite::api::renderer::IGraphicApi;
  using Component_t = ::covellite::api::Component;
  using Object_t = ::std::vector<Component_t::ComponentPtr_t>;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<int>::Set(1710282125);
    ::testing::DefaultValue<HDC>::Set((HDC)1806101721);
    ::testing::DefaultValue<HGLRC>::Set((HGLRC)1710282204);
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<int>::Clear();
    ::testing::DefaultValue<HDC>::Clear();
    ::testing::DefaultValue<HGLRC>::Clear();
  }

protected:
  class Data :
    public ::covellite::api::renderer::SettingsData
  {
  public:
    explicit Data(int _Top = 0)
    {
      Handle = (HWND)0;
      ClientRect.Top = _Top;
      IsFullScreen = false;
    }
  };

  using Data_t = Data;

  template<class T>
  void IntroduceBufferSize(::std::vector<T> & _Data)
  {
    (*reinterpret_cast<size_t *>(_Data.data())) = _Data.size() * sizeof(T);
  }

  template<class T>
  ::std::vector<uint8_t> GetExpected(const ::std::vector<T> & _Data)
  {
    ::std::vector<uint8_t> Result(sizeof(T) * _Data.size());
    memcpy(Result.data(), _Data.data(), Result.size());
    return Result;
  }
};

// Образец макроса для подстановки в класс OpenGL 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(OpenGL_test, Test_Function);

#define OpenGLShader_test OpenGL_test
#include "../OpenGLShader.Present_test.hpp"
