
#pragma once
#include <alicorn/std/memory/unique-ptr.hpp>
#include <alicorn/std/string.forward.hpp>
#include "IGraphicApi.hpp"

namespace covellite
{

namespace api
{

namespace render
{

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс обобщенной логики рендеринга GUI.
* \details
*  - Задача класса - скрыть от клиентского кода ссылку на libRocket 
*  (в разделе используется Rocket::Core::RenderInterface).
*  
* \version
*  1.0.0.0        \n
* \date
*  23 Август 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Render final :
  public Rocket::Core::RenderInterface,
  public IRender
{
  using String_t = ::alicorn::extension::std::String;
  using ImplPtr_t = ::alicorn::extension::std::unique_ptr<IGraphicApi>;

public:
  // Интерфейс RenderInterface
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
  // Интерфейс IRender:
  void ClearWindow(void) override;
  void Present(void) override;
  void ResizeWindow(int32_t, int32_t) override;

private:
  static ImplPtr_t MakeImpl(const String_t &, const Data &);
  static ::std::vector<String_t> GetRenders(void);

private:
  const ImplPtr_t m_pImpl;

public:
  Render(const String_t &, const Data &);
};

} // namespace render

} // namespace api

} // namespace covellite
