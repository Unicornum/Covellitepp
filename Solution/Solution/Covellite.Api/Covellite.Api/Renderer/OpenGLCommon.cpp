
#include "stdafx.h"
#include "OpenGLCommon.hpp"
#include <alicorn/cpp/math.hpp>
#include <Covellite/Api/Defines.hpp>
#include "Component.hpp"
#include "fx/Data.h"

namespace covellite
{

namespace api
{

namespace renderer
{

class OpenGLCommon::Data final
{
  using CameraId_t = String_t;

public:
  template<class T>
  T & Get(void) = delete;

  void SetCameraId(const CameraId_t & _Id);

private:
  class ClearedLights
  {
  public:
    void Clear(void)
    {
      memset(&m_Lights, 0x00, sizeof(m_Lights));
    }

  public:
    ::Lights m_Lights;

  public:
    ClearedLights(void)
    {
      Clear();
    }
  };

  ::std::map<CameraId_t, ClearedLights> m_Lights;
  CameraId_t                            m_CurrentCameraId = uT("Unknown");
};

template<>
inline ::Lights & OpenGLCommon::Data::Get(void)
{
  return m_Lights[m_CurrentCameraId].m_Lights;
}

void OpenGLCommon::Data::SetCameraId(const CameraId_t & _Id)
{
  m_CurrentCameraId = _Id;
  const auto Lights = Get<::Lights>();
  m_Lights[m_CurrentCameraId].Clear();

  if (Lights.Ambient.IsValid == 0 &&
    Lights.Direction.IsValid == 0 &&
    Lights.Points.UsedSlotCount == 0)
  {
    glDisable(GL_LIGHTING);
    return;
  }

  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);

  static const GLfloat DefaultAmbientColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, DefaultAmbientColor);

  int Index = GL_LIGHT0;

  auto SetLight = [&](
    const Color_t & _Ambient, const Color_t & _Diffuse, const Color_t & _Specular,
    const ::glm::vec4 & _Position = { 0.0f, 0.0f, 1.0f, 0.0f },
    const ::glm::vec4 & _Attenuation = ::glm::vec4{ 1.0f, 0.0f, 0.0f, 0.0f })
  {
    glEnable(Index);
    glLightfv(Index, GL_AMBIENT, _Ambient.data());
    glLightfv(Index, GL_DIFFUSE, _Diffuse.data());
    glLightfv(Index, GL_SPECULAR, _Specular.data());
    glLightfv(Index, GL_POSITION, ::glm::value_ptr(_Position));
    glLightf(Index, GL_CONSTANT_ATTENUATION, _Attenuation.x);
    glLightf(Index, GL_LINEAR_ATTENUATION, _Attenuation.y);
    glLightf(Index, GL_QUADRATIC_ATTENUATION, _Attenuation.z);
    Index++;
  };

  static const auto DefaultColor = ARGBtoFloat4(0xFF000000);

  if (Lights.Ambient.IsValid != 0)
  {
    const auto & Light = Lights.Ambient;

    const auto Color = ARGBtoFloat4(Light.ARGBColor);
    SetLight(Color, DefaultColor, DefaultColor);
  }

  if (Lights.Direction.IsValid != 0)
  {
    const auto & Light = Lights.Direction;

    const auto Color = ARGBtoFloat4(Light.ARGBColor);
    SetLight(DefaultColor, Color, Color, Light.Direction);
  }

  for (uint32_t i = 0; i < Lights.Points.UsedSlotCount; i++)
  {
    const auto & Light = Lights.Points.Lights[i];

    const auto Color = ARGBtoFloat4(Light.ARGBColor);
    SetLight(DefaultColor, Color, Color, Light.Position, Light.Attenuation);
  }

  while (glIsEnabled(Index) && Index < (GL_LIGHT0 + MAX_LIGHT_POINT_COUNT))
  {
    glDisable(Index++);
  }
}

OpenGLCommon::OpenGLCommon(const Data_t & _Data, const String_t & _PreVersion) :
  m_Top{ _Data.Top },
  m_PreVersion{ _PreVersion },
  m_pData{ ::std::make_shared<Data>() }
{
  m_Creators =
  {
    { uT("Data"), [=](const ComponentPtr_t & _pComponent)
      {
        m_ServiceComponents.Add(_pComponent);
        return Render_t{};
      } },
    { uT("Camera"), [=](const ComponentPtr_t & _pComponent)
      { return CreateCamera(_pComponent); } },
    { uT("State"), [=](const ComponentPtr_t & _pComponent)
      { return CreateState(_pComponent); } },
    { uT("Fog"), [=](const ComponentPtr_t & _pComponent)
      { return CreateFog(_pComponent); } },
    { uT("Light"), [=](const ComponentPtr_t & _pComponent)
      { return CreateLight(_pComponent); } },
    { uT("Material"), [=](const ComponentPtr_t & _pComponent)
      { return CreateMaterial(_pComponent); } },
    { uT("Texture"), [=](const ComponentPtr_t & _pComponent)
      { return CreateTexture(_pComponent); } },
    { uT("Buffer"), [=](const ComponentPtr_t & _pComponent)
      { return CreateBuffer(_pComponent); } },
    { uT("Present"), [=](const ComponentPtr_t & _pComponent)
      { return CreatePresent(_pComponent); } },
    m_Updater.GetCreator(),
  };
}

OpenGLCommon::String_t OpenGLCommon::GetUsingApi(void) const /*final*/
{
  using namespace ::alicorn::extension::std;

  auto Version = reinterpret_cast<const char *>(glGetString(GL_VERSION));

  return m_PreVersion +
    string_cast<String, Locale::Default>(::std::string{ Version });
}

void OpenGLCommon::ResizeWindow(int32_t _Width, int32_t _Height) /*final*/
{
  // (x, y) - левый нижний угол!
  glViewport(0, 0, _Width, _Height - m_Top);
}

auto OpenGLCommon::GetCreators(void) const -> const Creators_t & /*override*/
{
  return m_Creators;
}

auto OpenGLCommon::CreateCamera(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CameraId = _pComponent->Id;

  const auto Camera = (_pComponent->Kind == uT("Perspective")) ?
    GetCameraPerspective(_pComponent) :
    GetCameraOrthographic(_pComponent);

  return [=](void)
  {
    Camera();

    m_pData->SetCameraId(CameraId);
  };
}

auto OpenGLCommon::CreateState(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CreateBlendState = [](void)
  {
    return [](void)
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    };
  };

  const auto CreateSamplerState = [&](void)
  {
    auto * pSamplerState = &m_SampleState;

    return [=](void)
    {
      *pSamplerState = [](void)
      {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      };
    };
  };

  const auto CreateScissorState = [&](void)
  {
    const auto pScissorRect = 
      m_ServiceComponents.Get({ { uT("Rect"), api::Component::Make({}) } })[0];

    Render_t ScissorEnabled = [=](void)
    {
      const Component::Scissor Rect{ pScissorRect };

      glEnable(GL_SCISSOR_TEST);

      GLint ViewPort[4] = { 0 };
      glGetIntegerv(GL_VIEWPORT, ViewPort);

      // (x, y) - левый нижний угол!
      glScissor(Rect.Left, ViewPort[3] - Rect.Bottom,
        Rect.Right - Rect.Left, Rect.Bottom - Rect.Top);
    };

    Render_t ScissorDisabled = [](void)
    {
      glDisable(GL_SCISSOR_TEST);
    };

    const Component::Scissor Scissor{ _pComponent };
    return (Scissor.IsEnabled) ? ScissorEnabled : ScissorDisabled;
  };

  const auto CreateDepthState = [&](void)
  {
    return GetDepthRender(
      _pComponent->GetValue(uT("enabled"), false),
      _pComponent->GetValue(uT("clear"), false),
      _pComponent->GetValue(uT("overwrite"), true));
  };

  const auto CreateClearState = [&](void)
  {
    const auto BackgroundColor =
      ARGBtoFloat4(_pComponent->GetValue(uT("color"), 0xFF000000));

    return [=](void)
    {
      glClearColor(
        BackgroundColor[0],
        BackgroundColor[1],
        BackgroundColor[2],
        BackgroundColor[3]);
      glClear(GL_COLOR_BUFFER_BIT);
    };
  };

  const auto CreateAlphaTestState = [&](void)
  {
    const auto Value = _pComponent->GetValue(uT("discard"), 0.0f);

    return [=](void)
    {
      glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_GREATER, Value);
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Blend"),     CreateBlendState     },
    { uT("Sampler"),   CreateSamplerState   },
    { uT("Scissor"),   CreateScissorState   },
    { uT("Depth"),     CreateDepthState     },
    { uT("Clear"),     CreateClearState     },
    { uT("AlphaTest"), CreateAlphaTestState },
  };

  return Creators[_pComponent->Kind]();
}

auto OpenGLCommon::CreateFog(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto sStyle = _pComponent->GetValue(uT("style"), uT("linear"));

  const auto Style =
    (sStyle == uT("linear")) ? GL_LINEAR :
    (sStyle == uT("exp")) ? GL_EXP :
    (sStyle == uT("exp2")) ? GL_EXP2 :
    throw STD_EXCEPTION << "Unknown style fog: " << sStyle <<
      " [" << _pComponent->Id << "].";

  const auto pFogData =
    m_ServiceComponents.Get({ { uT("Fog"), _pComponent } })[0];

  return [=](void)
  {
    const Component::Fog Fog{ pFogData };

    glEnable(GL_FOG);
    glHint(GL_FOG_HINT, GL_NICEST);
    glFogfv(GL_FOG_COLOR, ARGBtoFloat4(Fog.Color).data());
    glFogi(GL_FOG_MODE, Style);
    glFogf(GL_FOG_START, Fog.Near);
    glFogf(GL_FOG_END, Fog.Far);
    glFogf(GL_FOG_DENSITY, Fog.Density);
  };
}

auto OpenGLCommon::CreateLight(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto GetColor = [=](void)
  {
    return _pComponent->GetValue(uT("color"), 0xFF000000);
  };

  const auto ServiceComponents = m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({ { uT("z"), 1.0f }, { uT("w"), 0.0f } }) },
      { uT("Direction"), api::Component::Make({ { uT("z"), 1.0f } }) },
      { uT("Attenuation"), api::Component::Make({ }) },
    });

  auto CreateAmbient = [&](void)
  {
    return [=](void)
    {
      auto & Light = m_pData->Get<::Lights>().Ambient;

      Light.IsValid = 1;
      Light.ARGBColor = GetColor();
    };
  };

  auto CreateDirection = [&](void)
  {
    auto pDirection = ServiceComponents[1];

    return [=](void)
    {
      const Component::Position Direction{ pDirection };

      auto & Light = m_pData->Get<::Lights>().Direction;

      Light.IsValid = 1;
      Light.ARGBColor = GetColor();
      Light.Direction = { Direction.X, Direction.Y, Direction.Z, 
        0.0f }; // w == 0.0f - direction light
    };
  };

  auto CreatePoint = [&](void)
  {
    auto pPosition = ServiceComponents[0];
    auto pAttenuation = ServiceComponents[2];

    return [=](void)
    {
      const Component::Position Position{ pPosition };

      auto & Lights = m_pData->Get<::Lights>().Points;

      if (Lights.UsedSlotCount >= MAX_LIGHT_POINT_COUNT)
      {
        // 04 Январь 2019 19:30 (unicornum.verum@gmail.com)
        TODO("Запись в лог информации об избыточных источниках света.");
        return;
      }

      auto & Light = Lights.Lights[Lights.UsedSlotCount++];

      Light.ARGBColor = GetColor();
      Light.Position = { Position.X, Position.Y, Position.Z,
        pPosition->GetValue(uT("w"), 1.0f) }; // w == 1.0f - point light
                                            // Такой сложный способ из-за того,
                                            // что здесь должен быть 0.0f, если
                                            // pPosition не задан.
      Light.Attenuation = 
      {
        pAttenuation->GetValue(uT("const"), 1.0f),
        pAttenuation->GetValue(uT("linear"), 0.0f),
        pAttenuation->GetValue(uT("exponent"), 0.0f),
        0.0f
      };
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Ambient"), CreateAmbient },
    { uT("Direction"), CreateDirection },
    { uT("Point"), CreatePoint },
  };

  return Creators[_pComponent->Kind]();
}

auto OpenGLCommon::CreateMaterial(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto GetColor = [&](const String_t & _Name)
  {
    return ARGBtoFloat4(_pComponent->GetValue(_Name, 0xFF000000));
  };

  const auto Ambient = GetColor(uT("ambient"));
  const auto Diffuse = GetColor(uT("diffuse"));
  const auto Specular = GetColor(uT("specular"));
  const auto Emission = GetColor(uT("emission"));
  const Color_t Shininess = { _pComponent->GetValue(uT("shininess"), 0.0f) };

  return [=](void)
  {
    // GL_FRONT на Android'е вызывает ошибку!!!
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shininess.data());
  };
}

class OpenGLCommon::Texture final
{
public:
  void Render(void) const
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
  }

private:
  GLuint m_TextureId;

public:
  explicit Texture(const Component::Texture & _Data)
  {
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);

    // glTexImage2D копирует переданные данные в видеопамять, поэтому копировать
    // их в промежуточный буфер не нужно.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
      _Data.Width, _Data.Height, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, _Data.pData);

    const auto Error = glGetError();
    if (Error != GL_NO_ERROR)
    {
      throw STD_EXCEPTION << "Create texture error: " << Error;
    }
  }
  ~Texture(void)
  {
    glDeleteTextures(1, &m_TextureId);
  }
};

auto OpenGLCommon::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto pTextureData = 
    m_ServiceComponents.Get({ { uT("Texture"), _pComponent } })[0];

  static const ::std::vector<String_t> IgnoreDestination =
  {
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  const auto IsContained = [](const auto & _Container, const auto & _Value)
  {
    const auto itValue = 
      ::std::find(_Container.cbegin(), _Container.cend(), _Value);
    return (itValue != _Container.cend());
  };

  const auto Destination = 
    pTextureData->GetValue(uT("destination"), uT("albedo"));

  if (Destination == uT("albedo"))
  {
    const auto pTexture =
      ::std::make_shared<Texture>(Component::Texture{ pTextureData });

    return [=](void)
    {
      pTexture->Render();
      m_SampleState();
    };
  }
  else if (!IsContained(IgnoreDestination, Destination))
  {
    throw STD_EXCEPTION << "Unexpected destination texture: " << Destination <<
      uT(" [id=") << _pComponent->Id << uT("].");
  }

  return nullptr;
}

auto OpenGLCommon::CreateBuffer(const ComponentPtr_t & _pBuffer) -> Render_t
{
  const auto pBufferData = 
    m_ServiceComponents.Get({ { uT("Buffer"), _pBuffer } })[0];

  auto CreateIndexBuffer = [&](void) -> Render_t
  {
    auto * pDrawElements = &m_DrawElements;

    using Type_t = int;

    if (!pBufferData->IsType<const Type_t *>(uT("data")))
    {
      throw STD_EXCEPTION << "Unexpected buffer format [" << 
        "id: " << _pBuffer->Id << ", " <<
        "type: " << _pBuffer->Type << ", " <<
        "kind: " << _pBuffer->Kind << "].";
    }

    const Component::Buffer<Type_t> Info{ pBufferData };

    const ::std::vector<Type_t> Data{ Info.pData, Info.pData + Info.Count };

    return [=](void)
    {
      *pDrawElements = [=](void)
      {
        glDrawElements(GL_TRIANGLES, (GLsizei)Data.size(),
          GL_UNSIGNED_INT, Data.data());
      };
    };
  };

  auto CreatePolyhedronVertexBuffer = [&](void) -> Render_t
  {
    using Type_t = ::covellite::api::vertex::Polyhedron;
    using BufferMapper_t = cbBufferMap_t<Type_t>;

    if (!pBufferData->IsType<const Type_t *>(uT("data")))
    {
      return CreateIndexBuffer();
    }

    const Component::Buffer<Type_t> Info{ pBufferData };

    const auto pData = ::std::make_shared<::std::vector<Type_t>>(
      Info.pData, Info.pData + Info.Count);

    const auto & cbBufferMapper =
      _pBuffer->GetValue<const BufferMapper_t &>(uT("mapper"), nullptr);

    const Render_t StaticRender = [=](void)
    {
      glVertexPointer(3, GL_FLOAT, sizeof(Type_t), &(pData->data()->x));
      glEnableClientState(GL_VERTEX_ARRAY);

      glDisableClientState(GL_COLOR_ARRAY);

      glNormalPointer(GL_FLOAT, sizeof(Type_t), &(pData->data()->nx));
      glEnableClientState(GL_NORMAL_ARRAY);

      glTexCoordPointer(2, GL_FLOAT, sizeof(Type_t), &(pData->data()->tu));
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    };

    const Render_t DynamicRender = [=](void)
    {
      const auto IsDirty = cbBufferMapper(nullptr);
      if (IsDirty) cbBufferMapper(pData->data());

      StaticRender();
    };

    return (cbBufferMapper == nullptr) ? StaticRender : DynamicRender;
  };

  auto CreatePolygonVertexBuffer = [&](void) -> Render_t
  {
    using Type_t = ::covellite::api::vertex::Polygon;

    if (!pBufferData->IsType<const Type_t *>(uT("data")))
    {
      return CreatePolyhedronVertexBuffer();
    }

    const Component::Buffer<Type_t> Info{ pBufferData };

    const ::std::vector<Type_t> Data{ Info.pData, Info.pData + Info.Count };

    class Normal
    {
    public:
      float nx, ny, nz;
    };

    ::std::vector<Normal> NormalData{ Data.size(), Normal{ 0.0f, 0.0f, 1.0f } };
    NormalData.push_back(Normal{ 0.0f, 0.0f, 0.0f }); // Для тестов.

    return [=](void)
    {
      glVertexPointer(2, GL_FLOAT, sizeof(Type_t), &Data[0].x);
      glEnableClientState(GL_VERTEX_ARRAY);

      glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Type_t), &Data[0].ABGRColor);
      glEnableClientState(GL_COLOR_ARRAY);

      glNormalPointer(GL_FLOAT, sizeof(Normal), &NormalData[0].nx);
      glEnableClientState(GL_NORMAL_ARRAY);

      glTexCoordPointer(2, GL_FLOAT, sizeof(Type_t), &Data[0].u);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    };
  };

  return CreatePolygonVertexBuffer();
}

auto OpenGLCommon::CreatePresent(const ComponentPtr_t & _pComponent) -> Render_t
{
  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Geometry"), [&](void) { return CreateGeometry(_pComponent); } },
  };

  return Creators[_pComponent->Kind]();
}

auto OpenGLCommon::GetCameraCommon(void) -> Render_t
{
  const auto DisableDepthRender = GetDepthRender(false, false, false);

  return [=](void)
  {
    DisableDepthRender();

    glDisable(GL_FOG);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DITHER);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
  };
}

auto OpenGLCommon::GetCameraOrthographic(const ComponentPtr_t &) -> Render_t
{
  const auto CommonRender = GetCameraCommon();

  const auto ServiceComponents = m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({}) },
    });

  return [=](void)
  {
    const Component::Position Pos{ ServiceComponents[0] };

    CommonRender();

    GLfloat Viewport[4] = { 0 };
    glGetFloatv(GL_VIEWPORT, Viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glOrtho(
      Viewport[0] + Pos.X, Viewport[0] + Viewport[2] + Pos.X,
      Viewport[1] + Viewport[3] + Pos.Y, Viewport[1] + Pos.Y,
      -1.0f, 1.0f);
  };
}

auto OpenGLCommon::GetCameraPerspective(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CommonRender = GetCameraCommon();

  const auto ServiceComponents = m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({}) },
      { uT("Rotation"), api::Component::Make({}) },
    });

  return [=](void)
  {
    CommonRender();

    GLfloat Viewport[4] = { 0 };
    glGetFloatv(GL_VIEWPORT, Viewport);

    // ************************** Матрица проекции ************************** //

    const auto AngleY = _pComponent->GetValue(uT("fov"), 90.0f) *
      static_cast<float>(::alicorn::extension::cpp::math::GreedToRadian);
    const float zFar = 200.0f;

    const ::glm::mat4 Perspective = ::glm::perspectiveFovRH(AngleY,
      Viewport[2], Viewport[3], 0.01f, zFar);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(::glm::value_ptr(Perspective));

    // **************************** Матрица вида **************************** //

    // Точка, куда смотрит камера - задается как компонент Data.Position.
    const Component::Position Look{ ServiceComponents[0] };

    auto GetEye = [&](void) -> ::glm::vec3
    {
      // Расстояние от камеры до Look.
      const auto Distance = _pComponent->GetValue(uT("distance"), 0.0f) + 0.1f;

      // Точка, где расположена камера - вычисляется на основе Look, Distance и
      // компонента Data.Rotation.

      const Component::Position Rot{ ServiceComponents[1] };

      ::glm::mat4 Transform = ::glm::identity<::glm::mat4>();

      Transform = ::glm::translate(Transform,
        ::glm::vec3{ Look.X, Look.Y, Look.Z });
      Transform = ::glm::rotate(Transform,
        Rot.Z, ::glm::vec3{ 0.0f, 0.0f, 1.0f });
      Transform = ::glm::rotate(Transform,
        Rot.Y, ::glm::vec3{ 0.0f, 1.0f, 0.0f });
      Transform = ::glm::rotate(Transform,
        Rot.X, ::glm::vec3{ 1.0f, 0.0f, 0.0f });

      return Transform * ::glm::vec4{ Distance, 0.0f, 0.0f, 1.0f };
    };

    const ::glm::mat4 LookAt = ::glm::lookAtRH(
      GetEye(),
      ::glm::vec3{ Look.X, Look.Y, Look.Z },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f }); // Up

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(::glm::value_ptr(LookAt));
  };
}

auto OpenGLCommon::GetDepthRender(
  const bool _IsEnabled, 
  const bool _IsClear,
  const bool _IsOverwrite) -> Render_t
{
  const Render_t DepthDisable = [](void)
  {
    glDisable(GL_DEPTH_TEST);
  };

  const auto IsOverwrite = _IsOverwrite ? GL_TRUE : GL_FALSE;

  const Render_t DepthEnable = [=](void)
  {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(IsOverwrite);
  };

  const Render_t DepthClear = [=](void)
  {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(IsOverwrite);
    glClear(GL_DEPTH_BUFFER_BIT);
  };

  return _IsEnabled ? (_IsClear ? DepthClear : DepthEnable) : DepthDisable;
}

auto OpenGLCommon::CreateGeometry(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto GetPreRenderStaticGeometry = [&](void) -> MatrixBuilder_t
  {
    ::glm::mat4 MatrixWorld = ::glm::identity<::glm::mat4>();
    GetPreRenderGeometry()(MatrixWorld);

    return [MatrixWorld](::glm::mat4 & _Matrix)
    {
      _Matrix *= MatrixWorld;
    };
  };

  const auto Variety = _pComponent->GetValue(uT("variety"), uT("Default"));

  const auto PreRender =
    (Variety == uT("Default")) ? GetPreRenderGeometry() :
    (Variety == uT("Static")) ? GetPreRenderStaticGeometry() :
    (Variety == uT("Billboard")) ? GetPreRenderBillboardGeometry() :
      throw STD_EXCEPTION << "Unexpected variety: " << Variety <<
      " [id=" << _pComponent->Id << "].";

  return [=](void)
  {
    // Сохраняем в стеке матрицу вида, сформированную камерой, чтобы после
    // рендеринга меша (при котором она будет домножена на матрицу 
    // трансформации) можно было восстановить ее обратно.
    glPushMatrix();

    ::glm::mat4 MatrixView;
    glGetFloatv(GL_MODELVIEW_MATRIX, ::glm::value_ptr(MatrixView));

    PreRender(MatrixView);

    glLoadMatrixf(::glm::value_ptr(MatrixView));

    m_DrawElements();

    glDisable(GL_TEXTURE_2D);

    // Восстанавливаем матрицу вида, сформированную камерой.
    glPopMatrix();
  };
}

auto OpenGLCommon::GetPreRenderGeometry(void) -> MatrixBuilder_t
{
  ::std::deque<MatrixBuilder_t> PreRenders;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    // OpenGL требует фомирования мартиц тансформации в обратном порядке!
    PreRenders.push_front([_pPosition](::glm::mat4 & _Matrix)
    {
      const Component::Position Data{ _pPosition };
      _Matrix = ::glm::translate(_Matrix, ::glm::vec3{ Data.X, Data.Y, Data.Z });
    });
  };

  auto CreateRotation = [&](const ComponentPtr_t & _pRotation)
  {
    // OpenGL требует фомирования мартиц тансформации в обратном порядке!
    PreRenders.push_front([_pRotation](::glm::mat4 & _Matrix)
    {
      const Component::Rotation Data{ _pRotation };
      _Matrix = ::glm::rotate(_Matrix, Data.Z, ::glm::vec3{ 0.0f, 0.0f, 1.0f });
      _Matrix = ::glm::rotate(_Matrix, Data.Y, ::glm::vec3{ 0.0f, 1.0f, 0.0f });
      _Matrix = ::glm::rotate(_Matrix, Data.X, ::glm::vec3{ 1.0f, 0.0f, 0.0f });
    });
  };

  auto CreateScale = [&](const ComponentPtr_t & _pScale)
  {
    // OpenGL требует фомирования мартиц тансформации в обратном порядке!
    PreRenders.push_front([_pScale](::glm::mat4 & _Matrix)
    {
      const Component::Scale Data{ _pScale };
      _Matrix = ::glm::scale(_Matrix, ::glm::vec3{ Data.X, Data.Y, Data.Z });
    });
  };

  m_ServiceComponents.Process(
    {
      { uT("Position"), CreatePosition },
      { uT("Rotation"), CreateRotation },
      { uT("Scale"), CreateScale },
    });

  return [PreRenders](::glm::mat4 & _Matrix)
  {
    for (auto & Render : PreRenders) Render(_Matrix);
  };
}

auto OpenGLCommon::GetPreRenderBillboardGeometry(void) -> MatrixBuilder_t
{
  ::std::deque<MatrixBuilder_t> PreRenders;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    // OpenGL требует фомирования мартиц тансформации в обратном порядке!
    PreRenders.push_front([_pPosition](::glm::mat4 & _Matrix)
    {
      const Component::Position Data{ _pPosition };
      _Matrix = ::glm::translate(_Matrix, ::glm::vec3{ Data.X, Data.Y, Data.Z });
    });
  };

  m_ServiceComponents.Process({ { uT("Position"), CreatePosition } });

  PreRenders.push_front([=](::glm::mat4 & _Matrix)
  {
    auto Matrix = ::glm::transpose(_Matrix);
    Matrix[0][3] = 0.0f;
    Matrix[1][3] = 0.0f;
    Matrix[2][3] = 0.0f;
    Matrix[3][0] = 0.0f;
    Matrix[3][1] = 0.0f;
    Matrix[3][2] = 0.0f;
    Matrix[3][3] = 1.0f;

    _Matrix *= Matrix;
  });

  return [PreRenders](::glm::mat4 & _Matrix)
  {
    for (auto & Render : PreRenders) Render(_Matrix);
  };
}

/*static*/ auto OpenGLCommon::ARGBtoFloat4(uint32_t _HexColor) -> Color_t
{
  return Color_t
  {
    ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
    ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
    ((_HexColor & 0x000000FF) >> 0) / 255.0f,
    ((_HexColor & 0xFF000000) >> 24) / 255.0f,
  };
};

} // namespace renderer

} // namespace api

} // namespace covellite
