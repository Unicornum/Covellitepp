  
#define CAMERA_BUFFER_INDEX 0
#define MATRICES_BUFFER_INDEX 1
#define LIGHTS_BUFFER_INDEX 6
#define FOG_BUFFER_INDEX 7
  
#define MAX_LIGHT_POINT_COUNT 16
  
struct Camera
{
  float4x4 Projection;
  float4x4 View;
  float4x4 ViewInverse;
};
  
struct Matrices
{
  float4x4 World;
  float4x4 Projection;
  float4x4 View;
  float4x4 ViewInverse;
};
  
struct Ambient_t
{
  lowp int IsValid;
  color_t  Color;
};
  
struct Direction_t
{
  lowp int IsValid;
  color_t  Color;
  float4   Direction;
};
  
struct Point_t
{
  color_t Color;
  float4  Position;
  float4  Attenuation; // Const, Linear, Exponent;
};
  
struct Points_t
{
  Point_t  Lights[MAX_LIGHT_POINT_COUNT];
  lowp int UsedSlotCount;
};
  
struct Lights
{
  Ambient_t Ambient;
  Direction_t Direction;
  Points_t Points;
};
  
struct Fog
{
  color_t Color;
  float   Near;
  float   Far;
  float   Density;
};
