
Texture2D TexDiffuse : register(t0);
SamplerState TexSampler : register(s0);

float4 DiscardTransparentPixel(Pixel _Value, float4 _Color)
{
  // Отсекаем прозрачные пиксели, чтобы они не попали в буфер глубины
  // и не нужно было упорядочивать отрисовку объектов.
  if (_Color.a < 0.5f) discard;

  return _Color;
}

float4 CalculateLights(Pixel _Value, float4 _Color)
{
  return _Value.Color * _Color;
}

float CalculateFogLinear(float _Distance)
{
  return saturate((FogData.Far - _Distance) / (FogData.Far - FogData.Near));
}

float CalculateFogExp(float _Distance)
{
  return 1.0f / pow(2.71828f, _Distance * FogData.Density);
}

float CalculateFogExp2(float _Distance)
{
  float Temp = _Distance * FogData.Density;
  return 1.0f / pow (2.71828f, Temp * Temp);
}

float4 CalculateFog(Pixel _Value, float4 _Color)
{
  float3 CameraPosition = float3(MatricesData.ViewInverse._41,
    MatricesData.ViewInverse._42, MatricesData.ViewInverse._43);
  float Distance = length(_Value.WorldPos.xyz - CameraPosition);
  float FogFactor = CalculateFogLinear(Distance);
  return FogFactor * _Color + (1.0f - FogFactor) * GetRGBAColor(FogData.ARGBColor);
}

float4 psExample(Pixel _Value) : SV_Target
{
  float4 Color = TexDiffuse.Sample(TexSampler, _Value.TexCoord);

  Color = DiscardTransparentPixel(_Value, Color);
  Color = CalculateLights(_Value, Color);
  Color = CalculateFog(_Value, Color);

  return Color;
}

Pixel vsParticles(Polyhedron _Value)
{
  Pixel Result = (Pixel)0;

  float4 LocalPosition = float4(
    _Value.Position.x - _Value.Normal.x,
    _Value.Position.y - _Value.Normal.y, 
    _Value.Position.z - _Value.Normal.z, 1.0f);

  float4 WorldPos = mul(LocalPosition, MatricesData.World) +
    float4(_Value.Normal, 0.0f);

  Result.ScreenPos = mul(WorldPos, mul(MatricesData.View, MatricesData.Projection));
  Result.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
  Result.Normal = float4(0.0f, 0.0f, 1.0f, 0.0f);
  Result.TexCoord = _Value.Texture;
  Result.WorldPos = WorldPos;

  return Result;
}

float4 psParticles(Pixel _Value) : SV_Target
{
  float4 Color = TexDiffuse.Sample(TexSampler, _Value.TexCoord);

  if (Color.a < 0.05f) discard;

  return Color;
}
