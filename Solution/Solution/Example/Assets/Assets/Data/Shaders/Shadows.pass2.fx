
struct Light_s
{
  float4x4 View;
  float4x4 Projection;
};

struct Object_s
{
  float IdX, IdY, dummy2, dummy3;
};

struct ShaderData
{
  Light_s  Light;
  Object_s Object;
};

COVELLITE_DECLARE_CONST_USER_BUFFER(ShaderData, cbShaderData, oShaderData);

#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////

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

Pixel vsScene(Vertex _Value)
{
  float4x4 WorldViewProjection = mul(ObjectData.World, 
    mul(CameraData.View, CameraData.Projection));
  float4x4 LightWorldViewProjection = mul(ObjectData.World, 
    mul(oShaderData.Light.View, oShaderData.Light.Projection));

  float3 Normal = normalize(
    mul(ToFloat3(_Value.Extra), ToMatrix3x3(ObjectData.World)));
  float4 WorldPos = mul(_Value.Position, ObjectData.World);

  Pixel Result;
  Result.ScreenPos = mul(_Value.Position, WorldViewProjection);
  Result.Color = GetGouraudLightsColor(WorldPos, Normal);
  Result.Normal = Normal;
  Result.TexCoord = _Value.TexCoord;
  Result.WorldPos = mul(_Value.Position, LightWorldViewProjection);
  return Result;
}

#elif defined COVELLITE_SHADER_PIXEL ///////////////////////////////////////////

COVELLITE_DECLARE_TEX2D(TexDiffuse, 0);
COVELLITE_DECLARE_TEX2D(TexDepth, 5);

COVELLITE_DECLARE_MULTI_TARGET_STRUCTURE(PsOutput, 3);

// ************************************************************************** //

float ConvertDeptToScreenZ(float _Depth)
{
#ifdef COVELLITE_SHADER_HLSL
  return _Depth;
#elif defined COVELLITE_SHADER_GLSL
  return _Depth * 2.0f - 1.0f;
#endif
}

float LinearDepthFromNearToFar(float _RawDepth, float4x4 _Projection)
{
  float A = COVELLITE_MATRIX_ROW(_Projection, 2).z;
  float B = COVELLITE_MATRIX_ROW(_Projection, 3).z;

  // gl_FragDepth  = 0.5*(-A*depth + B) / depth + 0.5;
  return B / (A + _RawDepth);
}

// ************************************************************************** //

PsOutput psLightened(Pixel _Value)
{
  float4 Color = _Value.Color;
  float4 LightPos = _Value.WorldPos;

  //re-homogenize position after interpolation
  LightPos.xyz /= LightPos.w;

  //if position is not visible to the light - dont illuminate it
  //results in hard light frustum
  if (LightPos.x < -1.0f || LightPos.x > 1.0f ||
    LightPos.y < -1.0f || LightPos.y > 1.0f ||
    LightPos.z < 0.0f || LightPos.z > 1.0f)
  {
    //Color = ObjectData.Lights.Ambient.Color;
  }
  else
  {
    //transform clip space coords to texture space coords (-1:1 to 0:1)
    LightPos.x = LightPos.x / 2.0f + 0.5f;
    LightPos.y = LightPos.y / 2.0f + 0.5f;
#ifdef COVELLITE_SHADER_HLSL
    LightPos.y = 1.0f - LightPos.y;
#endif

    float3 normal = normalize(_Value.Normal);
    float3 lightDir = normalize(ToFloat3(ObjectData.Lights.Direction.Direction));

    float bias = max(0.3f * (1.0f - dot(normal, lightDir)), 0.03f);

    //float bias = 0.1f * tan(acos(1.0f - dot(normal, lightDir)));
    //bias = clamp(bias, 0.0f, 1.0f);

    float DepthFromLight =
      LinearDepthFromNearToFar(LightPos.z, oShaderData.Light.Projection) - bias;

    {
      const float texelSize = 2.0f / 1024.0f;

      float4 SoftShadow = float4(0.0f, 0.0f, 0.0f, 0.0f);

      for (int x = -1; x <= 1; ++x)
      {
        for (int y = -1; y <= 1; ++y)
        {
          float2 TexCoord = LightPos.xy + 
            clamp(float2(x, y), 0.0f, 1.0f) * texelSize;
          float RawDepth = COVELLITE_TEX2D_COLOR(TexDepth, TexCoord).r;
          float DepthFromShadowMap = LinearDepthFromNearToFar(
            ConvertDeptToScreenZ(RawDepth), oShaderData.Light.Projection);

          SoftShadow += (DepthFromLight > DepthFromShadowMap) ?
            ObjectData.Lights.Ambient.Color : // Shadow
            Color;                            // Lights
        }
      }

      Color = SoftShadow / 9.0f;
    }
  }

  Color *= COVELLITE_TEX2D_COLOR(TexDiffuse, _Value.TexCoord);

  PsOutput Result;
  Result.Target[0] = float4(Color.r, oShaderData.Object.IdX, oShaderData.Object.IdY, 1.0f);
  Result.Target[1] = float4(0.0f, Color.g, 0.0f, 1.0f);
  Result.Target[2] = float4(0.0f, 0.0f, Color.b, 1.0f);
  return Result;
}

#endif
