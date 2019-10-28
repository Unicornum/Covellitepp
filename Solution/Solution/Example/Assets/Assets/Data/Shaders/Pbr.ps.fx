
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

float4 GetLightsColor(float4 _AmbientDirection, float4 _WorldPos, float3 _Normal)
{
  float4 Result = _AmbientDirection;

  for (int i = 0; i < ObjectData.Lights.Points.UsedSlotCount; i++)
  {
    Result += CalcPointLight(
      ObjectData.Lights.Points.Lights[i], _WorldPos, _Normal);
  }

  return float4(SyncSaturate(Result.rgb), 1.0f);
}

// ************************************************************************** //

// source: http://orsvarn.com/entry/pbr-shader-source/

COVELLITE_DECLARE_TEX2D(TexAlbedo, 0);
COVELLITE_DECLARE_TEX2D(TexMetalness, 1);
COVELLITE_DECLARE_TEX2D(TexRoughness, 2);
COVELLITE_DECLARE_TEX2D(TexNormal, 3);
COVELLITE_DECLARE_TEX2D(TexOcclusion, 4);

float4 psSimple(Pixel _Value)
{
  return COVELLITE_TEX2D_COLOR(TexAlbedo, _Value.TexCoord) *
    GetLightsColor(_Value.Color, _Value.WorldPos, _Value.Normal);
}

// Constants
COVELLITE_DECLARE_STATIC const float Pi = 3.14159265359f;
COVELLITE_DECLARE_STATIC const float OneDivPi = 1.0f / Pi;

// Lys constants
COVELLITE_DECLARE_STATIC const float k0 = 0.00098f, k1 = 0.9921f, fUserMaxSPow = 0.2425f;
COVELLITE_DECLARE_STATIC const float g_fMaxT = (exp2(-10.0 / sqrt(fUserMaxSPow)) - k0) / k1;
COVELLITE_DECLARE_STATIC const int nMipOffset = 0;

// Lys function, copy/pasted from 
// https://www.knaldtech.com/docs/doku.php?id=specular_lys
float GetSpecPowToMip(float fSpecPow, int nMips)
{
  // This line was added because ShaderTool destroys mipmaps.
  fSpecPow = 1.0f - pow(1.0f - fSpecPow, 8.0f);
  // Default curve - Inverse of Toolbag 2 curve with adjusted constants.
  float fSmulMaxT = (exp2(-10.0f / sqrt(fSpecPow)) - k0) / k1;
  return float(nMips - 1 - nMipOffset) * 
    (1.0f - clamp(fSmulMaxT / g_fMaxT, 0.0f, 1.0f));
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
  float3 CameraPosition = COVELLITE_MATRIX_ROW(CameraData.ViewInverse, 3).xyz;

  float3 camToPixelRaw = _Value.WorldPos.xyz - CameraPosition;
  float3 camToPixel = normalize(camToPixelRaw);

  // Calculate normal
  float3 normal = perturb_normal(_Value.Normal, camToPixelRaw, _Value.TexCoord);
  float3 camNormalReflect = reflect(camToPixel, -normal);

  float3 DiffuseLightColor =
    GetLightsColor(_Value.Color, _Value.WorldPos, normal).rgb;

  float3 BaseColorSample = COVELLITE_TEX2D_COLOR(TexAlbedo, _Value.TexCoord).rgb;
  float3 RoughnessSample = COVELLITE_TEX2D_COLOR(TexRoughness, _Value.TexCoord).rrr;
  float3 OcclusionSample = COVELLITE_TEX2D_COLOR(TexOcclusion, _Value.TexCoord).rrr;

  float3 SpecularLightColor =
    GetLightsColor(_Value.Color, _Value.WorldPos, camNormalReflect).rgb;

  float3 BaseColorSpecular = COVELLITE_TEX2D_COLOR(TexAlbedo, camNormalReflect.xy).rgb;
  float3 MetalnessSample = COVELLITE_TEX2D_COLOR(TexMetalness, _Value.TexCoord).rrr;

  float3 PixelColor = DiffuseLightColor *

    // !!! therefore Albedo, Roughness and Occlusion to one texture !!!
    (BaseColorSample * RoughnessSample * OcclusionSample) +

    SpecularLightColor * (BaseColorSpecular * MetalnessSample);

  return float4(PixelColor, 1.0f);
}
