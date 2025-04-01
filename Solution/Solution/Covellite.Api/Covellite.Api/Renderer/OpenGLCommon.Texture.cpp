
#include "stdafx.h"
#include "OpenGLCommon.Texture.hpp"
#include <alicorn/std.hpp>

#ifndef GL_DEPTH_COMPONENT
# define GL_DEPTH_COMPONENT 0
#endif

namespace covellite::api::renderer
{

OpenGLCommon::Texture::Texture(const Component::Texture & _Data) :
  m_Destination(GetDestination(_Data.Index, _Data.Destination, _Data.Name)),
  m_Format(GetFormat(_Data.Destination)),
  m_TextureId(BuildTexture()),
  m_IsMapping(_Data.IsMapping),
  m_Capacity(_Data.Capacity),
  m_Target(GL_TEXTURE_2D)
{
  MakeContent(_Data.Width, _Data.Height, _Data.pTextureData);
}

OpenGLCommon::Texture::Texture(const Component::Texture & _Data, bool) :
  m_Destination(GetDestination(_Data.Index, _Data.Destination, _Data.Name)),
  m_Format(GL_RGBA),
  m_TextureId(BuildTexture()),
  m_IsMapping(_Data.IsMapping),
  m_Capacity(_Data.Capacity),
  m_Target(GL_TEXTURE_2D_ARRAY)
{
  Bind();
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, _Data.IsUsingMipmapping ? 8 : 1, GL_RGBA8,
    _Data.Width, _Data.Height, _Data.DataCount);
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, _Data.Width, _Data.Height,
    _Data.DataCount, m_Format, GL_UNSIGNED_BYTE, ::std::data(_Data.Data));
  if (_Data.IsUsingMipmapping) glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  Bind(false);

  const auto Error = glGetError();
  if (Error != GL_NO_ERROR)
  {
    throw STD_EXCEPTION << "Create texture error: " << Error;
  }
}

/*virtual*/ OpenGLCommon::Texture::~Texture(void) noexcept
{
  glDeleteTextures(1, &m_TextureId);
}

// cppcheck-suppress unusedFunction
void OpenGLCommon::Texture::Bind(const bool _IsActivate) noexcept
{
  glBindTexture(m_Target, _IsActivate ? m_TextureId : 0);
}

void OpenGLCommon::Texture::MakeContent(
  const GLsizei _Width,
  const GLsizei _Height,
  const GLvoid * _pData)
{
  Bind(true);

# if !defined(GL_RGBA32F)
#   define GL_RGBA32F GL_RGBA
# endif

# if !defined(GL_RGBA16F)
#   define GL_RGBA16F GL_RGBA
# endif

# if !defined(GL_HALF_FLOAT)
#   define GL_HALF_FLOAT GL_FLOAT
# endif

  const GLint InternalFormat =
    (m_Format == GL_DEPTH_COMPONENT) ? GL_DEPTH_COMPONENT :
    (m_Capacity == 32) ? GL_RGBA32F :
    (m_Capacity == 16) ? GL_RGBA16F :
    GL_RGBA;

  const GLenum Format =
    (InternalFormat == GL_RGBA) ? GL_UNSIGNED_BYTE :
    (m_Capacity == 32) ? GL_FLOAT :
    (m_Capacity == 16) ? GL_HALF_FLOAT :
    GL_UNSIGNED_BYTE;

  // glTexImage2D копирует переданные данные в видеопамять, поэтому копировать
  // их в промежуточный буфер не нужно.
  glTexImage2D(GL_TEXTURE_2D, 0,
    InternalFormat,
    _Width, _Height, 0,
    m_Format,
    m_Format == GL_DEPTH_COMPONENT ? GL_UNSIGNED_INT : Format,
    _pData);

  // Это рабочий вариант на Android'e; для подключения этой текстуры
  // к фреймбуферу нужно использовать GL_DEPTH_STENCIL_ATTACHMENT.
  //glTexImage2D(GL_TEXTURE_2D, 0,
  //  m_Format == GL_DEPTH_COMPONENT ? GL_DEPTH24_STENCIL8 : GL_RGBA,
  //  _Width, _Height, 0,
  //  m_Format == GL_DEPTH_COMPONENT ? GL_DEPTH_STENCIL : GL_RGBA,
  //  m_Format == GL_DEPTH_COMPONENT ? GL_UNSIGNED_INT_24_8 : GL_UNSIGNED_BYTE,
  //  _pData);

  // Для такой текстуры нужно ставить (иначе в ней будут нули)
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  Bind(false);

  const auto Error = glGetError();
  if (Error != GL_NO_ERROR)
  {
    throw STD_EXCEPTION << "Create texture error: " << Error;
  }

  if (m_IsMapping)
  {
    m_ReadCopyData.resize(static_cast<size_t>(_Width) * _Height * 4, 0x00);
  }
}

/*static*/ auto OpenGLCommon::Texture::GetDestination(const int _Index,
  const String_t & _Destination, const String_t & _Name) -> Destination_t
{
  if (_Index >= 0)
  {
    using namespace ::alicorn::extension::std;

    return { _Index, string_cast<::std::string, Encoding::Ascii128>(_Name) };
  }

  if (_Destination == uT("diffuse")) return { 0, "TexDiffuse" };

  static const ::std::vector<::std::pair<String_t, const char *>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
    { uT("depth"),     "TexDepth" },
  };

  const auto itValue = ::std::find_if(Destinations.cbegin(),
    Destinations.cend(), [&](const ::std::pair<String_t, const char *> & _Dest)
  {
    return (_Destination == _Dest.first);
  });
  if (itValue == Destinations.cend())
  {
    throw STD_EXCEPTION << "Unexpected destination texture: " <<
      _Destination << uT(" [id=???") /*<< _pComponent->Id*/ << uT("].");
  }

  const auto IndexDestination = 
    static_cast<GLint>(::std::distance(Destinations.cbegin(), itValue));
  return { IndexDestination, Destinations[IndexDestination].second };
}

/*static*/ GLint OpenGLCommon::Texture::GetFormat(const String_t & _Destination)
{
  return (_Destination == uT("depth")) ? GL_DEPTH_COMPONENT : GL_RGBA;
}

/*static*/ GLuint OpenGLCommon::Texture::BuildTexture(void) noexcept
{
  GLuint TextureId = 0;
  glGenTextures(1, &TextureId);
  return TextureId;
}

} // namespace covellite::api::renderer
