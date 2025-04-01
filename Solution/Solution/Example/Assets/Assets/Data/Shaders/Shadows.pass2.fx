
struct Object_s
{
  float IdX, IdY, dummy2, dummy3;
};

COVELLITE_DECLARE_CONST_USER_BUFFER(Object_s, cbObject, oObject);

struct PixelShadows
{
  float4 ScreenPos        COVELLITE_INPUT_SEMANTIC(SV_POSITION);
  float3 Normal           COVELLITE_INPUT_SEMANTIC(NORMAL);
  mediump float2 TexCoord COVELLITE_INPUT_SEMANTIC(TEXCOORD0);
  float4 WorldPos         COVELLITE_INPUT_SEMANTIC(POSITION0);
  float kBiome            COVELLITE_INPUT_SEMANTIC(BLENDWEIGHT0);
};

// Демонстрация замены стандартной структуры пикселя пользовательской
#define Pixel PixelShadows

#ifdef COVELLITE_SHADER_VERTEX /////////////////////////////////////////////////

Pixel vsScene(Vertex _Value)
{
  float4x4 ViewProjection = 
    mul(CameraData.View, CameraData.Projection);

  float4 WorldPos = 
    mul(_Value.Position, ObjectData.World);
  float3 Normal = normalize(
    mul(_Value.Extra.xyz, ToMatrix3x3(ObjectData.World)));

  Pixel Result;
  Result.WorldPos = WorldPos;
  Result.ScreenPos = mul(WorldPos, ViewProjection);
  Result.Normal = Normal;
  Result.TexCoord = _Value.TexCoord;
  Result.kBiome = (distance(WorldPos.xy, float2(0.0f, 0.0f)) < 3.0f) ? 0.0f : 1.0f;
  return Result;
}

#elif defined COVELLITE_SHADER_PIXEL ///////////////////////////////////////////

COVELLITE_DECLARE_TEX2D_ARRAY(TexDiffuse, 2, 0);
COVELLITE_DECLARE_MULTI_TARGET_STRUCTURE(PsOutput, 4);

float4 GetBiomeColor(float _kBiome, vec2 _UV)
{
  float4 Albedo0 = COVELLITE_TEX2D_ARRAY_COLOR(TexDiffuse, 0, _UV);
  float4 Albedo1 = COVELLITE_TEX2D_ARRAY_COLOR(TexDiffuse, 1, _UV);

  return mix(Albedo0, Albedo1, _kBiome);
}

PsOutput psTextured(Pixel _Value)
{
  PsOutput Result;
  Result.Target[0] = GetBiomeColor(_Value.kBiome, _Value.TexCoord);
  Result.Target[1] = float4(0.5f * _Value.Normal.xyz + 0.5f, 1.0f);
  Result.Target[2] = float4(0.1f * _Value.WorldPos.xyz + 0.5f, 1.0f);
  Result.Target[3] = float4(oObject.IdX, oObject.IdY, 0.0f, 1.0f);
  return Result;
}

#endif
