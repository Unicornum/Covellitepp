
#include "stdafx.h"
#include <glm/glm.force.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Fx.
*/

// Общий тестовый класс класса Fx
class FxDefaultVertex_test :
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
// FRIEND_TEST(FxDefaultVertex_test, Test_Function);

#define lowp
#define mediump

using float2 = ::glm::vec2;
using float3 = ::glm::vec3;
using float4 = ::glm::vec4;
using uint = uint32_t;
using color_t = ::glm::vec4;

#define mul(a, b) ((a) * (b))
#define normalize(v) ::glm::normalize(v)
#define ToMatrix3x3(m) ::glm::mat3(m)
#define max(a, b) ::std::max(a, b)
#define dot(a, b) ::glm::dot(a, b)
#define ToFloat3(value) float3(value)

#define COVELLITE_DECLARE_CONST_BUFFER_INSIDE(type, name, index) \
  namespace { type name; }
#define COVELLITE_INPUT_SEMANTIC(value)

#include "../../Covellite.Api/Renderer/Shaders/Data.h"
#include "../../Covellite.Api/Renderer/Shaders/Input.fx"

#define COVELLITE_SHADER_VERTEX
#include "../../Covellite.Api/Renderer/Shaders/Default.fx"

// ************************************************************************** //
TEST_F(FxDefaultVertex_test, /*DISABLED_*/Test_vsFlat)
{
  Vertex Input;
  Input.Position = { 0.1f, 0.2f, 0.3f, 0.4f };
  Input.TexCoord = { 0.5f, 0.6f };
  Input.Extra = { 0.7f, 0.8f, 0.9f, 0.0f };

  ObjectData.World = ::glm::mat4{ 2.0f };
  CameraData.View = ::glm::mat4{ 3.0f };
  CameraData.Projection = ::glm::mat4{ 4.0f };

  const auto ExpectedScreenPos = Input.Position * 
    (ObjectData.World * (CameraData.View * CameraData.Projection));

  const auto Result = vsFlat(Input);
  EXPECT_EQ(ExpectedScreenPos, Result.ScreenPos);
  EXPECT_EQ(Input.Extra, Result.Color);
  EXPECT_EQ(float3(0.0f, 0.0f, 1.0f), Result.Normal);
  EXPECT_EQ(Input.TexCoord, Result.TexCoord);
  EXPECT_EQ(float4(0.0f, 0.0f, 0.0f, 1.0f), Result.WorldPos);
}

// ************************************************************************** //
TEST_F(FxDefaultVertex_test, /*DISABLED_*/Test_vsVolume)
{
  Vertex Input;
  Input.Position = { 1.90f, 8.20f, 1.1f, 4.9f };
  Input.TexCoord = { 19.0820f, 11.50f };
  Input.Extra = { 1.90f, 8.20f, 1.1f, 5.1f };

  ObjectData.World = ::glm::mat4{ 19082.01152f };
  CameraData.View = ::glm::mat4{ 19082.01153f };
  CameraData.Projection = ::glm::mat4{ 19082.01154f };

  const auto ExpectedNormal = normalize(
    mul(ToFloat3(Input.Extra), ToMatrix3x3(ObjectData.World)));
  float4 ExpectedWorldPos =
    mul(Input.Position, ObjectData.World);
  const auto ExpectedScreenPos = ExpectedWorldPos *
    (CameraData.View * CameraData.Projection);

  const auto Result = vsVolume(Input);
  EXPECT_EQ(ExpectedScreenPos, Result.ScreenPos);
  //EXPECT_EQ(GetAmbientDirectionColor(ExpectedNormal), Result.Color);
  EXPECT_EQ(ExpectedNormal, Result.Normal);
  EXPECT_EQ(Input.TexCoord, Result.TexCoord);
  EXPECT_EQ(ExpectedWorldPos, Result.WorldPos);
}
