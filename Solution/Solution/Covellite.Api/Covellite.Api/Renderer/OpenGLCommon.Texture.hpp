
#pragma once
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
  void Bind(void);

private:
  GLuint m_TextureId;

public:
  explicit Texture(const Component::Texture &);
  virtual ~Texture(void);
};

} // namespace renderer

} // namespace api

} // namespace covellite
