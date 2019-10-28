
#include "stdafx.h"
#include "Shadows.hpp"
#include <random>
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/format.inl>
#include <alicorn/logger.hpp>
#include <Covellite/Api/Constant.hpp>
#include "Landscape.hpp"
#include "Constants.hpp"

#ifdef max
#undef max
#endif

namespace basement
{

namespace model
{

using ::covellite::app::Vfs_t;
static const float m_LightDistance = 100.0f;
static const float m_LightFov = 6.0f;

static const auto Random = [](const size_t _From, const size_t _To)
{
  static ::std::mt19937 Generator{ ::std::random_device{}() };
  return static_cast<size_t>(::std::uniform_int_distribution<>{
    static_cast<int>(_From), static_cast<int>(_To) }(Generator));
};

Shadows::Shadows(void) :
  GameObject(Another::Shadows)
{
  using Mesh_t = model::Landscape::Mesh;

  AddTexture("demo.stone.jpg");

  const auto TextureRatioXY = GetTexture(0).GetRatioXY();

  AddMesh<Mesh_t>("Boulder1.obj",
    TextureRatioXY, Rect{ 0.5f, 0.25f,  1.0f, 0.0f });
  AddMesh<Mesh_t>("Boulder2.obj",
    TextureRatioXY, Rect{ 0.5f, 0.25f,  1.0f, 0.0f });
  AddMesh<Mesh_t>("Boulder3.obj",
    TextureRatioXY, Rect{ 0.5f, 0.25f,  1.0f, 0.0f });
  AddMesh<Mesh_t>("Stone1.obj",
    TextureRatioXY, Rect{ 0.5f, 0.5f, 1.0f, 0.25f });
  AddMesh<Mesh_t>(Landscape::None,
    TextureRatioXY, Rect{ });

  const ::std::vector<Vertex_t> Vertexes =
  {
    { 0.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f, },  // 0
    { 1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f, },  // 1
    { 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, },  // 2
    { 1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, },  // 3
  };

  const ::std::vector<int> Indexes = { 0,  1,  2,   2,  1,  3, };

  ::std::vector<Mesh::Triangle_t> Triangles;

  for (::std::size_t i = 0; i < Indexes.size(); i += 3)
  {
    Mesh::Triangle_t Triangle;
    Triangle.Vertexes[0].m_Vertex = Vertexes[Indexes[i]];
    Triangle.Vertexes[1].m_Vertex = Vertexes[Indexes[i + 1]];
    Triangle.Vertexes[2].m_Vertex = Vertexes[Indexes[i + 2]];

    Triangles.push_back(Triangle);
  }

  AddMesh<Mesh>(Triangles);

  m_pLightCamera = Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Camera.Shadow") },
      { uT("type"), uT("Camera") },
      { uT("kind"), uT("Perspective") },
      { uT("distance"), m_LightDistance },
      { uT("fov"), m_LightFov },
      { uT("znear"), 90.0f },
      { uT("zfar"), 200.0f },
    });

  m_ShadowMap =
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Texture") },
      { uT("destination"), uT("depth") },
    }),
  };

  m_SurfaceTextures =
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Texture") },
      { uT("destination"), uT("albedo") },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Texture") },
      { uT("destination"), uT("metalness") },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Texture") },
      { uT("destination"), uT("roughness") },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Texture") },
      { uT("destination"), uT("depth") },
    })
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

  m_pLights = ::std::make_shared<Lights_t>();
  m_pLights->Ambient.IsValid = 1;
  m_pLights->Ambient.Color = ARGBtoFloat4(0xFF8080A0);
  m_pLights->Direction.IsValid = 1;
  m_pLights->Direction.Color = ARGBtoFloat4(0xFFF0F0B0);
  m_pLights->Direction.Direction = ::glm::vec4{ 0.0f, 1.0f, 0.66f, 0.0f };
  m_pLights->Points.UsedSlotCount = 0;

  m_pShaderData = ::std::make_shared<ShaderData>();

  const auto CellSceneRadius = 6;
  const auto CellRockRadius = 4;

  for (int y = -CellSceneRadius; y <= CellSceneRadius; y++)
  {
    for (int x = -CellSceneRadius; x <= CellSceneRadius; x++)
    {
      const CubeCoords CellPosition{ x, y };

      if (abs(CellPosition.GetZ()) <= CellSceneRadius)
      {
        using namespace ::alicorn::extension::std;

        const auto MeshIndex =
          (x == 0 && y == 0 && CellPosition.GetZ() == 0) ? 3 :
          (abs(x) < CellRockRadius && abs(y) < CellRockRadius &&
            abs(CellPosition.GetZ()) < CellRockRadius) ? Random(0, 2) : 4;

        const auto SceneObject =
          GetTransform() +
          GetMesh(MeshIndex).GetObject();

        m_SceneObjects_pass1 +=
          GetTransformData(CellPosition) +
          SceneObject;

        m_SceneObjects_pass2 +=
          GetShaderData(CellPosition) +
          SceneObject;
      }
    }
  }
}

auto Shadows::GetObject(const Any_t & _Value) const -> Objects_t /*override*/
{
  m_pCursorData = ::covellite::any_cast<CursorDataPtr_t>(_Value);
  auto pCursorData = m_pCursorData;

  const ::covellite::api::cbBufferMap_t<const void> TextureMapper = 
    [=](const void * _pData)
  {
    if (_pData == nullptr) return pCursorData->IsClick;
    pCursorData->IsClick = false;

    const auto * pData = reinterpret_cast<const uint32_t *>(_pData);

    const auto TextureABGRSample = pData[pCursorData->Y *
      static_cast<int32_t>(pCursorData->WindowX) + pCursorData->X];

    const int CellX = ((TextureABGRSample & 0x0000FF00) >> 8) - 127;
    const int CellY = ((TextureABGRSample & 0x00FF0000) >> 16) - 127;

    LOGGER(Info) << "Click to object in cell: " << CellX << ":" << CellY;

    return false;
  };

  m_SurfaceTextures[0]->SetValue(uT("mapper"), TextureMapper);

  using namespace ::alicorn::extension::std;

  return
  {
    GetPass1() // Только первый проход: 650 fps
    + GetPass2() // Первый и второй вместе: 280 fps
    + GetPass3() // Все три прохода: 220 fps
  };
}

Object_t Shadows::GetPass1(void) const
{
  // Набор объектов для создания теневой карты

  using namespace ::alicorn::extension::std;

  return
    GetShadowCamera() +
    GetShadowShaders() +
    m_SceneObjects_pass1;
}

Object_t Shadows::GetPass2(void) const
{
  // Рендеринг сцены во внеэкранную поверхность

  using namespace ::alicorn::extension::std;

  return
    GetSceneCamera() +
    GetSceneShaders() +
    GetSceneLights() +
    GetTexture(0).GetObject() +
    Object_t
    {
      m_ShadowMap[0],
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shadows.Texture.ShadowMap.depth") },
        { uT("type"), uT("Texture") },
      }),
    } +
    m_SceneObjects_pass2;
}

Object_t Shadows::GetPass3(void) const
{
  // Отображение текстуры с изображением + добавление теней + глубина резкости

  using namespace ::alicorn::extension::std;

  return
    GetFlatCamera() +
    GetFlatShaders() + 
    GetTextureSurface(false) +
    GetMesh(5).GetObject() +

    // **** Отображение указанной текстуры поверх итогового изображения ***** //
    //GetFlatShaders(uT("psSimpleShadowDept")) +
    //GetTextureObject(uT("Demo.Shadows.Texture.ShadowMap.depth")) +
    //GetMesh(5).GetObject() +

    //GetFlatShaders(uT("psSimpleSceneDept")) +
    //GetTextureObject(uT("Demo.Shadows.Texture.depth"), 1.0f) +
    //GetMesh(5).GetObject() +

    //GetFlatShaders(uT("psSimpleTexture")) +
    //GetTextureObject(uT("Demo.Shadows.Texture.albedo")) +
    //GetTextureObject(uT("Demo.Shadows.Texture.metalness")) +
    //GetTextureObject(uT("Demo.Shadows.Texture.roughness")) +
    //GetMesh(5).GetObject() +
    // ********************************************************************** //

    Object_t{};
}

Object_t Shadows::GetTextureSurface(const bool _IsTarget) const
{
  using namespace ::alicorn::extension::std;

  if (_IsTarget) // Набор текстур как внеэкранных поверхностей
  {
    return 
      m_SurfaceTextures +
      Object_t
      {
        Component_t::Make(
        {
          { uT("id"), uT("Demo.Shadows.BkSurface") },
          { uT("type"), uT("BkSurface") },
        }),
      };
  }

  // Те же текстуры как источник для изображения

  const auto pScale = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Scale") },
    });

  const auto pSurfaceTexture = m_SurfaceTextures[0];

  const Updater_t UpdaterSource = [=](const float)
  {
    const float Width = pSurfaceTexture->GetValue(uT("width"), 1024);
    const float Height = pSurfaceTexture->GetValue(uT("height"), 1024);

    pScale->SetValue(uT("x"), Width);
    pScale->SetValue(uT("y"), Height);
  };

  Object_t Result =
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Back.Surfase.UpdaterSource") },
      { uT("type"), uT("Updater") },
      { uT("function"), UpdaterSource },
    }),
    pScale,
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Flat.Transform") },
      { uT("type"), uT("Transform") },
    })
  };

  for (const auto & Texture : m_SurfaceTextures)
  {
    auto pTexture = Texture;

    const auto Destination = 
      pTexture->GetValue(uT("destination"), uT("Unknown"));

    Result += 
      Object_t
    {
      pTexture,
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shadows.Texture.") + Destination },
        { uT("type"), uT("Texture") },
      })
    };
  }

  return Result;
}

Object_t Shadows::GetShadowCamera(void) const
{
  const auto pLights = m_pLights;

  const auto pCameraRotation = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
    });

  const auto Pitch = pLights->Direction.Direction.z;

  pCameraRotation->SetValue(uT("x"), 0.0f);
  pCameraRotation->SetValue(uT("y"), -Pitch);

  const Updater_t Updater = [=](const float _Time)
  {
    // Захватывать здесь ::glm::vec4 нельзя, программа упадет в Release версии

    namespace math = ::alicorn::extension::cpp::math;

    const float Direction = math::Constant<float>::Pi *
      (fmod(_Time / 5.0f, 2.0f) - 1.0f);

    pLights->Direction.Direction.x = math::radian::Cos(Direction);
    pLights->Direction.Direction.y = math::radian::Sin(Direction);

    pCameraRotation->SetValue(uT("z"), Direction);
  };

  return
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Shadow.Updater") },
      { uT("type"), uT("Updater") },
      { uT("function"), Updater },
    }),
    pCameraRotation,
    m_pLightCamera,
    m_ShadowMap[0],
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.BkSurface.ShadowMap") },
      { uT("type"), uT("BkSurface") },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.State.Depth") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), true },
    }),
  };
}

/*static*/ Object_t Shadows::GetShadowShaders(void)
{
  static const auto ShaderData =
    Vfs_t::GetInstance().GetData("Data\\Shaders\\Shadows.pass1.fx");

  return Object_t
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Shado.Shader.Vertex") },
      { uT("type"), uT("Shader") },
      { uT("entry"), uT("vsVolume") },
      //{ uT("data"), ShaderData.data() },
      //{ uT("count"), ShaderData.size() },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Shadow.Shader.Pixel") },
      { uT("type"), uT("Shader") },
      { uT("entry"), uT("psShadow") },
      { uT("data"), ShaderData.data() },
      { uT("count"), ShaderData.size() },
    }),
  };
}

Object_t Shadows::GetSceneCamera(void) const
{
  const auto pCameraRotation = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
      { uT("y"), -0.35f },
      { uT("z"), 2.0f },
    });

  const Updater_t CameraUpdater = [=](const float _Time)
  {
    namespace math = ::alicorn::extension::cpp::math;

    const float CameraDirection = math::Constant<float>::Pi *
      (fmod(_Time / 100.0f, 2.0f) - 1.0f);

    pCameraRotation->SetValue(uT("z"), CameraDirection);
  };

  using namespace ::alicorn::extension::std;

  return 
    Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shadows.Camera.Scene.Updater") },
        { uT("type"), uT("Updater") },
        { uT("function"), CameraUpdater },
      }),
      pCameraRotation,
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shadows.Camera.Scene") },
        { uT("type"), uT("Camera") },
        { uT("kind"), uT("Perspective") },
        { uT("distance"), 6.0f },
        { uT("fov"), Constant::Camera::Fov * math::Constant<float>::RadianToGreed },
        { uT("znear"), 0.5f },
        { uT("zfar"), 20.0f },
      })
    } +
    GetTextureSurface(true) +
    Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shadows.State.Depth") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Depth") },
        { uT("enabled"), true },
        { uT("clear"), true },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shadows.State.Clear") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Clear") },
        { uT("color"), 0xFF7EC0EE },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shadows.State.Sampler") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Sampler") },
      }),
      Component_t::Make(
      {
        { uT("id"), uT("Demo.Shadows.State.Blend") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Blend") },
      }),
    };
}

/*static*/ Object_t Shadows::GetSceneShaders(void)
{
  static const auto ShaderData = 
    Vfs_t::GetInstance().GetData("Data\\Shaders\\Shadows.pass2.fx");

  return Object_t
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Scene.Shader.Vertex") },
      { uT("type"), uT("Shader") },
      { uT("entry"), uT("vsScene") },
      { uT("data"), ShaderData.data() },
      { uT("count"), ShaderData.size() },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Scene.Shader.Pixel") },
      { uT("type"), uT("Shader") },
      { uT("entry"), uT("psLightened") },
      { uT("data"), ShaderData.data() },
      { uT("count"), ShaderData.size() },
    }),
  };
}

Object_t Shadows::GetSceneLights(void) const
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<::Lights_t>;

  const auto pLightCamera = m_pLightCamera;
  const auto pLights = m_pLights;
  const auto pShaderData = m_pShaderData;

  const BufferMapper_t LightsMapper = [=](Lights_t * _pLights)
  {
    memcpy(_pLights, &(*pLights), sizeof(Lights_t));

    pShaderData->Light.Projection = 
      pLightCamera->GetValue(uT("projection"), ::glm::mat4{ 0.0f });
    pShaderData->Light.View = 
      pLightCamera->GetValue(uT("view"), ::glm::mat4{ 0.0f });
    return false;
  };

  return
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Lights") },
      { uT("type"), uT("Buffer") },
      { uT("mapper"), LightsMapper },
    }),
  };
}

Object_t Shadows::GetShaderData(const CubeCoords & _CellPosition) const
{
  static int Index = 0;

  using ::alicorn::extension::boost::Format;

  // Использование здесь String::Replace() увеличивает время формирования
  // идентификатора в 10(!) раз, что существенно сказывается
  // на производительности.
  const auto Id = (Format{ uT("%1%") } % Index++).ToString();

  const auto pSrcShaderData = m_pShaderData;
  const auto ObjectColorIdX = (127 + _CellPosition.GetX()) / 255.0f;
  const auto ObjectColorIdY = (127 + _CellPosition.GetY()) / 255.0f;

  const ::covellite::api::cbBufferMap_t<void> ShaderDataMapper =
    [=](void * _pData)
  {
    auto * pDestShaderData = reinterpret_cast<ShaderData *>(_pData);
    pDestShaderData->Light = pSrcShaderData->Light;
    pDestShaderData->Object.IdX = ObjectColorIdX;
    pDestShaderData->Object.IdY = ObjectColorIdY;
    return false;
  };

  return
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.ShaderData") + Id },
      { uT("type"), uT("Buffer") },
      { uT("mapper"), ShaderDataMapper },
      { uT("name"), uT("cbShaderData") },
      { uT("size"), sizeof(ShaderData) },
    })
  };
}

Object_t Shadows::GetTransformData(const CubeCoords & _CellPosition) const
{
  const auto Rotate = Random(0, 5) * math::Constant<float>::Pi / 3.0f;
  const auto Scale = 1.0f + (m_ScaleFactor - 1.0f) * Random(0, 100) / 100.0f;
  const auto WorldPosition = _CellPosition.ToPlane();

  return
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
      { uT("z"), Rotate },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Scale") },
      { uT("z"), Scale },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
      { uT("x"), WorldPosition.first },
      { uT("y"), WorldPosition.second },
      { uT("z"), 0.0f },
    }),
  };
}

/*static*/ Object_t Shadows::GetTransform(void)
{
  static int Index = 0;

  using ::alicorn::extension::boost::Format;

  // Использование здесь String::Replace() увеличивает время формирования
  // идентификатора в 10(!) раз, что существенно сказывается
  // на производительности.
  const auto Id = (Format{ uT("%1%") } % Index++).ToString();

  return
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Transform.") + Id },
      { uT("type"), uT("Transform") },
    }),
  };
}

/*static*/ Object_t Shadows::GetFlatCamera(void)
{
  return
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Flat.Camera") },
      { uT("type"), uT("Camera") },
      { uT("kind"), uT("Orthographic") },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.State.Clear") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Clear") },
      { uT("color"), 0xFF7F7FA0 },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.State.Sampler") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Sampler") },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.State.Blend") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Blend") },
    }),
  };
}

Object_t Shadows::GetFlatShaders(const String_t & _psEntryPoint) const
{
  static const auto ShaderData =
    Vfs_t::GetInstance().GetData("Data\\Shaders\\Shadows.pass3.fx");

  const auto pCursorData = m_pCursorData;

  const ::covellite::api::cbBufferMap_t<void> CursorMapper =
    [=](void * _pData)
  {
    auto * pDestCursor = reinterpret_cast<Cursor *>(_pData);
    pDestCursor->X = pCursorData->X / pCursorData->WindowX;
    pDestCursor->Y = pCursorData->Y / pCursorData->WindowY;
    return false;
  };

  return Object_t
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Flat.Shader.Vertex") },
      { uT("type"), uT("Shader") },
      { uT("entry"), uT("vsFlat") },
      { uT("data"), ShaderData.data() },
      { uT("count"), ShaderData.size() },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Flat.Shader.Pixel.") + _psEntryPoint },
      { uT("type"), uT("Shader") },
      { uT("entry"), _psEntryPoint },
      { uT("data"), ShaderData.data() },
      { uT("count"), ShaderData.size() },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Cursor") },
      { uT("type"), uT("Buffer") },
      { uT("mapper"), CursorMapper },
      { uT("name"), uT("cbCursor") },
      { uT("size"), sizeof(Cursor) },
    })
  };
}

Object_t Shadows::GetTextureObject(
  const String_t & _TextureId,
  const float _Offset) const
{
  static int Index = 0;

  using ::alicorn::extension::boost::Format;

  // Использование здесь String::Replace() увеличивает время формирования
  // идентификатора в 10(!) раз, что существенно сказывается
  // на производительности.
  const auto Id = (Format{ uT("%1%") } % Index++).ToString();

  const auto pScale = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Scale") },
      { uT("x"), 512.0f },
      { uT("y"), 512.0f },
    });

  const auto pOffset = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
      { uT("x"), _Offset },
      //{ uT("y"), _Offset },
    });

  return
  {
    pOffset,
    pScale,
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shadows.Flat.Auxiliary.Transform.") + Id },
      { uT("type"), uT("Transform") },
    }),
    Component_t::Make(
    {
      { uT("id"), _TextureId },
      { uT("type"), uT("Texture") },
    })
  };
}

} // namespace model

} // namespace basement
