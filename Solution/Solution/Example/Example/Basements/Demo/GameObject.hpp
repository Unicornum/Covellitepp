
#pragma once
#include <GLMath.hpp>
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
public:
  using IGameObjectPtr_t = ::std::shared_ptr<IGameObject>;

protected:
  using Path_t = ::boost::filesystem::path;
  using Vertex_t = ::covellite::api::vertex::Polyhedron;
  using String_t = ::alicorn::extension::std::String;

public:
  // Интерфейс IGameObject:
  size_t GetType(void) const final;

public:
  /**
  * \brief
  *  Функции создания объекта игрового объекта указанного типа.
  */
  static IGameObjectPtr_t Create(const Support::Value);
  static IGameObjectPtr_t Create(const Extra::Value);
  static IGameObjectPtr_t Create(const Landscape::Value, const IGameWorld *);
  static IGameObjectPtr_t Create(const Another::Value, const GameScenePtr_t &);

public:
  class Mesh
  {
    using Index_t = int;

  protected:

  public:
    class Vertex final
    {
    public:
      using Hash_t = uint64_t;

    public:
      Hash_t Hash = static_cast<Hash_t>(-1);
      Vertex_t m_Vertex;
    };

    class Triangle_t final
    {
    public:
      Vertex Vertexes[3];
      String_t MaterialName = uT("Unknown");
    };

  public:
    virtual Object_t GetObject(const Any_t & = Any_t{}) const;

  protected:
    using cbVertex_t = ::std::function<void(const Vertex &, const ::std::size_t)>;
    void Add(const Triangle_t &, const cbVertex_t & = nullptr);
    ::std::size_t GetAllVertexCount(void) const { return m_Vertexes.size(); }
    void BuildBasementObject(const float);
    static Point GetPoint(const float, const float = 0.5f);

  private:
    ::std::map<Vertex::Hash_t, Index_t> m_iVertexes;

  protected:
    const String_t m_MeshId;
    ::std::vector<Vertex_t> m_Vertexes;
    mutable ::std::map<String_t, ::std::vector<Index_t>> m_MaterialIndices;

  public:
    explicit Mesh(const ::std::vector<Triangle_t> & = {});
    virtual ~Mesh(void) = default;
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
  static Object_t GetShaderObject(const bool = false);
  size_t AddTexture(const Path_t &);
  const Texture & GetTexture(const size_t) const;
  template<class TMesh, class TData>
  size_t AddMesh(const TData &) const;
  template<class TMesh, class TData>
  size_t AddMesh(const TData &, const float, const Rect &) const;
  Mesh & GetMesh(const size_t) const;

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
