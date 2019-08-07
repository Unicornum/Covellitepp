
#pragma once
#include <GLMath.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/boost/filesystem.hpp>
#include <assimp/importer.hpp>
#include <Covellite/Api/robin_hood.hpp>

struct aiNode;
struct aiMesh;
struct aiVectorKey;
struct aiQuatKey;

namespace basement
{

namespace model
{

class Assimp final
{
  using Path_t = ::boost::filesystem::path;
  using String_t = ::alicorn::extension::std::String;
  using BoneLocals_t = ::robin_hood::unordered_map<::std::string, aiMatrix4x4>;
  using VertexWeight_t = ::std::pair<::std::string, float>;
  using BoneWeights_t = ::std::map<unsigned int, ::std::vector<VertexWeight_t>>;

public:
  class Bone final
  {
  public:
    ::std::string Name;
    ::std::string ParentName;
    ::glm::mat4 GlobalTransform;
    ::glm::mat4 LocalTransform;
  };

  class Frames final
  {
  public:
    using Keys_t = ::std::vector<::std::pair<float, ::glm::mat4>>;
    ::std::string BoneName;
    Keys_t        Keys;
  };

  class Face final
  {
    class Vertex final
    {
    public:
      uint64_t   Hash;
      aiVector3D Position;
      aiVector3D Normal;
      aiVector3D TexCoords;
      ::std::vector<VertexWeight_t> WeightBones;
    };

  public:
    Vertex   Vertexes[3];
    String_t MaterialName;
  };

private:
  using BoneListener_t = ::std::function<void(const Bone &)>;
  using FrameListener_t = ::std::function<void(const Frames &)>;
  using FaceListener_t = ::std::function<void(const Face &)>;

public:
  void LoadSkeleton(const BoneListener_t &);
  void LoadGeometry(const FaceListener_t &);
  float LoadAnimation(const FrameListener_t &);

private:
  static ::glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &);
  static const ::aiScene * LoadFile(const Path_t &, ::Assimp::Importer &);
  static BoneLocals_t LoadBoneLocals(const aiScene * const);
  static void LoadSkeleton(const BoneListener_t &, const BoneLocals_t &,
    const aiNode * const, const ::glm::mat4 & = ::glm::mat4{ 1.0f });
  BoneWeights_t LoadBoneWeights(const aiMesh * const);
  void LoadSkin(const FaceListener_t &, const aiScene *, const aiNode *);
  Frames::Keys_t BuildFrames(const ::std::vector<aiVectorKey> &,
    const ::std::vector<aiQuatKey> &, const ::std::vector<aiVectorKey> &);

private:
  ::Assimp::Importer m_Importer;
  const ::aiScene *  m_pScene;

public:
  Assimp(const Path_t &);
};

} // namespace model

} // namespace basement
