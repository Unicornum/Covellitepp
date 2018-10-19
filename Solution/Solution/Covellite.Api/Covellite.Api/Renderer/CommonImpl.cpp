
#include "stdafx.h"
#include "CommonImpl.hpp"
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/lexical-cast.hpp>
#include "fx/Pixel.auto.hpp"
#include "fx/Vertex.auto.hpp"

using namespace covellite::api::renderer;

CommonImpl::CommonImpl(void)
{
  m_DefaultRenders = ::std::vector<Component>
  {
    Component{ 
      {
        { uT("type"), uT("Camera") }, 
        { uT("id"), uT("Covellite.Api.Camera") }
      } },
    Component{ 
      {
        { uT("type"), uT("BlendState") }, 
        { uT("id"), uT("Covellite.Api.BlendState") }
      } },
    Component{ 
      {
        { uT("type"), uT("SamplerState") }, 
        { uT("id"), uT("Covellite.Api.SamplerState") }
      } },
    Component{ 
      {
        { uT("type"), uT("Shader") },
        { uT("kind"), uT("Vertex") },
        { uT("version"), uT("vs_4_0") },
        { uT("entry"), uT("VS") },
        { uT("id"), uT("Covellite.Api.VertexShader.Common") },
        { uT("data"), ::Vertex.data() },
        { uT("count"), ::Vertex.size() },
      } },
  };
}

CommonImpl::~CommonImpl(void) = default;

CommonImpl::ITexture * CommonImpl::Create(const ITexture::Data & _SourceData) /*final*/
{
  class Texture final :
    public IGraphicApi::ITexture
  {
  public:
    void Render(void) override { }
  };

  ITexture * TextureId = new Texture;

  const auto strTextureId = uT("Covellite.Api.Texture.{ID}")
    .Replace(uT("{ID}"), (size_t)TextureId);

  Obtain(
    {
      Component{
      {
        { uT("type"), uT("Texture") },
        { uT("id"), strTextureId },
        { uT("data"), _SourceData.pData },
        { uT("width"), _SourceData.Width },
        { uT("height"), _SourceData.Height },
      } }
    });

  return TextureId;
}

void CommonImpl::Destroy(ITexture * _TextureId) /*final*/
{
  const auto strTextureId = uT("Covellite.Api.Texture.{ID}")
    .Replace(uT("{ID}"), (size_t)_TextureId);

  Remove({
    Component{ { { uT("id"), strTextureId } } }
    });

  delete _TextureId;
}

CommonImpl::IGeometry * CommonImpl::Create(const IGeometry::Data & _SourceData) /*final*/
{
  class GeometryId final :
    public IGraphicApi::IGeometry
  {
  public:
    void Update(FLOAT, FLOAT) override { }
    void Render(void) override { }
  };

  Object_t Object;

  if (_SourceData.pTexture == nullptr)
  {
    Object.push_back(Component{
      {
        { uT("type"), uT("Shader") },
        { uT("kind"), uT("Pixel") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psColored") },
        { uT("id"), uT("Covellite.Api.PixelShader.Colored") },
        { uT("data"), ::Pixel.data() },
        { uT("count"), ::Pixel.size() }
      } });
  }
  else
  {
    const auto strTextureId = uT("Covellite.Api.Texture.{ID}")
      .Replace(uT("{ID}"), (size_t)_SourceData.pTexture);

    Object.push_back(Component{
      {
        { uT("type"), uT("Shader") },
        { uT("kind"), uT("Pixel") },
        { uT("version"), uT("ps_4_0") },
        { uT("entry"), uT("psTextured") },
        { uT("id"), uT("Covellite.Api.PixelShader.Textured") },
        { uT("data"), ::Pixel.data() },
        { uT("count"), ::Pixel.size() }
      } });

    Object.push_back(Component{
      {
        { uT("type"), uT("Texture") },
        { uT("id"), strTextureId },
      } });
  }

  IGeometry * ObjectId = new GeometryId;
  const auto strObjectId = uT("{ID}").Replace(uT("{ID}"), (size_t)ObjectId);

  Object.push_back(Component{
    {
      { uT("type"), uT("Position") },
      { uT("id"), uT("Covellite.Api.Position.") + strObjectId },
      { uT("x"), &m_PositionX },
      { uT("y"), &m_PositionY },
      { uT("z"), &m_PositionZ },
    } });

  Object.push_back(Component{
    {
      { uT("type"), uT("VertexBuffer") },
      { uT("id"), uT("Covellite.Api.VertexBuffer.") + strObjectId },
      { uT("data"), static_cast<const Vertex *>(_SourceData.pVertices) },
      { uT("count"), (size_t)_SourceData.VerticesCount }
    } });

  Object.push_back(Component{
    {
      { uT("type"), uT("IndexBuffer") },
      { uT("id"), uT("Covellite.Api.IndexBuffer.") + strObjectId },
      { uT("data"), static_cast<const int *>(_SourceData.pIndices) },
      { uT("count"), (size_t)_SourceData.IndicesCount }
    } });

  Object.push_back(Component{
    {
      { uT("type"), uT("DrawCall") },
      { uT("id"), uT("Covellite.Api.DrawCall.Common") },
    } });

  m_Objects[ObjectId] = Obtain(Object);

  return ObjectId;
}

void CommonImpl::Destroy(IGeometry * _ObjectId) /*final*/
{
  const auto strObjectId = uT("{ID}").Replace(uT("{ID}"), (size_t)_ObjectId);

  Remove({
    Component{ { { uT("id"), uT("Covellite.Api.Position.") + strObjectId } } },
    Component{ { { uT("id"), uT("Covellite.Api.VertexBuffer.") + strObjectId } } },
    Component{ { { uT("id"), uT("Covellite.Api.IndexBuffer.") + strObjectId } } },
  });

  m_Objects.erase(_ObjectId);
  delete _ObjectId;
}

void CommonImpl::EnableScissorRegion(int _X, int _Y, int _Width, int _Height) /*final*/
{
  using namespace ::alicorn::extension::std;

  m_RenderQueue.push_back([&, _X, _Y, _Width, _Height]()
  {
    m_Left = _X;
    m_Right = _X + _Width;
    m_Top = _Y;
    m_Bottom = _Y + _Height;
  });

  m_RenderQueue += Obtain(
    {
      Component{
      {
        { uT("type"), uT("Scissor") },
        { uT("id"), uT("Covellite.Api.Scissor.Enabled") },
        { uT("IsEnabled"), uT("true") },
        { uT("left"), &m_Left },
        { uT("top"), &m_Top },
        { uT("right"), &m_Right },
        { uT("bottom"), &m_Bottom },
      } }
    });
}

void CommonImpl::DisableScissorRegion(void) /*override*/
{
  using namespace ::alicorn::extension::std;

  m_RenderQueue += Obtain(
    {
      Component{
      {
        { uT("type"), uT("Scissor") },
        { uT("id"), uT("Covellite.Api.Scissor.Disabled") },
        { uT("IsEnabled"), uT("false") }
      } 
    }});
}

void CommonImpl::Render(IGeometry * _ObjectId, float _X, float _Y) /*final*/
{
  using namespace ::alicorn::extension::std;

  m_RenderQueue.push_back([&, _X, _Y]()
  {
    m_PositionX = _X;
    m_PositionY = _Y;
    //m_PositionZ = -0.1f;
  });

  m_RenderQueue += m_Objects[_ObjectId];
}

void CommonImpl::BeginScene(void)
{
  m_RenderQueue = Obtain(m_DefaultRenders);
}

void CommonImpl::EndScene(void)
{
  for (const auto & Render : m_RenderQueue) Render();
}

auto CommonImpl::Obtain(const Object_t & _Object) -> ObjectRenders_t
{
  ObjectRenders_t Result;

  for (const auto & _Component : _Object)
  {
    auto itComponent = m_Components.find(_Component.Id);
    if (itComponent != m_Components.end())
    {
      Result.push_back(itComponent->second);
      continue;
    }

    const auto Creators = GetCreators();

    auto itCreator = Creators.find(_Component.Type);
    if (itCreator == Creators.end())
    {
      continue;
    }

    auto ComponentRender = itCreator->second(_Component);
    m_Components[_Component.Id] = ComponentRender;
    Result.push_back(ComponentRender);
  }

  return Result;
}

void CommonImpl::Remove(const Object_t & _Object)
{
  for (const auto & _Component : _Object)
  {
    m_Components.erase(_Component.Id);
  }
}
