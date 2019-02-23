
#include "stdafx.h"
#include "OpenGLCommon.hpp"
#include <alicorn/cpp/math.hpp>
#include <Covellite/Api/Vertex.hpp>
#include "Component.hpp"
#include "GLMath.hpp"
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

OpenGLCommon::OpenGLCommon(const Renderer::Data & _Data, const String_t & _PreVersion) :
  m_BackgroundColor(_Data.BkColor),
  m_Top{ _Data.Top },
  m_PreVersion{ _PreVersion },
  m_pData{ ::std::make_shared<Data>() }
{
  m_Creators =
  {
    { uT("Data"), [&](const ComponentPtr_t & _pComponent)
      {
        m_ServiceComponents.Add(_pComponent);
        return Render_t{};
      } },
    { uT("Camera"), [&](const ComponentPtr_t & _pComponent)
      { return CreateCamera(_pComponent); } },
    { uT("State"), [&](const ComponentPtr_t & _pComponent)
      { return CreateState(_pComponent); } },
    { uT("Light"), [&](const ComponentPtr_t & _pComponent)
      { return CreateLight(_pComponent); } },
    { uT("Material"), [&](const ComponentPtr_t & _pComponent)
      { return CreateMaterial(_pComponent); } },
    { uT("Texture"), [&](const ComponentPtr_t & _pComponent)
      { return CreateTexture(_pComponent); } },
    { uT("Buffer"), [&](const ComponentPtr_t & _pComponent)
      { return CreateBuffer(_pComponent); } },
    { uT("Present"), [&](const ComponentPtr_t & _pComponent)
      { return CreatePresent(_pComponent); } },
  };
}

OpenGLCommon::String_t OpenGLCommon::GetUsingApi(void) const /*final*/
{
  using namespace ::alicorn::extension::std;

  auto Version = reinterpret_cast<const char *>(glGetString(GL_VERSION));

  return m_PreVersion +
    string_cast<String, Locale::Default>(::std::string{ Version });
}

void OpenGLCommon::ClearFrame(void) /*final*/
{
  glClearColor(
    m_BackgroundColor.R,
    m_BackgroundColor.G,
    m_BackgroundColor.B,
    m_BackgroundColor.A);
  glClear(GL_COLOR_BUFFER_BIT);
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
  const auto Focal = _pComponent->GetValue(uT("focal"), uT("Disabled"));

  const auto Camera = 
    (_pComponent->Kind == uT("Perspective") || Focal == uT("Enabled")) ?
    GetCameraFocal(_pComponent) :
    GetCameraGui(_pComponent);

  return [=](void)
  {
    Camera();

    m_pData->SetCameraId(CameraId);
  };
}

auto OpenGLCommon::CreateState(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto CreateBlendState = [](void) -> Render_t
  {
    return []()
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    };
  };

  auto CreateSamplerState = [&](void) -> Render_t
  {
    return []()
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    };
  };

  auto CreateScissorState = [&](void) -> Render_t
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

    Render_t ScissorDisabled = []()
    {
      glDisable(GL_SCISSOR_TEST);
    };

    const Component::Scissor Scissor{ _pComponent };
    return (Scissor.IsEnabled) ? ScissorEnabled : ScissorDisabled;
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Blend"), CreateBlendState },
    { uT("Sampler"), CreateSamplerState },
    { uT("Scissor"), CreateScissorState },
  };

  return Creators[_pComponent->Kind]();
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
  explicit Texture(const ComponentPtr_t & _pComponent)
  {
    const Component::Texture Data{ _pComponent };

    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);

    // glTexImage2D копирует переданные данные в видеопамять, поэтому копировать
    // их в промежуточный буфер не нужно.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
      Data.Width, Data.Height, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, Data.pData);

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
  const auto pTexture = ::std::make_shared<Texture>(
    m_ServiceComponents.Get({ { uT("Texture"), _pComponent } })[0]);

  return [=](void)
  {
    pTexture->Render();
  };
}

auto OpenGLCommon::CreateBuffer(const ComponentPtr_t & _pBuffer) -> Render_t
{
  const auto pBufferData = 
    m_ServiceComponents.Get({ { uT("Buffer"), _pBuffer } })[0];

  auto CreateVertexGuiBuffer = [&](void) -> Render_t
  {
    using Vertex_t = ::covellite::api::Vertex::Polygon;

    const Component::Buffer<Vertex_t> Info{ pBufferData };

    ::std::vector<Vertex_t> Data{ Info.pData, Info.pData + Info.Count };

    return [=](void)
    {
      glVertexPointer(2, GL_FLOAT, sizeof(Vertex_t), &Data[0].x);
      glEnableClientState(GL_VERTEX_ARRAY);

      glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_t), &Data[0].ABGRColor);
      glEnableClientState(GL_COLOR_ARRAY);

      glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex_t), &Data[0].u);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    };
  };

  auto CreateVertexTexturedBuffer = [&](void) -> Render_t
  {
    using Vertex_t = ::covellite::api::Vertex::Polyhedron;

    const Component::Buffer<Vertex_t> Info{ pBufferData };

    ::std::vector<Vertex_t> Data{ Info.pData, Info.pData + Info.Count };

    return [=](void)
    {
      glVertexPointer(3, GL_FLOAT, sizeof(Vertex_t), &Data[0].x);
      glEnableClientState(GL_VERTEX_ARRAY);

      glNormalPointer(GL_FLOAT, sizeof(Vertex_t), &Data[0].nx);
      glEnableClientState(GL_NORMAL_ARRAY);

      glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex_t), &Data[0].tu);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    };
  };

  auto CreateIndexBuffer = [&](void) -> Render_t
  {
    auto * pDrawElements = &m_DrawElements;

    const Component::Buffer<int> IndexBufferInfo{ pBufferData };

    ::std::vector<int> IndexBufferData{
      IndexBufferInfo.pData, IndexBufferInfo.pData + IndexBufferInfo.Count };

    return [=](void)
    {
      *pDrawElements = [=](void)
      {
        glDrawElements(GL_TRIANGLES, (GLsizei)IndexBufferData.size(),
          GL_UNSIGNED_INT, IndexBufferData.data());
      };
    };
  };

  using Vertex_t = ::covellite::api::Vertex;

  if (pBufferData->IsType<const Vertex_t::Polygon *>(uT("data")))
  {
    return CreateVertexGuiBuffer();
  }
  else if (pBufferData->IsType<const Vertex_t::Polyhedron *>(uT("data")))
  {
    return CreateVertexTexturedBuffer();
  }

  return CreateIndexBuffer();
}

auto OpenGLCommon::CreatePresent(const ComponentPtr_t & _pComponent) -> Render_t
{
  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Camera"), [&](void) { return CreateCamera(_pComponent); } },
    { uT("Geometry"), [&](void) { return CreateGeometry(_pComponent); } },
  };

  return Creators[_pComponent->Kind]();
}

auto OpenGLCommon::GetCameraCommon(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto DeptRender = GetDeptRender(_pComponent);

  return [=](void)
  {
    DeptRender();

    glDisable(GL_BLEND);
    glDisable(GL_DITHER);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
  };
}

auto OpenGLCommon::GetCameraGui(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CommonRender = GetCameraCommon(_pComponent);

  return [=](void)
  {
    CommonRender();

    GLfloat Viewport[4] = { 0 };
    glGetFloatv(GL_VIEWPORT, Viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glOrthof(Viewport[0], Viewport[0] + Viewport[2],
      Viewport[1] + Viewport[3], Viewport[1], -1.0f, 1.0f);
  };
}

auto OpenGLCommon::GetCameraFocal(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CommonRender = GetCameraCommon(_pComponent);

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

    const auto AngleY = _pComponent->GetValue(uT("angle"), 90.0f) *
      static_cast<float>(::alicorn::extension::cpp::math::GreedToRadian);
    const float zFar = 200.0f;

    const ::glm::mat4 Perspective = ::glm::perspectiveFovLH(AngleY,
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

      ::glm::mat4 Transform = ::glm::mat4{ 1.0f };

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

    const ::glm::mat4 LookAt = ::glm::lookAt(
      GetEye(),
      ::glm::vec3{ Look.X, Look.Y, Look.Z },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f }); // Up

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(::glm::value_ptr(LookAt));
  };
}

auto OpenGLCommon::GetDeptRender(const ComponentPtr_t & _pComponent) -> Render_t
{
  Render_t DeptDisable = [](void)
  {
    glDisable(GL_DEPTH_TEST);
  };

  Render_t DeptEnable = [](void)
  {
    glEnable(GL_DEPTH_TEST);
  };

  Render_t DeptClear = [](void)
  {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
  };

  const auto Dept = _pComponent->GetValue(uT("dept"), uT("Disabled"));
  return 
    (Dept == uT("Clear")) ? DeptClear :
    (Dept == uT("Enabled")) ? DeptEnable : 
    DeptDisable;
}

auto OpenGLCommon::CreateGeometry(const ComponentPtr_t &) -> Render_t
{
  ::std::deque<Render_t> PreRenders;

  auto CreatePosition = [&](const ComponentPtr_t & _pPosition)
  {
    // OpenGL требует фомирования мартиц тансформации в обратном порядке!
    PreRenders.push_front([=](void)
    {
      const Component::Position Data{ _pPosition };
      glTranslatef(Data.X, Data.Y, Data.Z);
    });
  };

  const auto RadianToGreed = static_cast<float>(
    ::alicorn::extension::cpp::math::RadianToGreed);

  auto CreateRotation = [&](const ComponentPtr_t & _pRotation)
  {
    // OpenGL требует фомирования мартиц тансформации в обратном порядке!
    PreRenders.push_front([=](void)
    {
      const Component::Position Data{ _pRotation };
      glRotatef(Data.Z * RadianToGreed, 0.0f, 0.0f, 1.0f);
      glRotatef(Data.Y * RadianToGreed, 0.0f, 1.0f, 0.0f);
      glRotatef(Data.X * RadianToGreed, 1.0f, 0.0f, 0.0f);
    });
  };

  auto CreateScale = [&](const ComponentPtr_t & _pScale)
  {
    // OpenGL требует фомирования мартиц тансформации в обратном порядке!
    PreRenders.push_front([=](void)
    {
      const Component::Position Data{ _pScale };
      glScalef(Data.X, Data.Y, Data.Z);
    });
  };

  m_ServiceComponents.Process(
    {
      { uT("Position"), CreatePosition },
      { uT("Rotation"), CreateRotation },
      { uT("Scale"), CreateScale },
    });

  const auto * pDrawElements = &m_DrawElements;

  return [=](void)
  {
    // Сохраняем в стеке матрицу вида, сформированную камерой, чтобы после
    // рендеринга меша (при котором она будет домножена на матрицу 
    // трансформации) можно было восстановить ее обратно.
    glPushMatrix();

    for (auto & Render : PreRenders) Render();

    (*pDrawElements)();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

    // Восстанавливаем матрицу вида, сформированную камерой.
    glPopMatrix();
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
