
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
class FxPredefinedHlsl_test :
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
// FRIEND_TEST(FxPredefinedHlsl_test, Test_Function);

#define COVELLITE_SHADER_HLSL
#include "../../Covellite.Api/Renderer/Shaders/Predefined.fx"

#pragma warning(push)
#pragma warning(disable: 4003)

// ************************************************************************** //
TEST_F(FxPredefinedHlsl_test, /*DISABLED_*/Test_Predefined)
{
  EXPECT_STREQ("", "" __TO_STR(mediump));
  EXPECT_STREQ("", "" __TO_STR(lowp));

  EXPECT_STREQ("int4", __TO_STR(ivec4));
  EXPECT_STREQ("float4", __TO_STR(vec4));
  EXPECT_STREQ("float3", __TO_STR(vec3));
  EXPECT_STREQ("float2", __TO_STR(vec2));
  EXPECT_STREQ("float4x4", __TO_STR(mat4));
  EXPECT_STREQ("float3x3", __TO_STR(mat3));
  EXPECT_STREQ("(float3x3)m1909031226", __TO_STR(ToMatrix3x3(m1909031226)));

  EXPECT_STREQ("ddx(1909031227)", __TO_STR(dFdx(1909031227)));
  EXPECT_STREQ("ddy(1909031228)", __TO_STR(dFdy(1909031228)));
  EXPECT_STREQ("lerp(a, b, c)", __TO_STR(mix(a, b, c)));

  EXPECT_STREQ(": 1909031229", 
    __TO_STR(COVELLITE_INPUT_SEMANTIC(1909031229)));
  EXPECT_STREQ("cbuffer cbname1909031235 : register(bindex1909031236) "
    "{ const type1909031234 name1909031235; }",
    __TO_STR(COVELLITE_DECLARE_CONST_BUFFER(type1909031234, cbname, name1909031235, index1909031236)));
  EXPECT_STREQ("cbuffer cbname1909031235 : register(bindex1909031236) "
    "{ const type1909031234 name1909031235; }",
    __TO_STR(COVELLITE_DECLARE_CONST_BUFFER_INSIDE(type1909031234, name1909031235, index1909031236)));
  EXPECT_STREQ("cbuffer cbn1910131328 : register(bCOVELLITE_BUFFER_INDEX_USER) "
    "{ const t1910131326 n1910131328; }",
    __TO_STR(COVELLITE_DECLARE_CONST_USER_BUFFER(t1910131326, dummy, n1910131328)));

  EXPECT_STREQ("static", 
    __TO_STR(COVELLITE_DECLARE_STATIC));
  EXPECT_STREQ("m1910131321[i1910131322]", 
    __TO_STR(COVELLITE_MATRIX_ROW(m1910131321, i1910131322)));
  EXPECT_STREQ("float4(m1910131321[0][i1910131322], m1910131321[1][i1910131322],"
    " m1910131321[2][i1910131322], m1910131321[3][i1910131322])",
    __TO_STR(COVELLITE_MATRIX_COLUMN(m1910131321, i1910131322)));

  EXPECT_STREQ("Texture2D name1909031230 : register(tindex1909031231); "
    "SamplerState TexSamplerindex1909031231 : register(sindex1909031231);", 
    __TO_STR(COVELLITE_DECLARE_TEX2D(name1909031230, index1909031231)));
  EXPECT_STREQ("Texture2D name2504061806[count2504061808] : register(tindex2504061807); "
    "SamplerState TexSamplerindex2504061807 : register(sindex2504061807);",
    __TO_STR(COVELLITE_DECLARE_TEX2D_ARRAY(name2504061806, count2504061808, index2504061807)));
  EXPECT_STREQ("tex1909031232.Sample(TexSampler0, uv1909031233)", 
    __TO_STR(COVELLITE_TEX2D_COLOR(tex1909031232, uv1909031233)));
  EXPECT_STREQ("tex2504061809[i2504061810].Sample(TexSampler0, uv2504061811)",
    __TO_STR(COVELLITE_TEX2D_ARRAY_COLOR(tex2504061809, i2504061810, uv2504061811)));
  EXPECT_STREQ("struct n1910131324 { float4 Target[c1910131325] : SV_Target; }",
    __TO_STR(COVELLITE_DECLARE_MULTI_TARGET_STRUCTURE(n1910131324, c1910131325)));

  EXPECT_STREQ("1909031335.xyz", __TO_STR(ToFloat3(1909031335)));
}

#pragma warning(pop)
