
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
  return Result;
}

#elif defined COVELLITE_SHADER_PIXEL ///////////////////////////////////////////

COVELLITE_DECLARE_TEX2D(TexDiffuse, 0);
COVELLITE_DECLARE_MULTI_TARGET_STRUCTURE(PsOutput, 4);

PsOutput psTextured(Pixel _Value)
{
  PsOutput Result;
  Result.Target[0] = COVELLITE_TEX2D_COLOR(TexDiffuse, _Value.TexCoord);
  Result.Target[1] = float4(0.5f * _Value.Normal.xyz + 0.5f, 1.0f);
  Result.Target[2] = float4(0.1f * _Value.WorldPos.xyz + 0.5f, 1.0f);
  Result.Target[3] = float4(oObject.IdX, oObject.IdY, 0.0f, 1.0f);
  return Result;
}

#endif
