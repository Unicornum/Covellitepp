
#include "stdafx.h"
#include <Covellite/Gui/Renderer.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/patterns/factory.hpp>
#include <alicorn/boost/filesystem.hpp>
#include <alicorn/image.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Renders.hpp>
#include <Covellite/Api/Vertex.hpp>

// 19 Ноябрь 2018 19:27 (unicornum.verum@gmail.com)
TODO("Недопустимое обращение к файлам другого проекта.");
#include <Covellite.Api/Covellite.Api/Renderer/IGraphicApi.hpp>
#include <Covellite.Api/Covellite.Api/Renderer/fx/Vertex.auto.hpp>
#include <Covellite.Api/Covellite.Api/Renderer/fx/Pixel.auto.hpp>

namespace covellite
{

namespace gui
{

using Vertex_t = ::covellite::api::Vertex::Gui;

Renderer::Renderer(const RendersPtr_t & _pRenders) :
  m_pRenders(_pRenders),
  m_DefaultRenders(m_pRenders->Obtain(
    {
      Component_t::Make(
        {
          { uT("id"), uT("Covellite.Api.Present.Camera") },
          { uT("type"), uT("Present") },
          { uT("kind"), uT("Camera") },
        }),
      Component_t::Make(
        {
          { uT("id"), uT("Covellite.Api.State.Blend") },
          { uT("type"), uT("State") },
          { uT("kind"), uT("Blend") },
        }),
      Component_t::Make(
        {
          { uT("id"), uT("Covellite.Api.Shader.Vertex") },
          { uT("type"), uT("Shader") },
          { uT("kind"), Vertex_t::GetName() },
          { uT("version"), uT("vs_4_0") },
          { uT("entry"), uT("VS") },
          { uT("data"), ::Vertex.data() },
          { uT("count"), ::Vertex.size() },
        }),
    })),
    m_pScissorRect(Component_t::Make(
      {
        { uT("id"), uT("Covellite.Api.Data.Rect") },
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Rect") },
      }))
{
}

void Renderer::RenderGeometry(
  Rocket::Core::Vertex * _pVertex, int _VertexCount,
  int * _pIndex, int _IndexCount,
  Rocket::Core::TextureHandle _hTexture,
  const Rocket::Core::Vector2f & _Position) /*override*/
{
  const auto hGeometry = CompileGeometry(
    _pVertex, _VertexCount,
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
  static_assert(sizeof(Vertex_t) == sizeof(Rocket::Core::Vertex),
    "Unexpected framework vertex data size.");

  Object_t Object;

  if (_hTexture == 0)
  {
    Object.push_back(Component_t::Make(
      {
        { uT("id"), uT("Covellite.Api.Shader.Pixel.Colored") },
        { uT("type"), uT("Shader") },
        { uT("kind"), uT("Pixel") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psColored") },
        { uT("data"), ::Pixel.data() },
        { uT("count"), ::Pixel.size() }
      }));
  }
  else
  {
    Object.push_back(Component_t::Make(
      {
        { uT("id"), uT("Covellite.Api.Shader.Pixel.Textured") },
        { uT("type"), uT("Shader") },
        { uT("kind"), uT("Pixel") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psTextured") },
        { uT("data"), ::Pixel.data() },
        { uT("count"), ::Pixel.size() }
      }));

    const auto strTextureId = uT("Covellite.Api.Texture.{ID}")
      .Replace(uT("{ID}"), (size_t)_hTexture);

    Object.push_back(Component_t::Make(
      {
        { uT("id"), strTextureId },
        { uT("type"), uT("Texture") },
      }));

    // Sampler нужно обязательно ставить ПОСЛЕ активации КАЖДОЙ текстуры,
    // иначе OpenGL будет работать неправильно!
    // - Для DirectX 9/10/11 это не имеет значения.
    Object.push_back(Component_t::Make(
    {
      { uT("id"), uT("Covellite.Api.State.Sampler") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Sampler") },
    }));
  }

  static size_t ObjectId = 0;

  const auto strObjectId = uT("{ID}").Replace(uT("{ID}"), (size_t)++ObjectId);

  Object.push_back(Component_t::Make(
    {
      { uT("id"), uT("Covellite.Api.Buffer.Vertex.") + strObjectId },
      { uT("type"), uT("Buffer") },
      { uT("kind"), Vertex_t::GetName() },
      { uT("data"), reinterpret_cast<const Vertex_t *>(_pVertex) },
      { uT("count"), (size_t)_VertexCount }
    }));

  Object.push_back(Component_t::Make(
    {
      { uT("id"), uT("Covellite.Api.Buffer.Index.") + strObjectId },
      { uT("type"), uT("Buffer") },
      { uT("kind"), uT("Index") },
      { uT("data"), static_cast<const int *>(_pIndex) },
      { uT("count"), (size_t)_IndexCount }
    }));

  m_Objects[ObjectId].pPosition = Component_t::Make(
    {
      { uT("id"), uT("Covellite.Api.Data.Position.") + strObjectId },
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
    });

  Object.push_back(m_Objects[ObjectId].pPosition);

  Object.push_back(Component_t::Make(
    {
      { uT("id"), uT("Covellite.Api.Present.Geometry.") + strObjectId },
      { uT("type"), uT("Present") },
      { uT("kind"), uT("Geometry") },
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
    Component_t::Make(
      {
        { uT("id"), uT("Covellite.Api.Buffer.Vertex.") + strObjectId }
      }),
    Component_t::Make(
      {
        { uT("id"), uT("Covellite.Api.Buffer.Index.") + strObjectId }
      }),
    Component_t::Make(
      {
        { uT("id"), uT("Covellite.Api.Data.Position.") + strObjectId }
      }),
    Component_t::Make(
      {
        { uT("id"), uT("Covellite.Api.Present.Geometry.") + strObjectId }
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
      Component_t::Make(
      {
        { uT("id"), uT("Covellite.Api.State.Scissor.Disabled") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Scissor") },
        { uT("is_enabled"), uT("false") }
      })
    });
}

void Renderer::SetScissorRegion(int _X, int _Y, int _Width, int _Height) /*override*/
{
  m_RenderQueue.push_back([&, _X, _Y, _Width, _Height]()
  {
    m_pScissorRect->SetValue(uT("left"), _X);
    m_pScissorRect->SetValue(uT("right"), _X + _Width);
    m_pScissorRect->SetValue(uT("top"), _Y);
    m_pScissorRect->SetValue(uT("bottom"), _Y + _Height);
  });

  using namespace ::alicorn::extension::std;

  m_RenderQueue += m_pRenders->Obtain(
    { 
      m_pScissorRect,
      Component_t::Make(
      {
        { uT("id"), uT("Covellite.Api.State.Scissor.Enabled") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Scissor") },
        { uT("is_enabled"), uT("true") }
      }),
    });
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
}

bool Renderer::GenerateTexture(
  Rocket::Core::TextureHandle & _hTexture,
  const Rocket::Core::byte * _pSource,
  const Rocket::Core::Vector2i & _SourceDimensions) /*override*/
{
  static Rocket::Core::TextureHandle TextureId = 0;

  TextureId++;

  const auto strTextureId = uT("Covellite.Api.Texture.{ID}")
    .Replace(uT("{ID}"), TextureId);

  auto Render = m_pRenders->Obtain(
    {
      Component_t::Make(
      {
        { uT("id"), strTextureId },
        { uT("type"), uT("Texture") },
        { uT("data"), _pSource },
        { uT("width"), _SourceDimensions.x },
        { uT("height"), _SourceDimensions.y },
      })
    });

  if (Render.empty() || Render[0] == nullptr) return false;

  _hTexture = TextureId;

  return true;
}

void Renderer::ReleaseTexture(Rocket::Core::TextureHandle _hTexture) /*override*/
{
  const auto strTextureId = uT("Covellite.Api.Texture.{ID}")
    .Replace(uT("{ID}"), _hTexture);

  m_pRenders->Remove(
    {
      Component_t::Make(
        {
          { uT("id"), strTextureId },
          { uT("type"), uT("Texture") },
        })
    });
}

void Renderer::RenderScene(void)
{
  for (const auto & Render : m_DefaultRenders) Render();
  for (const auto & Render : m_RenderQueue) Render();

  m_RenderQueue.clear();
}

} // namespace gui

} // namespace covellite
