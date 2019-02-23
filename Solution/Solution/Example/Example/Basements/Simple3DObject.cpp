
#include "stdafx.h"
#include "Simple3DObject.hpp"
#include <chrono>
#include <alicorn/std/vector.hpp>
#include <alicorn/cpp/math.hpp>
#include <Covellite/Api/Vertex.hpp>
#include <Covellite/Api/Component.inl>

using namespace basement;
namespace math = ::alicorn::extension::cpp::math;

static const auto Begin = ::std::chrono::system_clock::now();

Simple3DObject::Simple3DObject(const RendersPtr_t & _pRenders, 
  int _PolygonsCount, int _CubesCount) :
  Common(_pRenders),
  m_pCubeRotation(Component_t::Make(
    {
      { uT("id"), uT("Example.Data.Rotation.Cube") },
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
    }))
{
  LoadTexture("title.png", uT("Example.Texture"));

  m_Scene.push_back(BuildCamera());

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
      m_Scene.push_back(BuildCube(
        _PolygonsCount / 12, 
        6.0f / Step,
        m_CenterX + (10.0f / Step) * (x - Offset),
        m_CenterY + (10.0f / Step) * (y - Offset)));
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
  // - OpenGL     - ??? кубов по 96 полигонов каждый.
  // - DirectX10  - ??? кубов по 96 полигонов каждый.
  // - DirectX11  - ??? кубов по 96 полигонов каждый.

  // Samsung Duos:
  // - OpenGLES   - 256 кубов по 96 полигонов каждый.
  // Samsung A5:
  // - OpenGLES   - ??? кубов по ??? полигонов каждый.
}

void Simple3DObject::Notify(int _Message, const ::boost::any & _Value) /*override*/
{
  if (_Message == ::events::Simple3DObject.LightsChanged)
  {
    m_Lights.clear();
    m_Lights.push_back(BuildLights(::boost::any_cast<int>(_Value)));
  }
}

void Simple3DObject::Render(void) /*override*/
{
  const ::std::chrono::duration<double> Time =
    (::std::chrono::system_clock::now() - Begin) / 5.0;
  const auto Angle = Time.count();

  m_pCubeRotation->SetValue(uT("x"), static_cast<float>(
    math::PI * (math::radian::Cos(Angle) + 1.0)));
  m_pCubeRotation->SetValue(uT("z"), static_cast<float>(
    math::PI * (math::radian::Sin(Angle) + 1.0)));

  //m_pCubeRotation->SetValue(uT("x"), 0.785f);
  //m_pCubeRotation->SetValue(uT("y"), -0.785f);
  //m_pCubeRotation->SetValue(uT("z"), 0.0f);

  Common::Render();

  // Источники света - в конце, чтобы убедиться, что порядок их добавления 
  // не влияет на результат.
  for (const auto Id : m_Lights)
  {
    for (auto & Render : m_Objects[Id])
    {
      Render();
    }
  }
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

  //m_Scene.push_back(BuildCube2(0.05f, X, 0.0f,  Z + m_CenterY));
  //m_Scene.push_back(BuildCube2(0.05f, X,    Y, -Z + m_CenterY));
  //m_Scene.push_back(BuildCube2(0.05f, X,   -Y, -Z + m_CenterY));

  using Object_t = Component_t::Renders::Object_t;
  using namespace ::alicorn::extension::std;

  Object_t Components;

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
        { uT("z"), Z + m_CenterY },
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
        { uT("z"), -Z + m_CenterY },
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
        { uT("z"), -Z + m_CenterY },
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

  m_Objects[Id] = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), 0.0f },
        { uT("y"), 0.0f },
        { uT("z"), 0.0f },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Camera.") + Id.GetStringId() },
        { uT("type"), uT("Camera") },
        { uT("kind"), uT("Perspective") },
        { uT("dept"), uT("Clear") },
        { uT("distance"), 10.0f },
        { uT("angle"), 90.0f },
      })
    });

  return Id;
}

auto Simple3DObject::BuildCube(int _PolygonsFactor, float _Scale,
  float _PositionX, float _PositionY) // положение относительно экранных координат
  -> Id
{
  using Vertex_t = ::covellite::api::Vertex::Polyhedron;

  const Id Id;

  const ::std::vector<Vertex_t> VertexData =
  {
    { -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, },  // 0
    { -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, },  // 1
    {  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, },  // 2
    {  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, },  // 3

    {  0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f, },  // 4
    {  0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, },  // 5
    {  0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f, },  // 6
    {  0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, },  // 7

    { -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, },  // 8
    { -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, },  // 9
    { -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, },  // 10
    { -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, },  // 11

    { -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, },  // 12
    { -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, },  // 13
    {  0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, },  // 14
    {  0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f, },  // 15

    { -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f, },  // 16
    { -0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f, },  // 17
    {  0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f, },  // 18
    {  0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f, },  // 19

    {  0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f, },  // 20
    {  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 1.0f, },  // 21
    { -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 0.0f, },  // 22
    { -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f, },  // 23
  };

  ::std::vector<int> IndexData =
  {
     0,  1,  2,   2,  1,  3,
     4,  5,  6,   6,  5,  7,
     8,  9, 10,  10,  9, 11,
    12, 13, 14,  14, 13, 15,
    16, 17, 18,  18, 17, 19,
    20, 21, 22,  22, 21, 23,
  };

  while (_PolygonsFactor /= 2)
  {
    using namespace ::alicorn::extension::std;

    IndexData += IndexData;
  }

  m_Objects[Id] = m_pRenders->Obtain(
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
        { uT("id"), uT("Example.Shader.Pixel.Cube") },
        { uT("type"), uT("Shader") },
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
        { uT("id"), uT("Example.Texture") },
      }),
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
      Component_t::Make(
      {
        { uT("id"), uT("Example.Buffer.Vertex.Cube") },
        { uT("type"), uT("Buffer") },
        { uT("data"), VertexData.data() },
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

  return Id;
}

auto Simple3DObject::BuildCube2(float _Scale, 
  float _PositionX, float _PositionY, float _PositionZ) -> Id
{
  const Id Id;

  ::std::vector<int> IndexData =
  {
     0,  1,  2,   2,  1,  3,
     4,  5,  6,   6,  5,  7,
     8,  9, 10,  10,  9, 11,
    12, 13, 14,  14, 13, 15,
    16, 17, 18,  18, 17, 19,
    20, 21, 22,  22, 21, 23,
  };

  m_Objects[Id] = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Pixel.Cube") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Material.Emission") },
        { uT("type"), uT("Material") },
        { uT("emission"), 0xFFFFFFFF },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Vertex.Cube") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Buffer.Vertex.Cube") },
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
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), _PositionX },
        { uT("y"), _PositionY },
        { uT("z"), _PositionZ },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Present.Geometry.") + Id.GetStringId() },
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Geometry") },
      })
    });

  return Id;
}
