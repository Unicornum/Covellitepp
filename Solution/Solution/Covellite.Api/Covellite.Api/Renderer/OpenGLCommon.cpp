
#include "stdafx.h"
#include "OpenGLCommon.hpp"
#include <deque>
#include <alicorn/cpp/math.hpp>
#include <Covellite/Api/Vertex.hpp>
#include "Component.hpp"
#include "GLMath.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

OpenGLCommon::OpenGLCommon(const Renderer::Data & _Data, const String_t & _PreVersion) :
  m_BackgroundColor(_Data.BkColor),
  m_Top(_Data.Top),
  m_PreVersion(_PreVersion)
{
  m_Creators =
  {
    { uT("Data"), [&](const ComponentPtr_t & _pComponent)
      { 
        m_PreRenderComponent.push_back(_pComponent);
        return Render_t{};
      } },
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

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    };
  };

  auto CreateScissorState = [&](void) -> Render_t
  {
    auto pScissorRect = api::Component::Make({});

    for (; !m_PreRenderComponent.empty(); m_PreRenderComponent.pop_front())
    {
      const auto pComponent = m_PreRenderComponent.front();
      const auto Kind = pComponent->GetValue(uT("kind"), uT("Unknown"));

      if (Kind == uT("Rect")) pScissorRect = pComponent;
    }

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

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto OpenGLCommon::CreateLight(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto * pCurrentCameraId = &m_CurrentCameraId;
  auto * pLights = &m_Lights;

  const auto DefaultColor = 0xFF000000;

  auto pDirection = ::covellite::api::Component::Make(
    {
      { uT("z"), 1.0f },
    });

  auto pPosition = ::covellite::api::Component::Make(
    {
      { uT("z"), 1.0f },
      { uT("w"), 0.0f },
    });

  auto pAttenuation = ::covellite::api::Component::Make({});

  for (; !m_PreRenderComponent.empty(); m_PreRenderComponent.pop_front())
  {
    const auto pComponent = m_PreRenderComponent.front();
    const auto Kind = pComponent->GetValue(uT("kind"), uT("Unknown"));

    if (Kind == uT("Direction")) pDirection = pComponent;
    if (Kind == uT("Position")) pPosition = pComponent;
    if (Kind == uT("Attenuation")) pAttenuation = pComponent;
  }

  auto CreateAmbient = [&](void)
  {
    return [=](void)
    {
      Light Value;
      Value.Ambient = ARGBtoFloat4(
        _pComponent->GetValue(uT("ambient"), DefaultColor));
      Value.Diffuse = ARGBtoFloat4(
        _pComponent->GetValue(uT("diffuse"), DefaultColor));
      Value.Specular = ARGBtoFloat4(
        _pComponent->GetValue(uT("specular"), DefaultColor));

      (*pLights)[*pCurrentCameraId].push_back(Value);
    };
  };

  auto CreateDirection = [&](void)
  {
    return [=](void)
    {
      Light Value;
      Value.Ambient = ARGBtoFloat4(
        _pComponent->GetValue(uT("ambient"), DefaultColor));
      Value.Diffuse = ARGBtoFloat4(
        _pComponent->GetValue(uT("diffuse"), DefaultColor));
      Value.Specular = ARGBtoFloat4(
        _pComponent->GetValue(uT("specular"), DefaultColor));

      const Component::Position Data{ pDirection };

      Value.Position[0] = Data.X;
      Value.Position[1] = Data.Y;
      Value.Position[2] = Data.Z;
      Value.Position[3] = 0.0f; // w == 0.0f - direction

      (*pLights)[*pCurrentCameraId].push_back(Value);
    };
  };

  auto CreatePoint = [&](void)
  {
    return [=](void)
    {
      Light Value;
      Value.Ambient = ARGBtoFloat4(
        _pComponent->GetValue(uT("ambient"), DefaultColor));
      Value.Diffuse = ARGBtoFloat4(
        _pComponent->GetValue(uT("diffuse"), DefaultColor));
      Value.Specular = ARGBtoFloat4(
        _pComponent->GetValue(uT("specular"), DefaultColor));

      const Component::Position Data{ pPosition };

      Value.Position[0] = Data.X;
      Value.Position[1] = Data.Y;
      Value.Position[2] = Data.Z;
      Value.Position[3] =                   // w == 1.0f - point
        pPosition->GetValue(uT("w"), 1.0f); // “акой сложный способ из-за того,
                                            // что здесь должен быть 0.0f, если
                                            // pPosition не задан.

      Value.Const = pAttenuation->GetValue(uT("const"), 1.0f);
      Value.Linear = pAttenuation->GetValue(uT("linear"), 0.0f);
      Value.Exponent = pAttenuation->GetValue(uT("exponent"), 0.0f);

      (*pLights)[*pCurrentCameraId].push_back(Value);
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Ambient"), CreateAmbient },
    { uT("Direction"), CreateDirection },
    { uT("Point"), CreatePoint },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
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

    // glTexImage2D копирует переданные данные в видеопам€ть, поэтому копировать
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
  auto pData = _pComponent;

  for (; !m_PreRenderComponent.empty(); m_PreRenderComponent.pop_front())
  {
    const auto pComponent = m_PreRenderComponent.front();
    const auto Kind = pComponent->GetValue(uT("kind"), uT("Unknown"));

    if (Kind == uT("Texture")) pData = pComponent;
  }

  auto pTexture = ::std::make_shared<Texture>(pData);

  return [=](void)
  {
    pTexture->Render();
  };
}

auto OpenGLCommon::CreateBuffer(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto pData = _pComponent;

  for (; !m_PreRenderComponent.empty(); m_PreRenderComponent.pop_front())
  {
    const auto pComponent = m_PreRenderComponent.front();
    const auto Kind = pComponent->GetValue(uT("kind"), uT("Unknown"));

    if (Kind.Find(uT("Vertex.")) == 0 || Kind == uT("Index"))
    {
      pData = pComponent;
    }
  }

  auto CreateVertexGuiBuffer = [&](void) -> Render_t
  {
    using Vertex_t = ::covellite::api::Vertex::Gui;

    const Component::Buffer<Vertex_t> Info{ pData };

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
    using Vertex_t = ::covellite::api::Vertex::Textured;

    const Component::Buffer<Vertex_t> Info{ pData };

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
    const Component::Buffer<int> IndexBufferInfo{ pData };

    ::std::vector<int> IndexBufferData{
      IndexBufferInfo.pData, IndexBufferInfo.pData + IndexBufferInfo.Count };

    m_DrawElements = [=](void)
    {
      glDrawElements(GL_TRIANGLES, (GLsizei)IndexBufferData.size(),
        GL_UNSIGNED_INT, IndexBufferData.data());
    };

    return nullptr;
  };

  using Vertex_t = ::covellite::api::Vertex;

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { Vertex_t::Gui::GetName(), CreateVertexGuiBuffer },
    { Vertex_t::Textured::GetName(), CreateVertexTexturedBuffer },
    { uT("Index"), CreateIndexBuffer },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto OpenGLCommon::CreatePresent(const ComponentPtr_t & _pComponent) -> Render_t
{
  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Camera"), [&](void) { return CreateCamera(_pComponent); } },
    { uT("Geometry"), [&](void) { return CreateGeometry(_pComponent); } },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto OpenGLCommon::CreateCamera(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto Focal = _pComponent->GetValue(uT("focal"), uT("Disabled"));

  return (Focal == uT("Enabled")) ? 
    GetCameraFocal(_pComponent) :
    GetCameraGui(_pComponent);
}

auto OpenGLCommon::GetCameraCommon(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto DeptRender = GetDeptRender(_pComponent);
  const auto LightsRender = GetLightsRender(_pComponent);

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

    LightsRender();
  };
}

auto OpenGLCommon::GetCameraGui(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CommonRender = GetCameraCommon(_pComponent);

  return [=](void)
  {
    CommonRender();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat Viewport[4] = { 0 };
    glGetFloatv(GL_VIEWPORT, Viewport);

    glOrtho(Viewport[0], Viewport[0] + Viewport[2],
      Viewport[1] + Viewport[3], Viewport[1], -1, 1);
  };
}

auto OpenGLCommon::GetCameraFocal(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CommonRender = GetCameraCommon(_pComponent);

  auto pPosition = ::covellite::api::Component::Make({});
  auto pRotation = ::covellite::api::Component::Make({});

  for (; !m_PreRenderComponent.empty(); m_PreRenderComponent.pop_front())
  {
    const auto pComponent = m_PreRenderComponent.front();
    const auto Kind = pComponent->GetValue(uT("kind"), uT("Unknown"));

    if (Kind == uT("Position")) pPosition = pComponent;
    if (Kind == uT("Rotation")) pRotation = pComponent;
  }

  return [=](void)
  {
    CommonRender();

    // ************************** ћатрица проекции ************************** //

    const auto AngleY = _pComponent->GetValue(uT("angle"), 90.0f) *
      static_cast<float>(::alicorn::extension::cpp::math::GreedToRadian);
    const float zFar = 200.0f;

    GLfloat Viewport[4] = { 0 };
    glGetFloatv(GL_VIEWPORT, Viewport);

    const ::glm::mat4 Perspective = ::glm::perspectiveFovLH(AngleY,
      Viewport[2], Viewport[3], 0.01f, zFar);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(::glm::value_ptr(Perspective));

    // **************************** ћатрица вида **************************** //

    // “очка, куда смотрит камера - задаетс€ как компонент Data.Position.
    const Component::Position Look{ pPosition };

    auto GetEye = [&](void) -> ::glm::vec3
    {
      // –ассто€ние от камеры до Look.
      const auto Distance = _pComponent->GetValue(uT("distance"), 0.0f) + 0.1f;

      // “очка, где расположена камера - вычисл€етс€ на основе Look, Distance и
      // компонента Data.Rotation.

      const Component::Position Rot{ pRotation };

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
  Render_t DeptEnable = [](void)
  {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
  };

  Render_t DeptDisable = [](void)
  {
    glDisable(GL_DEPTH_TEST);
  };

  const auto Dept = _pComponent->GetValue(uT("dept"), uT("Disabled"));
  return (Dept == uT("Enabled")) ? DeptEnable : DeptDisable;
}

auto OpenGLCommon::GetLightsRender(const ComponentPtr_t & _pComponent) -> Render_t
{
  auto * pCurrentCameraId = &m_CurrentCameraId;
  auto * pLights = &m_Lights;
  const auto CameraId = _pComponent->Id;

  return [=](void)
  {
    if ((*pLights)[CameraId].empty())
    {
      glDisable(GL_LIGHTING);
    }
    else
    {
      glEnable(GL_LIGHTING);
    }

    int Index = GL_LIGHT0;

    for (const auto & Light : (*pLights)[CameraId])
    {
      glEnable(Index);
      glLightfv(Index, GL_AMBIENT, Light.Ambient.data());
      glLightfv(Index, GL_DIFFUSE, Light.Diffuse.data());
      glLightfv(Index, GL_SPECULAR, Light.Specular.data());
      glLightfv(Index, GL_POSITION, Light.Position);
      glLightf(Index, GL_CONSTANT_ATTENUATION, Light.Const);
      glLightf(Index, GL_LINEAR_ATTENUATION, Light.Linear);
      glLightf(Index, GL_QUADRATIC_ATTENUATION, Light.Exponent);
      Index++;
    }

    (*pLights)[CameraId].clear();
    *pCurrentCameraId = CameraId;
  };
}

auto OpenGLCommon::CreateGeometry(const ComponentPtr_t &) -> Render_t
{
  ::std::deque<Render_t> PreRenders;

  for (; !m_PreRenderComponent.empty(); m_PreRenderComponent.pop_front())
  {
    const auto pComponent = m_PreRenderComponent.front();
    const auto Kind = pComponent->GetValue(uT("kind"), uT("Unknown"));

    auto PositionRender = [=](void)
    {
      const Component::Position Data{ pComponent };
      glTranslatef(Data.X, Data.Y, Data.Z);
    };

    const auto RadianToGreed = static_cast<float>(
      ::alicorn::extension::cpp::math::RadianToGreed);

    auto RotationRender = [=](void)
    {
      const Component::Position Data{ pComponent };
      glRotatef(Data.X * RadianToGreed, 1.0f, 0.0f, 0.0f);
      glRotatef(Data.Y * RadianToGreed, 0.0f, 1.0f, 0.0f);
      glRotatef(Data.Z * RadianToGreed, 0.0f, 0.0f, 1.0f);
    };

    auto ScaleRender = [=](void)
    {
      const Component::Position Data{ pComponent };
      glScalef(Data.X, Data.Y, Data.Z);
    };

    ::std::map<String_t, Render_t> Renders =
    {
      { uT("Position"), PositionRender },
      { uT("Rotation"), RotationRender },
      { uT("Scale"), ScaleRender },
    };

    auto itRender = Renders.find(Kind);
    if (itRender == Renders.end()) continue;

    // OpenGL требует фомировани€ мартиц тансформации в обратном пор€дке!
    PreRenders.push_front(itRender->second);
  }

  PreRenders.push_front([](void)
  {
    // —охран€ем в стеке матрицу вида, сформированную камерой, чтобы после
    // рендеринга меша (при котором она будет домножена на матрицу 
    // трансформации) можно было восстановить ее обратно.
    glPushMatrix();
  });

  // «десь нужен захват локальной копии рендера (захват по значению
  // m_DrawElements приводит к отрисовке мусора), причем в тестах повторный 
  // вызов с другими данными индексного буфера проблем не вызывает.
  // ???  ак это протестировать ???
  const auto DrawElements = m_DrawElements;

  return [=](void)
  {
    for (auto & Render : PreRenders) Render();

    DrawElements();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

    // ¬осстанавливаем матрицу вида, сформированную камерой.
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
