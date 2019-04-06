
Texture2D diffuseMap : register(t0);
SamplerState diffuseSampler : register(s0);

float4 psColored(Pixel _Value) : SV_Target
{
  return _Value.Color; 
}

float4 psTextured(Pixel _Value) : SV_Target
{
  float4 Color = diffuseMap.Sample(diffuseSampler, _Value.TexCoord) * 
    _Value.Color;

  // Отсекаем прозрачные пиксели, чтобы они не попали в буфер глубины
  // и не нужно было упорядочивать отрисовку объектов.
  if (Color.w == 0.0f) discard;

  return Color;
}
