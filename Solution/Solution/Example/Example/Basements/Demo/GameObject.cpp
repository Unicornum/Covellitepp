
#include "stdafx.h"
#include "GameObject.hpp"
#include <boost/any.hpp>
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/exception.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/boost/filesystem.hpp>
#include <alicorn/image.hpp>
#include <alicorn/logger.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/App/Settings.hpp>
#include "CubeCoords.hpp"
#include "Camera.hpp"
#include "Loader.hpp"
#include "Landscape.hpp"
#include "Water.hpp"
#include "Compass.hpp"

namespace basement
{

namespace model
{

namespace math = ::alicorn::extension::cpp::math;
namespace image = ::alicorn::source::image;

class GameObject::Image final
{
public:
  const image::Universal_t<image::pixel::RGBA> m_Image;

public:
  explicit Image(const Path_t & _PathToTextureFile) :
    m_Image(::boost::filesystem::load_binary_file(_PathToTextureFile))
  {

  }
};

GameObject::GameObject(const Type::Value _Type) :
  m_Type(_Type)
{

}

auto GameObject::GetType(void) const /*final*/ -> Type::Value
{
  return m_Type;
}

/*static*/ auto GameObject::Create(const Type::Value _Type,
  const IGameWorld & _GameWorld) -> IGameObjectPtr_t
{
  if (_Type == Type::Camera)
  {
    return IGameObjectPtr_t{ new Camera };
  }
  else if (_Type == Type::Loader)
  {
    return IGameObjectPtr_t{ new Loader };
  }
  else if (_Type == Type::Water)
  {
    return IGameObjectPtr_t{ new Water };
  }
  else if (_Type == Type::Compass)
  {
    return IGameObjectPtr_t{ new Compass };
  }

  return IGameObjectPtr_t{ new Landscape{ _Type, _GameWorld } };
}

Object_t GameObject::LoadTexture(const Path_t & _TextureFileName) const
{
  LOGGER(Trace) << "Load texture file: " << _TextureFileName.c_str();

  using ::covellite::app::Settings_t;

  const auto PathToTextureDirectory =
    Settings_t::GetInstance().Get<Path_t>(uT("PathToTextureDirectory"));

  m_pTextureImage = ::std::make_shared<Image>(
    PathToTextureDirectory / "demo" / _TextureFileName);
  auto & ImageData = m_pTextureImage->m_Image.GetData();

  return
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Texture") },
      { uT("data"), ImageData.Buffer.data() },
      { uT("width"), static_cast<int>(ImageData.Width) },
      { uT("height"), static_cast<int>(ImageData.Height) },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Texture.%TYPE%").Replace(uT("%TYPE%"), GetType()) },
      { uT("type"), uT("Texture") },
    }),
  };
}

Object_t GameObject::GetShaderObject(void) const
{
  return Object_t
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Shader.HLSL") },
      { uT("version"), uT("ps_4_0") },
      { uT("entry"), uT("psTextured") },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shader.Pixel.Compass") },
      { uT("type"), uT("Shader") },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Shader.HLSL") },
      { uT("version"), uT("vs_4_0") },
      { uT("entry"), uT("vsTextured") },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shader.Vertex") },
      { uT("type"), uT("Shader") },
    }),
  };
}

Object_t GameObject::GetMeshObject(
  const ::std::vector<Vertex_t> & _VertexData,
  const ::std::vector<int> & _IndexData) const
{
  static size_t Index = 0;
  Index++;

  return Object_t
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Buffer") },
      { uT("data"), _VertexData.data() },
      { uT("count"), _VertexData.size() },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Buffer.Vertex.%ID%").Replace(uT("%ID%"), Index) },
      { uT("type"), uT("Buffer") },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Buffer") },
      { uT("data"), _IndexData.data() },
      { uT("count"), _IndexData.size() },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Buffer.Index.%ID%").Replace(uT("%ID%"), Index) },
      { uT("type"), uT("Buffer") },
    }),
  };
}

Object_t GameObject::GetCommonObject(
  const ::std::vector<Vertex_t> & _VertexData,
  const ::std::vector<int> & _IndexData) const
{
  using namespace ::alicorn::extension::std;

  return GetShaderObject() +
    GetMeshObject(_VertexData, _IndexData);
}

} // namespace model

} // namespace basement
