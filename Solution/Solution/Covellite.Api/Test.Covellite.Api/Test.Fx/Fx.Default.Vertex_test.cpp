
#include "stdafx.h"
#include <GLMath.hpp>

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

// ************************************************************************** //
TEST_F(FxDefaultVertex_test, /*DISABLED_*/Test_Default_Deprecated)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"DefaultDeprecated.expected.fx",
    THIS_DIRECTORY / L"../../Covellite.Api/Renderer/Shaders/DefaultDeprecated.fx");
}

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
TEST_F(FxDefaultVertex_test, /*DISABLED_*/Test_GetAmbientDirectionColor)
{
  ObjectData.Lights.Ambient.IsValid = 0;
  ObjectData.Lights.Direction.IsValid = 0;

  auto Result = GetAmbientDirectionColor(float3(1.0f, 2.0f, 3.0f));
  EXPECT_EQ(float4(0.0f, 0.0f, 0.0f, 0.0f), Result);

  ObjectData.Lights.Ambient.IsValid = 1;
  ObjectData.Lights.Ambient.Color = { 1.91f, 0.13f, 0.20f, 0.13f };

  Result = GetAmbientDirectionColor(float3(1.0f, 2.0f, 3.0f));
  EXPECT_EQ(ObjectData.Lights.Ambient.Color, Result);

  ObjectData.Lights.Direction.IsValid = 1;
  ObjectData.Lights.Direction.Color = { 19.1f, 0.1f, 3.20f, 18.0f };
  ObjectData.Lights.Direction.Direction = { -1.0f, -1.0f, -1.0f, 0.0f };
  Result = GetAmbientDirectionColor(float3(1.0f, 1.0f, 1.0f));
  EXPECT_EQ(ObjectData.Lights.Ambient.Color, Result);

  ObjectData.Lights.Direction.Direction = { 0.0f, 1.0f, 1.0f, 0.0f };
  Result = GetAmbientDirectionColor(float3(0.0f, 0.0f, 1.0f));

  const auto ExpectedColor = ObjectData.Lights.Ambient.Color +
    ObjectData.Lights.Direction.Color * 0.7071067811865f;

  Result = GetAmbientDirectionColor(float3(0.0f, 0.0f, 1.0f));
  EXPECT_EQ(ExpectedColor, Result);
}

// ************************************************************************** //
TEST_F(FxDefaultVertex_test, /*DISABLED_*/Test_vsVolume)
{
  ObjectData.Lights.Ambient.IsValid = 1;
  ObjectData.Lights.Direction.IsValid = 1;

  Vertex Input;
  Input.Position = { 1.90f, 8.20f, 1.1f, 4.9f };
  Input.TexCoord = { 19.0820f, 11.50f };
  Input.Extra = { 1.90f, 8.20f, 1.1f, 5.1f };

  ObjectData.World = ::glm::mat4{ 19082.01152f };
  CameraData.View = ::glm::mat4{ 19082.01153f };
  CameraData.Projection = ::glm::mat4{ 19082.01154f };

  const auto ExpectedScreenPos = Input.Position *
    (ObjectData.World * (CameraData.View * CameraData.Projection));
  const auto ExpectedNormal = normalize(
    mul(ToFloat3(Input.Extra), ToMatrix3x3(ObjectData.World)));
  float4 ExpectedWorldPos =
    mul(Input.Position, ObjectData.World);

  const auto Result = vsVolume(Input);
  EXPECT_EQ(ExpectedScreenPos, Result.ScreenPos);
  EXPECT_EQ(GetAmbientDirectionColor(ExpectedNormal), Result.Color);
  EXPECT_EQ(ExpectedNormal, Result.Normal);
  EXPECT_EQ(Input.TexCoord, Result.TexCoord);
  EXPECT_EQ(ExpectedWorldPos, Result.WorldPos);
}
