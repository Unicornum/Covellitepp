
#pragma once
#include <Covellite/Api/Component.hpp>

namespace mock
{

namespace covellite
{

namespace gui
{

class Renderer final :
  public CovelliteGui::Core::RenderInterface
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
  // Интерфейс CovelliteGui::Core::RenderererInterface
  void RenderGeometry(CovelliteGui::Core::Vertex *, int, int *, int,
    CovelliteGui::Core::TextureHandle, const CovelliteGui::Core::Vector2f &) override
  {
    throw ::std::exception{};
  }
  CovelliteGui::Core::CompiledGeometryHandle CompileGeometry(CovelliteGui::Core::Vertex *,
    int, int *, int, CovelliteGui::Core::TextureHandle) override
  {
    throw ::std::exception{};
  }
  void RenderCompiledGeometry(CovelliteGui::Core::CompiledGeometryHandle,
    const CovelliteGui::Core::Vector2f &) override
  {
    throw ::std::exception{};
  }
  void ReleaseCompiledGeometry(CovelliteGui::Core::CompiledGeometryHandle) override
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
  bool LoadTexture(CovelliteGui::Core::TextureHandle &, CovelliteGui::Core::Vector2i &,
    const CovelliteGui::Core::String &) override
  {
    throw ::std::exception{};
  }
  bool GenerateTexture(CovelliteGui::Core::TextureHandle &,
    const CovelliteGui::Core::byte *, const CovelliteGui::Core::Vector2i &) override
  {
    throw ::std::exception{};
  }
  void ReleaseTexture(CovelliteGui::Core::TextureHandle) override
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
    RenderInterface(Proxy::GetInstance()->Constructor(_pRenders))
  {

  }
};

} // namespace gui

} // namespace covellite

} // namespace mock
