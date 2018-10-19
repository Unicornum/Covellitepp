
Texture2D diffuseMap : register(t0);
SamplerState diffuseSampler : register(s0);

float4 psColored(PixelInput _Value) : SV_Target
{
  return _Value.Color; 
}

float4 psTextured(PixelInput _Value) : SV_Target
{
  return diffuseMap.Sample(diffuseSampler, _Value.TexCoord) * _Value.Color;
}
