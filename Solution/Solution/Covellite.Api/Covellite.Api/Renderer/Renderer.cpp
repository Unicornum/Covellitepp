
#include "stdafx.h"
#include "Renderer.hpp"
#include <alicorn/std/string.hpp>
#include <alicorn/patterns/factory.hpp>
#include <alicorn/boost/filesystem.hpp>
#include <alicorn/image.hpp>

#ifndef __USING_GTEST

#include <alicorn/logger.hpp>

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
  m_pImpl(MakeImpl(_Api, _Data))
{
  LOGGER(Info) << uT("Using graphics API: ") << m_pImpl->GetUsingApi();
}

void Renderer::RenderGeometry(
  Rocket::Core::Vertex * _pVertex, int _VertexCount, 
  int * _pIndex, int _IndexCount,
  Rocket::Core::TextureHandle _hTexture, 
  const Rocket::Core::Vector2f & _Position) /*override*/ 
{
  auto hGeometry = CompileGeometry(_pVertex, _VertexCount, _pIndex, _IndexCount,
    _hTexture);
  RenderCompiledGeometry(hGeometry, _Position);
  ReleaseCompiledGeometry(hGeometry);
}

Rocket::Core::CompiledGeometryHandle Renderer::CompileGeometry(
  Rocket::Core::Vertex * _pVertex, int _VertexCount,
  int * _pIndex, int _IndexCount,
  Rocket::Core::TextureHandle _hTexture) /*override*/
{
  static_assert(sizeof(IGraphicApi::Vertex) == sizeof(Rocket::Core::Vertex),
    "Unexpected framework vertex data size.");

  IGraphicApi::IGeometry::Data SourceData;
  SourceData.pVertices = reinterpret_cast<IGraphicApi::Vertex *>(_pVertex);
  SourceData.VerticesCount = _VertexCount;
  SourceData.pIndices = _pIndex;
  SourceData.IndicesCount = _IndexCount;
  SourceData.pTexture = reinterpret_cast<IGraphicApi::ITexture *>(_hTexture);

  auto * pGeometry = m_pImpl->Create(SourceData);

  return reinterpret_cast<Rocket::Core::CompiledGeometryHandle>(pGeometry);
}

void Renderer::RenderCompiledGeometry(
  Rocket::Core::CompiledGeometryHandle _hGeometry,
  const Rocket::Core::Vector2f & _Position) /*override*/ 
{
  auto * pGeometry = reinterpret_cast<IGraphicApi::IGeometry *>(_hGeometry);

  // 11 ќкт€брь 2018 17:21 (unicornum.verum@gmail.com)
  TODO("ƒобавить тест вызова функции.");
  m_pImpl->Render(pGeometry, _Position.x, _Position.y);

  {
    // 11 ќкт€брь 2018 17:20 (unicornum.verum@gmail.com)
    TODO("”далить блок после изменени€ логики работы.");

    m_pImpl->Render();

    pGeometry->Update(_Position.x, _Position.y);
    pGeometry->Render();
  }
}

void Renderer::ReleaseCompiledGeometry(
  Rocket::Core::CompiledGeometryHandle _hGeometry) /*override*/
{
  m_pImpl->Destroy(reinterpret_cast<IGraphicApi::IGeometry *>(_hGeometry));
}

void Renderer::EnableScissorRegion(bool _IsEnable) /*override*/ 
{
  if (!_IsEnable)
  {
    m_pImpl->DisableScissorRegion();
  }
}

void Renderer::SetScissorRegion(int _X, int _Y, int _W, int _H) /*override*/ 
{
  m_pImpl->EnableScissorRegion(_X, _Y, _W, _H);
}

bool Renderer::LoadTexture(
  Rocket::Core::TextureHandle & _hTexture,
  Rocket::Core::Vector2i & _TextureDimensions,
  const Rocket::Core::String & _PathToFile) /*override*/
{
  // ќдин файл грузитс€ один раз, оптимизировать загрузку не нужно.

  using namespace ::alicorn::source;

  const image::Universal_t<image::pixel::RGBA> Image
  {
    ::boost::filesystem::load_binary_file(_PathToFile.CString())
  };

  _TextureDimensions.x = static_cast<int>(Image.GetData().Width);
  _TextureDimensions.y = static_cast<int>(Image.GetData().Height);

  return GenerateTexture(_hTexture,
    Image.GetData().Buffer.data(), _TextureDimensions);
}

bool Renderer::GenerateTexture(
  Rocket::Core::TextureHandle & _hTexture,
  const Rocket::Core::byte * _pSource,
  const Rocket::Core::Vector2i & _SourceDimensions) /*override*/
{
  IGraphicApi::ITexture::Data SourceData;
  SourceData.pData = _pSource;
  SourceData.Width = _SourceDimensions.x;
  SourceData.Height = _SourceDimensions.y;

  try
  {
    auto * pTexture = m_pImpl->Create(SourceData);
    _hTexture = reinterpret_cast<Rocket::Core::TextureHandle>(pTexture);
  }
  catch (const ::std::exception &)
  {
    return false;
  }

  return true;
}

void Renderer::ReleaseTexture(Rocket::Core::TextureHandle _hTexture) /*override*/
{
  m_pImpl->Destroy(reinterpret_cast<IGraphicApi::ITexture *>(_hTexture));
}

void Renderer::ClearWindow(void) /*override*/
{
  m_pImpl->ClearWindow();
}

void Renderer::Present(void) /*override*/ 
{
  m_pImpl->Present();
}

void Renderer::ResizeWindow(int32_t _X, int32_t _Y) /*override*/
{
  m_pImpl->ResizeWindow(_X, _Y);
}

/*static*/ Renderer::ImplPtr_t Renderer::MakeImpl(const String_t & _Api, 
  const Data & _Data)
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
        // —юда попадаем, если указанного рендера нет или при он недоступен 
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
