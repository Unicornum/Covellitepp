
#pragma once
#include <Covellite/Api/Component.hpp>

namespace mock
{

namespace covellite
{

namespace gui
{

class Renderer final :
  public Rocket::Core::RenderInterface
{
  using Component_t = ::covellite::api::Component;
  using RendersPtr_t = ::std::shared_ptr<Component_t::Renders>;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Constructor, ::mock::Id_t(RendersPtr_t));
    MOCK_METHOD1(RenderScene, void(::mock::Id_t));
  };

public:
  const ::mock::Id_t m_Id;

public:
  // Интерфейс Rocket::Core::RenderererInterface
  void RenderGeometry(Rocket::Core::Vertex *, int, int *, int,
    Rocket::Core::TextureHandle, const Rocket::Core::Vector2f &) override
  {
    throw ::std::exception{};
  }
  Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex *,
    int, int *, int, Rocket::Core::TextureHandle) override
  {
    throw ::std::exception{};
  }
  void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle,
    const Rocket::Core::Vector2f &) override
  {
    throw ::std::exception{};
  }
  void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle) override
  {
    throw ::std::exception{};
  }
  void EnableScissorRegion(bool) override
  {
    throw ::std::exception{};
  }
  void SetScissorRegion(int, int, int, int) override
  {
    throw ::std::exception{};
  }
  bool LoadTexture(Rocket::Core::TextureHandle &, Rocket::Core::Vector2i &,
    const Rocket::Core::String &) override
  {
    throw ::std::exception{};
  }
  bool GenerateTexture(Rocket::Core::TextureHandle &,
    const Rocket::Core::byte *, const Rocket::Core::Vector2i &) override
  {
    throw ::std::exception{};
  }
  void ReleaseTexture(Rocket::Core::TextureHandle) override
  {
    throw ::std::exception{};
  }

public:
  void RenderScene(void)
  {
    Proxy::GetInstance()->RenderScene(m_Id);
  }

public:
  explicit Renderer(const RendersPtr_t & _pRenders) :
    m_Id(Proxy::GetInstance()->Constructor(_pRenders))
  {

  }
};

} // namespace gui

} // namespace covellite

} // namespace mock
