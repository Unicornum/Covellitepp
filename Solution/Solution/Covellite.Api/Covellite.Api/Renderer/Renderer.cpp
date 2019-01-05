
#include "stdafx.h"
#include "Renderer.hpp"
#include <alicorn/std/vector.hpp>
#include <alicorn/patterns/factory.hpp>
#include <alicorn/boost/filesystem.hpp>
#include <alicorn/image.hpp>
#include <Covellite/Api/Component.inl>
#include "IGraphicApi.hpp"
#include "fx/Vertex.auto.hpp"
#include "fx/Pixel.auto.hpp"

#ifndef __USING_GTEST

# if BOOST_OS_WINDOWS

#include "OpenGL.hpp"
#include "DirectX9.hpp"
#include "DirectX10.hpp"
#include "DirectX11.hpp"

# elif BOOST_OS_ANDROID

#include "OpenGLES.hpp"

# endif

#endif

namespace covellite
{

namespace api
{

namespace renderer
{

Renderer::Renderer(const String_t & _Api, const Data & _Data) :
  m_pImpl(MakeImpl(_Api, _Data)),
  m_Renderer(::std::make_shared<Component::Renders>(m_pImpl->GetCreators()))
{
  LOGGER(Info) << uT("Using graphics API: ") << m_pImpl->GetUsingApi();

  // 27 Октябрь 2018 19:59 (unicornum.verum@gmail.com)
  TODO("_Api добавить как поле в класс Data?");
}

void Renderer::RenderGeometry(
  Rocket::Core::Vertex * _pVertex, int _VertexCount, 
  int * _pIndex, int _IndexCount,
  Rocket::Core::TextureHandle _hTexture, 
  const Rocket::Core::Vector2f & _Position) /*override*/ 
{
  const auto hGeometry = CompileGeometry(_pVertex, _VertexCount, 
    _pIndex, _IndexCount,
    _hTexture);
  RenderCompiledGeometry(hGeometry, _Position);
  ReleaseCompiledGeometry(hGeometry);
}

Rocket::Core::CompiledGeometryHandle Renderer::CompileGeometry(
  Rocket::Core::Vertex * _pVertex, int _VertexCount,
  int * _pIndex, int _IndexCount,
  Rocket::Core::TextureHandle _hTexture) /*override*/
{
  return m_Renderer.CompileGeometry(_pVertex, _VertexCount, 
    _pIndex, _IndexCount, _hTexture);
}

void Renderer::RenderCompiledGeometry(
  Rocket::Core::CompiledGeometryHandle _hGeometry,
  const Rocket::Core::Vector2f & _Position) /*override*/ 
{
  m_Renderer.RenderCompiledGeometry(_hGeometry, _Position);
}

void Renderer::ReleaseCompiledGeometry(
  Rocket::Core::CompiledGeometryHandle _hGeometry) /*override*/
{
  m_Renderer.ReleaseCompiledGeometry(_hGeometry);
}

void Renderer::EnableScissorRegion(bool _IsEnable) /*override*/ 
{
  m_Renderer.EnableScissorRegion(_IsEnable);
}

void Renderer::SetScissorRegion(int _X, int _Y, int _Width, int _Height) /*override*/
{
  m_Renderer.SetScissorRegion(_X, _Y, _Width, _Height);
}

bool Renderer::LoadTexture(
  Rocket::Core::TextureHandle & _hTexture,
  Rocket::Core::Vector2i & _TextureDimensions,
  const Rocket::Core::String & _PathToFile) /*override*/
{
  return m_Renderer.LoadTexture(_hTexture, _TextureDimensions, _PathToFile);
}

bool Renderer::GenerateTexture(
  Rocket::Core::TextureHandle & _hTexture,
  const Rocket::Core::byte * _pSource,
  const Rocket::Core::Vector2i & _SourceDimensions) /*override*/
{
  return m_Renderer.GenerateTexture(_hTexture, _pSource, _SourceDimensions);
}

void Renderer::ReleaseTexture(Rocket::Core::TextureHandle _hTexture) /*override*/
{
  m_Renderer.ReleaseTexture(_hTexture);
}

auto Renderer::GetRenders(void) const /*override*/ -> RendersPtr_t
{
  return m_Renderer.m_pRenders;
}

void Renderer::StartDrawingFrame(void)
{
  m_pImpl->ClearFrame();
}

void Renderer::PresentFrame(void)
{
  m_Renderer.RenderScene();
 
  m_pImpl->PresentFrame();
}

void Renderer::ResizeWindow(int32_t _X, int32_t _Y)
{
  m_pImpl->ResizeWindow(_X, _Y);
}

/*static*/ auto Renderer::MakeImpl(const String_t & _Api, const Data & _Data)
  -> IGraphicApiPtr_t
{
  using namespace ::alicorn::modules::patterns;

  if (_Api == uT("Auto"))
  {
    for (const auto & Name : GetRenderers())
    {
      try
      {
        return factory::make_unique<IGraphicApi>(Name, _Data);
      }
      catch (const ::std::exception & _Ex)
      {
        // Сюда попадаем, если указанного рендера нет или при он недоступен 
        // на текущем устройстве.

        LOGGER(Warning) << uT("Create graphics API ") << Name << " error: " 
          << _Ex.what();
      }
    }
  }

  return factory::make_unique<IGraphicApi>(_Api, _Data);
}

} // namespace renderer

} // namespace api

} // namespace covellite
