
struct UserPointLights
{
  Point_t Lights[8];
  int     UsedSlotCount;
};

COVELLITE_DECLARE_CONST_USER_BUFFER(UserPointLights, cbUserData, UserData);

#ifdef COVELLITE_SHADER_PIXEL //////////////////////////////////////////////////

COVELLITE_DECLARE_TEX2D(TexDiffuse, 0);

float4 DiscardTransparentPixel(Pixel _Value, float4 _Color)
{
  if (_Color.a < 0.5f) discard;
  return _Color;
}

float4 CalcPointLight(Point_t _Light, float4 _Position, float3 _Normal)
{
  float3 Direction = _Light.Position.xyz - _Position.xyz;

  float LightFactor = dot(_Normal, normalize(Direction));
  if (LightFactor <= 0.0f) return float4(0.0f, 0.0f, 0.0f, 1.0f);

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

float4 CalculateLights(Pixel _Value, float4 _Color)
{
  float4 LightColor = _Value.Color; // Ambient + Direction

  // Points

  for (int i = 0; i < UserData.UsedSlotCount; i++)
  {
    LightColor += CalcPointLight(UserData.Lights[i],
      _Value.WorldPos, _Value.Normal);
  }

  LightColor = float4(SyncSaturate(LightColor.rgb), 1.0f);

  return LightColor * _Color;
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
  float3 CameraPosition = COVELLITE_MATRIX_ROW(CameraData.ViewInverse, 3).xyz;

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

#endif /////////////////////////////////////////////////////////////////////////
