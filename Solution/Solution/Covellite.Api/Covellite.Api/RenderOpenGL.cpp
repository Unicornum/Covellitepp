
#include "stdafx.h"
#include <Covellite\Api\RenderOpenGL.hpp>
#include <alicorn\boost\filesystem.hpp>
#include <alicorn\image.hpp>

namespace covellite
{

namespace api
{

RenderOpenGL::RenderOpenGL(int _StatusBarHeight) :
  m_StatusBarHeight(_StatusBarHeight)
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_DITHER);
  glDisable(GL_LIGHTING);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glShadeModel(GL_SMOOTH);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void RenderOpenGL::RenderGeometry(
  Rocket::Core::Vertex * _pVertices, int ALICORN_UNUSED(_VerticesCount),
  int * _pIndices, int _IndicesCount,
  const Rocket::Core::TextureHandle _hTexture,
  const Rocket::Core::Vector2f & _Translation) /*override*/
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPushMatrix();

  glTranslatef(_Translation.x, _Translation.y + m_StatusBarHeight, 0);

  glVertexPointer(2, GL_FLOAT, sizeof(Rocket::Core::Vertex),
    &_pVertices[0].position);
  glEnableClientState(GL_VERTEX_ARRAY);

  glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Rocket::Core::Vertex),
    &_pVertices[0].colour);
  glEnableClientState(GL_COLOR_ARRAY);

  if (_hTexture != 0)
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(_hTexture));

    glTexCoordPointer(2, GL_FLOAT, sizeof(Rocket::Core::Vertex),
      &_pVertices[0].tex_coord);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  glDrawElements(GL_TRIANGLES, _IndicesCount, GL_UNSIGNED_INT, _pIndices);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();
}

void RenderOpenGL::EnableScissorRegion(bool _IsEnable) /*override*/
{
  if (_IsEnable) glEnable(GL_SCISSOR_TEST);
  else glDisable(GL_SCISSOR_TEST);
}

void RenderOpenGL::SetScissorRegion(int _X, int _Y,
  int _Width, int _Height) /*override*/
{
  GLint ViewPort[4];
  glGetIntegerv(GL_VIEWPORT, ViewPort);

  glScissor(_X, ViewPort[3] - (_Y + _Height + m_StatusBarHeight), _Width, _Height);
}

bool RenderOpenGL::LoadTexture(
  Rocket::Core::TextureHandle & _hTexture,
  Rocket::Core::Vector2i & _TextureDimensions,
  const Rocket::Core::String & _PathToFile) /*override*/
{
  using namespace ::alicorn::source;

  const image::Universal_t<image::pixel::RGBA> Image
  {
    ::boost::filesystem::load_binary_file(_PathToFile.CString())
  };

  _TextureDimensions.x = static_cast<int>(Image.GetData().Width);
  _TextureDimensions.y = static_cast<int>(Image.GetData().Height);

  return GenerateTexture(_hTexture,
    Image.GetData().Buffer.data(), _TextureDimensions);
}

bool RenderOpenGL::GenerateTexture(
  Rocket::Core::TextureHandle & _hTexture,
  const Rocket::Core::byte * _pSource,
  const Rocket::Core::Vector2i & _SourceDimensions) /*override*/
{
  GLuint TextureId = 0;
  glGenTextures(1, &TextureId);

  glBindTexture(GL_TEXTURE_2D, TextureId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
    _SourceDimensions.x, _SourceDimensions.y, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, _pSource);

  _hTexture = TextureId;

  return (glGetError() == GL_NO_ERROR);
}

void RenderOpenGL::ReleaseTexture(Rocket::Core::TextureHandle _hTexture) /*override*/
{
  glDeleteTextures(1, reinterpret_cast<GLuint *>(&_hTexture));
}

} // namespace api

} // namespace covellite
