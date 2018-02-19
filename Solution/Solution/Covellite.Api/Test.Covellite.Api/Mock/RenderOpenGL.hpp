
#pragma once

namespace mock
{

namespace covellite
{

namespace api
{

class RenderOpenGL final :
  public ::mock::Rocket::Core::RenderInterface
{
public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Constructor, Id_t(int));
  };

public:
  RenderOpenGL & operator= (const RenderOpenGL &) = delete;
  bool operator== (const RenderOpenGL & _Value) const { return (m_Id == _Value.m_Id); }

public:
  void RenderGeometry(Rocket::Core::Vertex *, int, int *, int,
    Rocket::Core::TextureHandle, const Rocket::Core::Vector2f &) override {}
  void EnableScissorRegion(bool) override {}
  void SetScissorRegion(int, int, int, int) override {}
  bool LoadTexture(Rocket::Core::TextureHandle &, Rocket::Core::Vector2i &,
    const Rocket::Core::String &) override { return false; }
  bool GenerateTexture(Rocket::Core::TextureHandle &,
    const Rocket::Core::byte *, const Rocket::Core::Vector2i &) override { return false; }
  void ReleaseTexture(Rocket::Core::TextureHandle) override {}

public:
  explicit RenderOpenGL(int _StatusBarHeight) :
    RenderInterface(Proxy::GetInstance()->Constructor(_StatusBarHeight))
  {

  }
};

} // namespace api

} // namespace covellite

} // namespace mock
