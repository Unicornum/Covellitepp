
Texture2D diffuseMap : register(t0);
SamplerState diffuseSampler : register(s0);

float4 psColored(Pixel _Value) : SV_Target
{
  return _Value.Color; 
}

float4 psTextured(Pixel _Value) : SV_Target
{
  return diffuseMap.Sample(diffuseSampler, _Value.TexCoord) * _Value.Color;
}
