
#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////

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
  //Result.Color = GetGouraudLightsColor(WorldPos, Normal);
  return Result;
}

#endif /////////////////////////////////////////////////////////////////////////
