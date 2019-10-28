
#ifndef COVELLITE_API_CONSTANT_BUFFERS_HEADER
#define COVELLITE_API_CONSTANT_BUFFERS_HEADER
  
#ifdef GLM_VERSION
  
using float4 = ::glm::vec4;
using float4x4 = ::glm::mat4;
#define mediump
#define lowp
 
#endif // GLM_VERSION
  
#define color_t float4
  
#define COVELLITE_BUFFER_INDEX_CAMERA 0
#define COVELLITE_BUFFER_INDEX_FOG 1
#define COVELLITE_BUFFER_INDEX_OBJECT 2
#define COVELLITE_BUFFER_INDEX_USER 3
  
#define COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT 4
    
struct Camera
{
  float4x4 Projection;
  float4x4 View;
  float4x4 ViewInverse;
};
  
struct Fog
{
  color_t Color;
  float   Near;
  float   Far;
  float   Density;
};
  
struct Ambient_t
{
  color_t  Color;
  int IsValid, align1, align2, align3;
};
  
struct Direction_t
{
  color_t  Color;
  float4   Direction;
  int IsValid, align1, align2, align3;
};
  
struct Point_t
{
  color_t Color;
  float4  Position;
  float4  Attenuation; // Const, Linear, Exponent, Radius;
};
  
struct Points_t
{
  Point_t Lights[COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT];
  int     UsedSlotCount;
};
  
struct Lights_t
{
  Ambient_t   Ambient;
  Direction_t Direction;
  Points_t    Points;
};
  
struct Object
{
  float4x4 World;
  Lights_t Lights;
};
  
// **************************** deprecated ********************************** //
  
#define COVELLITE_BUFFER_INDEX_MATRICES 4
#define COVELLITE_BUFFER_INDEX_LIGHTS 5
  
#define COVELLITE_MAX_LIGHT_POINT_SCENE_COUNT 16
  
struct ScenePoints
{
  Point_t Lights[COVELLITE_MAX_LIGHT_POINT_SCENE_COUNT];
  int     UsedSlotCount;
};
  
struct SceneLights
{
  Ambient_t   Ambient;
  Direction_t Direction;
  ScenePoints Points;
};
  
struct Matrices
{
  float4x4 World;
  float4x4 Projection;
  float4x4 View;
  float4x4 ViewInverse;
};
  
// ************************************************************************** //
  
#endif // COVELLITE_API_CONSTANT_BUFFERS_HEADER
