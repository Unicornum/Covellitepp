
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
  using Destination_t = ::std::pair<GLint, ::std::string>;

public:
  void Bind(const bool = true) noexcept;
  void MakeContent(const GLsizei, const GLsizei, const GLvoid *);

protected:
  static Destination_t GetDestination(const String_t &);
  static GLuint GetFormat(const String_t &);
  static GLuint BuildTexture(void) noexcept;

public:
  const Destination_t m_Destination;
  const bool          m_IsMapping;
  const GLuint        m_Format;
  const GLuint        m_TextureId;
  BinaryData_t        m_ReadCopyData;

public:
  explicit Texture(const Component::Texture &);
  Texture(const Texture &) = delete;
  Texture(Texture &&) = delete;
  Texture & operator= (const Texture &) = delete;
  Texture & operator= (Texture &&) = delete;
  virtual ~Texture(void) noexcept;
};

} // namespace renderer

} // namespace api

} // namespace covellite
