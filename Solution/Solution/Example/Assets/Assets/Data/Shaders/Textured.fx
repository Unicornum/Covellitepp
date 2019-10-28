
#ifdef COVELLITE_SHADER_PIXEL //////////////////////////////////////////////////

COVELLITE_DECLARE_TEX2D(TexAlbedo, 0);

float4 psTextured(Pixel _Value)
{
  return COVELLITE_TEX2D_COLOR(TexAlbedo, _Value.TexCoord) * _Value.Color;
}

#endif /////////////////////////////////////////////////////////////////////////
