
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
class Fx_test :
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
// FRIEND_TEST(Fx_test, Test_Function);

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_Default_Deprecated)
{
  EXPECT_FILES_STREQ(THIS_DIRECTORY / L"DefaultDeprecated.expected.fx",
    THIS_DIRECTORY / L"../../Covellite.Api/Renderer/Shaders/DefaultDeprecated.fx");
}

#define lowp
#define mediump

using float2 = ::glm::vec2;
using float3 = ::glm::vec3;
using float4 = ::glm::vec4;
using float4x4 = ::glm::mat4;
using uint = uint32_t;
using color_t = ::glm::vec4;

#include "../../Covellite.Api/Renderer/shaders/Data.h"

#define COVELLITE_INPUT_SEMANTIC(value)
#define COVELLITE_DECLARE_CONST_BUFFER(class, name, index) class name

#include "../../Covellite.Api/Renderer/Shaders/Input.fx"

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_Data)
{
  const ::std::vector<int> BufferIndexes =
  {
    CAMERA_BUFFER_INDEX,
    MATRICES_BUFFER_INDEX,
    LIGHTS_BUFFER_INDEX,
    FOG_BUFFER_INDEX,
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

  EXPECT_TRUE(MAX_LIGHT_POINT_COUNT > 0);
}

// ========================================================================== //

#define COVELLITE_SHADER_VERTEX

#define mul(a, b) b * a
#define normalize(v) ::glm::normalize(v)
#define ToMatrix3x3(m) ::glm::mat3(m)
#define max(a, b) ::std::max(a, b)
#define min(a, b) ::std::min(a, b)

float3 ToFloat3(const float4 & _Value)
{
  return float3{ _Value };
}

#include "../../Covellite.Api/Renderer/Shaders/Default.fx"

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_vsGui)
{
  Vertex Input;
  Input.Position = { 0.1f, 0.2f, 0.3f, 0.4f };
  Input.TexCoord = { 0.5f, 0.6f };
  Input.Extra = { 0.7f, 0.8f, 0.9f, 0.0f };

  MatricesData.World = ::glm::mat4{ 2.0f };
  MatricesData.View = ::glm::mat4{ 3.0f };
  MatricesData.Projection = ::glm::mat4{ 4.0f };

  const auto ExpectedScreenPos = MatricesData.Projection * MatricesData.View *
    MatricesData.World * Input.Position;

  const auto Result = vsGui(Input);
  EXPECT_EQ(ExpectedScreenPos, Result.ScreenPos);
  EXPECT_EQ(Input.TexCoord, Result.TexCoord);
  EXPECT_EQ(Input.Extra, Result.Color);
}

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_vsLights)
{
  Vertex Input;
  Input.Position = { 1.90f, 8.20f, 1.1f, 4.9f };
  Input.TexCoord = { 19.0820f, 11.50f };
  Input.Extra = { 1.90f, 8.20f, 1.1f, 5.1f };

  MatricesData.World = ::glm::mat4{ 19082.01152f };
  MatricesData.View = ::glm::mat4{ 19082.01153f };
  MatricesData.Projection = ::glm::mat4{ 19082.01154f };

  const auto ExpectedScreenPos = MatricesData.Projection * MatricesData.View *
    MatricesData.World * Input.Position;
  const auto ExpectedNormal = normalize(
    mul(ToFloat3(Input.Extra), ToMatrix3x3(MatricesData.World)));
  float4 ExpectedWorldPos =
    mul(Input.Position, MatricesData.World);

  const auto Result = vsLights(Input);
  EXPECT_EQ(ExpectedScreenPos, Result.ScreenPos);

  // 03 Сентябрь 2019 14:03 (unicornum.verum@gmail.com)
  TODO("Здесь нужен тест для разных источников света");
  EXPECT_EQ(GetGouraudLightsColor(float4{}, float3{}), Result.Color);

  EXPECT_EQ(ExpectedNormal, Result.Normal);
  EXPECT_EQ(Input.TexCoord, Result.TexCoord);
  EXPECT_EQ(ExpectedWorldPos, Result.WorldPos);
}

#undef COVELLITE_SHADER_VERTEX

// ========================================================================== //

#define COVELLITE_TEX2D_COLOR(tex, uv) \
  tex.GetColorFrom(uv)

class Texture
{
public:
  void SetAddition(const float4 & _Value)
  {
    m_Addition = _Value;
  }

  float4 GetColorFrom(const float2 & _TexCoord)
  {
    return float4{ _TexCoord.x, _TexCoord.y, 0.0f, 0.0f } + m_Addition;
  }

private:
  float4 m_Addition;
};

#define COVELLITE_DECLARE_TEX2D(name, index) \
  Texture name

#define COVELLITE_SHADER_PIXEL
#include "../../Covellite.Api/Renderer/Shaders/Default.fx"

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_psColored)
{
  const float4 Expected = { 19.0f, 81.9f, 21.0f, 50.0f };

  Pixel Input;
  Input.Color = Expected;

  const auto Result = psColored(Input);
  EXPECT_EQ(Expected, Result);
}

// ************************************************************************** //
TEST_F(Fx_test, /*DISABLED_*/Test_psTextured)
{
  TexDiffuse.SetAddition({ 19.0f, 82.0f, 11.0f, 42.0f });

  Pixel Input;
  Input.Color = float4{ 19.0f, 81.9f, 21.0f, 51.0f };
  Input.TexCoord = float2{ 19.082f, 11.43f };

  const auto Expected = 
    float4{ 19.0f + 19.082f, 82.0f + 11.43f, 11.0f, 42.0f } *
    float4{ 19.0f, 81.9f, 21.0f, 51.0f };

  const auto Result = psTextured(Input);
  EXPECT_EQ(Expected, Result);
}

#undef COVELLITE_SHADER_PIXEL

// ========================================================================== //
