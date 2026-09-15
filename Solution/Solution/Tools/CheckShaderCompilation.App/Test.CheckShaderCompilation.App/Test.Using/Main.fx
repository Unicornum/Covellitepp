
//#define COVELLITE_SHADER_HLSL
//#define COVELLITE_SHADER_GLSL

#ifdef COVELLITE_SHADER_HLSL

void vsMain(Vertex _Vertex)
{
}

#elif defined COVELLITE_SHADER_GLSL

Pixel vsMain(Vertex _Vertex)
{
  Pixel Result;
  return Result;
}

#endif
