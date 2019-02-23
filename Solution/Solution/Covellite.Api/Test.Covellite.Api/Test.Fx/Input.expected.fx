
cbuffer cbMatrices : register(BUFFER_INDEX(MATRICES_BUFFER_INDEX))
{
  const ::Matrices MatricesData;
};

cbuffer cbMaterial : register(BUFFER_INDEX(MATERIAL_BUFFER_INDEX))
{
  const ::Material MaterialData;
};

cbuffer cbLights : register(BUFFER_INDEX(LIGHTS_BUFFER_INDEX))
{
  const ::Lights LightsData;
};

/// [Input vertex shared structures]

// ¬ходные данные вертексного шейдера (используетс€ совместно с форматом
// буфера дл€ отрисовки плоских объектов).
struct Polygon
{
  float2 Position : POSITION;
  uint Color : COLOR;
  float2 TexCoord : TEXCOORD;
};
  
// ¬ходные данные вертексного шейдера (используетс€ совместно с форматом
// буфера дл€ отрисовки объемных объектов).
struct Polyhedron
{
  float3 Position : POSITION;
  float3 Normal : NORMAL;
  float2 Texture : TEXCOORD;
};
  
// ¬ходные данные пиксельного шейдера
struct Pixel
{
  float4 ScreenPos : SV_POSITION;
  float4 Color : COLOR;
  float3 Normal : NORMAL;
  float2 TexCoord : TEXCOORD0;
  float4 WorldPos : TEXCOORD1;
};

/// [Input vertex shared structures]

float4 GetRGBAColor(uint _ARGB)
{
  return float4(
    ((_ARGB & 0x00FF0000) >> 16) / 255.0f,
    ((_ARGB & 0x0000FF00) >> 8) / 255.0f,
    (_ARGB & 0x000000FF) / 255.0f,
    ((_ARGB & 0xFF000000) >> 24) / 255.0f
    );
}
