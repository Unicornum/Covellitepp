
#pragma once
#include "Animation.hpp"
#include "GameObject.hpp"
#include "CubeCoords.hpp"
#include "Defines.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс рендеринга анимированного объекта.
*
* \version
*  1.0.0.0        \n
* \date
*  22 Июнь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Animated final :
  public GameObject
{
  using ComponentPtr_t = ::covellite::api::Component::ComponentPtr_t;
  using cbMaterial_t = ::std::function<Object_t(const String_t &)>;
  class Animations;

  class Value_t
  {
  public:
    cbMaterial_t    Material;
    CubeCoords      Coords;
    ComponentPtr_t  pAnimation;
  };

public:
  Objects_t GetObject(const Any_t &) const override;

private:
  class Animation final
  {
  public:
    Object_t GetObject(void) const;

  private:
    float m_TicksPerSecond = 1.0f;
    animation::Frames_t m_RawFrames;

  public:
    explicit Animation(const Path_t &);
  };

  class Mesh final :
    public GameObject::Mesh
  {
    using Skeleton_t = animation::Skeleton_t;
    using Bone_t = animation::Bone;
    using Skin_t = animation::Skin_t;
    using WeightBones_t = ::std::vector<::std::pair<Bone_t::Index_t, float>>;
    using BoneTransforms_t = ::std::map<String_t, ::glm::mat4>;

    class TriangleWeightBones final
    {
    public:
      WeightBones_t WeightBones[3];
    };

    class Skeleton final
    {
    public:
      inline void Add(
        const ::std::string & _BoneName,
        const ::std::string & _ParentBoneName,
        const ::glm::mat4 & _GlobalTransformation,
        const ::glm::mat4 & _LocalTransformation)
      {
        m_Bones.push_back(
          {
            _BoneName,
            GetBoneIndex(_ParentBoneName),
            _GlobalTransformation,
            _LocalTransformation
          });
      }

      inline const Skeleton_t & GetBones(void) const { return m_Bones; }

      inline Bone_t::Index_t GetBoneIndex(const ::std::string & _Name) const
      {
        for (::std::size_t i = 0; i < m_Bones.size(); i++)
        {
          if (m_Bones[i].Name == _Name) return i;
        }

        return Bone_t::iNonexistent;
      };

    private:
      Skeleton_t m_Bones;
    };

  public:
    Object_t GetObject(const Any_t &) const override;

  private:
    Object_t GetHoldInHandObject(const String_t &) const;
    void Add(const Triangle_t &, const TriangleWeightBones &);
    void AddSkin(const Vertex &, const ::std::size_t, const WeightBones_t &);
    ::std::size_t GetSkinVertexCount(void) const { return m_SkinVertexes.size(); }

  private:
    mutable ::std::shared_ptr<Animations> m_pAnimations;
    Skeleton m_Skeleton;
    Skin_t m_SkinVertexes;
    mutable Component_t::ComponentPtr_t m_pVertexData;
    mutable Component_t::ComponentPtr_t m_pSkinData;
    mutable Component_t::ComponentPtr_t m_pTransformBones;

  public:
    explicit Mesh(const Path_t &);
  };

private:
  static Object_t GetCameraObject(const GameScenePtr_t &);
  static Object_t GetLightsObject(void);
  static Object_t GetPresentObject(const CubeCoords &);

private:
  const GameScenePtr_t m_pGameScene;
  Object_t m_Object;
  mutable ::std::map<::std::string, ::std::size_t> m_Textures;

private:
  explicit Animated(const GameScenePtr_t &);
  friend GameObject;
};

} // namespace model

} // namespace basement
