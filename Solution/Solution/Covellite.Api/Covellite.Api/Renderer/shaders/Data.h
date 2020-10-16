
#ifndef COVELLITE_API_CONSTANT_BUFFERS_HEADER
#define COVELLITE_API_CONSTANT_BUFFERS_HEADER

#ifdef GLM_VERSION

using float4x4 = ::glm::mat4;
#define mediump
#define lowp

#endif // GLM_VERSION

#define COVELLITE_BUFFER_INDEX_CAMERA 0
#define COVELLITE_BUFFER_INDEX_OBJECT 1
#define COVELLITE_BUFFER_INDEX_USER 2

struct Camera
{
  float4x4 Projection;
  float4x4 View;
  float4x4 ViewInverse;
};

struct Object
{
  float4x4 World;
};

#endif // COVELLITE_API_CONSTANT_BUFFERS_HEADER
