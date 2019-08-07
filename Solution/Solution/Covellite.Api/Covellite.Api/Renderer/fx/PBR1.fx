
// source: http://orsvarn.com/entry/pbr-shader-source/

Texture2D TexAlbedo : register(t0);
Texture2D TexMetalness : register(t1);
Texture2D TexRoughness : register(t2);
Texture2D TexNormal : register(t3);
Texture2D TexOcclusion : register(t4);
SamplerState TexSampler : register(s0);

// Constants
static const float Pi = 3.14159265359;
static const float OneDivPi = 1 / Pi;

// Lys constants
static const float k0 = 0.00098, k1 = 0.9921, fUserMaxSPow = 0.2425;
static const float g_fMaxT = (exp2(-10.0 / sqrt(fUserMaxSPow)) - k0) / k1;
static const int nMipOffset = 0;

// Lys function, copy/pasted from https://www.knaldtech.com/docs/doku.php?id=specular_lys
float GetSpecPowToMip(float fSpecPow, int nMips)
{
  // This line was added because ShaderTool destroys mipmaps.
  fSpecPow = 1 - pow(1 - fSpecPow, 8);
  // Default curve - Inverse of Toolbag 2 curve with adjusted constants.
  float fSmulMaxT = (exp2(-10.0 / sqrt(fSpecPow)) - k0) / k1;
  return float(nMips - 1 - nMipOffset)*(1.0 - clamp(fSmulMaxT / g_fMaxT, 0.0, 1.0));
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
  float3 map = TexNormal.Sample(TexSampler, texcoord).xyz;
  map = map * 2 - 1;
  //map = float3(0.0f, 0.0f, 1.0f); // Flat normal.
  return normalize(mul(map, TBN));
}

float4 psPbrSimple(Pixel _Value) : SV_Target
{
  float3 camPos = float3(MatricesData.ViewInverse._41,
    MatricesData.ViewInverse._42, MatricesData.ViewInverse._43);
  float3 camToPixelRaw = _Value.WorldPos - camPos;
  float3 camToPixel = normalize(camToPixelRaw);

  // Calculate normal
  float3 normal = perturb_normal(_Value.Normal, camToPixelRaw, _Value.TexCoord);
  float3 camNormalReflect = reflect(camToPixel, -normal);

  float4 DiffuseLightColor = 
    GetGouraudLightsColor(_Value.WorldPos, normal);
  float4 BaseColorSample = TexAlbedo.Sample(TexSampler, _Value.TexCoord);
  float RoughnessSample = TexRoughness.Sample(TexSampler, _Value.TexCoord).r;

  float4 SpecularLightColor = 
    GetGouraudLightsColor(_Value.WorldPos, camNormalReflect);
  float4 BaseColorSpecular = TexAlbedo.Sample(TexSampler, camNormalReflect);
  float MetalnessSample = TexMetalness.Sample(TexSampler, _Value.TexCoord).r;

  float OcclusionSample = TexOcclusion.Sample(TexSampler, _Value.TexCoord).r;

  float4 PixelColor = DiffuseLightColor * 
    (BaseColorSample * RoughnessSample * OcclusionSample) + // Из этого прямо
    // следует, что Albedo и Roughness можно хранить в одной текстуре.
    SpecularLightColor * (BaseColorSpecular * MetalnessSample);
  return PixelColor;
}

float4 psPBR1(Pixel _Value) : SV_Target
{
  float3 camPos = float3(MatricesData.ViewInverse._41,
    MatricesData.ViewInverse._42, MatricesData.ViewInverse._43);
  float3 camToPixelRaw = _Value.WorldPos - camPos;
  float3 camToPixel = normalize(camToPixelRaw);

  // Calculate normal
  float3 normal = -perturb_normal(_Value.Normal, camToPixelRaw, _Value.TexCoord);
  float3 camNormalReflect = reflect(camToPixel, normal);

  // Sample textures
  //int mipLevels, width, height;
  //TexCubemapSpecular.GetDimensions(0, width, height, mipLevels);
  float4 baseColorSample = TexAlbedo.Sample(TexSampler, _Value.TexCoord);
  float metallicSample = TexMetalness.Sample(TexSampler, _Value.TexCoord).r;
  float roughnessSample = abs(TexRoughness.Sample(TexSampler, _Value.TexCoord).r);
  //float4 normalSample = TexNormal.Sample(TexSampler, _Value.TexCoord);
  float occlusionSample = TexOcclusion.Sample(TexSampler, _Value.TexCoord).r;

  float4 cubemapSampleAmbient = GetRGBAColor(LightsData.Ambient.ARGBColor);
  float4 cubemapSampleSpec =
    TexAlbedo.Sample(TexSampler, camNormalReflect);
    //TexCubemapSpecular.SampleLevel(TexSampler, camNormalReflect,
      //GetSpecPowToMip(roughnessSample, 1));
  roughnessSample = pow(roughnessSample, 0.4);

  // Creating specular color and intensity, this needs to be done before gamma correction
  float4 specularColor = float4(lerp(0.04f.rrr, baseColorSample.rgb, metallicSample), 1.0f);
  baseColorSample.rgb = lerp(baseColorSample.rgb, 0.0f.rrr, metallicSample);

  // Gamma correct textures
  baseColorSample = pow(abs(baseColorSample), 2.2);
  roughnessSample = pow(abs(roughnessSample), 2.2);
  cubemapSampleAmbient = pow(abs(cubemapSampleAmbient), 2.2);
  cubemapSampleSpec = pow(abs(cubemapSampleSpec), 2.2);

  // Add in occlusion
  float occlusion = occlusionSample; //1 - (1 - occlusionSample) * 0.75f;
  cubemapSampleAmbient *= occlusion;
  cubemapSampleSpec *= occlusion;

  // Material settings
  float roughPow2 = roughnessSample * roughnessSample;
  float roughPow22 = roughPow2 * roughPow2;
  float roughPow2Half = roughPow2 * 0.5f;

  // Directional lights
  float4 diffuseLight = 0;
  float4 specularLight = 0;
  //for (int i = 0; i < DirectionalLightCount; i++)
  {
  float3 lightDir =
    -normalize(LightsData.Direction.Direction.xyz);
    //normalize(DirectionalLightDirection[i].xyz);
  float4 DirectColor = GetRGBAColor(LightsData.Direction.ARGBColor);
  float3 lightColor =
    DirectColor.rgb;
  //DirectionalLightColor[i].rgb;
  float lightIntensity =
    1.0f;//DirectColor.a * 10000.0f;
    //DirectionalLightColor[i].a * 10000;

  // DIFFUSE
  diffuseLight += float4((saturate(dot(-lightDir, -normal)) * OneDivPi) *
    lightColor * lightIntensity * baseColorSample.rgb, 1.0f);

  // SPECULAR
  float3 halfVector = normalize(camToPixel + lightDir);
  float halfVecDotNorm = dot(halfVector, normal);
  float normDotLight = max(dot(normal, -lightDir), 0);
  float normDotCam = max(dot(normal, -camToPixel), 0);

  // Fresnel term
  float4 schlickFresnel = specularColor + (1 - specularColor) *
    (pow(1 - dot(camToPixel, halfVector), 5) /
    (6 - 5 * (1 - roughnessSample)));

  // Distribution term
  float denominator = halfVecDotNorm * halfVecDotNorm*(roughPow22 - 1) + 1;
  float ggxDistribution = roughPow22 / (Pi * denominator * denominator);

  // Geometry term
  float schlickGgxGeometry =
    (normDotCam / (normDotCam * (1 - roughPow2Half) + roughPow2Half));
  float ggxGeometry = (2 * normDotCam) / (normDotCam + sqrt(roughPow2 +
    (1 - roughPow2) * pow(normDotCam, 2))); // ggxG without schlick approximation

  // Add the spec from this light
  specularLight += float4(
    ((schlickFresnel * ggxDistribution * schlickGgxGeometry) /
    4 * normDotLight*normDotCam).rrr *
    lightColor * specularColor.rgb * lightIntensity, 1.0f);
  }

  // Ambient cubemap light
  diffuseLight.rgb += cubemapSampleAmbient.rgb * baseColorSample.rgb;

  // Specular cubemap light
  float normDotCam =
    max(dot(lerp(_Value.Normal, normal, max(dot(_Value.Normal, -camToPixel), 0)),
    -camToPixel), 0);
  float4 schlickFresnel =
    saturate(specularColor + (1 - specularColor) * pow(1 - normDotCam, 5));

  // Composite
  float4 output = 0;
  output = lerp(diffuseLight, cubemapSampleSpec, schlickFresnel);
  output += specularLight;

  output = pow(abs(output), .4545);
  output.a = 1;
  return output;
}
