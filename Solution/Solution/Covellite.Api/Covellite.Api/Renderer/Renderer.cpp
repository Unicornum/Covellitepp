
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
  m_pImpl(MakeImpl(_Api, _Data)),
  m_pRenders(::std::make_shared<Component::Renders>(m_pImpl->GetCreators())),
  m_DefaultRenders(m_pRenders->Obtain(
    {
      Component::Make(
        {
          { uT("type"), uT("Camera") },
          { uT("id"), uT("Covellite.Api.Camera") }
        }),
      Component::Make(
        {
          { uT("type"), uT("State") },
          { uT("kind"), uT("Blend") },
          { uT("id"), uT("Covellite.Api.State.Blend") }
        }),
      Component::Make(
        {
          { uT("type"), uT("State") },
          { uT("kind"), uT("Sampler") },
          { uT("id"), uT("Covellite.Api.State.Sampler") }
        }),
      Component::Make(
        {
          { uT("type"), uT("Shader") },
          { uT("kind"), uT("Vertex") },
          { uT("version"), uT("vs_4_0") },
          { uT("entry"), uT("VS") },
          { uT("id"), uT("Covellite.Api.Shader.Vertex") },
          { uT("data"), ::Vertex.data() },
          { uT("count"), ::Vertex.size() },
        }),
    })),
  m_pScissorEnabled(Component::Make(
    {
      { uT("type"), uT("State") },
      { uT("kind"), uT("Scissor") },
      { uT("id"), uT("Covellite.Api.State.Scissor.Enabled") },
      { uT("is_enabled"), uT("true") },
    }))
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
  static_assert(sizeof(IGraphicApi::Vertex) == sizeof(Rocket::Core::Vertex),
    "Unexpected framework vertex data size.");

  Object_t Object;

  if (_hTexture == 0)
  {
    Object.push_back(Component::Make(
      {
        { uT("type"), uT("Shader") },
        { uT("kind"), uT("Pixel") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psColored") },
        { uT("id"), uT("Covellite.Api.Shader.Pixel.Colored") },
        { uT("data"), ::Pixel.data() },
        { uT("count"), ::Pixel.size() }
      }));
  }
  else
  {
    Object.push_back(Component::Make(
      {
        { uT("type"), uT("Shader") },
        { uT("kind"), uT("Pixel") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psTextured") },
        { uT("id"), uT("Covellite.Api.Shader.Pixel.Textured") },
        { uT("data"), ::Pixel.data() },
        { uT("count"), ::Pixel.size() }
      }));

    const auto strTextureId = uT("Covellite.Api.Texture.{ID}")
      .Replace(uT("{ID}"), (size_t)_hTexture);

    Object.push_back(Component::Make(
      {
        { uT("type"), uT("Texture") },
        { uT("id"), strTextureId },
      }));
  }

  static size_t ObjectId = 0;

  const auto strObjectId = uT("{ID}").Replace(uT("{ID}"), (size_t)++ObjectId);

  m_Objects[ObjectId].pPosition = Component::Make(
    {
      { uT("type"), uT("Position") },
      { uT("id"), uT("Covellite.Api.Position.") + strObjectId },
    });

  Object.push_back(m_Objects[ObjectId].pPosition);

  Object.push_back(Component::Make(
    {
      { uT("type"), uT("Buffer") },
      { uT("id"), uT("Covellite.Api.Buffer.Vertex.") + strObjectId },
      { uT("kind"), uT("Vertex") },
      { uT("data"), reinterpret_cast<const IGraphicApi::Vertex *>(_pVertex) },
      { uT("count"), (size_t)_VertexCount }
    }));

  Object.push_back(Component::Make(
    {
      { uT("type"), uT("Buffer") },
      { uT("id"), uT("Covellite.Api.Buffer.Index.") + strObjectId },
      { uT("kind"), uT("Index") },
      { uT("data"), static_cast<const int *>(_pIndex) },
      { uT("count"), (size_t)_IndexCount }
    }));

  Object.push_back(Component::Make(
    {
      { uT("type"), uT("DrawCall") },
      { uT("id"), uT("Covellite.Api.DrawCall") },
    }));

  m_Objects[ObjectId].Renders = m_pRenders->Obtain(Object);

  return (Rocket::Core::CompiledGeometryHandle)ObjectId;
}

void Renderer::RenderCompiledGeometry(
  Rocket::Core::CompiledGeometryHandle _hGeometry,
  const Rocket::Core::Vector2f & _Position) /*override*/ 
{
  auto itObject = m_Objects.find((size_t)_hGeometry);
  if (itObject == m_Objects.end()) return;

  using namespace ::alicorn::extension::std;

  itObject->second.pPosition->SetValue(uT("x"), _Position.x);
  itObject->second.pPosition->SetValue(uT("y"), _Position.y);

  m_RenderQueue += itObject->second.Renders;
}

void Renderer::ReleaseCompiledGeometry(
  Rocket::Core::CompiledGeometryHandle _hGeometry) /*override*/
{
  const auto strObjectId = uT("{ID}").Replace(uT("{ID}"), (size_t)_hGeometry);

  m_pRenders->Remove({
    Component::Make(
      {
        { uT("id"), uT("Covellite.Api.Position.") + strObjectId }
      }),
    Component::Make(
      {
        { uT("id"), uT("Covellite.Api.Buffer.Vertex.") + strObjectId }
      }),
    Component::Make(
      {
        { uT("id"), uT("Covellite.Api.Buffer.Index.") + strObjectId }
      }),
    });

  m_Objects.erase((size_t)_hGeometry);
}

void Renderer::EnableScissorRegion(bool _IsEnable) /*override*/ 
{
  if (_IsEnable) return;

  using namespace ::alicorn::extension::std;

  m_RenderQueue += m_pRenders->Obtain(
    {
      Component::Make(
      {
        { uT("type"), uT("State") },
        { uT("kind"), uT("Scissor") },
        { uT("id"), uT("Covellite.Api.State.Scissor.Disabled") },
        { uT("is_enabled"), uT("false") }
      })
    });
}

void Renderer::SetScissorRegion(int _X, int _Y, int _Width, int _Height) /*override*/
{
  m_RenderQueue.push_back([&, _X, _Y, _Width, _Height]()
  {
    m_pScissorEnabled->SetValue(uT("left"), _X);
    m_pScissorEnabled->SetValue(uT("right"), _X + _Width);
    m_pScissorEnabled->SetValue(uT("top"), _Y);
    m_pScissorEnabled->SetValue(uT("bottom"), _Y + _Height);
  });

  using namespace ::alicorn::extension::std;

  m_RenderQueue += m_pRenders->Obtain({ m_pScissorEnabled });
}

bool Renderer::LoadTexture(
  Rocket::Core::TextureHandle & _hTexture,
  Rocket::Core::Vector2i & _TextureDimensions,
  const Rocket::Core::String & _PathToFile) /*override*/
{
  // Один файл грузится один раз, оптимизировать загрузку не нужно.

  using namespace ::alicorn::source;

  const image::Universal_t<image::pixel::RGBA> Image
  {
    ::boost::filesystem::load_binary_file(_PathToFile.CString())
  };

  _TextureDimensions.x = static_cast<int>(Image.GetData().Width);
  _TextureDimensions.y = static_cast<int>(Image.GetData().Height);

  return GenerateTexture(_hTexture,
    Image.GetData().Buffer.data(), _TextureDimensions);

  return true;
}

bool Renderer::GenerateTexture(
  Rocket::Core::TextureHandle & _hTexture,
  const Rocket::Core::byte * _pSource,
  const Rocket::Core::Vector2i & _SourceDimensions) /*override*/
{
  static Rocket::Core::TextureHandle TextureId = 0;

  try
  {
    TextureId++;

    const auto strTextureId = uT("Covellite.Api.Texture.{ID}")
      .Replace(uT("{ID}"), TextureId);

    m_pRenders->Obtain(
      {
        Component::Make(
        {
          { uT("type"), uT("Texture") },
          { uT("id"), strTextureId },
          { uT("data"), _pSource },
          { uT("width"), _SourceDimensions.x },
          { uT("height"), _SourceDimensions.y },
        })
      });

    _hTexture = TextureId;
  }
  catch (const ::std::exception &)
  {
    return false;
  }

  return true;
}

void Renderer::ReleaseTexture(Rocket::Core::TextureHandle _hTexture) /*override*/
{
  const auto strTextureId = uT("Covellite.Api.Texture.{ID}")
    .Replace(uT("{ID}"), _hTexture);

  m_pRenders->Remove(
    {
      Component::Make(
        {
          { uT("type"), uT("Texture") },
          { uT("id"), strTextureId },
        })
    });
}

auto Renderer::GetRenders(void) const /*override*/ -> RendersPtr_t
{
  return m_pRenders;
}

void Renderer::StartDrawingFrame(void)
{
  m_pImpl->ClearFrame();
}

void Renderer::PresentFrame(void)
{
  for (const auto & Render : m_DefaultRenders) Render();
  for (const auto & Render : m_RenderQueue) Render();

  m_RenderQueue.clear();
 
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
