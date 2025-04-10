
#include "stdafx.h"
#include <glm/glm.force.hpp>

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Fx.
*/

// ����� �������� ����� ������ Fx
class FxDefaultPixel_test :
  public ::testing::Test
{
protected:
  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
  }
};

// ������� ������� ��� ����������� � ����� Fx 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
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
