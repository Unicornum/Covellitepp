
#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////
  
// ************************************************************************** //
  
Pixel vsFlat(Vertex _Value)
{
  float4x4 ViewProjection = mul(CameraData.View, CameraData.Projection);
  float4 WorldPos = mul(_Value.Position, ObjectData.World);

  Pixel Result;
  Result.ScreenPos = mul(WorldPos, ViewProjection);
  Result.Color = _Value.Extra;
  Result.Normal = float3(0.0f, 0.0f, 1.0f);
  Result.TexCoord = _Value.TexCoord;
  Result.WorldPos = float4(0.0f, 0.0f, 0.0f, 1.0f);
  return Result;
}
  
Pixel vsVolume(Vertex _Value)
{
  float4x4 ViewProjection = mul(CameraData.View, CameraData.Projection);
  float4 WorldPos = mul(_Value.Position, ObjectData.World);

  Pixel Result;
  Result.ScreenPos = mul(WorldPos, ViewProjection);
  Result.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
  Result.Normal = normalize(mul(ToFloat3(_Value.Extra), ToMatrix3x3(ObjectData.World)));
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
