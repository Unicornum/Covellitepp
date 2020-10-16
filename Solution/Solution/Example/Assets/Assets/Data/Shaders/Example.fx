
#define COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT 8

struct Ambient_t
{
  float4 Color;
  int IsValid, align1, align2, align3;
};

struct Direction_t
{
  float4 Color;
  float4 Direction;
  int IsValid, align1, align2, align3;
};

struct Point_t
{
  float4 Color;
  float4 Position;
  float4 Attenuation; // Const, Linear, Exponent, Radius;
};

struct Points_t
{
  Point_t Lights[COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT];
  int     UsedSlotCount;
};

struct Lights_t
{
  Ambient_t   Ambient;
  Direction_t Direction;
  Points_t    Points;
};

struct FogRecord
{
  float4 Color;
  float Near;
  float Far;
  float Density;
  float Dummy;
};

struct UserConstantBuffer
{
  FogRecord Fog;
  Lights_t  Lights;
};

COVELLITE_DECLARE_CONST_USER_BUFFER(UserConstantBuffer, cbUserData, UserData);

#ifdef COVELLITE_SHADER_PIXEL //////////////////////////////////////////////////

COVELLITE_DECLARE_TEX2D(TexDiffuse, 0);

float4 DiscardTransparentPixel(Pixel _Value, float4 _Color)
{
  if (_Color.a < 0.25f) discard;
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
  float4 Result = float4(0.0f, 0.0f, 0.0f, 0.0f);

  if (UserData.Lights.Ambient.IsValid == 1) // Ambient
  {
    Result += UserData.Lights.Ambient.Color;
  }

  if (UserData.Lights.Direction.IsValid == 1) // Direction
  {
    float3 Direction = normalize(ToFloat3(UserData.Lights.Direction.Direction));
    Result += max(dot(_Value.Normal, Direction), 0.0f) *
      UserData.Lights.Direction.Color;
  }

  // Points

  for (int i = 0; i < UserData.Lights.Points.UsedSlotCount; i++)
  {
    Result += CalcPointLight(UserData.Lights.Points.Lights[i],
      _Value.WorldPos, _Value.Normal);
  }

  Result = float4(SyncSaturate(Result.rgb), 1.0f);

  return Result * _Color;
}

float CalculateFogLinear(float _Distance)
{
  return saturate((UserData.Fog.Far - _Distance) / (UserData.Fog.Far - UserData.Fog.Near));
}

float CalculateFogExp(float _Distance)
{
  return 1.0f / pow(2.71828f, _Distance * UserData.Fog.Density);
}

float CalculateFogExp2(float _Distance)
{
  float Temp = _Distance * UserData.Fog.Density;
  return 1.0f / pow (2.71828f, Temp * Temp);
}

float4 CalculateFog(Pixel _Value, float4 _Color)
{
  float3 CameraPosition = COVELLITE_MATRIX_ROW(CameraData.ViewInverse, 3).xyz;

  float Distance = length(_Value.WorldPos.xyz - CameraPosition);
  float FogFactor = CalculateFogLinear(Distance);
  return FogFactor * _Color + (1.0f - FogFactor) * UserData.Fog.Color;
}

float4 psTextured(Pixel _Value)
{
  return COVELLITE_TEX2D_COLOR(TexDiffuse, _Value.TexCoord);
}

float4 psLight(Pixel _Value)
{
  float4 Color = COVELLITE_TEX2D_COLOR(TexDiffuse, _Value.TexCoord);
  Color = CalculateLights(_Value, Color);
  return Color;
}

float4 psExample(Pixel _Value)
{
  float4 Color = COVELLITE_TEX2D_COLOR(TexDiffuse, _Value.TexCoord);

  Color = DiscardTransparentPixel(_Value, Color);
  Color = CalculateLights(_Value, Color);
  Color = CalculateFog(_Value, Color);

  return float4(Color.rgb, 1.0f);
}

#endif /////////////////////////////////////////////////////////////////////////
