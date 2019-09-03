
#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////
  
// ************************************************************************** //
  
float4 CalcPointLight(Point_t _Light, float4 _Position, float3 _Normal)
{
  float4 Direction = _Light.Position - _Position;

  float LightFactor = max(dot(_Normal, normalize(ToFloat3(Direction))), 0.0f);
  float4 Color = LightFactor * _Light.Color;
  float Distance = length(Direction);

  float Attenuation =
    _Light.Attenuation.x +
    _Light.Attenuation.y * Distance +
    _Light.Attenuation.z * Distance * Distance;

  return Color / Attenuation;
}
  
float3 SyncSaturate(float3 _Color)
{
  float MaxXYZ = max(max(_Color.x, _Color.y), _Color.z);
  if (MaxXYZ < 1.0f) return _Color;

  return _Color / MaxXYZ;
}
  
float4 GetGouraudLightsColor(float4 _WorldPos, float3 _Normal)
{
  // Source: https://habr.com/ru/post/338254/

  float4 Result = float4(0.0f, 0.0f, 0.0f, 0.0f);

  if (LightsData.Ambient.IsValid == 1) // Ambient
  {
    Result += LightsData.Ambient.Color;
  }

  if (LightsData.Direction.IsValid == 1) // Direction
  {
    float3 Direction = normalize(ToFloat3(LightsData.Direction.Direction));
    Result += max(dot(_Normal, Direction), 0.0f) * LightsData.Direction.Color;
  }

  // Points

  int MaxCount = MAX_LIGHT_POINT_COUNT;
  int PointLightSlotCount = min(LightsData.Points.UsedSlotCount, MaxCount);

  for (int i = 0; i < PointLightSlotCount; i++)
  {
    Result += CalcPointLight(LightsData.Points.Lights[i], _WorldPos, _Normal);
  }

  return float4(SyncSaturate(ToFloat3(Result)), 1.0f);
}
  
// ************************************************************************** //
  
Pixel vsGui(Vertex _Value)
{
  float4x4 WorldViewProjection =
    mul(MatricesData.World, mul(MatricesData.View, MatricesData.Projection));

  Pixel Result;
  Result.ScreenPos = mul(_Value.Position, WorldViewProjection);
  Result.Color = _Value.Extra;
  Result.TexCoord = _Value.TexCoord;
  return Result;
}
  
Pixel vsLights(Vertex _Value)
{
  float4x4 WorldViewProjection =
    mul(MatricesData.World, mul(MatricesData.View, MatricesData.Projection));
  
  float3 Normal = normalize(
    mul(ToFloat3(_Value.Extra), ToMatrix3x3(MatricesData.World)));
  float4 WorldPos =
    mul(_Value.Position, MatricesData.World);

  Pixel Result;
  Result.ScreenPos = mul(_Value.Position, WorldViewProjection);
  Result.Color = GetGouraudLightsColor(WorldPos, Normal);
  Result.Normal = Normal;
  Result.TexCoord = _Value.TexCoord;
  Result.WorldPos = WorldPos;
  return Result;
}
  
#elif defined COVELLITE_SHADER_PIXEL ///////////////////////////////////////////
  
COVELLITE_DECLARE_TEX2D(TexDiffuse, 0);
  
float4 psColored(Pixel _Value)
{
  return _Value.Color;
}
  
float4 psTextured(Pixel _Value)
{
  return COVELLITE_TEX2D_COLOR(TexDiffuse, _Value.TexCoord) * _Value.Color;
}
  
#endif /////////////////////////////////////////////////////////////////////////
