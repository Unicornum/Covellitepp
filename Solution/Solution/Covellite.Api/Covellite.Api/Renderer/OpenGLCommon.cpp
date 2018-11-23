
#include "stdafx.h"
#include "OpenGLCommon.hpp"
#include "Component.hpp"

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
    { uT("Camera"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateCamera(_pData); } },
    { uT("State"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateState(_pData); } },
    { uT("Texture"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateTexture(_pData); } },
    { uT("Buffer"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateBuffer(_pData); } },
    { uT("DrawCall"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreateDrawCall(_pData); } },
    { uT("Position"), [&](const ComponentPtr_t & _pData) -> Render_t
      { return CreatePosition(_pData); } },
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
  glViewport(0, 0, _Width, _Height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  DoOrtho(_Width, _Height);
}

auto OpenGLCommon::GetCreators(void) const -> const Creators_t & /*override*/
{
  return m_Creators;
}

auto OpenGLCommon::CreateCamera(const ComponentPtr_t &) -> Render_t
{
  return [&]()
  {
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  };
}

auto OpenGLCommon::CreateState(const ComponentPtr_t & _pComponent) const -> Render_t
{
  auto CreateBlendState = [](void) -> Render_t
  {
    return [=]()
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    };
  };

  auto CreateSamplerState = [&](void) -> Render_t
  {
    return [=]()
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    };
  };

  auto CreateScissorState = [&](void) -> Render_t
  {
    const Component::Scissor Data{ _pComponent };

    Render_t ScissorEnabled = [&, _pComponent]()
    {
      const Component::Scissor Data{ _pComponent };

      glEnable(GL_SCISSOR_TEST);

      GLint ViewPort[4] = { 0 };
      glGetIntegerv(GL_VIEWPORT, ViewPort);
      glScissor(Data.Left, ViewPort[3] - (Data.Bottom + m_Top),
        Data.Right - Data.Left, Data.Bottom - Data.Top);
    };

    Render_t ScissorDisabled = []()
    {
      glDisable(GL_SCISSOR_TEST);
    };

    return (Data.IsEnabled) ? ScissorEnabled : ScissorDisabled;
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Blend"), CreateBlendState },
    { uT("Sampler"), CreateSamplerState },
    { uT("Scissor"), CreateScissorState },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto OpenGLCommon::CreatePosition(const ComponentPtr_t & _pComponent) -> Render_t
{
  return [&, _pComponent]()
  {
    const Component::Position Data{ _pComponent };

    glPushMatrix();
    glTranslatef(Data.X, Data.Y + m_Top, Data.Z);
  };
}

auto OpenGLCommon::CreateBuffer(const ComponentPtr_t & _pComponent) const->Render_t
{
  auto CreateVertexBuffer = [&](void) -> Render_t
  {
    const Component::Buffer<Vertex> Data{ _pComponent };

    return [=]()
    {
      glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &Data.pData->x);
      glEnableClientState(GL_VERTEX_ARRAY);

      glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &Data.pData->Color);
      glEnableClientState(GL_COLOR_ARRAY);

      glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &Data.pData->u);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    };
  };

  auto CreateIndexBuffer = [&](void)->Render_t
  {
    const Component::Buffer<int> Data{ _pComponent };

    return [=]()
    {
      glDrawElements(GL_TRIANGLES, (GLsizei)Data.Count, GL_UNSIGNED_INT,
        Data.pData);
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Vertex"), CreateVertexBuffer },
    { uT("Index"), CreateIndexBuffer },
  };

  return Creators[_pComponent->GetValue(uT("kind"), uT("Unknown"))]();
}

auto OpenGLCommon::CreateDrawCall(const ComponentPtr_t &) const -> Render_t
{
  return []()
  {
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
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

auto OpenGLCommon::CreateTexture(const ComponentPtr_t & _pComponent) const -> Render_t
{
  auto pTexture = ::std::make_shared<Texture>(_pComponent);

  return [=]()
  {
    pTexture->Render();
  };
}

} // namespace renderer

} // namespace api

} // namespace covellite
