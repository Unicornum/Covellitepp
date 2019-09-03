
#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////

Pixel vsParticles(Vertex _Value)
{
  float4 LocalPosition = float4(
    _Value.Position.x,
    _Value.Position.y,
    0.0f, 
    1.0f);

  float4 WorldPos = mul(LocalPosition, MatricesData.World) + _Value.Extra;

  Pixel Result;
  Result.ScreenPos = mul(WorldPos, mul(MatricesData.View, MatricesData.Projection));
  Result.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
  Result.Normal = float3(0.0f, 0.0f, 1.0f);
  Result.TexCoord = _Value.TexCoord;
  Result.WorldPos = WorldPos;

  return Result;
}

#elif defined COVELLITE_SHADER_PIXEL ///////////////////////////////////////////

COVELLITE_DECLARE_TEX2D(TexDiffuse, 0);

float4 DiscardTransparentPixel(Pixel _Value, float4 _Color)
{
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
  float3 CameraPosition = MatricesData.ViewInverse[3].xyz;

  float Distance = length(_Value.WorldPos.xyz - CameraPosition);
  float FogFactor = CalculateFogLinear(Distance);
  return FogFactor * _Color + (1.0f - FogFactor) * FogData.Color;
}

float4 psExample(Pixel _Value)
{
  float4 Color = COVELLITE_TEX2D_COLOR(TexDiffuse, _Value.TexCoord);

  Color = DiscardTransparentPixel(_Value, Color);
  Color = CalculateLights(_Value, Color);
  Color = CalculateFog(_Value, Color);

  return Color;
}

float4 psParticles(Pixel _Value)
{
  float4 Color = COVELLITE_TEX2D_COLOR(TexDiffuse, _Value.TexCoord);
  if (Color.a < 0.05f) discard;

  return Color;
}

#endif /////////////////////////////////////////////////////////////////////////
