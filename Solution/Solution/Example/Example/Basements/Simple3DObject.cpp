
#include "stdafx.h"
#include "Simple3DObject.hpp"
#include <alicorn/std/vector.hpp>
#include <alicorn/logger.hpp>
#include <GLMath.hpp>
#include <Covellite.Api/Covellite.Api/Renderer/fx/Hlsl.hpp>

using namespace basement;
namespace math = ::alicorn::extension::cpp::math;

Simple3DObject::Simple3DObject(
  const RendersPtr_t & _pRenders, 
  const int _PolygonsCount, 
  const int _CubesCount) :
  Common(_pRenders),
  m_pCubeRotation(Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
    }))
{
  LoadTexture("draw3dobject.title.png", 
    uT("Example.Texture.Albedo"), uT("albedo"));
  LoadTexture("draw3dobject.title.metalness.png", 
    uT("Example.Texture.Metalness"), uT("metalness"));
  LoadTexture("draw3dobject.title.roughness.png",
    uT("Example.Texture.Roughness"), uT("roughness"));
  LoadTexture("draw3dobject.title.normal.png", 
    uT("Example.Texture.Normal"), uT("normal"));
  LoadTexture("draw3dobject.title.occlusion.png",
    uT("Example.Texture.Occlusion"), uT("occlusion"));

  m_Camera = BuildCamera();

  const auto Step = static_cast<int>(sqrt(_CubesCount));

  auto Offset = 0.0f;
  for (int i = Step; i > 1; i /= 2)
  {
    Offset += (float)Step / (2 * i);
  }

  for (int x = 0; x < Step; x++)
  {
    for (int y = 0; y < Step; y++)
    {
      m_Cubes.push_back(BuildCube(
        _PolygonsCount / 12, 
        6.0f / Step,
        (10.0f / Step) * (x - Offset),
        (10.0f / Step) * (y - Offset)));
    }
  }

  // Вызов для большого количества кубов только Present'a не увеличивает fps!

  // 21.02.2019 - c универсальным освещением по Гуро

  // Рендеринг ~15 fps (Release версия после оптимизации):

  // Windows (старый ноутбук):
  // - OpenGL     - 4096 кубов по 768 полигонов каждый.
  // - DirectX10  - 4096 кубов по 1536 полигонов каждый.
  // - DirectX11  - 4096 кубов по 1536 полигонов каждый.

  // Windows (новый ноутбук):
  // - OpenGL     - ??? кубов по ??? полигонов каждый.
  // - DirectX10  - ??? кубов по ??? полигонов каждый.
  // - DirectX11  - 16384 кубов по 3072 полигона каждый.

  // Samsung Duos:
  // - OpenGLES   - 256 кубов по 96 полигонов каждый.
  // Samsung Galaxy A5:
  // - OpenGLES   - 1024 куба по 768 полигонов каждый.
}

Simple3DObject::~Simple3DObject(void)
{
  LOGGER(Info) << "Basements Simple3DObject destoyed.";

  // Рендеры не удаляются, т.к. объект класса Simple3DObject содержит уникальный
  // объект m_pRenders, который удаляется вместе со всеми рендерами.
  // m_pRenders->Remove(...);
}

void Simple3DObject::Notify(int _Message, const ::boost::any & _Value) /*override*/
{
  m_Scene.clear();
  m_Scene.push_back(m_Camera);

  using namespace ::alicorn::extension::std;

  if (_Message == ::events::Simple3DObject.LightsChanged)
  {
    m_Scene.push_back(BuildShader(::boost::any_cast<int>(_Value)));
    m_Scene += m_Cubes;

    // Источники света - в конце, чтобы убедиться, что порядок их добавления 
    // не влияет на результат.
    m_Scene.push_back(BuildLights(::boost::any_cast<int>(_Value)));
  }
}

auto Simple3DObject::GetUpdater(void) -> Updater_t
{
  return [this](const float _TimeFromStartProgram)
  {
    const auto Angle = _TimeFromStartProgram / 5.0f;

    m_pCubeRotation->SetValue(uT("x"), static_cast<float>(
      math::PI * (math::radian::Cos(Angle) + 1.0)));
    m_pCubeRotation->SetValue(uT("z"), static_cast<float>(
      math::PI * (math::radian::Sin(Angle) + 1.0)));
  };
}

auto Simple3DObject::BuildShader(int _LightsFlags) -> Id
{
  using Object_t = Component_t::Renders::Object_t;

  Object_t Components =
  {
    Component_t::Make(
    {
      { uT("id"), uT("Example.State.Sampler") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Sampler") },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Shader.HLSL") },
      { uT("version"), uT("vs_4_0") },
      { uT("entry"), uT("vsTextured") },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Example.Shader.Vertex.Cube") },
      { uT("type"), uT("Shader") },
    }),
  };

  using namespace ::alicorn::extension::std;

  if ((_LightsFlags & (1 << Lights::PBR)) != 0)
  {
    static const auto PixelShaderData = Vertex + PBR1;

    Components += Object_t
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("ps_4_0") },
        { uT("data"), PixelShaderData.data() },
        { uT("count"), PixelShaderData.size() },
        { uT("entry"), uT("psPbrSimple") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Pixel.Cube.PBR") },
        { uT("type"), uT("Shader") },
      }),
    };
  }
  else
  {
    Components += Object_t
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psTextured") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Pixel.Cube.Textured") },
        { uT("type"), uT("Shader") },
      }),
    };
  }

  const Id Id;

  m_Objects[Id] = m_pRenders->Obtain(Components);

  return Id;
}

auto Simple3DObject::BuildLights(int _LightsFlags) -> Id
{
  const auto IsActive = [=](Lights::Type _Type)
  {
    return (_LightsFlags & (1 << _Type)) != 0;
  };

  const float X = 5.0f;
  const float Y = 1.0f;
  const float Z = 1.0f;

  const float Const = 0.0f;
  const float Linear = 0.1f;
  const float Exponent = 0.05f;

  using Object_t = Component_t::Renders::Object_t;
  using namespace ::alicorn::extension::std;

  Object_t Components;

  /// [Create lights]
  if (IsActive(Lights::Ambient))
  {
    Components += Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Ambient") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Ambient") },
        { uT("color"), 0xFF303030 }, // ARGB
      }),
    };
  }
    
  if (IsActive(Lights::Directional))
  {
    Components += Object_t
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Direction") },
        { uT("x"), 1.0f },
        { uT("y"), -1.0f },
        { uT("z"), 1.0f },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Direction") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Direction") },
        { uT("color"), 0xFF808080 }, // ARGB
      }),
    };
  }
  /// [Create lights]

  if (IsActive(Lights::Red))
  {
    Components += Object_t
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), X },
        { uT("y"), 0.0f },
        { uT("z"), Z },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Attenuation") },
        { uT("const"), Const },
        { uT("linear"), Linear },
        { uT("exponent"), Exponent },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Point.1") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Point") },
        { uT("color"), 0xFFFF0000 }, // ARGB
      }),
    };
  }

  if (IsActive(Lights::Green))
  {
    Components += Object_t
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), X },
        { uT("y"), Y },
        { uT("z"), -Z },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Attenuation") },
        { uT("const"), Const },
        { uT("linear"), Linear },
        { uT("exponent"), Exponent },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Point.2") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Point") },
        { uT("color"), 0xFF00FF00 }, // ARGB
      }),
    };
  }

  if (IsActive(Lights::Blue))
  {
    Components += Object_t
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), X },
        { uT("y"), -Y },
        { uT("z"), -Z },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Attenuation") },
        { uT("const"), Const },
        { uT("linear"), Linear },
        { uT("exponent"), Exponent },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Point.3") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Point") },
        { uT("color"), 0xFF0000FF }, // ARGB
      }),
    };
  }

  const Id Id;

  m_Objects[Id] = m_pRenders->Obtain(Components);

  return Id;
}

auto Simple3DObject::BuildCamera(void) -> Id
{
  const Id Id;

  /// [Create camera]
  m_Objects[Id] = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), 0.0f },
        { uT("y"), 0.0f },
        { uT("z"), -1.6f },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Camera.") + Id.GetStringId() },
        { uT("type"), uT("Camera") },
        { uT("kind"), uT("Perspective") },
        { uT("distance"), 10.0f },
        { uT("fov"), 90.0f },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.State.Clear") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Clear") },
        { uT("color"), 0xFF0000FF },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.State.Depth") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("clear"), true },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Material.Cube") },
        { uT("type"), uT("Material") },
        { uT("ambient"), 0xFFFFFFFF },
        { uT("diffuse"), 0xFFFFFFFF },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Texture.Albedo") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Texture.Metalness") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Texture.Roughness") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Texture.Normal") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Texture.Occlusion") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Vertex.Default") },
        { uT("type"), uT("Shader") },
        { uT("version"), uT("vs_4_0") },
        { uT("entry"), uT("vsTextured") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Pixel.Default") },
        { uT("type"), uT("Shader") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psTextured") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Updater.Simple3DObject.") + Id.GetStringId() },
        { uT("type"), uT("Updater") },
        { uT("function"), GetUpdater() },
      }),
    });
  /// [Create camera]

  return Id;
}

auto Simple3DObject::BuildCube(
  int _PolygonsFactor, 
  float _Scale,
  float _PositionX, 
  float _PositionY) // положение относительно экранных координат
  -> Id
{
  /// [Vertex format]
  using Vertex_t = ::covellite::api::vertex::Polyhedron;
  /// [Vertex format]

  using namespace ::alicorn::extension::std;

  ::std::vector<Vertex_t> VertexData;
  ::std::vector<int> IndexData;

  const auto dAngle = 90.0f / _PolygonsFactor;

  for (auto Angle = 0.0f; Angle < 90.0f; Angle += dAngle)
  {
    namespace math = ::alicorn::extension::cpp::math;

    const auto Lenght1 = 1.0f * math::degree::Sin(45.0f) / 
      math::degree::Sin(180.0f - 45.0f - Angle);

    const auto X1 = 0.5f - Lenght1 * math::degree::Cos(Angle);
    const auto Y1 = 0.5f - Lenght1 * math::degree::Sin(Angle);

    const auto Lenght2 = 1.0f * math::degree::Sin(45.0f) /
      math::degree::Sin(180.0f - 45.0f - (Angle + dAngle));

    const auto X2 = 0.5f - Lenght2 * math::degree::Cos(Angle + dAngle);
    const auto Y2 = 0.5f - Lenght2 * math::degree::Sin(Angle + dAngle);

    VertexData += ::std::vector<Vertex_t>
    {
      {  0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,  1.0f,       0.0f, },
      {  X1,    Y1,   0.5f,   0.0f, 0.0f, 1.0f,  0.5f + X1,  0.5f - Y1, },
      {  X2,    Y2,   0.5f,   0.0f, 0.0f, 1.0f,  0.5f + X2,  0.5f - Y2, },
      { -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,  0.0f,       1.0f, },
      { -X1,   -Y1,   0.5f,   0.0f, 0.0f, 1.0f,  0.5f - X1,  0.5f + Y1, },
      { -X2,   -Y2,   0.5f,   0.0f, 0.0f, 1.0f,  0.5f - X2,  0.5f + Y2, },

      { 0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,       0.0f, },
      { 0.5f, X1,    Y1,    1.0f, 0.0f, 0.0f,  0.5f + X1,  0.5f - Y1, },
      { 0.5f, X2,    Y2,    1.0f, 0.0f, 0.0f,  0.5f + X2,  0.5f - Y2, },
      { 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f,       1.0f, },
      { 0.5f, -X1,   -Y1,   1.0f, 0.0f, 0.0f,  0.5f - X1,  0.5f + Y1, },
      { 0.5f, -X2,   -Y2,   1.0f, 0.0f, 0.0f,  0.5f - X2,  0.5f + Y2, },

      {  0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  1.0f,      0.0f, },
      {  X2,   0.5f,  Y2,   0.0f, 1.0f, 0.0f,  0.5f + X2, 0.5f - Y2, },
      {  X1,   0.5f,  Y1,   0.0f, 1.0f, 0.0f,  0.5f + X1, 0.5f - Y1, },
      { -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f,      1.0f, },
      { -X2,   0.5f, -Y2,   0.0f, 1.0f, 0.0f,  0.5f - X2, 0.5f + Y2, },
      { -X1,   0.5f, -Y1,   0.0f, 1.0f, 0.0f,  0.5f - X1, 0.5f + Y1, },

      {  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f,      1.0f, },
      {  X2,    Y2,   -0.5f,   0.0f, 0.0f, -1.0f,  0.5f + X2, 0.5f + Y2, },
      {  X1,    Y1,   -0.5f,   0.0f, 0.0f, -1.0f,  0.5f + X1, 0.5f + Y1, },
      { -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f,      0.0f, },
      { -X2,   -Y2,   -0.5f,   0.0f, 0.0f, -1.0f,  0.5f - X2, 0.5f - Y2, },
      { -X1,   -Y1,   -0.5f,   0.0f, 0.0f, -1.0f,  0.5f - X1, 0.5f - Y1, },

      { -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  1.0f,       1.0f, },
      { -0.5f, X2,    Y2,    -1.0f, 0.0f, 0.0f,  0.5f + X2,  0.5f + Y2, },
      { -0.5f, X1,    Y1,    -1.0f, 0.0f, 0.0f,  0.5f + X1,  0.5f + Y1, },
      { -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f,       0.0f, },
      { -0.5f, -X2,   -Y2,   -1.0f, 0.0f, 0.0f,  0.5f - X2,  0.5f - Y2, },
      { -0.5f, -X1,   -Y1,   -1.0f, 0.0f, 0.0f,  0.5f - X1,  0.5f - Y1, },

      { 0.5f,  -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  1.0f,      1.0f, },
      { X1,    -0.5f,  Y1,   0.0f, -1.0f, 0.0f,  0.5f + X1, 0.5f + Y1, },
      { X2,    -0.5f,  Y2,   0.0f, -1.0f, 0.0f,  0.5f + X2, 0.5f + Y2, },
      { -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f,      0.0f, },
      { -X1,   -0.5f, -Y1,   0.0f, -1.0f, 0.0f,  0.5f - X1, 0.5f - Y1, },
      { -X2,   -0.5f, -Y2,   0.0f, -1.0f, 0.0f,  0.5f - X2, 0.5f - Y2, },
    };
  }

  for (int i = 0; i < static_cast<int>(VertexData.size()); i++)
  {
    IndexData.push_back(i);
  }

  const Id Id;

  /// [Textured object]
  m_Objects[Id] = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("id"), uT("Example.Buffer.Vertex.Cube") },
        { uT("type"), uT("Buffer") },
        { uT("data"), (const Vertex_t *)VertexData.data() },
        { uT("count"), VertexData.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Buffer.Index.Cube.") + Id.GetStringId() },
        { uT("type"), uT("Buffer") },
        { uT("kind"), uT("Index") },
        { uT("data"), (const int *)IndexData.data() },
        { uT("count"), IndexData.size() },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Scale") },
        { uT("x"), _Scale },
        { uT("y"), _Scale },
        { uT("z"), _Scale },
      }),
      m_pCubeRotation,
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), 0.0f },
        { uT("y"), -_PositionX },
        { uT("z"), _PositionY },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Present.Geometry.") + Id.GetStringId() },
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Geometry") },
      })
    });
  /// [Textured object]

  return Id;
}
