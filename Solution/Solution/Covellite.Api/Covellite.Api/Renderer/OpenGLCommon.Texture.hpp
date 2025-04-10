
#pragma once
#include <Covellite/Api/Defines.hpp>
#include "OpenGLCommon.hpp"
#include "Component.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

class OpenGLCommon::Texture final
{
public:
  using Ptr_t = ::std::shared_ptr<Texture>;
  using Destination_t = ::std::pair<GLuint, ::std::string>;

public:
  void Bind(const bool = true) noexcept;
  void MakeContent(const GLsizei, const GLsizei, const GLvoid *);
  static Destination_t GetDestination(const int, const String_t &, const String_t &);

protected:
  static GLint GetFormat(const String_t &);
  static GLuint BuildTexture(void) noexcept;

public:
  BinaryData_t        m_ReadCopyData;
  const Destination_t m_Destination;
  const GLint         m_Format;
  const GLuint        m_TextureId;
  const bool          m_IsMapping;
  const GLenum        m_Capacity;
  const GLenum        m_Target;
  const uint8_t Align[3] = { 0 };

public:
  explicit Texture(const Component::Texture &);
  Texture(const Component::Texture &, bool /*_IsTextureArray*/);
  Texture(const Texture &) = delete;
  Texture(Texture &&) = delete;
  Texture & operator= (const Texture &) = delete;
  Texture & operator= (Texture &&) = delete;
  virtual ~Texture(void) noexcept;
};

} // namespace renderer

} // namespace api

} // namespace covellite
