
#pragma once
#include <vector>
#include <functional>
#include <alicorn/std/string.forward.hpp>
#include <alicorn/std/memory/unique-ptr.hpp>
#include <Covellite/Predefined.hpp>
#include <Covellite/Os/Predefined.hpp>
#include <Covellite/Api/RenderInterface.hpp>
#include <Covellite/Gui/Renderer.hpp>

namespace covellite
{

namespace api
{

class Component;

namespace renderer
{

class IGraphicApi;

/**
* \ingroup CovelliteApiRendererGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс обобщенной логики рендеринга GUI.
* \details
*  - Задача класса - скрыть от клиентского кода ссылку на libRocket 
*  (в разделе используется Rocket::Core::RenderererInterface).
* \deprecated
* \todo
*  Удалить в следующей стабильной версии.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  23 Август 2018    \n
*  25 Октябрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Renderer final :
  public RenderInterface
{
  using String_t = ::alicorn::extension::std::String;
  using IGraphicApiPtr_t = ::alicorn::extension::std::unique_ptr<IGraphicApi>;
  using Render_t = ::std::function<void(void)>;
  using Renders_t = ::std::vector<Render_t>;
  using ComponentPtr_t = ::std::shared_ptr<Component>;
  using Object_t = ::std::vector<ComponentPtr_t>;

public:
  /// Класс для передачи значений настроек программы реализации рендера.
  class Data final
  {
  public:
    class Color final
    {
    public:
      float R, G, B, A;
    };

  public:
    Handle_t  Handle;       ///< Хэнд окна операционной системы.
    int       Top;          ///< Смещение верхнего края клиентской части окна.
    Color     BkColor;      ///< Цвет фона окна по умолчанию.
    bool      IsFullScreen; ///< Признак работы программы в полноэкранном 
                            ///< режиме.
  };

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
  // Интерфейс covellite::api::RenderInterface
  RendersPtr_t GetRenders(void) const override;

public:
  void StartDrawingFrame(void);
  void PresentFrame(void);
  void ResizeWindow(int32_t, int32_t);

private:
  static IGraphicApiPtr_t MakeImpl(const String_t &, const Data &);
  static ::std::vector<String_t> GetRenderers(void);

private:
  const IGraphicApiPtr_t m_pImpl;
  ::covellite::gui::Renderer m_Renderer;

public:
  Renderer(const String_t &, const Data &);
};

} // namespace renderer

} // namespace api

} // namespace covellite
