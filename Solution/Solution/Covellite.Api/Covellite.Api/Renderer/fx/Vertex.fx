
cbuffer ConstantBuffer : register(b0)
{
  float4x4 World;
  float4x4 Projection;
  float2 IsTextureDisabled;
};

PixelInput VS(VertexIn _Value)
{
  PixelInput Result = (PixelInput)0;

  float4x4 worldViewProjection = mul(World, Projection);
  Result.PosH = mul(float4(_Value.PosL.x, _Value.PosL.y, 0.0f, 1.0f),
    worldViewProjection);

  float R = (_Value.Color & 0x000000FF) / 255.0f;
  float G = ((_Value.Color & 0x0000FF00) >> 8) / 255.0f;
  float B = ((_Value.Color & 0x00FF0000) >> 16) / 255.0f;
  float A = ((_Value.Color & 0xFF000000) >> 24) / 255.0f;
  Result.Color = float4(R, G, B, A);

  Result.TexCoord = _Value.TexCoord;

  return Result;
}
