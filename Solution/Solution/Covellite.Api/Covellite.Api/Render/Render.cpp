
#include "stdafx.h"
#include "Render.hpp"
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

namespace render
{

Render::Render(const String_t & _Api, const Data & _Data) :
  m_pImpl(MakeImpl(_Api, _Data))
{
  LOGGER(Info) << uT("Using graphics API: ") << m_pImpl->GetUsingApi();
}

void Render::RenderGeometry(
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

Rocket::Core::CompiledGeometryHandle Render::CompileGeometry(
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

void Render::RenderCompiledGeometry(
  Rocket::Core::CompiledGeometryHandle _hGeometry,
  const Rocket::Core::Vector2f & _Position) /*override*/ 
{
  m_pImpl->Render();

  auto * pGeometry = reinterpret_cast<IGraphicApi::IGeometry *>(_hGeometry);

  pGeometry->Update(_Position.x, _Position.y);
  pGeometry->Render();
}

void Render::ReleaseCompiledGeometry(
  Rocket::Core::CompiledGeometryHandle _hGeometry) /*override*/
{
  m_pImpl->Destroy(reinterpret_cast<IGraphicApi::IGeometry *>(_hGeometry));
}

void Render::EnableScissorRegion(bool _IsEnable) /*override*/ 
{
  if (!_IsEnable)
  {
    m_pImpl->DisableScissorRegion();
  }
}

void Render::SetScissorRegion(int _X, int _Y, int _W, int _H) /*override*/ 
{
  m_pImpl->EnableScissorRegion(_X, _Y, _W, _H);
}

bool Render::LoadTexture(
  Rocket::Core::TextureHandle & _hTexture,
  Rocket::Core::Vector2i & _TextureDimensions,
  const Rocket::Core::String & _PathToFile) /*override*/
{
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

bool Render::GenerateTexture(
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

void Render::ReleaseTexture(Rocket::Core::TextureHandle _hTexture) /*override*/
{
  m_pImpl->Destroy(reinterpret_cast<IGraphicApi::ITexture *>(_hTexture));
}

void Render::ClearWindow(void) /*override*/
{
  m_pImpl->ClearWindow();
}

void Render::Present(void) /*override*/ 
{
  m_pImpl->Present();
}

void Render::ResizeWindow(int32_t _X, int32_t _Y) /*override*/
{
  m_pImpl->ResizeWindow(_X, _Y);
}

/*static*/ Render::ImplPtr_t Render::MakeImpl(const String_t & _Api, 
  const Data & _Data)
{
  using namespace ::alicorn::modules::patterns;

  return factory::make_unique<IGraphicApi>(_Api, _Data);
}

} // namespace render

} // namespace api

} // namespace covellite
