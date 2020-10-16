
#include "stdafx.h"
#include <glm/glm.force.hpp>
#include <alicorn/cpp/to-str.hpp>

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Fx.
*/

// ����� �������� ����� ������ Fx
class FxData_test :
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
TEST_F(FxData_test, /*DISABLED_*/Test_ConstantBufferIndex)
{
  const ::std::vector<int> BufferIndexes =
  {
    COVELLITE_BUFFER_INDEX_CAMERA,
    COVELLITE_BUFFER_INDEX_OBJECT,
    COVELLITE_BUFFER_INDEX_USER,
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
TEST_F(FxData_test, /*DISABLED_*/Test_Object)
{
  EXPECT_EQ(0, sizeof(::Object) % 16);

  ::Object Example;
  memset(&Example, 0x00, sizeof(Example));

  EXPECT_EQ(::glm::mat4{ 0.0f }, Example.World);
}
