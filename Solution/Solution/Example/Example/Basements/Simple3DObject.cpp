
#include "stdafx.h"
#include "Simple3DObject.hpp"
#include <boost/any.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/logger.hpp>
#include <glm/glm.force.hpp>
#include <Covellite/Api/Constant.hpp>
#include "Demo/Constants.hpp"

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

  const auto Step = static_cast<int>(sqrt(_CubesCount));

  auto Offset = 0.0f;
  for (int i = Step; i > 1; i /= 2)
  {
    Offset += (float)Step / (2 * i);
  }

  m_Camera = BuildCamera();
  BuildCubeData(_PolygonsCount / 12);

  if (Constant::GetSettings<bool>(uT("IsCubeInstance")))
  {
    BuildInstanceCubes(_CubesCount, Step, Offset);
  }
  else
  {
    BuildSimpleCubes(Step, Offset);
  }

  // Вызов для большого количества кубов только Present'a не увеличивает fps!

  // Рендеринг ~15 fps (Release версия после оптимизации):

  // ********** 21.02.2019 - обычный рендеринг c освещением по Гуро ********* //

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

  // ************* 24.09.2019 - инстансинг с PBR текстурами ***************** //

  // Рендеринг ~15 fps (Release версия после оптимизации):

  // Windows (старый ноутбук):
  // - OpenGL     - 4096 кубов по 1536 полигонов каждый.
  // - DirectX10  - 16384 кубов по 768 полигона каждый.
  // - DirectX11  - 16384 кубов по 768 полигона каждый.

  // Windows (новый ноутбук):
  // - OpenGL     - ??? кубов по ??? полигонов каждый.
  // - DirectX10  - ??? кубов по ??? полигонов каждый.
  // - DirectX11  - 16384 кубов по 3072 полигона каждый.

  // Samsung Galaxy A5:
  // - OpenGLES3  - 1024 куба по 768 полигонов каждый (без PBR, с PBR - 10 fps).
}

Simple3DObject::~Simple3DObject(void) noexcept
{
  LOGGER(Info) << "Basements Simple3DObject destoyed.";

  // Рендеры не удаляются, т.к. объект класса Simple3DObject содержит уникальный
  // объект m_pRenders, который удаляется вместе со всеми рендерами.
  // m_pRenders->Remove(...);
}

void Simple3DObject::Notify(int _Message, const ::boost::any & _Value) /*override*/
{
  using namespace ::alicorn::extension::std;

  m_Scene.clear();
  m_Scene.push_back(m_Camera);

  if (_Message == ::events::Simple3DObject.LightsChanged)
  {
    const auto Value = ::boost::any_cast<int>(_Value);

    m_Scene.push_back(BuildShader(Value, 
      Constant::GetSettings<bool>(uT("IsCubeInstance"))));
    m_Scene.push_back(BuildLights(Value));
    m_Scene += m_Cubes;
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
        { uT("id"), uT("Example.Shader.Vertex.Default") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("vsVolume") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Pixel.Default") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("psLightened") },
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

auto Simple3DObject::BuildShader(int _LightsFlags, bool _IsInstanceMode) -> Id
{
  using Object_t = Component_t::Renders::Object_t;

  Component_t::ComponentPtr_t pVertexShader;

  if (_IsInstanceMode)
  {
    static const auto VertexShaderData = ::covellite::app::Vfs_t::GetInstance()
      .GetData("Data\\Shaders\\Cube.instance.fx");

    pVertexShader = Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Vertex.Cube") },
        { uT("type"), uT("Shader") },
        { uT("data"), VertexShaderData.data() },
        { uT("count"), VertexShaderData.size() },
        { uT("entry"), uT("vsVolume") },
        { uT("instance"), uT("f4") },
      });
  }
  else
  {
    pVertexShader = Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Vertex.Cube") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("vsVolume") },
      });
  }

  Object_t Components =
  {
    Component_t::Make(
    {
      { uT("id"), uT("Example.State.Sampler") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Sampler") },
    }),
    pVertexShader
  };

  using namespace ::alicorn::extension::std;

  static const auto PixelShaderData =
    ::covellite::app::Vfs_t::GetInstance().GetData("Data\\Shaders\\Pbr.ps.fx");

  if ((_LightsFlags & (1 << Lights::PBR)) != 0)
  {
    Components += Object_t
    {
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader") },
        { uT("data"), PixelShaderData.data() },
        { uT("count"), PixelShaderData.size() },
        { uT("entry"), uT("psPbrSimple") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Pixel.Cube.PBR") },
        { uT("type"), uT("Shader") },
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
    };
  }
  else
  {
    Components += Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Example.Shader.Pixel.Cube.Textured") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("psSimple") },
        { uT("data"), PixelShaderData.data() },
        { uT("count"), PixelShaderData.size() },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Texture.Albedo") },
      }),
    };
  }

  const Id Id;

  m_Objects[Id] = m_pRenders->Obtain(Components);

  return Id;
}

auto Simple3DObject::BuildLights(int _LightsFlags) -> Id
{
  using Lights_t = ::Lights_t;
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<Lights_t>;

  const auto IsActive = [=](Lights::Type _Type)
  {
    return (_LightsFlags & (1 << _Type)) != 0;
  };

  const auto ARGBtoFloat4 = [](uint32_t _HexColor)
  {
    return ::glm::vec4
    {
      ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
      ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
      ((_HexColor & 0x000000FF) >> 0) / 255.0f,
      ((_HexColor & 0xFF000000) >> 24) / 255.0f
    };
  };

  const auto pLights = ::std::make_shared<Lights_t>();
  auto & Lights = *pLights;
  memset(&Lights, 0x00, sizeof(Lights_t));

  const float X = 5.0f;
  const float Y = 1.0f;
  const float Z = 1.0f;

  const float Const = 0.0f;
  const float Linear = 0.1f;
  const float Exponent = 0.05f;

  /// [Create lights]
  if (IsActive(Lights::Ambient))
  {
    Lights.Ambient.IsValid = 1;
    Lights.Ambient.Color = ARGBtoFloat4(0xFF303030);
  }
    
  if (IsActive(Lights::Directional))
  {
    Lights.Direction.IsValid = 1;
    Lights.Direction.Color = ARGBtoFloat4(0xFF808080);
    Lights.Direction.Direction = ::glm::vec4{ 1.0f, -1.0f, 1.0f, 1.0f };
  }
  /// [Create lights]

  if (IsActive(Lights::Red))
  {
    auto & PointLight = Lights.Points.Lights[Lights.Points.UsedSlotCount++];
    PointLight.Color = ARGBtoFloat4(0xFFFF0000);
    PointLight.Position = ::glm::vec4{ X, 0.0f, Z, 1.0f };
    PointLight.Attenuation = ::glm::vec4{ Const, Linear, Exponent, 0.0f };
  }

  if (IsActive(Lights::Green))
  {
    auto & PointLight = Lights.Points.Lights[Lights.Points.UsedSlotCount++];
    PointLight.Color = ARGBtoFloat4(0xFF00FF00);
    PointLight.Position = ::glm::vec4{ X, Y, -Z, 1.0f };
    PointLight.Attenuation = ::glm::vec4{ Const, Linear, Exponent, 0.0f };
  }

  if (IsActive(Lights::Blue))
  {
    auto & PointLight = Lights.Points.Lights[Lights.Points.UsedSlotCount++];
    PointLight.Color = ARGBtoFloat4(0xFF0000FF);
    PointLight.Position = ::glm::vec4{ X, -Y, -Z, 1.0f };
    PointLight.Attenuation = ::glm::vec4{ Const, Linear, Exponent, 0.0f };
  }

  const Id Id;

  const BufferMapper_t Mapper = [pLights](Lights_t * _pLights)
  {
    *_pLights = *pLights;
    return false;
  };

  m_Objects[Id] = m_pRenders->Obtain(Component_t::Renders::Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Example.3DCube.Lights.") +  Id.GetStringId() },
        { uT("type"), uT("Buffer") },
        { uT("mapper"), Mapper },
      }),
    });

  return Id;
}

void Simple3DObject::BuildSimpleCubes(int _Step, float _Offset)
{
  for (int x = 0; x < _Step; x++)
  {
    for (int y = 0; y < _Step; y++)
    {
      m_Cubes.push_back(BuildSimpleCubeObject(
        (10.0f / _Step) * (x - _Offset),
        (10.0f / _Step) * (y - _Offset),
        6.0f / _Step));
    }
  }
}

void Simple3DObject::BuildInstanceCubes(
  int _CubesCount, 
  int _Step, 
  float _Offset)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  m_InstanceData.resize(_Step * _Step * 4);

  for (int x = 0; x < _Step; x++)
  {
    for (int y = 0; y < _Step; y++)
    {
      auto * Dest = m_InstanceData.data() + 4 * ((y * _Step) + x);

      Dest[0] = 0.0f;
      Dest[1] = -(10.0f / _Step) * (x - _Offset);
      Dest[2] = (10.0f / _Step) * (y - _Offset);
      Dest[3] = 0.0f;
    }
  }

  const BufferMapper_t Mapper = [=](void * _pData)
  {
    if (_pData == nullptr) return true;

    memcpy(_pData, m_InstanceData.data(), m_InstanceData.size() * sizeof(float));
    return false;
  };

  const Id Id;

  m_Objects[Id] = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("id"), uT("Example.Buffer.Vertex.Cube") },
        { uT("type"), uT("Buffer") },
        { uT("data"), static_cast<const Vertex_t *>(m_VertexData.data()) },
        { uT("count"), m_VertexData.size() },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Scale") },
        { uT("x"), 6.0f / _Step },
        { uT("y"), 6.0f / _Step },
        { uT("z"), 6.0f / _Step },
      }),
      m_pCubeRotation,
      Component_t::Make(
      {
        { uT("id"), uT("Example.Transform.Cubes") },
        { uT("type"), uT("Transform") },
      }),
      Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Buffer") },
        { uT("size"), (::std::size_t)(_CubesCount * 4 * sizeof(float)) },
        { uT("count"), (::std::size_t)_CubesCount },
        { uT("mapper"), Mapper },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Present") },
        { uT("type"), uT("Present") },
        { uT("data"), static_cast<const int *>(m_IndexData.data()) },
        { uT("count"), m_IndexData.size() },
      })
    });

  m_Cubes.push_back(Id);
}

void Simple3DObject::BuildCubeData(int _PolygonsFactor)
{
  using namespace ::alicorn::extension::std;

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

    m_VertexData += ::std::vector<Vertex_t>
    {
      {  0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, },
      { X1,    Y1,   0.5f,  1.0f,      0.5f + X1,  0.5f - Y1,     0.0f, 0.0f, 1.0f,  0.0f, },
      { X2,    Y2,   0.5f,  1.0f,      0.5f + X2,  0.5f - Y2,     0.0f, 0.0f, 1.0f,  0.0f, },
      { -0.5f, -0.5f, 0.5f,  1.0f,      0.0f,       1.0f,          0.0f, 0.0f, 1.0f,  0.0f, },
      { -X1,   -Y1,   0.5f,  1.0f,      0.5f - X1,  0.5f + Y1,     0.0f, 0.0f, 1.0f,  0.0f, },
      { -X2,   -Y2,   0.5f,  1.0f,      0.5f - X2,  0.5f + Y2,     0.0f, 0.0f, 1.0f,  0.0f, },

      { 0.5f, 0.5f,  0.5f,   1.0f,      1.0f,       0.0f,          1.0f, 0.0f, 0.0f,  0.0f, },
      { 0.5f, X1,    Y1,     1.0f,      0.5f + X1,  0.5f - Y1,     1.0f, 0.0f, 0.0f,  0.0f, },
      { 0.5f, X2,    Y2,     1.0f,      0.5f + X2,  0.5f - Y2,     1.0f, 0.0f, 0.0f,  0.0f, },
      { 0.5f, -0.5f, -0.5f,  1.0f,      0.0f,       1.0f,          1.0f, 0.0f, 0.0f,  0.0f, },
      { 0.5f, -X1,   -Y1,    1.0f,      0.5f - X1,  0.5f + Y1,     1.0f, 0.0f, 0.0f,  0.0f, },
      { 0.5f, -X2,   -Y2,    1.0f,      0.5f - X2,  0.5f + Y2,     1.0f, 0.0f, 0.0f,  0.0f, },

      { 0.5f, 0.5f,  0.5f,  1.0f,      1.0f,      0.0f,           0.0f, 1.0f, 0.0f,  0.0f, },
      { X2,   0.5f,  Y2,    1.0f,      0.5f + X2, 0.5f - Y2,      0.0f, 1.0f, 0.0f,  0.0f, },
      { X1,   0.5f,  Y1,    1.0f,      0.5f + X1, 0.5f - Y1,      0.0f, 1.0f, 0.0f,  0.0f, },
      { -0.5f, 0.5f, -0.5f,  1.0f,      0.0f,      1.0f,           0.0f, 1.0f, 0.0f,  0.0f, },
      { -X2,   0.5f, -Y2,    1.0f,      0.5f - X2, 0.5f + Y2,      0.0f, 1.0f, 0.0f,  0.0f, },
      { -X1,   0.5f, -Y1,    1.0f,      0.5f - X1, 0.5f + Y1,      0.0f, 1.0f, 0.0f,  0.0f, },

      { 0.5f,  0.5f, -0.5f, 1.0f,      1.0f,      1.0f,           0.0f, 0.0f, -1.0f, 0.0f, },
      { X2,    Y2,   -0.5f, 1.0f,      0.5f + X2, 0.5f + Y2,      0.0f, 0.0f, -1.0f, 0.0f, },
      { X1,    Y1,   -0.5f, 1.0f,      0.5f + X1, 0.5f + Y1,      0.0f, 0.0f, -1.0f, 0.0f, },
      { -0.5f, -0.5f, -0.5f, 1.0f,      0.0f,      0.0f,           0.0f, 0.0f, -1.0f, 0.0f, },
      { -X2,   -Y2,   -0.5f, 1.0f,      0.5f - X2, 0.5f - Y2,      0.0f, 0.0f, -1.0f, 0.0f, },
      { -X1,   -Y1,   -0.5f, 1.0f,      0.5f - X1, 0.5f - Y1,      0.0f, 0.0f, -1.0f, 0.0f, },

      { -0.5f, 0.5f,  0.5f,  1.0f,      1.0f,       1.0f,         -1.0f, 0.0f, 0.0f,  0.0f, },
      { -0.5f, X2,    Y2,    1.0f,      0.5f + X2,  0.5f + Y2,    -1.0f, 0.0f, 0.0f,  0.0f, },
      { -0.5f, X1,    Y1,    1.0f,      0.5f + X1,  0.5f + Y1,    -1.0f, 0.0f, 0.0f,  0.0f, },
      { -0.5f, -0.5f, -0.5f, 1.0f,      0.0f,       0.0f,         -1.0f, 0.0f, 0.0f,  0.0f, },
      { -0.5f, -X2,   -Y2,   1.0f,      0.5f - X2,  0.5f - Y2,    -1.0f, 0.0f, 0.0f,  0.0f, },
      { -0.5f, -X1,   -Y1,   1.0f,      0.5f - X1,  0.5f - Y1,    -1.0f, 0.0f, 0.0f,  0.0f, },

      { 0.5f,  -0.5f,  0.5f, 1.0f,      1.0f,      1.0f,           0.0f, -1.0f, 0.0f, 0.0f, },
      { X1,    -0.5f,  Y1,   1.0f,      0.5f + X1, 0.5f + Y1,      0.0f, -1.0f, 0.0f, 0.0f, },
      { X2,    -0.5f,  Y2,   1.0f,      0.5f + X2, 0.5f + Y2,      0.0f, -1.0f, 0.0f, 0.0f, },
      { -0.5f, -0.5f, -0.5f, 1.0f,      0.0f,      0.0f,           0.0f, -1.0f, 0.0f, 0.0f, },
      { -X1,   -0.5f, -Y1,   1.0f,      0.5f - X1, 0.5f - Y1,      0.0f, -1.0f, 0.0f, 0.0f, },
      { -X2,   -0.5f, -Y2,   1.0f,      0.5f - X2, 0.5f - Y2,      0.0f, -1.0f, 0.0f, 0.0f, },
    };
  }

  m_IndexData.reserve(m_VertexData.size());

  for (int i = 0; i < static_cast<int>(m_VertexData.size()); i++)
  {
    m_IndexData.push_back(i);
  }
}

auto Simple3DObject::BuildSimpleCubeObject(
  float _PositionX, float _PositionY, // Положение относительно экранных координат
  float _Scale) -> Id
{
  const Id Id;

  /// [Textured object]
  m_Objects[Id] = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("id"), uT("Example.Buffer.Vertex.Cube") },
        { uT("type"), uT("Buffer") },
        { uT("data"), static_cast<const Vertex_t *>(m_VertexData.data()) },
        { uT("count"), m_VertexData.size() },
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
        { uT("id"), uT("Example.Transform.") + Id.GetStringId() },
        { uT("type"), uT("Transform") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Example.Present") },
        { uT("type"), uT("Present") },
        { uT("data"), static_cast<const int *>(m_IndexData.data()) },
        { uT("count"), m_IndexData.size() },
      })
    });
  /// [Textured object]

  return Id;
}
