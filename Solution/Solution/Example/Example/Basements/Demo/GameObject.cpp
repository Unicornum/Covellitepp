
#include "stdafx.h"
#include "GameObject.hpp"
#include <alicorn/std/exception.hpp>
#include <Covellite/Api/Component.inl>
#include "Skybox.hpp"
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

GameObject::GameObject(const size_t _Type) :
  m_Type(_Type)
{

}

size_t GameObject::GetType(void) const /*final*/
{
  return m_Type;
}

/*static*/ auto GameObject::Create(const Support::Value _Type) -> IGameObjectPtr_t
{
  if (_Type == Support::Skybox)
  {
    return IGameObjectPtr_t{ new Skybox };
  }
  else if (_Type == Support::Camera)
  {
    return IGameObjectPtr_t{ new Camera };
  }

  throw STD_EXCEPTION << "Unknown type object: Supoport::" << _Type;
}

/*static*/ auto GameObject::Create(const Extra::Value _Type) -> IGameObjectPtr_t
{
  if (_Type == Extra::Water)
  {
    return IGameObjectPtr_t{ new Water };
  }
  else if (_Type == Extra::Compass)
  {
    return IGameObjectPtr_t{ new Compass };
  }
  else if (_Type == Extra::Loader)
  {
    return IGameObjectPtr_t{ new Loader };
  }

  throw STD_EXCEPTION << "Unknown type object: Extra::" << _Type;
}

/*static*/ auto GameObject::Create(const Landscape::Value _Type,
  const IGameWorld * _pGameWorld) -> IGameObjectPtr_t
{
  return IGameObjectPtr_t{ new model::Landscape{ _Type, *_pGameWorld } };
}

/*static*/ Object_t GameObject::GetShaderObject(void)
{
  return Object_t
  {
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Shader.HLSL") },
      { uT("version"), uT("ps_4_0") },
      { uT("entry"), uT("psDiscardAlpha") },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shader.Pixel") },
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
    Component_t::Make(
    {
      { uT("id"), uT("Demo.State.AlphaTest") },
      { uT("type"), uT("State") },
      { uT("kind"), uT("AlphaTest") },
      { uT("discard"), 0.45f },
    }),
  };
}

size_t GameObject::AddTexture(const Path_t & _FileName) const
{
  m_Textures.push_back(::std::make_unique<Texture>(_FileName));
  return m_Textures.size() - 1;
}

const GameObject::Texture & GameObject::GetTexture(const size_t _Index) const
{
  if (_Index >= m_Textures.size())
  {
    throw STD_EXCEPTION << "Unexpected index texture: " << _Index;
  }

  return *m_Textures[_Index].get();
}

const GameObject::Mesh & GameObject::GetMesh(const size_t _Index) const
{
  if (_Index >= m_Meshes.size())
  {
    throw STD_EXCEPTION << "Unexpected index mesh: " << _Index;
  }

  return *m_Meshes[_Index].get();
}

} // namespace model

} // namespace basement
