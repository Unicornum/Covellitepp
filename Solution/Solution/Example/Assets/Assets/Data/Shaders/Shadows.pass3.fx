
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

struct Light_s
{
  float4x4 View;
  float4x4 Projection;
};

struct Cursor_s
{
  float4 Position;
};

struct ShaderData
{
  Light_s  Light;
  Cursor_s Cursor;
  Lights_t Lights;
};

COVELLITE_DECLARE_CONST_USER_BUFFER(ShaderData, cbShaderData, oShaderData);

#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////

Pixel vsFlat(Vertex _Value)
{
  float4x4 WorldViewProjection =
    mul(ObjectData.World, mul(CameraData.View, CameraData.Projection));

  Pixel Result;
  Result.ScreenPos = mul(_Value.Position, WorldViewProjection);
  Result.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
  Result.Normal = float3(0.0f, 0.0f, 1.0f);
  Result.TexCoord = _Value.TexCoord;
  Result.WorldPos = float4(0.0f, 0.0f, 0.0f, 1.0f);
  return Result;
}

#elif defined COVELLITE_SHADER_PIXEL ///////////////////////////////////////////

COVELLITE_DECLARE_TEX2D(TexAlbedo, 0);
COVELLITE_DECLARE_TEX2D(TexNormal, 1);
COVELLITE_DECLARE_TEX2D(TexWorldPos, 2);
COVELLITE_DECLARE_TEX2D(TexObjectId, 3);
COVELLITE_DECLARE_TEX2D(TexSceneDepth, 4);
COVELLITE_DECLARE_TEX2D(TexShadowDepth, 5);

// ************************************************************************** //

float ConvertDeptToScreenZ(float _Depth)
{
#ifdef COVELLITE_SHADER_HLSL
  return _Depth;
#elif defined COVELLITE_SHADER_GLSL
  return _Depth * 2.0f - 1.0f;
#endif
}

float2 ConvertTexCoord(float2 _UV)
{
#ifdef COVELLITE_SHADER_HLSL
  return _UV;
#elif defined COVELLITE_SHADER_GLSL
  return float2(_UV.x, 1.0f - _UV.y);
#endif
}

float LinearDepthFromNearToFar(float _RawDepth, float _zNear, float _zFar)
{
  float A = -_zFar / (_zFar - _zNear);
  float B = _zNear * A;

  // gl_FragDepth  = 0.5*(-A*depth + B) / depth + 0.5;
  return B / (A + _RawDepth);
}

float LinearDepthFromNearToFar(float _RawDepth, float4x4 _Projection)
{
  float A = COVELLITE_MATRIX_ROW(_Projection, 2).z;
  float B = COVELLITE_MATRIX_ROW(_Projection, 3).z;

  // gl_FragDepth  = 0.5*(-A*depth + B) / depth + 0.5;
  return B / (A + _RawDepth);
}

float4 CalculateColorFromDept(float _Depth, float _Min, float _Max)
{
  float Depth = _Depth;

  if (Depth < _Min) return float4(0.0f, 1.0f, 0.0f, 1.0f);
  if (Depth > _Max) return float4(1.0f, 0.0f, 0.0f, 1.0f);

  Depth = (Depth - _Min) / (_Max - _Min);
  return float4(Depth, Depth, Depth, 1.0f);
}

float4 CalculateColorFromRawDept(float _RawDepth, float _Min, float _Max,
  float _zNear, float _zFar)
{
  float Depth = LinearDepthFromNearToFar(
    ConvertDeptToScreenZ(_RawDepth), _zNear, _zFar);
  return CalculateColorFromDept(Depth, _Min, _Max);
}

// ************************************************************************** //

float4 psMonochrome(Pixel _Value)
{
  float3 Color = COVELLITE_TEX2D_COLOR(TexAlbedo, _Value.TexCoord).rgb;
  float Grey = dot(float3(0.21f, 0.72f, 0.07f), Color);
  return float4(Grey, Grey, Grey, 1.0f);
}

COVELLITE_DECLARE_STATIC const float PI = 3.14159265359f;
COVELLITE_DECLARE_STATIC const float E = 2.71828182846f;
COVELLITE_DECLARE_STATIC const float SAMPLES = 4.0f;
COVELLITE_DECLARE_STATIC const float stDevSquared2 = 2.0f * 0.25f * 0.25f;
COVELLITE_DECLARE_STATIC const float BlurSize = 7.5f / 1024.0f;

int IsSkyPixel(float3 _Color)
{
  return (_Color.r == 0.0f && _Color.g == 0.0f && _Color.b == 1.0f) ? 1 : 0;
}

float4 GetGouraudLightsColor(float3 _Normal)
{
  // Source: https://habr.com/ru/post/338254/

  float4 Result = float4(0.0f, 0.0f, 0.0f, 0.0f);

  if (oShaderData.Lights.Ambient.IsValid == 1) // Ambient
  {
    Result += oShaderData.Lights.Ambient.Color;
  }

  if (oShaderData.Lights.Direction.IsValid == 1) // Direction
  {
    float3 Direction =
      normalize(oShaderData.Lights.Direction.Direction.xyz);
    Result +=
      max(dot(_Normal, Direction), 0.0f) * oShaderData.Lights.Direction.Color;
  }

  return Result;
}

float3 SharpnessOfDepth(float2 _TexCoord)
{
  float3 Color = COVELLITE_TEX2D_COLOR(TexAlbedo, _TexCoord).rgb;
  float Depth = 0.125f * LinearDepthFromNearToFar(
    COVELLITE_TEX2D_COLOR(TexSceneDepth, _TexCoord).r, 20.0f, 0.5f);
  if (Depth > 1.0f) Depth = 1.0f;

  // blur source: https://www.ronja-tutorials.com/2018/08/27/postprocessing-blur.html

  float3 BlurColor = float3(0.0f, 0.0f, 0.0f);

  float sum = 0.0f;

  for (float i = 0.0f; i < SAMPLES; i++)
  {
    float offset = (i / (SAMPLES - 1.0f) - 0.5f) * BlurSize;
    float2 uv = float2(0.0f, offset);

    float3 Color = COVELLITE_TEX2D_COLOR(TexAlbedo, _TexCoord + uv).rgb;
    if (IsSkyPixel(Color) == 0)
    {
      float gauss = (1.0f / sqrt(PI * stDevSquared2)) *
        pow(E, -(offset * offset / stDevSquared2));
      sum += gauss;
      BlurColor += Color * gauss;
    }
  }

  //return float4(BlurColor / sum, 1.0f);

  // source: https://habr.com/ru/post/453692/

  float focalLengthSharpness = 10.0f;
  float blurRate = 6.0f;
  float blur = clamp(pow(blurRate, Depth) / focalLengthSharpness, 0.0f, 1.0f);

  return lerp(Color, BlurColor / sum, blur);
}

float4 psSimpleShadowDept(Pixel _Value)
{
  float RawDepth = COVELLITE_TEX2D_COLOR(TexShadowDepth,
    ConvertTexCoord(_Value.TexCoord)).r;

  float Depth = LinearDepthFromNearToFar(
    ConvertDeptToScreenZ(RawDepth), oShaderData.Light.Projection);
  return CalculateColorFromDept(Depth, 95.0f, 105.0f);
}

float4 psSimpleSceneDept(Pixel _Value)
{
  float RawDepth = COVELLITE_TEX2D_COLOR(TexSceneDepth,
    ConvertTexCoord(_Value.TexCoord)).r;
  return CalculateColorFromRawDept(RawDepth, 2.0f, 11.0f, 20.0f, 0.5f);
}

float4 psSimpleTexture(Pixel _Value)
{
  //return float4(COVELLITE_TEX2D_COLOR(TexAlbedo,
  //return float4(COVELLITE_TEX2D_COLOR(TexNormal,
  return float4(COVELLITE_TEX2D_COLOR(TexWorldPos,
  //return -12.7f + 25.5f * float4(COVELLITE_TEX2D_COLOR(TexObjectId,
    ConvertTexCoord(_Value.TexCoord)).rgb, 1.0f);
}

float3 Shadow(float3 _WorldPos, float3 _Normal)
{
  float4 LightColor = GetGouraudLightsColor(_Normal);

  float4 LightPos = mul(float4(_WorldPos, 1.0f), 
    mul(oShaderData.Light.View, oShaderData.Light.Projection));

  //re-homogenize position after interpolation
  LightPos.xyz /= LightPos.w;

  //if position is not visible to the light - dont illuminate it
  //results in hard light frustum
  if (LightPos.x < -1.0f || LightPos.x > 1.0f ||
    LightPos.y < -1.0f || LightPos.y > 1.0f ||
    LightPos.z < 0.0f || LightPos.z > 1.0f)
  {
    return 
      //float3(1.0f, 0.0f, 0.0f) * 
      LightColor.rgb;
  }

  //transform clip space coords to texture space coords (-1:1 to 0:1)
  LightPos.x = 0.5f * LightPos.x + 0.5f;
  LightPos.y = 0.5f * LightPos.y + 0.5f;
#ifdef COVELLITE_SHADER_HLSL
  LightPos.y = 1.0f - LightPos.y;
#endif

  float3 normal = normalize(_Normal);
  float3 lightDir = normalize(oShaderData.Lights.Direction.Direction.xyz);

  float bias = 0.15f * (1.0f + dot(normal, lightDir));

  float DepthFromLight =
    LinearDepthFromNearToFar(LightPos.z, oShaderData.Light.Projection) - bias;

  const float texelSize = 3.0f / 2048.0f;

  float4 SoftShadow = float4(0.0f, 0.0f, 0.0f, 0.0f);
  float4 ShadowColor = oShaderData.Lights.Ambient.Color;

  for (int x = -1; x <= 1; ++x)
  {
    for (int y = -1; y <= 1; ++y)
    {
      float2 TexCoord = LightPos.xy +
        clamp(float2(x, y), 0.0f, 1.0f) * texelSize;
      float RawDepth = COVELLITE_TEX2D_COLOR(TexShadowDepth, TexCoord).r;
      float DepthFromShadowMap = LinearDepthFromNearToFar(
        ConvertDeptToScreenZ(RawDepth), oShaderData.Light.Projection);

      SoftShadow += 
        (DepthFromLight > DepthFromShadowMap) ? ShadowColor : LightColor;
    }
  }

  return SoftShadow.rgb / 9.0f;
}

float4 psExperimental(Pixel _Value)
{
  //return psSimpleTexture(_Value);
  //return psSimpleSceneDept(_Value);
  //return psSimpleShadowDept(_Value);

  float2 TexCoord = ConvertTexCoord(_Value.TexCoord);
  float3 Albedo = COVELLITE_TEX2D_COLOR(TexAlbedo, TexCoord).rgb;
  if (IsSkyPixel(Albedo) == 1) discard;

  float4 WorldPos = 
    10.0f * (COVELLITE_TEX2D_COLOR(TexWorldPos, TexCoord) - 0.5f);
  float3 Normal = 
    2.0f * COVELLITE_TEX2D_COLOR(TexNormal, TexCoord).xyz - 1.0f;
  float3 Result = 
    SharpnessOfDepth(TexCoord) *
    Shadow(WorldPos.xyz, Normal);

  float2 CurrentPixelObjectId = COVELLITE_TEX2D_COLOR(TexObjectId, TexCoord).xy;
  float2 CursorPixelObjectId = COVELLITE_TEX2D_COLOR(TexObjectId, 
    ConvertTexCoord(oShaderData.Cursor.Position.xy)).xy;

  if (CurrentPixelObjectId.x == CursorPixelObjectId.x &&
    CurrentPixelObjectId.y == CursorPixelObjectId.y)
  {
    Result += float3(0.2f, 0.2f, 0.2f);
  }

  return float4(Result, 1.0f);
}

#endif
