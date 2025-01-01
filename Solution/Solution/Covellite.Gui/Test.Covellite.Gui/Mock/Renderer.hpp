
#pragma once
#include <Covellite/Api/Component.hpp>

namespace mock
{

namespace covellite
{

namespace gui
{

class Renderer final :
  public CovelliteGui::RenderInterface
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
  void RenderGeometry(CovelliteGui::Vertex *, int, int *, int,
    CovelliteGui::TextureHandle, const CovelliteGui::Vector2f &) override
  {
    throw ::std::exception{};
  }
  CovelliteGui::CompiledGeometryHandle CompileGeometry(CovelliteGui::Vertex *,
    int, int *, int, CovelliteGui::TextureHandle) override
  {
    throw ::std::exception{};
  }
  void RenderCompiledGeometry(CovelliteGui::CompiledGeometryHandle,
    const CovelliteGui::Vector2f &) override
  {
    throw ::std::exception{};
  }
  void ReleaseCompiledGeometry(CovelliteGui::CompiledGeometryHandle) override
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
  bool LoadTexture(CovelliteGui::TextureHandle &, CovelliteGui::Vector2i &,
    const CovelliteGui::String &) override
  {
    throw ::std::exception{};
  }
  bool GenerateTexture(CovelliteGui::TextureHandle &,
    const CovelliteGui::byte *, const CovelliteGui::Vector2i &) override
  {
    throw ::std::exception{};
  }
  void ReleaseTexture(CovelliteGui::TextureHandle) override
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
