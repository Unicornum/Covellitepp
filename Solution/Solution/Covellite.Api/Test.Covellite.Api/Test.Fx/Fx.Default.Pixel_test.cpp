
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
class FxDefaultPixel_test :
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
// FRIEND_TEST(FxDefaultPixel_test, Test_Function);

#define lowp
#define mediump

using float2 = ::glm::vec2;
using float3 = ::glm::vec3;
using float4 = ::glm::vec4;
using uint = uint32_t;
using color_t = ::glm::vec4;

#define max(a, b) ::std::max(a, b)
#define ToFloat3(value) float3(value)

#define COVELLITE_TEX2D_COLOR(tex, uv) \
  tex.GetColorFrom(uv)

class Texture
{
public:
  void SetAddition(const float4 & _Value)
  {
    m_Addition = _Value;
  }

  float4 GetColorFrom(const float2 & _TexCoord) const
  {
    return float4{ _TexCoord.x, _TexCoord.y, 0.0f, 0.0f } + m_Addition;
  }

private:
  float4 m_Addition;
};

#define COVELLITE_DECLARE_TEX2D(name, index) \
  Texture name
#define COVELLITE_DECLARE_CONST_BUFFER_INSIDE(type, name, index) \
  namespace { type name; }
#define COVELLITE_INPUT_SEMANTIC(value)

#include "../../Covellite.Api/Renderer/Shaders/Data.h"
#include "../../Covellite.Api/Renderer/Shaders/Input.fx"

#define COVELLITE_SHADER_PIXEL
#include "../../Covellite.Api/Renderer/Shaders/Default.fx"

// ************************************************************************** //
TEST_F(FxDefaultPixel_test, /*DISABLED_*/Test_psColored)
{
  const float4 Expected = { 19.0f, 81.9f, 21.0f, 50.0f };

  Pixel Input;
  Input.Color = Expected;

  const auto Result = psColored(Input);
  EXPECT_EQ(Expected, Result);
}

// ************************************************************************** //
TEST_F(FxDefaultPixel_test, /*DISABLED_*/Test_psLightened_NoLights)
{
  ObjectData.Lights.Points.UsedSlotCount = 0;

  TexDiffuse.SetAddition({ 19.0f, 82.0f, 11.0f, 42.0f });

  Pixel Input;
  Input.Color = float4{ 19.0f, 81.9f, 21.0f, 51.0f };
  Input.TexCoord = float2{ 19.082f, 11.43f };

  const auto Expected = 
    float4{ 19.0f + 19.082f, 82.0f + 11.43f, 11.0f, 42.0f } *
    float4{ 19.0f / 81.9f, 1.0f, 21.0f / 81.9f, 1.0f };

  const auto Result = psLightened(Input);
  EXPECT_EQ(Expected, Result);
}

// ************************************************************************** //
TEST_F(FxDefaultPixel_test, DISABLED_Test_psLightened_FullLights)
{
  ObjectData.Lights.Points.UsedSlotCount = 4;
  ObjectData.Lights.Points.Lights[0].Color = {};
  ObjectData.Lights.Points.Lights[0].Position = {};
  ObjectData.Lights.Points.Lights[0].Attenuation = {};

  ObjectData.Lights.Points.Lights[1].Color = {};
  ObjectData.Lights.Points.Lights[1].Position = {};
  ObjectData.Lights.Points.Lights[1].Attenuation = {};

  ObjectData.Lights.Points.Lights[2].Color = {};
  ObjectData.Lights.Points.Lights[2].Position = {};
  ObjectData.Lights.Points.Lights[2].Attenuation = {};

  ObjectData.Lights.Points.Lights[3].Color = {};
  ObjectData.Lights.Points.Lights[3].Position = {};
  ObjectData.Lights.Points.Lights[3].Attenuation = {};

  TexDiffuse.SetAddition({ 19.0f, 82.0f, 11.0f, 42.0f });

  Pixel Input;
  Input.Color = float4{ 19.0f, 81.9f, 21.0f, 51.0f };
  Input.TexCoord = float2{ 19.082f, 11.43f };

  const auto Expected =
    float4{ 19.0f + 19.082f, 82.0f + 11.43f, 11.0f, 42.0f } *
    float4{ 19.0f, 81.9f, 21.0f, 1.0f };

  const auto Result = psLightened(Input);
  EXPECT_EQ(Expected, Result);
}
