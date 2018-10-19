
#include "stdafx.h"
#include "OpenGLCommon.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

class OpenGLTexture final :
  public IGraphicApi::ITexture
{
public:
  void Render(void) final
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
  }

private:
  GLuint m_TextureId;

public:
  explicit OpenGLTexture(const Data _Source)
  {
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
      _Source.Width, _Source.Height, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, _Source.pData);

    const auto Error = glGetError();
    if (Error != GL_NO_ERROR)
    {
      throw STD_EXCEPTION << "Create texture error: " << Error;
    }
  }
  ~OpenGLTexture(void)
  {
    glDeleteTextures(1, &m_TextureId);
  }
};

class OpenGLGeometry final :
  public IGraphicApi::IGeometry
{
public:
  void Update(float _X, float _Y) final
  {
    glPushMatrix();
    glTranslatef(_X, _Y + m_Top, 0);
  }

  void Render(void) final
  {
    glVertexPointer(2, GL_FLOAT, sizeof(IGraphicApi::Vertex), &m_Vertices[0].x);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(IGraphicApi::Vertex),
      &m_Vertices[0].Color);
    glEnableClientState(GL_COLOR_ARRAY);

    if (m_pTexture != nullptr)
    {
      m_pTexture->Render();

      glTexCoordPointer(2, GL_FLOAT, sizeof(IGraphicApi::Vertex),
        &m_Vertices[0].u);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT,
      m_Indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
  }

private:
  ::std::vector<IGraphicApi::Vertex> m_Vertices;
  ::std::vector<int> m_Indices;
  IGraphicApi::ITexture * m_pTexture;
  const int m_Top;

public:
  explicit OpenGLGeometry(const IGeometry::Data & _SourceData, int _Top = 0) :
    m_Vertices(_SourceData.VerticesCount),
    m_Indices(_SourceData.IndicesCount),
    m_pTexture(_SourceData.pTexture),
    m_Top(_Top)
  {
    memcpy(m_Vertices.data(), _SourceData.pVertices,
      m_Vertices.size() * sizeof(IGraphicApi::Vertex));
    memcpy(m_Indices.data(), _SourceData.pIndices,
      m_Indices.size() * sizeof(int));
  }

private:
  FRIEND_TEST(OpenGL_test, /*DISABLED_*/Test_CreateGeometry_WithoutTexture);
  FRIEND_TEST(OpenGL_test, /*DISABLED_*/Test_CreateGeometry);
  FRIEND_TEST(OpenGLES_test, /*DISABLED_*/Test_CreateGeometry_WithoutTexture);
  FRIEND_TEST(OpenGLES_test, /*DISABLED_*/Test_CreateGeometry);
};

OpenGLCommon::OpenGLCommon(const Data & _Data, const String_t & _PreVersion) :
  m_BackgroundColor(_Data.BkColor),
  m_Top(_Data.Top),
  m_PreVersion(_PreVersion)
{
}

void OpenGLCommon::ClearWindow(void) /*final*/
{
  glDisable(GL_BLEND);

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

OpenGLCommon::String_t OpenGLCommon::GetUsingApi(void) const /*final*/
{
  using namespace ::alicorn::extension::std;

  auto Version = reinterpret_cast<const char *>(glGetString(GL_VERSION));

  return m_PreVersion + 
    string_cast<String, Locale::Default>(::std::string{ Version });
}

OpenGLCommon::ITexture * OpenGLCommon::Create(const ITexture::Data & _SourceData) /*final*/
{
  return new OpenGLTexture(_SourceData);
}

void OpenGLCommon::Destroy(ITexture * _pTexture) /*final*/
{
  delete _pTexture;
}

OpenGLCommon::IGeometry * OpenGLCommon::Create(const IGeometry::Data & _SourceData) /*final*/
{
  return new OpenGLGeometry(_SourceData, m_Top);
}

void OpenGLCommon::Destroy(IGeometry * _pGeometry) /*final*/
{
  delete _pGeometry;
}

void OpenGLCommon::EnableScissorRegion(int _X, int _Y, int _Width, int _Height) /*final*/
{
  glEnable(GL_SCISSOR_TEST);

  GLint ViewPort[4] = { 0 };
  glGetIntegerv(GL_VIEWPORT, ViewPort);
  glScissor(_X, ViewPort[3] - (_Y + _Height + m_Top), _Width, _Height);
}

void OpenGLCommon::DisableScissorRegion(void) /*final*/
{
  glDisable(GL_SCISSOR_TEST);
}

void OpenGLCommon::Render(void) /*final*/
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_DITHER);
  glDisable(GL_LIGHTING);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glShadeModel(GL_SMOOTH);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

} // namespace renderer

} // namespace api

} // namespace covellite
