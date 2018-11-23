
#pragma once
#include <vector>
#include <functional>
#include <alicorn/std/string.forward.hpp>
#include <alicorn/std/memory/unique-ptr.hpp>
#include <Covellite/Predefined.hpp>
#include <Covellite/Os/Predefined.hpp>
#include <Covellite/Api/Component.hpp>

namespace covellite
{

namespace gui
{

/**
* \ingroup CovellitGuiGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Класс обобщенной логики рендеринга GUI.
* \details
*  - Задача класса - скрыть от клиентского кода ссылку на libRocket 
*  (в разделе используется Rocket::Core::RenderererInterface).
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
  public Rocket::Core::RenderInterface
{
  using String_t = ::alicorn::extension::std::String;
  using Render_t = ::std::function<void(void)>;
  using Renders_t = ::std::vector<Render_t>;
  using Component_t = ::covellite::api::Component;
  using RendersPtr_t = ::std::shared_ptr<Component_t::Renders>;
  using ComponentPtr_t = ::std::shared_ptr<Component_t>;
  using Object_t = ::std::vector<ComponentPtr_t>;

public:
  // Интерфейс Rocket::Core::RenderererInterface
  void RenderGeometry(Rocket::Core::Vertex *, int, int *, int,
    Rocket::Core::TextureHandle, const Rocket::Core::Vector2f &) override;
  Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex *,
    int, int *, int, Rocket::Core::TextureHandle) override;
  void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle,
    const Rocket::Core::Vector2f &) override;
  void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle) override;
  void EnableScissorRegion(bool) override;
  void SetScissorRegion(int, int, int, int) override;
  bool LoadTexture(Rocket::Core::TextureHandle &, Rocket::Core::Vector2i &,
    const Rocket::Core::String &) override;
  bool GenerateTexture(Rocket::Core::TextureHandle &,
    const Rocket::Core::byte *, const Rocket::Core::Vector2i &) override;
  void ReleaseTexture(Rocket::Core::TextureHandle) override;

public:
  void RenderScene(void);

private:
  const RendersPtr_t m_pRenders;

private:
  const Renders_t m_DefaultRenders;
  ComponentPtr_t m_pScissorEnabled;

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
};

} // namespace gui

} // namespace covellite
