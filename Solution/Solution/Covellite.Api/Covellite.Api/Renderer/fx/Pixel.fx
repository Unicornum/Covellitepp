
Texture2D TexDiffuse : register(t0);
SamplerState TexSampler : register(s0);

float4 psColored(Pixel _Value) : SV_Target
{
  return _Value.Color; 
}

float4 psTextured(Pixel _Value) : SV_Target
{
  return TexDiffuse.Sample(TexSampler, _Value.TexCoord) * _Value.Color;
}
