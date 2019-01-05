
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

float4 CalcPointLight(Light::Point _Light, float4 _Position, float3 _Normal)
{
  float4 Direction = float4(_Light.Position, 1.0f) - _Position;

  float LightFactor = dot(_Normal, Direction);
  if (LightFactor < 0) return float4(0.0f, 0.0f, 0.0f, 0.0f);

  float4 Color = GetRGBAColor(_Light.Color.ARGBDiffuse);
  float Distance = length(Direction);

  float Attenuation = _Light.Attenuation.x +
    (_Light.Attenuation.y + _Light.Attenuation.z * Distance) * Distance;

  return Color / Attenuation;
}

float3 SyncSaturate(float3 _Color)
{
  float MaxXYZ = max(max(_Color.x, _Color.y), _Color.z);
  if (MaxXYZ < 1.0f) return _Color;

  return _Color / MaxXYZ;
}

float4 psLighted(PixelInput _Value, bool _IsFrontFace : SV_IsFrontFace) : SV_Target
{
  if (!_IsFrontFace) discard;

  float3 Color = float3(0.0f, 0.0f, 0.0f);

  // Ambient

  float4 AmbientColor = GetRGBAColor(LightsData.Ambient.Color.ARGBAmbient);

  Color += AmbientColor.xyz;

  // Direction

  float4 DirectColor = GetRGBAColor(LightsData.Direction.Color.ARGBDiffuse);
  float lightIntensity = saturate(dot(_Value.Normal, LightsData.Direction.Direction));

  Color += DirectColor.xyz * lightIntensity;

  // Points

  uint PointLightSlotCount = 
    min(LightsData.Points.UsedSlotCount, MAX_LIGHT_POINT_COUNT);

  for (uint i = 0; i < PointLightSlotCount; i++)
  {
    Color += 
      CalcPointLight(LightsData.Points.Lights[i], _Value.WorldPos, _Value.Normal).xyz;
  }

  // Result

  return diffuseMap.Sample(diffuseSampler, _Value.TexCoord) * 
    float4(SyncSaturate(Color), 1.0f);
}
