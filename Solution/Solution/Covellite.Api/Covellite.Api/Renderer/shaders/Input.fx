  
#define COVELLITE_INSTANCE_DATA_STRUCTURE \
  /* place for instance variables */

#ifdef COVELLITE_SHADER_GLSL // ********************************************* //
  
#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////
  
#define COVELLITE_IN in
  
// Covellite++ GLSL input data vertex shader.
in vec4 Covellite_VertexPosition;
in mediump vec2 Covellite_VertexTexCoord;
in vec4 Covellite_VertexExtra;
COVELLITE_INSTANCE_DATA_STRUCTURE
  
#undef COVELLITE_IN
  
#endif /////////////////////////////////////////////////////////////////////////
  
#endif // ******************************************************************* //
  
#define COVELLITE_IN
  
COVELLITE_DECLARE_CONST_BUFFER_INSIDE(Camera, CameraData, COVELLITE_BUFFER_INDEX_CAMERA);
COVELLITE_DECLARE_CONST_BUFFER_INSIDE(Object, ObjectData, COVELLITE_BUFFER_INDEX_OBJECT);
  
// Input data vertex shader (union objects).
struct Vertex
{
  float4 Position           COVELLITE_INPUT_SEMANTIC(POSITION);
  mediump float2 TexCoord   COVELLITE_INPUT_SEMANTIC(TEXCOORD);
  float4 Extra              COVELLITE_INPUT_SEMANTIC(NORMAL);
  COVELLITE_INSTANCE_DATA_STRUCTURE
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
