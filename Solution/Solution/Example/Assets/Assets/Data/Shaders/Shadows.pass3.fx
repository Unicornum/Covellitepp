
struct Cursor
{
  float4 Position;
};

COVELLITE_DECLARE_CONST_USER_BUFFER(Cursor, cbCursor, oCursor);

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
COVELLITE_DECLARE_TEX2D(TexMetalness, 1);
COVELLITE_DECLARE_TEX2D(TexRoughness, 2);
COVELLITE_DECLARE_TEX2D(TexDepth, 5);

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
  float3 Coefficients = float3(0.21f, 0.72f, 0.07f);
  float Grey = dot(Coefficients, Color);
  return float4(Grey, Grey, Grey, 1.0f);
}

float3 GetTexColor(float2 _TexCoords)
{
  float R = COVELLITE_TEX2D_COLOR(TexAlbedo, _TexCoords).r;
  float G = COVELLITE_TEX2D_COLOR(TexMetalness, _TexCoords).g;
  float B = COVELLITE_TEX2D_COLOR(TexRoughness, _TexCoords).b;
  return float3(R, G, B);
}

COVELLITE_DECLARE_STATIC const float PI = 3.14159265359f;
COVELLITE_DECLARE_STATIC const float E = 2.71828182846f;
COVELLITE_DECLARE_STATIC const float SAMPLES = 4.0f;
COVELLITE_DECLARE_STATIC const float stDevSquared2 = 2.0f * 0.25f * 0.25f;
COVELLITE_DECLARE_STATIC const float BlurSize = 7.5f / 1024.0f;

float4 SharpnessOfDepth(Pixel _Value)
{
  float2 TexCoord = ConvertTexCoord(_Value.TexCoord);

  float Depth = 0.125f * LinearDepthFromNearToFar(
    COVELLITE_TEX2D_COLOR(TexDepth, TexCoord).r, 20.0f, 0.5f);
  if (Depth > 1.0f) Depth = 1.0f;
  //return float4(Depth, Depth, Depth, 1.0f);

  // blur source: https://www.ronja-tutorials.com/2018/08/27/postprocessing-blur.html

  float3 Color = GetTexColor(TexCoord);
  float3 BlurColor = float3(0.0f, 0.0f, 0.0f);

  float sum = 0.0f;

  for (float i = 0.0f; i < SAMPLES; i++)
  {
    float offset = (i / (SAMPLES - 1.0f) - 0.5f) * BlurSize;
    float2 uv = float2(0.0f, offset);

    float gauss = (1.0f / sqrt(PI * stDevSquared2)) * 
      pow(E, -(offset * offset / stDevSquared2));
    sum += gauss;
    BlurColor += GetTexColor(TexCoord + uv) * gauss;
  }

  //return float4(BlurColor / sum, 1.0f);

  // source: https://habr.com/ru/post/453692/

  float focalLengthSharpness = 10.0f;
  float blurRate = 6.0f;
  float blur = clamp(pow(blurRate, Depth) / focalLengthSharpness, 0.0f, 1.0f);

  return float4(lerp(Color, BlurColor / sum, blur), 1.0f);
}

float4 psExperimental(Pixel _Value)
{
  float4 Result = SharpnessOfDepth(_Value);

  float2 CurrentPixelObjectId = 
    COVELLITE_TEX2D_COLOR(TexAlbedo, ConvertTexCoord(_Value.TexCoord)).gb;
  float2 CursorPixelObjectId = 
    COVELLITE_TEX2D_COLOR(TexAlbedo, ConvertTexCoord(oCursor.Position.xy)).gb;

  if (CurrentPixelObjectId.x == CursorPixelObjectId.x &&
    CurrentPixelObjectId.y == CursorPixelObjectId.y)
  {
    Result += float4(0.2f, 0.2f, 0.2f, 0.0f);
  }

  return Result;
}

float4 psSimpleShadowDept(Pixel _Value)
{
  float RawDepth = COVELLITE_TEX2D_COLOR(TexDepth, 
    ConvertTexCoord(_Value.TexCoord)).r;
  return CalculateColorFromRawDept(RawDepth, 95.0f, 105.0f, 200.0f, 90.0f);
}

float4 psSimpleSceneDept(Pixel _Value)
{
  float RawDepth = COVELLITE_TEX2D_COLOR(TexDepth, 
    ConvertTexCoord(_Value.TexCoord)).r;
  return CalculateColorFromRawDept(RawDepth, 2.0f, 11.0f, 20.0f, 0.5f);
}

float4 psSimpleTexture(Pixel _Value)
{
  return float4(COVELLITE_TEX2D_COLOR(TexAlbedo, 
    ConvertTexCoord(_Value.TexCoord)).rgb, 1.0f);
}

#endif
