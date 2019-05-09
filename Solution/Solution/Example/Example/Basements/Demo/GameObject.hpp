
#pragma once
#include <alicorn/boost/filesystem.forward.hpp>
#include <Covellite/Api/Vertex.hpp>
#include "IGameObject.hpp"

namespace basement
{

namespace model
{

class CubeCoords;
class IGameWorld;

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Общий базовый класс игрового объекта.
*
* \version
*  1.0.0.0        \n
* \date
*  28 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class GameObject :
  public IGameObject
{
  using String_t = ::alicorn::extension::std::String;

public:
  using IGameObjectPtr_t = ::std::shared_ptr<IGameObject>;

protected:
  using Path_t = ::boost::filesystem::path;
  using Vertex_t = ::covellite::api::Vertex::Polyhedron;

public:
  // Интерфейс IGameObject:
  size_t GetType(void) const final;

public:
  /**
  * \brief
  *  Функция создания объекта игрового объекта указанного типа.
  */
  static IGameObjectPtr_t Create(const Support::Value);
  static IGameObjectPtr_t Create(const Extra::Value);
  static IGameObjectPtr_t Create(const Landscape::Value, const IGameWorld *);

protected:
  class Mesh
  {
  protected:
    using Triangle_t = Vertex_t[3];

  public:
    class Data final
    {
    public:
      ::std::vector<Vertex_t> Vertex;
      ::std::vector<int>      Index;
    };

  public:
    virtual Object_t GetObject(void) const;

  protected:
    void Add(const Triangle_t &);

  private:
    Data m_Data;
    const String_t m_VertexBufferId;
    const String_t m_IndexBufferId;

  public:
    explicit Mesh(const Data & = {});
  };

  class Texture final
  {
    class Image;
    using ImagePtr_t = ::std::shared_ptr<Image>;

  public:
    Object_t GetObject(void) const;
    float GetRatioXY(void) const;

  private:
    const ImagePtr_t m_pImage;
    const String_t m_Id;

  public:
    explicit Texture(const Path_t &);
  };

protected:
  static Object_t GetShaderObject(void);
  size_t AddTexture(const Path_t &) const;
  const Texture & GetTexture(const size_t) const;
  template<class TMesh, class TData>
  size_t AddMesh(const TData &) const;
  template<class TMesh, class TData>
  size_t AddMesh(const TData &, const float, const Rect &) const;
  const Mesh & GetMesh(const size_t) const;

private:
  const size_t m_Type;
  mutable ::std::vector<::std::unique_ptr<Texture>> m_Textures;
  mutable ::std::vector<::std::unique_ptr<Mesh>> m_Meshes;

protected:
  explicit GameObject(const size_t);
};

template<class TMesh, class TData>
inline size_t GameObject::AddMesh(const TData & _Data) const
{
  m_Meshes.push_back(::std::make_unique<TMesh>(_Data));
  return m_Meshes.size() - 1;
}

template<class TMesh, class TData>
inline size_t GameObject::AddMesh(
  const TData & _Data, 
  const float _TextureRatioXY,
  const Rect & _TextureCoord) const
{
  m_Meshes.push_back(
    ::std::make_unique<TMesh>(_Data, _TextureRatioXY, _TextureCoord));
  return m_Meshes.size() - 1;
}

} // namespace model

} // namespace basement
