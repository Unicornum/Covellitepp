
#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////
  
// ************************************************************************** //
  
float4 GetAmbientDirectionColor(float3 _Normal)
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
  
Pixel vsFlat(Vertex _Value)
{
  float4x4 WorldViewProjection =
    mul(ObjectData.World, mul(CameraData.View, CameraData.Projection));
    
  Pixel Result;
  Result.ScreenPos = mul(_Value.Position, WorldViewProjection);
  Result.Color = _Value.Extra;
  Result.Normal = float3(0.0f, 0.0f, 1.0f);
  Result.TexCoord = _Value.TexCoord;
  Result.WorldPos = float4(0.0f, 0.0f, 0.0f, 1.0f);
  return Result;
}
  
Pixel vsVolume(Vertex _Value)
{
  float4x4 WorldViewProjection =
    mul(ObjectData.World, mul(CameraData.View, CameraData.Projection));
    
   float3 Normal = normalize(
    mul(ToFloat3(_Value.Extra), ToMatrix3x3(ObjectData.World)));
  float4 WorldPos = mul(_Value.Position, ObjectData.World);
    
  Pixel Result;
  Result.ScreenPos = mul(_Value.Position, WorldViewProjection);
  Result.Color = GetAmbientDirectionColor(Normal);
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
  
float4 psLightened(Pixel _Value)
{
  float4 LightColor = _Value.Color; // Ambient + Direction
    
  for (int i = 0; i < ObjectData.Lights.Points.UsedSlotCount; i++)
  {
    LightColor += CalcPointLight(ObjectData.Lights.Points.Lights[i],
      _Value.WorldPos, _Value.Normal);
  }
    
  return COVELLITE_TEX2D_COLOR(TexDiffuse, _Value.TexCoord) *
    float4(SyncSaturate(ToFloat3(LightColor)), 1.0f);
}
  
#endif /////////////////////////////////////////////////////////////////////////
