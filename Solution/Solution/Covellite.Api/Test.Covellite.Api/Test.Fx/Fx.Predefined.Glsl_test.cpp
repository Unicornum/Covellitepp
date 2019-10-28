
#include "stdafx.h"
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
class FxPredefinedGlsl_test :
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
// FRIEND_TEST(FxPredefinedGlsl_test, Test_Function);

const auto BaseIndex = __COUNTER__;

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define precision using
#define highp MACRO_CONCAT(highp, __COUNTER__) =

#define COVELLITE_SHADER_GLSL
#include "../../Covellite.Api/Renderer/Shaders/Predefined.fx"

#pragma warning(push)
#pragma warning(disable: 4003)

// ************************************************************************** //
TEST_F(FxPredefinedGlsl_test, /*DISABLED_*/Test_Predefined)
{
  //EXPECT_EQ(2, BaseIndex);
  EXPECT_EQ(typeid(float), typeid(highp20));
  EXPECT_EQ(typeid(int), typeid(highp21));

  EXPECT_STREQ("ivec4", __TO_STR(int4));
  EXPECT_STREQ("vec4", __TO_STR(float4));
  EXPECT_STREQ("vec3", __TO_STR(float3));
  EXPECT_STREQ("vec2", __TO_STR(float2));
  EXPECT_STREQ("mat4", __TO_STR(float4x4));
  EXPECT_STREQ("mat3", __TO_STR(float3x3));
  EXPECT_STREQ("mat3(m1909031226)", __TO_STR(ToMatrix3x3(m1909031226)));

  EXPECT_STREQ("dFdx(1909031227)", __TO_STR(ddx(1909031227)));
  EXPECT_STREQ("dFdy(1909031228)", __TO_STR(ddy(1909031228)));
  EXPECT_STREQ("((1909031251) * (1909031252))", __TO_STR(mul(1909031251, 1909031252)));
  EXPECT_STREQ("clamp(1909031253, 0.0f, 1.0f)", __TO_STR(saturate(1909031253)));
  EXPECT_STREQ("(1.0f / sqrt(1909031254))", __TO_STR(rsqrt(1909031254)));
  EXPECT_STREQ("mix(a, b, c)", __TO_STR(lerp(a, b, c)));

  EXPECT_STREQ("", 
    "" __TO_STR(COVELLITE_INPUT_SEMANTIC(1909031229)));
  EXPECT_STREQ("vec4(m1910131357[0][i1910131358], m1910131357[1][i1910131358], "
    "m1910131357[2][i1910131358], m1910131357[3][i1910131358])",
    __TO_STR(COVELLITE_MATRIX_ROW(m1910131357, i1910131358)));
  EXPECT_STREQ("m1910131359[i1910131400]",
    __TO_STR(COVELLITE_MATRIX_COLUMN(m1910131359, i1910131400)));

  EXPECT_STREQ("layout (std140) uniform cn1910131349 { t1909031234 n1909031235; }",
    __TO_STR(COVELLITE_DECLARE_CONST_BUFFER(t1909031234, cn1910131349, n1909031235, 0)));
  EXPECT_STREQ("uniform t1909031234 n1909031235",
    __TO_STR(COVELLITE_DECLARE_CONST_BUFFER_INSIDE(t1909031234, n1909031235, 0)));
  EXPECT_STREQ("layout (std140) uniform cn1910131352 { t1910131351 n1910131353; }",
    __TO_STR(COVELLITE_DECLARE_CONST_USER_BUFFER(t1910131351, cn1910131352, n1910131353)));

  EXPECT_STREQ("uniform sampler2D name1909031230", 
    __TO_STR(COVELLITE_DECLARE_TEX2D(name1909031230, 0)));
  EXPECT_STREQ("texture(tex1909031232, uv1909031233)", 
    __TO_STR(COVELLITE_TEX2D_COLOR(tex1909031232, uv1909031233)));
  EXPECT_STREQ("struct n1910131354 { vec4 Target[c1910131355]; }; "
    "out vec4 Covellite_MultiOutPixelColor[c1910131355]",
    __TO_STR(COVELLITE_DECLARE_MULTI_TARGET_STRUCTURE(n1910131354, c1910131355)));

  EXPECT_STREQ("1909031335.xyz", __TO_STR(ToFloat3(1909031335)));
}

#pragma warning(pop)
