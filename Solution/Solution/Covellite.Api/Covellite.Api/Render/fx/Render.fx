
cbuffer ConstantBuffer : register(b0)
{
  float4x4 World;
  float4x4 Projection;
  float2 IsTextureDisabled;
};

Texture2D diffuseMap : register(t0);
SamplerState diffuseSampler : register(s0);

struct VertexIn
{
  float2 PosL : POSITION;
  uint Color : COLOR;
  float2 TexCoord : TEXCOORD;
};

struct VertexOut
{
  float4 PosH : SV_POSITION;
  float4 Color : COLOR;
  float2 TexCoord : TEXCOORD0;
  float2 IsTextureDisabled : TEXCOORD1;
};

VertexOut VS(VertexIn vin)
{
  VertexOut vout = (VertexOut)0;

  float4x4 worldViewProjection = mul(World, Projection);
  vout.PosH = mul(float4(vin.PosL.x, vin.PosL.y, 0.0f, 1.0f), worldViewProjection);
  float R = (vin.Color & 0x000000FF) / 255.0f;
  float G = ((vin.Color & 0x0000FF00) >> 8) / 255.0f;
  float B = ((vin.Color & 0x00FF0000) >> 16) / 255.0f;
  float A = ((vin.Color & 0xFF000000) >> 24) / 255.0f;
  vout.Color = float4(R, G, B, A);
  vout.TexCoord = vin.TexCoord;
  vout.IsTextureDisabled = IsTextureDisabled;

  return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
  if (pin.IsTextureDisabled.x == 0.0f) return pin.Color;
  return diffuseMap.Sample(diffuseSampler, pin.TexCoord) * pin.Color;
}
