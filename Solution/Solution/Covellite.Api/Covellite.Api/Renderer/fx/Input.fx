
cbuffer cbMatrices : register(BUFFER_INDEX(MATRICES_BUFFER_INDEX))
{
  const ::Matrices MatricesData;
};

/// [Input vertex shared structures]

// Используется совместно с covellite::api::Vertex::Gui
struct VertexGui
{
  float2 Position : POSITION;
  uint Color : COLOR;
  float2 TexCoord : TEXCOORD;
};
  
// Используется совместно с covellite::api::Vertex::Textured
struct VertexTextured
{
  float3 Position : POSITION;
  float3 Normal : NORMAL;
  float2 Texture : TEXCOORD;
};

/// [Input vertex shared structures]

cbuffer cbMaterial : register(BUFFER_INDEX(MATERIAL_BUFFER_INDEX))
{
  const ::Material MaterialData;
};

cbuffer cbLights : register(BUFFER_INDEX(LIGHTS_BUFFER_INDEX))
{
  const ::Lights LightsData;
};

struct PixelInput
{
  float4 ScreenPos : SV_POSITION;
  float4 Color : COLOR;
  float3 Normal : NORMAL;
  float2 TexCoord : TEXCOORD0;
  float4 WorldPos : TEXCOORD1;
};

float4 GetRGBAColor(uint _ARGB)
{
  return float4(
    ((_ARGB & 0x00FF0000) >> 16) / 255.0f,
    ((_ARGB & 0x0000FF00) >> 8) / 255.0f,
    (_ARGB & 0x000000FF) / 255.0f,
    ((_ARGB & 0xFF000000) >> 24) / 255.0f
    );
}
