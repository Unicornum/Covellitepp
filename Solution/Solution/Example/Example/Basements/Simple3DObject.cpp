
#include "stdafx.h"
#include "Simple3DObject.hpp"
#include <chrono>
#include <alicorn/std/vector.hpp>
#include <Covellite/Api/Vertex.hpp>
#include <Covellite/Api/Component.inl>

// 26 Ноябрь 2018 17:54 (unicornum.verum@gmail.com)
TODO("Недопустимое обращение к файлам другого проекта.");
#include <Covellite.Api/Covellite.Api/Renderer/fx/Pixel.auto.hpp>
#include <Covellite.Api/Covellite.Api/Renderer/fx/Vertex.auto.hpp>

using namespace basement;

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

  const float CenterX = 0.0f;
  const float CenterY = 0.8f;

  for (int x = 0; x < Step; x++)
  {
    for (int y = 0; y < Step; y++)
    {
      m_Scene.push_back(BuildCube(_PolygonsCount / 12, 3.0f / Step,
        CenterX + (5.0f / Step) * (x - Offset),
        CenterY + (5.0f / Step) * (y - Offset)));
    }
  }

  m_Scene.push_back(BuildLights()); // Источники света - в конце, чтобы 
                                    // убедиться, что порядок их добавления 
                                    // не влияет на результат.

  // Рендеринг 30+ fps (Release версия после оптимизации):

  // Это было без освещения???:
  // - DirectX10 - 5000 кубов по 120 полигонов каждый.
  // - DirectX10 - 2500 кубов по 240 полигонов каждый.

  // 26.12.2018 - c освещением:
  // - OpenGL - 2300 кубов по 96 полигонов каждый.
  // - OpenGL - 1100 кубов по 192 полигонов каждый.
  // - DirectX10 - 600 кубов по 96 полигонов каждый.
  // - DirectX10 - 300 кубов по 192 полигонов каждый.
  // - DirectX11 - 600 кубов по 96 полигонов каждый.
  // - DirectX11 - 300 кубов по 192 полигонов каждый.

  // 28.01.2019 - c освещением:
  // - DirectX11 - 4096 кубов по 768 полигонов каждый.

  // Рендеринг 15+ fps (Release версия после оптимизации):

  // При включенном android:hardwareAccelerated="true"
  // - OpenGLES - 250 кубов по 48 полигонов каждый.
  // - OpenGLES - 150 кубов по 96 полигонов каждый.

  // Samsung A5:
  // - OpenGLES - 1024 кубов по 1536 полигонов каждый (15 fps).
  // - OpenGLES - 4096 кубов по 768 полигонов каждый (5 fps).

  // Вызов для большого количества кубов только Present'a не увеличивает fps!
}

void Simple3DObject::Render(void) /*override*/
{
  const ::std::chrono::duration<float> Time =
    (::std::chrono::system_clock::now() - Begin) / 5.0;

  const float AngleX = 3.14f * (cosf(Time.count()) + 1.0f);
  const float AngleZ = 3.14f * (sinf(Time.count()) + 1.0f);

  m_pCubeRotation->SetValue(uT("x"), AngleX);
  m_pCubeRotation->SetValue(uT("z"), AngleZ);

  Common::Render();
}

auto Simple3DObject::BuildLights(void) -> Id
{
  const Id Id;

  m_Objects[Id] = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Ambient") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Ambient") },
        { uT("ambient"), 0xFF404040 }, // ARGB
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Direction") },
        { uT("x"), 1.0f },
        { uT("y"), 1.0f },
        { uT("z"), 1.0f },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Direction") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Direction") },
        { uT("diffuse"), 0xFFF0F0F0 }, // ARGB
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), 3.0f },
        { uT("y"), 2.0f },
        { uT("z"), 2.0f },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Attenuation") },
        { uT("const"), 0.0f },
        { uT("linear"), 0.0f },
        { uT("exponent"), 0.5f },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Point.1") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Point") },
        { uT("diffuse"), 0xFFFF0000 }, // ARGB
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), 3.0f },
        { uT("y"), 2.0f },
        { uT("z"), -2.0f },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Attenuation") },
        { uT("const"), 0.0f },
        { uT("linear"), 0.0f },
        { uT("exponent"), 0.5f },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Point.2") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Point") },
        { uT("diffuse"), 0xFF00FF00 }, // ARGB
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), 3.0f },
        { uT("y"), -2.0f },
        { uT("z"), 2.0f },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Attenuation") },
        { uT("const"), 0.0f },
        { uT("linear"), 0.0f },
        { uT("exponent"), 0.5f },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Point.3") },
        { uT("type"), uT("Light") },
        { uT("kind"), uT("Point") },
        { uT("diffuse"), 0xFF0000FF }, // ARGB
      }),

    });

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
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Camera") },
        { uT("focal"), uT("Enabled") },
        { uT("dept"), uT("Enabled") },
        { uT("distance"), 5.0f },
        { uT("angle"), 90.0f },
      })
    });

  return Id;
}

auto Simple3DObject::BuildCube(int _PolygonsFactor, float _Scale,
  float _PositionX, float _PositionY) // положение относительно экранных координат
  -> Id
{
  using Vertex_t = ::covellite::api::Vertex::Textured;

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
        { uT("id"), uT("Example.Shader.Data") },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psLighted") },
        { uT("data"), ::Pixel.data() },
        { uT("count"), ::Pixel.size() }
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Pixel.Cube") },
        { uT("type"), uT("Shader") },
        { uT("kind"), uT("Pixel") },
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
        { uT("id"), uT("Example.Shader.Data") },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("vs_4_0") },
        { uT("entry"), uT("vsTextured") },
        { uT("data"), ::Vertex.data() },
        { uT("count"), ::Vertex.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Vertex.Cube") },
        { uT("type"), uT("Shader") },
        { uT("kind"), Vertex_t::GetName() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Buffer.Vertex.Cube") },
        { uT("type"), uT("Buffer") },
        { uT("kind"), Vertex_t::GetName() },
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
        { uT("id"), uT("Example.Data.Scale.") + Id.GetStringId() },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Scale") },
        { uT("x"), _Scale },
        { uT("y"), _Scale },
        { uT("z"), _Scale },
      }),
      m_pCubeRotation,
      Component_t::Make(
      {
        { uT("id"), uT("Example.Data.Position.") + Id.GetStringId() },
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
