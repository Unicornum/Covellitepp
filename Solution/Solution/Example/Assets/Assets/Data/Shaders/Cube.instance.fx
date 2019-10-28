
#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////

// ************************************************************************** //

float4 GetGouraudLightsColor(float4 _WorldPos, float3 _Normal)
{
  // Source: https://habr.com/ru/post/338254/

  float4 Result = float4(0.0f, 0.0f, 0.0f, 0.0f);

  if (ObjectData.Lights.Ambient.IsValid == 1) // Ambient
  {
    Result += ObjectData.Lights.Ambient.Color;
  }

  if (ObjectData.Lights.Direction.IsValid == 1) // Direction
  {
    float3 Direction = normalize(ToFloat3(ObjectData.Lights.Direction.Direction));
    Result += max(dot(_Normal, Direction), 0.0f) *
      ObjectData.Lights.Direction.Color;
  }

  return Result;
}

// ************************************************************************** //

Pixel vsVolume(Vertex _Value)
{
  float4x4 ViewProjection = mul(CameraData.View, CameraData.Projection);

  float3 Normal = normalize(
    mul(ToFloat3(_Value.Extra), ToMatrix3x3(ObjectData.World)));
  float4 WorldPos = mul(_Value.Position, ObjectData.World) + _Value.iValue1;

  Pixel Result;
  Result.ScreenPos = mul(WorldPos, ViewProjection);
  Result.WorldPos = WorldPos;
  Result.Normal = Normal;
  Result.TexCoord = _Value.TexCoord;
  Result.Color = GetGouraudLightsColor(WorldPos, Normal);
  return Result;
}

#endif /////////////////////////////////////////////////////////////////////////
