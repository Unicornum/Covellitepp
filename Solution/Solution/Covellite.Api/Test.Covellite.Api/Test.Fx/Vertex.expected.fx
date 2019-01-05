
PixelInput VS(VertexGui _Value)
{
  PixelInput Result = (PixelInput)0;

  float4x4 WorldViewProj = mul(MatricesData.World, 
    mul(MatricesData.View, MatricesData.Projection));
  float4 Pos = float4(_Value.Position, 0.0f, 1.0f);

  float A = ((_Value.Color & 0xFF000000) >> 24) / 255.0f;
  float B = ((_Value.Color & 0x00FF0000) >> 16) / 255.0f;
  float G = ((_Value.Color & 0x0000FF00) >> 8) / 255.0f;
  float R = (_Value.Color & 0x000000FF) / 255.0f;

  Result.ScreenPos = mul(Pos, WorldViewProj);
  Result.Color = float4(R, G, B, A);
  Result.TexCoord = _Value.TexCoord;

  return Result;
}

PixelInput vsTextured(VertexTextured _Value)
{
  PixelInput Result = (PixelInput)0;

  float4x4 WorldViewProj = mul(MatricesData.World, 
    mul(MatricesData.View, MatricesData.Projection));
  float4 Pos = float4(_Value.Position, 1.0f);

  Result.ScreenPos = mul(Pos,  WorldViewProj);
  Result.Normal = normalize(mul(_Value.Normal, (float3x3)MatricesData.World));
  Result.TexCoord = _Value.Texture;
  Result.WorldPos = mul(Pos, MatricesData.World);

  return Result;
}
