
#include "stdafx.h"
#include <Covellite/Gui/Renderer.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/filesystem.hpp>
#include <alicorn/patterns/factory.hpp>
#include <alicorn/image.hpp>
#include <Covellite/App/Vfs.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Renders.hpp>

#if BOOST_COMP_MSVC 
# pragma warning(push)
# pragma warning(disable: 4996)
#elif defined BOOST_COMP_GNUC
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <Covellite/Api/Vertex.hpp>
#if BOOST_COMP_MSVC 
# pragma warning(pop)
#elif defined BOOST_COMP_GNUC
# pragma GCC diagnostic pop
#endif

namespace covellite
{

namespace gui
{

Renderer::Renderer(const RendersPtr_t & _pRenders) :
  m_pRenders(_pRenders),
  m_DefaultRenders(m_pRenders->Obtain(
    {
      Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.Camera.Gui") },
          { uT("type"), uT("Camera") },
          { uT("kind"), uT("Orthographic") },
        }),
      Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.State.Blend") },
          { uT("type"), uT("State") },
          { uT("kind"), uT("Blend") },
        }),
      Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.State.Sampler") },
          { uT("type"), uT("State") },
          { uT("kind"), uT("Sampler") },
        }),
      Component_t::Make(
        {
          { uT("id"), uT("Covellite.Gui.Shader.Vertex") },
          { uT("type"), uT("Shader") },
          { uT("entry"), uT("vsFlat") },
        }),
    })),
    m_pScissorRect(Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Rect") },
      }))
{
}

Renderer::~Renderer(void) noexcept
{
  // Рендеры общих компонентов не удаляются, т.к. они будут удалены вместе с
  // уникальным (для объектв класса Renderer) объектом m_pRenders.
  // m_pRenders->Remove(...);
}

void Renderer::RenderGeometry(
  CovelliteGui::Core::Vertex * _pVertex, int _VertexCount,
  int * _pIndex, int _IndexCount,
  CovelliteGui::Core::TextureHandle _hTexture,
  const CovelliteGui::Core::Vector2f & _Position) /*override*/
{
  const auto hGeometry = CompileGeometry(
    _pVertex, _VertexCount,
    _pIndex, _IndexCount,
    _hTexture);
  RenderCompiledGeometry(hGeometry, _Position);
  ReleaseCompiledGeometry(hGeometry);
}

CovelliteGui::Core::CompiledGeometryHandle Renderer::CompileGeometry(
  CovelliteGui::Core::Vertex * _pVertex, int _VertexCount,
  int * _pIndex, int _IndexCount,
  CovelliteGui::Core::TextureHandle _hTexture) /*override*/
{
  static size_t ObjectId = 0;

  const auto strObjectId = String_t{ uT("%ID%") }
    .Replace(uT("%ID%"), (size_t)++ObjectId);

  Object_t Object;

  using namespace ::alicorn::extension::std;

  if (_hTexture == 0)
  {
    Object.push_back(Component_t::Make(
      {
        { uT("id"), uT("Covellite.Gui.Shader.Pixel.Colored") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("psColored") },
      }));
  }
  else
  {
    const auto strTextureId = String_t{ uT("Covellite.Gui.Texture.%ID%") }
      .Replace(uT("%ID%"), (size_t)_hTexture);

    Object += Object_t
    {
      Component_t::Make(
      {
        { uT("id"), uT("Covellite.Gui.Shader.Pixel.Textured") },
        { uT("type"), uT("Shader") },
        { uT("entry"), uT("psLightened") },
      }),
      Component_t::Make(
      {
        { uT("id"), strTextureId },
        { uT("type"), uT("Texture") },
      })
    };
  }

  const auto Convert = 
    [](CovelliteGui::Core::Vertex * _pVertex, const ::std::size_t _VertexCount)
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

  const auto Vertexes = 
    Convert(_pVertex, static_cast<::std::size_t>(_VertexCount));

  const auto pPosition = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
    });

  Object += Object_t
  {
    Component_t::Make(
    {
      { uT("id"), uT("Covellite.Gui.Buffer.Vertex.") + strObjectId },
      { uT("type"), uT("Buffer") },
      { uT("data"), Vertexes.data() },
      { uT("count"), Vertexes.size() },
      { uT("dimension"), 2 },
    }),
    pPosition,
    Component_t::Make(
    {
      { uT("id"), uT("Covellite.Gui.Transform.") + strObjectId },
      { uT("type"), uT("Transform") },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Covellite.Gui.Present.") + strObjectId },
      { uT("type"), uT("Present") },
      { uT("data"), static_cast<const int *>(_pIndex) },
      { uT("count"), (size_t)_IndexCount }
    })
  };

  m_Objects[ObjectId].pPosition = pPosition;
  m_Objects[ObjectId].Renders = m_pRenders->Obtain(Object);

  return (CovelliteGui::Core::CompiledGeometryHandle)ObjectId;
}

void Renderer::RenderCompiledGeometry(
  CovelliteGui::Core::CompiledGeometryHandle _hGeometry,
  const CovelliteGui::Core::Vector2f & _Position) /*override*/
{
  auto itObject = m_Objects.find((size_t)_hGeometry);
  if (itObject == m_Objects.end()) return;

  using namespace ::alicorn::extension::std;

  itObject->second.pPosition->SetValue(uT("x"), _Position.x);
  itObject->second.pPosition->SetValue(uT("y"), _Position.y);

  m_RenderQueue += itObject->second.Renders;
}

void Renderer::ReleaseCompiledGeometry(
  CovelliteGui::Core::CompiledGeometryHandle _hGeometry) /*override*/
{
  const auto strObjectId = String_t{ uT("%ID%") }
    .Replace(uT("%ID%"), (size_t)_hGeometry);

  m_pRenders->Remove({
    Component_t::Make(
      {
        { uT("id"), uT("Covellite.Gui.Buffer.Vertex.") + strObjectId }
      }),
    Component_t::Make(
      {
        { uT("id"), uT("Covellite.Gui.Present.") + strObjectId }
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
        { uT("id"), uT("Covellite.Gui.State.Scissor.Disabled") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Scissor") },
        { uT("enabled"), uT("false") }
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
        { uT("id"), uT("Covellite.Gui.State.Scissor.Enabled") },
        { uT("type"), uT("State") },
        { uT("kind"), uT("Scissor") },
        { uT("enabled"), uT("true") }
      }),
    });
}

bool Renderer::LoadTexture(
  CovelliteGui::Core::TextureHandle & _hTexture,
  CovelliteGui::Core::Vector2i & _TextureDimensions,
  const CovelliteGui::Core::String & _PathToFile) /*override*/
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

    return GenerateTexture(_hTexture,
      Image.GetData().Buffer.data(), _TextureDimensions);
  }
  catch (const ::std::exception &)
  {
    // Сюда попадаем, если нет файла текстуры или он имеет невалидый формат
    // (записывать в лог ничего не нужно, это используемая библиотека GUI
    // сделает сама).
  }

  return false;
}

bool Renderer::GenerateTexture(
  CovelliteGui::Core::TextureHandle & _hTexture,
  const CovelliteGui::Core::byte * _pSource,
  const CovelliteGui::Core::Vector2i & _SourceDimensions) /*override*/
{
  static CovelliteGui::Core::TextureHandle TextureId = 0;

  const auto strTextureId = String_t{ uT("Covellite.Gui.Texture.%ID%") }
    .Replace(uT("%ID%"), (size_t)++TextureId);

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

void Renderer::ReleaseTexture(CovelliteGui::Core::TextureHandle _hTexture) /*override*/
{
  const auto strTextureId = String_t{ uT("Covellite.Gui.Texture.%ID%") }
    .Replace(uT("%ID%"), (size_t)_hTexture);

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
