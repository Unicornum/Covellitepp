
#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////

#ifdef COVELLITE_SHADER_HLSL // ********************************************* //

float4 GetColorFromABGR(uint _ABGR)
{
  return float4(
    (_ABGR & 0x000000FF) / 255.0f,
    ((_ABGR & 0x0000FF00) >> 8) / 255.0f,
    ((_ABGR & 0x00FF0000) >> 16) / 255.0f,
    ((_ABGR & 0xFF000000) >> 24) / 255.0f
    );
}

float4 CalcPointLight(Point_t _Light, float4 _Position, float3 _Normal)
{
  float4 Direction = _Light.Position - _Position;

  float LightFactor = max(dot(_Normal, normalize(Direction.xyz)), 0.0f);
  float4 Color = LightFactor * _Light.Color;
  float Distance = length(Direction);

  float Attenuation =
    _Light.Attenuation.x +
    _Light.Attenuation.y * Distance +
    _Light.Attenuation.z * Distance * Distance;

  return Color / Attenuation;
}

float4 SyncSaturate(float4 _Color)
{
  float MaxXYZ = max(max(_Color.x, _Color.y), _Color.z);
  if (MaxXYZ < 1.0f) return _Color;

  return _Color / MaxXYZ;
}

float4 GetGouraudLightsColor(float4 _WorldPos, float3 _Normal)
{
  // Полноценную реализацию, включающую specular и материал, см
  // https://habr.com/ru/post/338254/

  // Ambient

  float4 AmbientColor = LightsData.Ambient.Color;

  // Direction

  float3 Direction = normalize(LightsData.Direction.Direction.xyz);
  float4 DirectColor = 
    max(dot(_Normal, Direction), 0.0f) * LightsData.Direction.Color;

  // Points

  uint PointLightSlotCount =
    min(LightsData.Points.UsedSlotCount, MAX_LIGHT_POINT_COUNT);

  float4 PointsColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

  for (uint i = 0; i < PointLightSlotCount; i++)
  {
    PointsColor +=
      CalcPointLight(LightsData.Points.Lights[i], _WorldPos, _Normal);
  }

  // Result

  return saturate(AmbientColor + DirectColor + PointsColor);
}

#elif defined COVELLITE_SHADER_GLSL // ************************************** //

#define GetColorFromABGR(_Color) _Color

float4 GetGouraudLightsColor(float4 _WorldPos, float3 _Normal)
{
  return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

#endif // ******************************************************************* //

Pixel VS(Polygon _Value)
{
  float4x4 WorldViewProj =
    mul(MatricesData.World, mul(MatricesData.View, MatricesData.Projection));

  Pixel Result;
  Result.ScreenPos = mul(float4(_Value.Position, 0.0f, 1.0f), WorldViewProj);
  Result.Color = GetColorFromABGR(_Value.Color);
  Result.TexCoord = _Value.TexCoord;
  return Result;
}

Pixel vsTextured(Polyhedron _Value)
{
  float4x4 WorldViewProj =
    mul(MatricesData.World, mul(MatricesData.View, MatricesData.Projection));
  float3 Normal =
    normalize(mul(_Value.Normal, ToMatrix3x3(MatricesData.World)));
  float4 WorldPos =
    mul(float4(_Value.Position, 1.0f), MatricesData.World);

  Pixel Result;
  Result.ScreenPos = mul(float4(_Value.Position, 1.0f), WorldViewProj);
  Result.Color = GetGouraudLightsColor(WorldPos, Normal);
  Result.Normal = Normal;
  Result.TexCoord = _Value.TexCoord;
  Result.WorldPos = WorldPos;
  return Result;
}

#endif /////////////////////////////////////////////////////////////////////////
