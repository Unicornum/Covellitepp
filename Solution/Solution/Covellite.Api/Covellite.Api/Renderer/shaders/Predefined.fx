
#ifdef COVELLITE_SHADER_HLSL // ********************************************* //
  
#define mediump
#define lowp
  
#define ivec4 int4
#define vec4 float4   
#define vec3 float3   
#define vec2 float2   
#define mat4 float4x4 
#define mat3 float3x3
#define ToMatrix3x3(m) (float3x3)m
  
#define dFdx(a) ddx(a)
#define dFdy(a) ddy(a)
#define mix(a, b, c) lerp(a, b, c)
  
#define COVELLITE_INPUT_SEMANTIC(value) \
  : value
#define COVELLITE_DECLARE_CONST_BUFFER(type, cbname, name, index) \
  cbuffer cb##name : register(b##index) { const type name; }
#define COVELLITE_DECLARE_CONST_BUFFER_INSIDE(type, name, index) \
  COVELLITE_DECLARE_CONST_BUFFER(type, dummy, name, index)
  
#define COVELLITE_DECLARE_STATIC \
  static
#define COVELLITE_MATRIX_ROW(m, index) \
  m[index]
#define COVELLITE_MATRIX_COLUMN(m, index) \
  float4(m[0][index], m[1][index], m[2][index], m[3][index])
  
#define COVELLITE_DECLARE_TEX2D(name, index) \
  Texture2D name : register(t##index); \
  SamplerState TexSampler##index : register(s##index);
#define COVELLITE_TEX2D_COLOR(tex, uv) \
  tex.Sample(TexSampler0, uv)
#define COVELLITE_DECLARE_MULTI_TARGET_STRUCTURE(name, count) \
  struct name { float4 Target[count] : SV_Target; }
  
#elif defined COVELLITE_SHADER_GLSL // ************************************** //
  
precision highp float;
precision highp int;
  
#define int4 ivec4
#define float4 vec4
#define float3 vec3
#define float2 vec2
#define float4x4 mat4
#define float3x3 mat3
#define ToMatrix3x3(m) mat3(m)
  
#define ddx(a) dFdx(a)
#define ddy(a) dFdy(a)
#define mul(a, b) ((a) * (b))
#define saturate(a) clamp(a, 0.0f, 1.0f)
#define rsqrt(a) (1.0f / sqrt(a))
#define lerp(a, b, c) mix(a, b, c)
  
#define COVELLITE_INPUT_SEMANTIC(value)
#define COVELLITE_DECLARE_CONST_BUFFER(type, cbname, name, index) \
  layout (std140) uniform cbname { type name; }
#define COVELLITE_DECLARE_CONST_BUFFER_INSIDE(type, name, index) \
  uniform type name
  
#define COVELLITE_DECLARE_STATIC
#define COVELLITE_MATRIX_ROW(m, index) \
  float4(m[0][index], m[1][index], m[2][index], m[3][index])
#define COVELLITE_MATRIX_COLUMN(m, index) \
  m[index]
  
#define COVELLITE_DECLARE_TEX2D(name, index) \
  uniform sampler2D name
#define COVELLITE_TEX2D_COLOR(tex, uv) \
  texture(tex, uv)
#define COVELLITE_DECLARE_MULTI_TARGET_STRUCTURE(name, count) \
  struct name { float4 Target[count]; }; \
  out float4 Covellite_MultiOutPixelColor[count]
  
#endif // ******************************************************************* //
  
#define COVELLITE_DECLARE_CONST_USER_BUFFER(type, cbname, name) \
  COVELLITE_DECLARE_CONST_BUFFER(type, cbname, name, COVELLITE_BUFFER_INDEX_USER)
  
#define ToFloat3(v) v.xyz
