
#pragma once
#include <vector>
#include <functional>
#include <alicorn/std/string.forward.hpp>
#include <alicorn/std.memory/unique-ptr.hpp>
#include <Covellite/Predefined.hpp>
#include <Covellite/Api/Component.hpp>

namespace covellite
{

namespace api { namespace renderer { class Renderer; } }

namespace gui
{

/**
* \ingroup CovellitGuiGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Класс обобщенной логики рендеринга GUI.
* \details
*  - Задача класса - скрыть от клиентского кода ссылку на код библиотеки GUI 
*  (в разделе используется CovelliteGui::RenderererInterface).
* \todo
*  Класс используется для внутренней реализации и не должен попадать в Externals.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
*  2.1.0.0        \n
* \date
*  23 Август 2018    \n
*  25 Октябрь 2018    \n
*  19 Ноябрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Renderer final :
  public CovelliteGui::RenderInterface
{
  using String_t = ::alicorn::extension::std::String;
  using Render_t = ::std::function<void(void)>;
  using Renders_t = ::std::vector<Render_t>;
  using Component_t = ::covellite::api::Component;
  using RendersPtr_t = ::std::shared_ptr<Component_t::Renders>;
  using ComponentPtr_t = ::std::shared_ptr<Component_t>;
  using Object_t = ::std::vector<ComponentPtr_t>;

public:
  // Интерфейс CovelliteGui::RenderererInterface
  void RenderGeometry(CovelliteGui::Vertex *, int, int *, int,
    CovelliteGui::TextureHandle, const CovelliteGui::Vector2f &) override;
  CovelliteGui::CompiledGeometryHandle CompileGeometry(CovelliteGui::Vertex *,
    int, int *, int, CovelliteGui::TextureHandle) override;
  void RenderCompiledGeometry(CovelliteGui::CompiledGeometryHandle,
    const CovelliteGui::Vector2f &) override;
  void ReleaseCompiledGeometry(CovelliteGui::CompiledGeometryHandle) override;
  void EnableScissorRegion(bool) override;
  void SetScissorRegion(int, int, int, int) override;
  bool LoadTexture(CovelliteGui::TextureHandle &, CovelliteGui::Vector2i &,
    const CovelliteGui::String &) override;
  bool GenerateTexture(CovelliteGui::TextureHandle &,
    const CovelliteGui::byte *, const CovelliteGui::Vector2i &) override;
  void ReleaseTexture(CovelliteGui::TextureHandle) override;

public:
  void RenderScene(void);

private:
  const RendersPtr_t m_pRenders;

private:
  const Renders_t m_DefaultRenders;
  ComponentPtr_t m_pScissorRect;

  class Object
  {
  public:
    ComponentPtr_t pPosition;
    Renders_t Renders;
  };

  ::std::map<size_t, Object> m_Objects;

  Renders_t m_RenderQueue;

public:
  explicit Renderer(const RendersPtr_t &);

private:
  friend ::covellite::api::renderer::Renderer;
};

} // namespace gui

} // namespace covellite
