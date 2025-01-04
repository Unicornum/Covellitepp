
#include "stdafx.h"
#include "Animation.hpp"

namespace basement
{

namespace model
{

Animation::Animation(
  const float _TicksPerSecond, 
  const animation::Frames_t & _RawFrames) :
  m_TicksPerSecond(_TicksPerSecond),
  m_RawFrames(_RawFrames)
{

}

::std::size_t Animation::GetFrameIndex(const float _FrameTime) const
{
  if (m_RawFrames.empty()) return 0;

  const auto AnimationTime =
    fmod(_FrameTime * m_TicksPerSecond, m_RawFrames.back().first);

  for (::std::size_t i = 1; i < m_RawFrames.size(); i++)
  {
    if (AnimationTime < m_RawFrames[i].first)
    {
      const auto dLeftTimeFrame = AnimationTime - m_RawFrames[i - 1].first;
      const auto dNextTimeFrame = m_RawFrames[i].first - AnimationTime;
      return (dLeftTimeFrame < dNextTimeFrame) ? i - 1 : i;
    }
  }

  return 0;
}

Animation::Frame_t Animation::GetFrame(
  const ::std::size_t _iFrame, 
  const animation::Skeleton_t & _Skeleton) const
{
  if (m_RawFrames.empty()) return Frame_t{};

  const auto FramesCount = m_RawFrames.size();

  if (m_Frames.size() < FramesCount)
  {
    m_Frames.resize(FramesCount);
  }

  if (m_Frames[_iFrame].empty())
  {
    m_Frames[_iFrame].resize(_Skeleton.size());
    Frame_t BoneTransformParents;
    BoneTransformParents.resize(_Skeleton.size());

    // !!! «десь параллелить обработку нельз€ !!!
    for (::std::size_t i = 0; i < _Skeleton.size(); i++)
    {
      const auto & Bone = _Skeleton[i];

      auto Global = Get(_iFrame, Bone.Name);

      if (Bone.iParent != animation::Bone::iNonexistent)
      {
        Global = BoneTransformParents[Bone.iParent] * Global;
      }
      else
      {
        //  орнева€ кость, ...

        // ... добавл€ем трансформацию корневой кости скелета ...
        Global = Bone.Global * Global;

        // ... удал€ем смещение в плоскости Oxy ...
        Global[3] = ::glm::vec4{ 0.0f, 0.0f, Global[3][2], Global[3][3] };
      }

      BoneTransformParents[i] = Global;
      m_Frames[_iFrame][i] = Global * Bone.Local;
    }
  }

  return m_Frames[_iFrame];
}

::glm::mat4 Animation::Get(
  ::std::size_t _iFrame, 
  const ::std::string & _BoneName) const
{
  const auto itBone = m_RawFrames[_iFrame].second.find(_BoneName);
  if (itBone != m_RawFrames[_iFrame].second.end())
  {
    return itBone->second.Value;
  }

  return ::glm::mat4{ 1.0f };
}

} // namespace model

} // namespace basement
