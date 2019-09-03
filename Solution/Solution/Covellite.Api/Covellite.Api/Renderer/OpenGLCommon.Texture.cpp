
#include "stdafx.h"
#include "OpenGLCommon.Texture.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

OpenGLCommon::Texture::Texture(const Component::Texture & _Data)
{
  glGenTextures(1, &m_TextureId);
  glBindTexture(GL_TEXTURE_2D, m_TextureId);

  // glTexImage2D копирует переданные данные в видеопамять, поэтому копировать
  // их в промежуточный буфер не нужно.
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
    _Data.Width, _Data.Height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, _Data.pData);

  glBindTexture(GL_TEXTURE_2D, 0);

  const auto Error = glGetError();
  if (Error != GL_NO_ERROR)
  {
    throw STD_EXCEPTION << "Create texture error: " << Error;
  }
}
/*virtual*/ OpenGLCommon::Texture::~Texture(void)
{
  glDeleteTextures(1, &m_TextureId);
}

void OpenGLCommon::Texture::Bind(void)
{
  glBindTexture(GL_TEXTURE_2D, m_TextureId);
}

} // namespace renderer

} // namespace api

} // namespace covellite
