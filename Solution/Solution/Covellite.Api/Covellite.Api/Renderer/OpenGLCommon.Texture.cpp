
#include "stdafx.h"
#include "OpenGLCommon.Texture.hpp"

#ifndef GL_DEPTH_COMPONENT
# define GL_DEPTH_COMPONENT 0
#endif

namespace covellite
{

namespace api
{

namespace renderer
{

OpenGLCommon::Texture::Texture(const Component::Texture & _Data) :
  m_Destination(GetDestination(_Data)),
  m_Format(GetFormat(_Data.Destination)),
  m_TextureId(BuildTexture()),
  m_IsMapping(_Data.IsMapping),
  m_Capacity(_Data.Capacity)
{
  MakeContent(_Data.Width, _Data.Height, _Data.pTextureData);
}

/*virtual*/ OpenGLCommon::Texture::~Texture(void) noexcept
{
  glDeleteTextures(1, &m_TextureId);
}

// cppcheck-suppress unusedFunction
void OpenGLCommon::Texture::Bind(const bool _IsActivate) noexcept
{
  glBindTexture(GL_TEXTURE_2D, _IsActivate ? m_TextureId : 0);
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

  // glTexImage2D �������� ���������� ������ � �����������, ������� ����������
  // �� � ������������� ����� �� �����.
  glTexImage2D(GL_TEXTURE_2D, 0,
    InternalFormat,
    _Width, _Height, 0,
    m_Format,
    m_Format == GL_DEPTH_COMPONENT ? GL_UNSIGNED_INT : Format,
    _pData);

  // ��� ������� ������� �� Android'e; ��� ����������� ���� ��������
  // � ����������� ����� ������������ GL_DEPTH_STENCIL_ATTACHMENT.
  //glTexImage2D(GL_TEXTURE_2D, 0,
  //  m_Format == GL_DEPTH_COMPONENT ? GL_DEPTH24_STENCIL8 : GL_RGBA,
  //  _Width, _Height, 0,
  //  m_Format == GL_DEPTH_COMPONENT ? GL_DEPTH_STENCIL : GL_RGBA,
  //  m_Format == GL_DEPTH_COMPONENT ? GL_UNSIGNED_INT_24_8 : GL_UNSIGNED_BYTE,
  //  _pData);

  // ��� ����� �������� ����� ������� (����� � ��� ����� ����)
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

/*static*/ auto OpenGLCommon::Texture::GetDestination(
  const Component::Texture & _TextureData) -> Destination_t
{
  if (_TextureData.Index >= 0)
  {
    using namespace ::alicorn::extension::std;

    const auto Name = 
      string_cast<::std::string, Encoding::Ascii128>(_TextureData.Name);
    return { _TextureData.Index, Name };
  }

  if (_TextureData.Destination == uT("diffuse")) return { 0, "TexDiffuse" };

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
    return (_TextureData.Destination == _Dest.first);
  });
  if (itValue == Destinations.cend())
  {
    throw STD_EXCEPTION << "Unexpected destination texture: " <<
      _TextureData.Destination << uT(" [id=???") /*<< _pComponent->Id*/ << uT("].");
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

} // namespace renderer

} // namespace api

} // namespace covellite
