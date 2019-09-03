  
#ifdef COVELLITE_SHADER_GLSL // ********************************************* //
  
#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////
  
// Covellite++ GLSL input data vertex shader.
in vec4 CovelliteVertexPosition;
in mediump vec2 CovellteVertexTexCoord;
in vec4 CovelliteVertexExtra;
  
#endif /////////////////////////////////////////////////////////////////////////
  
#endif // ******************************************************************* //
  
COVELLITE_DECLARE_CONST_BUFFER(Matrices, MatricesData, MATRICES_BUFFER_INDEX);
COVELLITE_DECLARE_CONST_BUFFER(Lights,   LightsData,   LIGHTS_BUFFER_INDEX);
COVELLITE_DECLARE_CONST_BUFFER(Fog,      FogData,      FOG_BUFFER_INDEX);
  
// Input data vertex shader (flat objects).
struct Polygon
{
  float2 Position  COVELLITE_INPUT_SEMANTIC(POSITION);
  uint Color       COVELLITE_INPUT_SEMANTIC(COLOR);
  float2 TexCoord  COVELLITE_INPUT_SEMANTIC(TEXCOORD);
};
  
// Input data vertex shader (volume objects).
struct Polyhedron
{
  float3 Position  COVELLITE_INPUT_SEMANTIC(POSITION);
  float3 Normal    COVELLITE_INPUT_SEMANTIC(NORMAL);
  float2 TexCoord  COVELLITE_INPUT_SEMANTIC(TEXCOORD);
};
  
// Input data vertex shader (union objects).
struct Vertex
{
  float4 Position         COVELLITE_INPUT_SEMANTIC(POSITION);
  mediump float2 TexCoord COVELLITE_INPUT_SEMANTIC(TEXCOORD);
  float4 Extra            COVELLITE_INPUT_SEMANTIC(NORMAL);
};
  
// Output data vertex shader and input data pixel shader.
struct Pixel
{
  float4 ScreenPos        COVELLITE_INPUT_SEMANTIC(SV_POSITION);
  lowp float4 Color       COVELLITE_INPUT_SEMANTIC(COLOR);
  float3 Normal           COVELLITE_INPUT_SEMANTIC(NORMAL);
  mediump float2 TexCoord COVELLITE_INPUT_SEMANTIC(TEXCOORD0);
  float4 WorldPos         COVELLITE_INPUT_SEMANTIC(POSITION0);
};
  
#undef COVELLITE_INPUT_SEMANTIC
#undef COVELLITE_DECLARE_CONST_BUFFER
