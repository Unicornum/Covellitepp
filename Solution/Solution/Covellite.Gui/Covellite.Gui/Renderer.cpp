
#include "stdafx.h"
#include <Covellite/Gui/Renderer.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/filesystem.hpp>
#include <alicorn/patterns/factory.hpp>
#include <alicorn/image.hpp>
#include <Covellite/App/Vfs.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Renders.hpp>
#include <Covellite/Api/Vertex.hpp>

namespace covellite
{

namespace gui
{

Renderer::Renderer(const RendersPtr_t & _pRenders) :
  m_pRenders(_pRenders),
  m_DefaultRenders(
    {
      m_pRenders->Obtain(Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.Camera.Gui") },
          { uT("type"), uT("Camera") },
          { uT("kind"), uT("Orthographic") },
        })),
      m_pRenders->Obtain(Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.State.Blend") },
          { uT("type"), uT("State") },
          { uT("kind"), uT("Blend") },
        })),
      m_pRenders->Obtain(Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.State.Sampler") },
          { uT("type"), uT("State") },
          { uT("kind"), uT("Sampler") },
        })),
      m_pRenders->Obtain(Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.Shader.Vertex") },
          { uT("type"), uT("Shader") },
          { uT("entry"), uT("vsFlat") },
        })),
    }),
    m_pScissorRect(Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Rect") },
      }))
{
}

//Renderer::~Renderer(void)
//{
  // Рендеры общих компонентов не удаляются, т.к. они будут удалены вместе с
  // уникальным (для объектв класса Renderer) объектом m_pRenders.
  // m_pRenders->Remove(...);
//}

CovelliteGui::CompiledGeometryHandle Renderer::CompileGeometry(
  CovelliteGui::Span<const CovelliteGui::Vertex> _Vertices,
  CovelliteGui::Span<const int> _Indices) /*override*/
{
  static size_t ObjectId = 0;

  const auto strObjectId = String_t{ uT("%ID%") }
  .Replace(uT("%ID%"), (size_t)++ObjectId);

  Renders_t & Object = m_Objects[ObjectId].Renders;
  m_Objects[ObjectId].pPosition = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
      { uT("x"), 0.0f },
      { uT("y"), 0.0f },
    });

  using namespace ::alicorn::extension::std;

  const auto Convert =
    [](const CovelliteGui::Vertex * _pVertex, const ::std::size_t _VertexCount)
  {
    ::std::vector<::covellite::api::Vertex> Result{ _VertexCount };

    for (::std::size_t i = 0; i < _VertexCount; i++)
    {
      auto & Vertex = Result[i];

      Vertex.px = _pVertex[i].position.x;
      Vertex.py = _pVertex[i].position.y;
      Vertex.pz = 0.0f;
      Vertex.pw = 1.0f;

      Vertex.tu = _pVertex[i].tex_coord.x;
      Vertex.tv = _pVertex[i].tex_coord.y;

      Vertex.ex = _pVertex[i].colour.red / 255.0f;
      Vertex.ey = _pVertex[i].colour.green / 255.0f;
      Vertex.ez = _pVertex[i].colour.blue / 255.0f;
      Vertex.ew = _pVertex[i].colour.alpha / 255.0f;
    }

    return Result;
  };

  Object += Renders_t
  {
    m_pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), uT("Covellite.Gui.Buffer.Vertex.") + strObjectId },
      { uT("type"), uT("Buffer") },
      { uT("content"), Convert(_Vertices.data(), _Vertices.size())},
      { uT("dimension"), 2 },
    })),
    m_pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), uT("Covellite.Gui.Transform.") + strObjectId },
      { uT("type"), uT("Transform") },
      { uT("service"), Object_t{ m_Objects[ObjectId].pPosition } },
    })),
    m_pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), uT("Covellite.Gui.Present.") + strObjectId },
      { uT("type"), uT("Present") },
      { uT("content"), ::std::vector<int>{ _Indices.begin(), _Indices.end() }},
    }))
  };

  return (CovelliteGui::CompiledGeometryHandle)ObjectId;
}

void Renderer::RenderGeometry(CovelliteGui::CompiledGeometryHandle _hGeometry,
  CovelliteGui::Vector2f _Position, CovelliteGui::TextureHandle _hTexture) /*override*/
{
  const auto itObject = m_Objects.find((size_t)_hGeometry);
  if (itObject == m_Objects.end()) return;

  auto & Position = *itObject->second.pPosition;

  Position[uT("x")] = _Position.x;
  Position[uT("y")] = _Position.y;

  using namespace ::alicorn::extension::std;

  if (_hTexture == 0)
  {
    m_RenderQueue += Renders_t
    {
      m_pRenders->Obtain(Component_t::Make(
      {
        { uT("id"), uT("Covellite.Gui.Shader.Pixel.Colored") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("psColored") },
      }))
    };
  }
  else
  {
    const auto strTextureId = uT("Covellite.Gui.Texture.%ID%")
      .Replace(uT("%ID%"), (size_t)_hTexture);

    m_RenderQueue += Renders_t
    {
      m_pRenders->Obtain(Component_t::Make(
      {
        { uT("id"), uT("Covellite.Gui.Shader.Pixel.Textured") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("psTextured") },
      })),
      m_pRenders->Obtain(Component_t::Make(
      {
        { uT("id"), strTextureId },
        { uT("type"), uT("Texture") },
      }))
    };
  }

  m_RenderQueue += itObject->second.Renders;
}

void Renderer::ReleaseGeometry(
  CovelliteGui::CompiledGeometryHandle _hGeometry) /*override*/
{
  const auto strObjectId = String_t{ uT("%ID%") }
    .Replace(uT("%ID%"), (size_t)_hGeometry);

  m_pRenders->Remove(Component_t::Make(
    {
      { uT("id"), uT("Covellite.Gui.Buffer.Vertex.") + strObjectId }
    }));
  m_pRenders->Remove(Component_t::Make(
    {
      { uT("id"), uT("Covellite.Gui.Present.") + strObjectId }
    }));

  m_Objects.erase((size_t)_hGeometry);
}

CovelliteGui::TextureHandle Renderer::LoadTexture(
  CovelliteGui::Vector2i & _TextureDimensions,
  const CovelliteGui::String & _PathToFile) /*override*/
{
  // В Android'e файлы грузятся из 'папки' assets, кроме того, возможно
  // использование виртуальной файловой системы, поэтому exists использовать 
  // НЕЛЬЗЯ!!!
  //if (!::boost::filesystem::exists(_PathToFile.CString())) return false;

  // Один файл грузится один раз, оптимизировать загрузку не нужно.

  using namespace ::alicorn::source;

  try
  {
    const image::Universal_t<image::pixel::RGBA> Image
    {
      app::Vfs_t::GetInstance().GetData(CovelliteGuiStringToUtf8(_PathToFile))
    };

    _TextureDimensions.x = static_cast<int>(Image.GetData().Width);
    _TextureDimensions.y = static_cast<int>(Image.GetData().Height);

    return GenerateTexture(Image.GetData().Buffer, _TextureDimensions);
  }
  catch (const ::std::exception &)
  {
    // Сюда попадаем, если нет файла текстуры или он имеет невалидый формат
    // (записывать в лог ничего не нужно, это используемая библиотека GUI
    // сделает сама).
  }

  return 0;
}

CovelliteGui::TextureHandle Renderer::GenerateTexture(
  CovelliteGui::Span<const CovelliteGui::byte> _Source,
  CovelliteGui::Vector2i _SourceDimensions) /*override*/
{
  static CovelliteGui::TextureHandle TextureId = 0;

  const auto strTextureId = String_t{ uT("Covellite.Gui.Texture.%ID%") }
    .Replace(uT("%ID%"), (size_t)++TextureId);

  //const auto Count = 4 * _SourceDimensions.x * _SourceDimensions.y;

  m_pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), strTextureId },
      { uT("type"), uT("Texture") },
      { uT("content"), ::std::vector<uint8_t>{ _Source.begin(), _Source.end() } },
      { uT("width"), _SourceDimensions.x },
      { uT("height"), _SourceDimensions.y },
    }));

  return TextureId;
}

void Renderer::ReleaseTexture(CovelliteGui::TextureHandle _hTexture) /*override*/
{
  const auto strTextureId = String_t{ uT("Covellite.Gui.Texture.%ID%") }
    .Replace(uT("%ID%"), (size_t)_hTexture);

  m_pRenders->Remove(Component_t::Make(
    {
      { uT("id"), strTextureId },
      { uT("type"), uT("Texture") },
    }));
}

void Renderer::EnableScissorRegion(bool _IsEnable) /*override*/
{
  if (_IsEnable) return;

  using namespace ::alicorn::extension::std;

  m_RenderQueue.push_back(m_pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), uT("Covellite.Gui.State.Scissor.Disabled") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Scissor") },
      { uT("enabled"), uT("false") }
    })));
}

void Renderer::SetScissorRegion(CovelliteGui::Rectanglei _Area) /*override*/
{
  m_RenderQueue.push_back([&, _Area]()
  {
    auto & ScissorRect = *m_pScissorRect;

    ScissorRect[uT("left")] = _Area.Left();
    ScissorRect[uT("right")] = _Area.Right();
    ScissorRect[uT("top")] = _Area.Top();
    ScissorRect[uT("bottom")] = _Area.Bottom();
  });

  using namespace ::alicorn::extension::std;

  m_RenderQueue.push_back(m_pRenders->Obtain(Component_t::Make(
    {
      { uT("id"), uT("Covellite.Gui.State.Scissor.Enabled") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("Scissor") },
      { uT("enabled"), uT("true") },
      { uT("service"), Object_t{ m_pScissorRect } }
    })));
}

void Renderer::RenderScene(void)
{
  for (const auto & Render : m_DefaultRenders) Render();
  for (const auto & Render : m_RenderQueue) Render();

  m_RenderQueue.clear();
}

} // namespace gui

} // namespace covellite
