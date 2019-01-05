
#include "stdafx.h"
#include "ExtraWindow.hpp"
#include <chrono>
#include <alicorn/std/vector.hpp>
#include <alicorn/image.hpp>
#include <Covellite/Api/Vertex.hpp>
#include <Covellite/App/Settings.hpp>
#include <Covellite/Api/Component.inl>

// 26 Ноябрь 2018 17:54 (unicornum.verum@gmail.com)
TODO("Недопустимое обращение к файлам другого проекта.");
#include <Covellite.Api/Covellite.Api/Renderer/fx/Pixel.auto.hpp>
#include <Covellite.Api/Covellite.Api/Renderer/fx/Vertex.auto.hpp>

class Id final
{
  using String_t = ::alicorn::extension::std::String;

public:
  size_t GetId(void) const { return m_Id; }
  String_t GetStringId(void) const { return uT("ID").Replace(uT("ID"), m_Id); }

private:
  static size_t GetUniqueId(void)
  {
    static size_t Id = 0;
    return Id++;
  }

private:
  const size_t m_Id;

public:
  Id(void) :
    m_Id(GetUniqueId())
  {

  }
};

ExtraWindow::ExtraWindow(const WindowApi_t & _WindowApi) :
  m_WindowApi(_WindowApi),
  m_Events(_WindowApi),
  m_pRenders(m_WindowApi.GetRenders())
{
  LoadTexture("title.png", uT("Example.Texture"));

  auto pCameraRotation = Component_t::Make(
    {
      { uT("id"), uT("Example.Data.Rotation.Camera") },
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
    });

  auto pCubeRotation = Component_t::Make(
    {
      { uT("id"), uT("Example.Data.Rotation.Cube") },
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
    });

  using namespace ::alicorn::extension::std;

  /*const*/ ::std::vector<size_t> Scene = 
  {
    BuildCamera(pCameraRotation),
    BuildCube(pCubeRotation, 2.0f, 2.0f),
    BuildCube(pCubeRotation, -2.0f, 2.0f),
    BuildLights(),  // Источники света - в конце, чтобы убедиться, что
                    // порядок их добавления не влияет на результат.
  };

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

  // Рендеринг 15+ fps (Release версия после оптимизации):

  // При включенном android:hardwareAccelerated="true"
  // - OpenGLES - 250 кубов по 48 полигонов каждый.
  // - OpenGLES - 150 кубов по 96 полигонов каждый.

  // Вызов для большого количества кубов только Present'a не увеличивает fps!

  //const auto CubeId = BuildCube(pCubeRotation, 0.0f, 1.0f);

  //for (size_t i = 0; i < 300; i++) 
  //{
  //  Scene.push_back(CubeId);
  //}

  const ::std::vector<size_t> XYZPlaneScene =
  {
    BuildCamera(pCameraRotation),
    BuildPlane(0x5000FF00, 0.0f, 0.0f, 0.0f),
    BuildPlane(0x50FF00FF, 1.57f, 0.0f, 0.0f),
    BuildPlane(0x500000FF, 0.0f, 1.57f, 0.0f),
  };

  const auto Begin = ::std::chrono::system_clock::now();

  using namespace ::covellite::events;

  m_Events[Drawing.Do].Connect([=](void)
  {
    const ::std::chrono::duration<double> Time = 
      (::std::chrono::system_clock::now() - Begin) / 5.0;

    const float AngleX = 3.14f * (cosf(Time.count()) + 1.0f);
    const float AngleZ = 3.14f * (sinf(Time.count()) + 1.0f);

    pCubeRotation->SetValue(uT("x"), AngleX);
    pCubeRotation->SetValue(uT("z"), AngleZ);

    auto RenderScene = [&](const ::std::vector<size_t> & Scene)
    {
      for (const auto Id : Scene)
      {
        for (auto & Render : m_Objects[Id])
        {
          Render();
        }
      }
    };

    RenderScene(XYZPlaneScene);
    RenderScene(Scene);
  });

  m_Events[Cursor.Motion].Connect([=](const Cursor_t::Position & _Position)
  {
    {
      static int OldX = _Position.X;

      auto AngleZ = pCameraRotation->GetValue(uT("z"), 0.0f);
      AngleZ += (_Position.X - OldX) / 300.0f;
      pCameraRotation->SetValue(uT("z"), AngleZ);

      OldX = _Position.X;
    }

    {
      static int OldY = _Position.Y;

      auto AngleY = pCameraRotation->GetValue(uT("y"), 0.0f);
      AngleY -= (_Position.Y - OldY) / 200.0f;
      pCameraRotation->SetValue(uT("y"), AngleY);

      OldY = _Position.Y;
    }
  });
}

ExtraWindow::operator const WindowApi_t & (void) const
{
  return m_WindowApi;
}

void ExtraWindow::LoadTexture(const Path_t & _RelativePathToSourceFile, 
  const String_t & _TextureId)
{
  const auto CovelliteppSection = ::covellite::core::Settings_t::GetInstance();

  const auto PathToFontsDirectory =
    CovelliteppSection.Get<Path_t>(uT("PathToFontsDirectory"));

  using namespace ::alicorn::source;

  const image::Universal_t<image::pixel::RGBA> Image
  {
    ::boost::filesystem::load_binary_file(
      PathToFontsDirectory.parent_path() / _RelativePathToSourceFile)
  };

  m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("id"), _TextureId + uT(".Data") },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Texture") },
        { uT("data"), Image.GetData().Buffer.data() },
        { uT("width"), static_cast<int>(Image.GetData().Width) },
        { uT("height"), static_cast<int>(Image.GetData().Height) },
      }),
      Component_t::Make(
      {
        { uT("id"), _TextureId },
        { uT("type"), uT("Texture") },
      }),
    });
}

size_t ExtraWindow::BuildLights(void)
{
  const Id Id;

  m_Objects[Id.GetId()] = m_pRenders->Obtain(
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
        { uT("id"), uT("Example.Light.Direction.Direction") },
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
        { uT("id"), uT("Example.Light.Point.Position") },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), 0.0f },
        { uT("y"), 0.0f },
        { uT("z"), 0.0f },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Point.Attenuation") },
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
        { uT("id"), uT("Example.Light.Point.Position") },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), 0.0f },
        { uT("y"), 0.0f },
        { uT("z"), 4.0f },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Light.Point.Attenuation") },
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
    });

  return Id.GetId();
}

size_t ExtraWindow::BuildCamera(const Component_t::ComponentPtr_t & _pRotation)
{
  const Id Id;

  m_Objects[Id.GetId()] = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        //{ uT("id"), uT("Example.Data.Position.Camera") },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), 0.0f },
        { uT("y"), 0.0f },
        { uT("z"), 0.0f },
      }),
      _pRotation,
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

  return Id.GetId();
}

size_t ExtraWindow::BuildCube(const Component_t::ComponentPtr_t & _pRotation, 
  float _PositionZ, float _Scale)
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

  using namespace ::alicorn::extension::std;

  /*const*/ ::std::vector<int> IndexData =
  {
     0,  1,  2,   2,  1,  3,
     4,  5,  6,   6,  5,  7,
     8,  9, 10,  10,  9, 11,
    12, 13, 14,  14, 13, 15,
    16, 17, 18,  18, 17, 19,
    20, 21, 22,  22, 21, 23,
  };

  //IndexData += IndexData; // 24 треугольников
  //IndexData += IndexData; // 48 треугольников
  //IndexData += IndexData; // 96 треугольников
  //IndexData += IndexData; // 192 треугольников

  m_Objects[Id.GetId()] = m_pRenders->Obtain(
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
        { uT("id"), uT("Example.Buffer.Index.Cube") },
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
      _pRotation,
      Component_t::Make(
      {
        { uT("id"), uT("Example.Data.Position.") + Id.GetStringId() },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Position") },
        { uT("x"), 0.0f },
        { uT("y"), 0.0f },
        { uT("z"), _PositionZ },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Present.Geometry.") + Id.GetStringId() },
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Geometry") },
      })
    });

  return Id.GetId();
}

size_t ExtraWindow::BuildPlane(uint32_t _Color, float _A, float _B, float _C)
{
  using Vertex_t = ::covellite::api::Vertex::Gui;

  const Id Id;

  const ::std::vector<Vertex_t> VertexData =
  {
    { -2.0f, -2.0f,   _Color,  0.0f, 0.0f, },  // 0
    { -2.0f,  2.0f,   _Color,  0.0f, 1.0f, },  // 1
    {  2.0f, -2.0f,   _Color,  1.0f, 0.0f, },  // 2
    {  2.0f,  2.0f,   _Color,  1.0f, 1.0f, },  // 3
  };

  const ::std::vector<int> IndexData =
  {
     0,  1,  2,   2,  1,  3,
     0,  2,  1,   2,  3,  1,
  };

  m_Objects[Id.GetId()] = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("id"), uT("Example.State.Blend") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Blend") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Data") },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psColored") },
        { uT("data"), ::Pixel.data() },
        { uT("count"), ::Pixel.size() }
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Pixel.Plane") },
        { uT("type"), uT("Shader") },
        { uT("kind"), uT("Pixel") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Data") },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader.HLSL") },
        { uT("version"), uT("vs_4_0") },
        { uT("entry"), uT("VS") },
        { uT("data"), ::Vertex.data() },
        { uT("count"), ::Vertex.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Vertex.Plane") },
        { uT("type"), uT("Shader") },
        { uT("kind"), Vertex_t::GetName() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Vertex.Data") },
        { uT("type"), uT("Data") },
        { uT("kind"), Vertex_t::GetName() },
        { uT("data"), VertexData.data() },
        { uT("count"), VertexData.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Buffer.Vertex.Plane.") + Id.GetStringId() },
        { uT("type"), uT("Buffer") },
        { uT("kind"), Vertex_t::GetName() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Index.Data") },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Index") },
        { uT("data"), IndexData.data() },
        { uT("count"), IndexData.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Buffer.Index.Plane.") + Id.GetStringId() },
        { uT("type"), uT("Buffer") },
        { uT("kind"), uT("Index") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Data.Rotation.") + Id.GetStringId() },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Rotation") },
        { uT("x"), _A },
        { uT("y"), _B },
        { uT("z"), _C },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Present.Geometry.") + Id.GetStringId() },
        { uT("type"), uT("Present") },
        { uT("kind"), uT("Geometry") },
      })
    });

  return Id.GetId();
}
