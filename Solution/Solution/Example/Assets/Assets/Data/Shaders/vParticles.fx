
#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////

Pixel vsParticles(Vertex _Value)
{
  float4 LocalPosition = float4(
    _Value.Position.x,
    _Value.Position.y,
    0.0f, 
    1.0f);

  float4 WorldPos = mul(LocalPosition, ObjectData.World) + _Value.Extra;

  Pixel Result;
  Result.ScreenPos = mul(WorldPos, mul(CameraData.View, CameraData.Projection));
  Result.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
  Result.Normal = float3(0.0f, 0.0f, 1.0f);
  Result.TexCoord = _Value.TexCoord;
  Result.WorldPos = WorldPos;

  return Result;
}

#elif defined COVELLITE_SHADER_PIXEL ///////////////////////////////////////////

COVELLITE_DECLARE_TEX2D(TexDiffuse, 0);

float4 psParticles(Pixel _Value)
{
  float4 Color = COVELLITE_TEX2D_COLOR(TexDiffuse, _Value.TexCoord);
  if (Color.a < 0.05f) discard;

  return Color;
}

#endif /////////////////////////////////////////////////////////////////////////
