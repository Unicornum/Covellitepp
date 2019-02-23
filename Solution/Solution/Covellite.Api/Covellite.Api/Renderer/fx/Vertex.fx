
Pixel VS(Polygon _Value)
{
  Pixel Result = (Pixel)0;

  float4x4 WorldViewProj = mul(MatricesData.World, 
    mul(MatricesData.View, MatricesData.Projection));
  float4 Pos = float4(_Value.Position, 0.0f, 1.0f);

  float A = ((_Value.Color & 0xFF000000) >> 24) / 255.0f;
  float B = ((_Value.Color & 0x00FF0000) >> 16) / 255.0f;
  float G = ((_Value.Color & 0x0000FF00) >> 8) / 255.0f;
  float R = (_Value.Color & 0x000000FF) / 255.0f;

  Result.ScreenPos = mul(Pos, WorldViewProj);
  Result.Color = float4(R, G, B, A);
  Result.TexCoord = _Value.TexCoord;

  return Result;
}

float4 CalcPointLight(Light::Point _Light, float4 _Position, float3 _Normal)
{
  float4 Direction = _Light.Position - _Position;

  float LightFactor = max(dot(_Normal, normalize(Direction.xyz)), 0.0f);
  float4 Color = GetRGBAColor(_Light.ARGBColor) * LightFactor;
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

  float4 AmbientColor = GetRGBAColor(LightsData.Ambient.ARGBColor);

  // Direction

  float3 Direction = normalize(LightsData.Direction.Direction.xyz);
  float4 DirectColor = GetRGBAColor(LightsData.Direction.ARGBColor) * 
    max(dot(_Normal, Direction), 0.0f);

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

Pixel vsTextured(Polyhedron _Value)
{
  Pixel Result = (Pixel)0;

  float4x4 WorldViewProj = mul(MatricesData.World, 
    mul(MatricesData.View, MatricesData.Projection));
  float4 Pos = float4(_Value.Position, 1.0f);
  float3 Normal = normalize(mul(_Value.Normal, (float3x3)MatricesData.World));
  float4 WorldPos = mul(Pos, MatricesData.World);

  Result.ScreenPos = mul(Pos,  WorldViewProj);
  Result.Color = GetGouraudLightsColor(WorldPos, Normal);
  Result.Normal = Normal;
  Result.TexCoord = _Value.Texture;
  Result.WorldPos = WorldPos;

  return Result;
}
