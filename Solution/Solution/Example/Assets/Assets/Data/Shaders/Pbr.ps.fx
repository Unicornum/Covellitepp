
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

COVELLITE_DECLARE_CONST_USER_BUFFER(Lights_t, cbLights, Lights);

// ************************************************************************** //

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

float4 GetLightsColor(float4 _WorldPos, float3 _Normal)
{
  // Source: https://habr.com/ru/post/338254/

  float4 Result = float4(0.0f, 0.0f, 0.0f, 0.0f);

  if (Lights.Ambient.IsValid == 1) // Ambient
  {
    Result += Lights.Ambient.Color;
  }

  if (Lights.Direction.IsValid == 1) // Direction
  {
    float3 Direction = normalize(ToFloat3(Lights.Direction.Direction));
    Result += max(dot(_Normal, Direction), 0.0f) *
      Lights.Direction.Color;
  }

  for (int i = 0; i < Lights.Points.UsedSlotCount; i++)
  {
    Result += CalcPointLight(
      Lights.Points.Lights[i], _WorldPos, _Normal);
  }

  return float4(SyncSaturate(Result.rgb), 1.0f);
}

// ************************************************************************** //

COVELLITE_DECLARE_TEX2D(TexAlbedo, 0);
COVELLITE_DECLARE_TEX2D(TexReflection, 1);
COVELLITE_DECLARE_TEX2D(TexNormal, 2);
COVELLITE_DECLARE_TEX2D(TexEnvironment, 3);

float4 psSimple(Pixel _Value)
{
  return COVELLITE_TEX2D_COLOR(TexAlbedo, _Value.TexCoord) *
    GetLightsColor(_Value.WorldPos, _Value.Normal);
}

float3x3 cotangent_frame(float3 N, float3 p, float2 uv)
{
  // get edge vectors of the pixel triangle
  float3 dp1 = ddx(p);
  float3 dp2 = ddy(p);
  float2 duv1 = ddx(uv);
  float2 duv2 = ddy(uv);

  // solve the linear system
  float3 dp2perp = cross(dp2, N);
  float3 dp1perp = cross(N, dp1);
  float3 T = dp2perp * duv1.x + dp1perp * duv2.x;
  float3 B = dp2perp * duv1.y + dp1perp * duv2.y;

  // construct a scale-invariant frame 
  float invmax = rsqrt(max(dot(T, T), dot(B, B)));
  return float3x3(T * invmax, B * invmax, N);
}

float3 perturb_normal(float3 N, float3 V, float2 texcoord)
{
  float3x3 TBN = cotangent_frame(N, -V, texcoord);

  // assume N, the interpolated vertex normal and 
  // V, the view vector (vertex to eye)
  float3 map = COVELLITE_TEX2D_COLOR(TexNormal, texcoord).xyz * 2.0f - 1.0f;
  //map = float3(0.0f, 0.0f, 1.0f); // Flat normal.

# ifdef COVELLITE_SHADER_HLSL
  return normalize(mul(map, TBN));
# elif defined COVELLITE_SHADER_GLSL
  return normalize(mul(TBN, map));
# endif
}

float4 psPbrSimple(Pixel _Value)
{
  // source: 
  // http://orsvarn.com/entry/pbr-shader-source/
  // https://www.jordanstevenstechart.com/physically-based-rendering

  float3 CameraPosition = COVELLITE_MATRIX_ROW(CameraData.ViewInverse, 3).xyz;

  float3 camToPixelRaw = _Value.WorldPos.xyz - CameraPosition;
  float3 camToPixel = normalize(camToPixelRaw);

  // Calculate normal
  float3 normal = perturb_normal(_Value.Normal, camToPixelRaw, _Value.TexCoord);
  float3 camNormalReflect = reflect(camToPixel, -normal);

  float3 DiffuseLightColor =
    GetLightsColor(_Value.WorldPos, normal).rgb;

  float3 BaseColorSample = COVELLITE_TEX2D_COLOR(TexAlbedo, _Value.TexCoord).rgb;
  float RoughnessSample = COVELLITE_TEX2D_COLOR(TexReflection, _Value.TexCoord).g;
  float OcclusionSample = COVELLITE_TEX2D_COLOR(TexReflection, _Value.TexCoord).b;

  float3 SpecularLightColor =
    GetLightsColor(_Value.WorldPos, camNormalReflect).rgb;

  float3 BaseColorSpecular = COVELLITE_TEX2D_COLOR(TexEnvironment, camNormalReflect.xy).rgb;
  float MetalnessSample = COVELLITE_TEX2D_COLOR(TexReflection, _Value.TexCoord).r;

  float3 PixelColor = BaseColorSample * OcclusionSample *
    SyncSaturate(DiffuseLightColor * RoughnessSample * RoughnessSample +
    SpecularLightColor * BaseColorSpecular * MetalnessSample);

  return float4(PixelColor, 1.0f);
}
