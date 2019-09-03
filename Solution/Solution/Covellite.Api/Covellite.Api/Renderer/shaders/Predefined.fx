
#ifdef COVELLITE_SHADER_HLSL // ********************************************* //
  
#define mediump
#define lowp
  
#define vec4 float4   
#define vec3 float3   
#define vec2 float2   
#define mat4 float4x4 
#define mat3 float3x3
#define ToMatrix3x3(m) (float3x3)m
  
#define dFdx(a) ddx(a)
#define dFdy(a) ddy(a)
  
#define COVELLITE_INPUT_SEMANTIC(value) \
  : value
#define COVELLITE_DECLARE_TEX2D(name, index) \
  Texture2D name : register(t##index); \
  SamplerState TexSampler##index : register(s##index);
#define COVELLITE_TEX2D_COLOR(tex, uv) \
  tex.Sample(TexSampler0, uv)
#define COVELLITE_DECLARE_CONST_BUFFER(type, name, index) \
  cbuffer cb##name : register(b##index) { const type name; }
  
#elif defined COVELLITE_SHADER_GLSL // ************************************** //
    
precision highp float;
precision highp int;
    
#define float4 vec4
#define float3 vec3
#define float2 vec2
#define float4x4 mat4
#define float3x3 mat3
#define ToMatrix3x3(m) mat3(m)
  
#define ddx(a) dFdx(a)
#define ddy(a) dFdy(a)
#define mul(a, b) b * a
#define saturate(a) clamp(a, 0.0f, 1.0f)
#define rsqrt(a) (1.0f / sqrt(a))
  
#define COVELLITE_INPUT_SEMANTIC(value)
#define COVELLITE_DECLARE_TEX2D(name, index) \
  uniform sampler2D name
#define COVELLITE_TEX2D_COLOR(tex, uv) \
  texture(tex, uv)
#define COVELLITE_DECLARE_CONST_BUFFER(type, name, index) \
  uniform type name
     
#endif // ******************************************************************* //
   
#define ToFloat3(v) v.xyz
#define color_t lowp float4
