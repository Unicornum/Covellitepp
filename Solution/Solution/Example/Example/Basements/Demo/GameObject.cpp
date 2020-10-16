
#include "stdafx.h"
#include "GameObject.hpp"
#include <alicorn/std/exception.hpp>
#include <Covellite/Api/Component.inl>
#include "Skybox.hpp"
#include "Camera.hpp"
#include "Loader.hpp"
#include "Landscape.hpp"
#include "Water.hpp"
#include "Animated.hpp"
#include "Particles.hpp"
#include "Shadows.hpp"

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

/*static*/ auto GameObject::Create(const Another::Value _Type,
  const GameScenePtr_t & _pGameScene) -> IGameObjectPtr_t
{
  if (_Type == Another::Animated)
  {
    return IGameObjectPtr_t{ new Animated{ _pGameScene} };
  }
  else if (_Type == Another::Particles)
  {
    return IGameObjectPtr_t{ new Particles };
  }
  else if (Another::IsShadow(_Type))
  {
    return IGameObjectPtr_t{ new Shadows{ _Type } };
  }

  throw STD_EXCEPTION << "Unknown type object: Another::" << _Type;
}

/*static*/ Object_t GameObject::GetShaderObject(const bool _IsSimple)
{
  const auto Vfs = ::covellite::app::Vfs_t::GetInstance();

  const auto pShaderData = _IsSimple ?
    Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("psLight") },
        { uT("content"), Vfs.GetData("Data\\Shaders\\Example.fx") },
      }) :
    Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("psExample") },
        { uT("content"), Vfs.GetData("Data\\Shaders\\Example.fx") },
      });

  return Object_t
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shader.Vertex") },
      { uT("type"), uT("Shader") },
      { uT("entry"), uT("vsVolume") },
    }),
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Shader.Pixel") },
      { uT("type"), uT("Shader") },
      { uT("service"), Object_t{ pShaderData } },
    }),
  };
}

size_t GameObject::AddTexture(const Path_t & _FileName)
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

GameObject::Mesh & GameObject::GetMesh(const size_t _Index) const
{
  if (_Index >= m_Meshes.size())
  {
    throw STD_EXCEPTION << "Unexpected index mesh: " << _Index;
  }

  return *m_Meshes[_Index].get();
}

} // namespace model

} // namespace basement
