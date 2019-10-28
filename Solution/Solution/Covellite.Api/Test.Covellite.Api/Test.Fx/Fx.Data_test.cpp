
#include "stdafx.h"
#include <GLMath.hpp>
#include <alicorn/cpp/to-str.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Fx.
*/

// Общий тестовый класс класса Fx
class FxData_test :
  public ::testing::Test
{
protected:
  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс Fx 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(FxData_test, Test_Function);

using float2 = ::glm::vec2;
using float3 = ::glm::vec3;
using float4 = ::glm::vec4;
using float4x4 = ::glm::mat4;
#define lowp
#define mediump

#include "../../Covellite.Api/Renderer/shaders/Data.h"

// ************************************************************************** //
TEST_F(FxData_test, /*DISABLED_*/Test_Copy)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"../../Covellite.Api/Constant.hpp",
    THIS_DIRECTORY / L"../../Covellite.Api/Renderer/Shaders/Data.h");
}

// ************************************************************************** //
TEST_F(FxData_test, /*DISABLED_*/Test_Types)
{
  EXPECT_STREQ("float4", __TO_STR(color_t));
}

// ************************************************************************** //
TEST_F(FxData_test, /*DISABLED_*/Test_ConstantBufferIndex)
{
  const ::std::vector<int> BufferIndexes =
  {
    COVELLITE_BUFFER_INDEX_CAMERA,
    COVELLITE_BUFFER_INDEX_FOG,
    COVELLITE_BUFFER_INDEX_OBJECT,
    COVELLITE_BUFFER_INDEX_USER,

    // deprecated
    COVELLITE_BUFFER_INDEX_MATRICES,
    COVELLITE_BUFFER_INDEX_LIGHTS,
  };

  for (::std::size_t i = 0; i < BufferIndexes.size(); i++)
  {
    for (::std::size_t j = 0; j < BufferIndexes.size(); j++)
    {
      if (i == j)
      {
        EXPECT_EQ(BufferIndexes[i], BufferIndexes[j]);
      }
      else
      {
        EXPECT_NE(BufferIndexes[i], BufferIndexes[j]);
      }
    }
  }
}

// ************************************************************************** //
TEST_F(FxData_test, /*DISABLED_*/Test_MaxLightCount)
{
  EXPECT_EQ(4, COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT);

  // deprecated
  EXPECT_TRUE(COVELLITE_MAX_LIGHT_POINT_SCENE_COUNT > 0);
}

// ************************************************************************** //
TEST_F(FxData_test, /*DISABLED_*/Test_Camera)
{
  EXPECT_EQ(0, sizeof(::Camera) % 16);

  ::Camera Example;
  memset(&Example, 0x00, sizeof(Example));

  EXPECT_EQ(::glm::mat4{ 0.0f }, Example.Projection);
  EXPECT_EQ(::glm::mat4{ 0.0f }, Example.View);
  EXPECT_EQ(::glm::mat4{ 0.0f }, Example.ViewInverse);
}

// ************************************************************************** //
TEST_F(FxData_test, /*DISABLED_*/Test_Fog)
{
  EXPECT_EQ(0, sizeof(::Fog) % 16);

  ::Fog Example;
  memset(&Example, 0x00, sizeof(Example));

  EXPECT_EQ(::glm::vec4{ 0.0f }, Example.Color);
  EXPECT_EQ(0.0f, Example.Near);
  EXPECT_EQ(0.0f, Example.Far);
  EXPECT_EQ(0.0f, Example.Density);
}

// ************************************************************************** //
TEST_F(FxData_test, /*DISABLED_*/Test_Object)
{
  EXPECT_EQ(0, sizeof(::Object) % 16);

  ::Object Example;
  memset(&Example, 0x00, sizeof(Example));

  EXPECT_EQ(::glm::mat4{ 0.0f }, Example.World);

  EXPECT_EQ(0, sizeof(Example.Lights.Ambient) % 16);
  EXPECT_EQ(::glm::vec4{ 0.0f }, Example.Lights.Ambient.Color);
  EXPECT_EQ(0, Example.Lights.Ambient.IsValid);

  EXPECT_EQ(0, sizeof(Example.Lights.Direction) % 16);
  EXPECT_EQ(::glm::vec4{ 0.0f }, Example.Lights.Direction.Color);
  EXPECT_EQ(::glm::vec4{ 0.0f }, Example.Lights.Direction.Direction);
  EXPECT_EQ(0, Example.Lights.Direction.IsValid);

  EXPECT_EQ(0, sizeof(Example.Lights.Points) % 16);
  EXPECT_EQ(0, Example.Lights.Points.UsedSlotCount);

  for (int i = 0; i < COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT; i++)
  {
    EXPECT_EQ(::glm::vec4{ 0.0f }, Example.Lights.Points.Lights[i].Color);
    EXPECT_EQ(::glm::vec4{ 0.0f }, Example.Lights.Points.Lights[i].Position);
    EXPECT_EQ(::glm::vec4{ 0.0f }, Example.Lights.Points.Lights[i].Attenuation);
  }
}

// ************************************************************************** //
TEST_F(FxData_test, /*DISABLED_*/Test_StructSizeAlign16Bytes_deprecated)
{
  // deprecated
  EXPECT_EQ(0, sizeof(::SceneLights) % 16);
  EXPECT_EQ(0, sizeof(::Matrices) % 16);
}
